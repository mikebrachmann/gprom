/*-----------------------------------------------------------------------------
 *
 * rewriter.c
 *			  
 *		
 *		AUTHOR: lord_pretzel
 *
 *		
 *
 *-----------------------------------------------------------------------------
 */

#include "rewriter.h"

#include "common.h"
#include "mem_manager/mem_mgr.h"
#include "log/logger.h"
#include "configuration/option.h"
#include "configuration/option_parser.h"
#include "exception/exception.h"
#include "model/node/nodetype.h"
#include "model/query_block/query_block.h"
#include "model/query_operator/query_operator.h"
#include "model/query_operator/query_operator_model_checker.h"
#include "model/datalog/datalog_model.h"
#include "provenance_rewriter/prov_rewriter.h"
#include "analysis_and_translate/analyzer.h"
#include "analysis_and_translate/translator.h"
#include "sql_serializer/sql_serializer.h"
#include "operator_optimizer/operator_optimizer.h"
#include "parser/parser.h"
#include "metadata_lookup/metadata_lookup.h"
#include "operator_optimizer/cost_based_optimizer.h"
#include "execution/executor.h"

#include "instrumentation/timing_instrumentation.h"
#include "instrumentation/memory_instrumentation.h"

#include "provenance_rewriter/transformation_rewrites/transformation_prov_main.h"

static char *rewriteParserOutput (Node *parse, boolean applyOptimizations);

int
initBasicModulesAndReadOptions (char *appName, char *appHelpText, int argc, char* argv[])
{
    initMemManager();
    mallocOptions();
    initLogger();

    if(parseOption(argc, argv) != 0)
    {
        printOptionParseError(stdout);
        printOptionsHelp(stdout, appName, appHelpText, TRUE);
        return EXIT_FAILURE;
    }

    if (getBoolOption("help"))
    {
        printOptionsHelp(stdout, appName, appHelpText, FALSE);
        return EXIT_FAILURE;
    }

    // set log level from options
    setMaxLevel(getIntOption("log.level"));

    if (opt_memmeasure)
        setupMemInstrumentation();

    return EXIT_SUCCESS;
}

int
initBasicModules (void)
{
    initMemManager();
    mallocOptions();
    initLogger();
    if (opt_memmeasure)
        setupMemInstrumentation();

    return EXIT_SUCCESS;
}

void
setupPluginsFromOptions(void)
{
    char *be = getStringOption("backend");
    char *pluginName = be;

    // setup parser - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.parser")) != NULL)
        chooseParserPluginFromString(pluginName);
    else
        chooseParserPluginFromString(be);

    // setup metadata lookup - individual option overrides backend option
    pluginName = getStringOption("plugin.metadata");
    if (strpeq(pluginName,"external"))
    {
        printf("\nPLUGIN******************************************\n\n");
    }
    else
    {
        initMetadataLookupPlugins();
        if (pluginName != NULL)
            chooseMetadataLookupPluginFromString(pluginName);
        else
            chooseMetadataLookupPluginFromString(be);
        initMetadataLookupPlugin();
    }

    // setup analyzer - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.analyzer")) != NULL)
        chooseAnalyzerPluginFromString(pluginName);
    else
        chooseAnalyzerPluginFromString(be);

    // setup analyzer - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.translator")) != NULL)
        chooseTranslatorPluginFromString(pluginName);
    else
        chooseTranslatorPluginFromString(be);

    // setup analyzer - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.sqlserializer")) != NULL)
        chooseSqlserializerPluginFromString(pluginName);
    else
        chooseSqlserializerPluginFromString(be);
    // setup analyzer - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.executor")) != NULL)
        chooseExecutorPluginFromString(pluginName);
    else
        chooseExecutorPluginFromString("sql");

    // setup cost-based optimizer
    if ((pluginName = getStringOption("plugin.cbo")) != NULL)
        chooseOptimizerPluginFromString(pluginName);
    else
        chooseOptimizerPluginFromString("exhaustive");
}

void
resetupPluginsFromOptions (void)
{
    char *be = getStringOption("backend");
    char *pluginName = be;

    // setup parser - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.parser")) != NULL)
        chooseParserPluginFromString(pluginName);
    else
        chooseParserPluginFromString(be);

    // setup analyzer - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.analyzer")) != NULL)
        chooseAnalyzerPluginFromString(pluginName);
    else
        chooseAnalyzerPluginFromString(be);

    // setup translator - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.translator")) != NULL)
        chooseTranslatorPluginFromString(pluginName);
    else
        chooseTranslatorPluginFromString(be);

    // setup serializer - individual option overrides backend option
    if ((pluginName = getStringOption("plugin.sqlserializer")) != NULL)
        chooseSqlserializerPluginFromString(pluginName);
    else
        chooseSqlserializerPluginFromString(be);

    // setup executor- individual option overrides backend option
    if ((pluginName = getStringOption("plugin.executor")) != NULL)
        chooseExecutorPluginFromString(pluginName);
    else
        chooseExecutorPluginFromString("sql");
}

int
readOptionsAndIntialize(char *appName, char *appHelpText, int argc, char* argv[])
{
    int retVal;
    retVal = initBasicModulesAndReadOptions(appName, appHelpText, argc, argv);
    if (retVal != EXIT_SUCCESS)
        return retVal;

    setupPluginsFromOptions();

    return EXIT_SUCCESS;
}

int
shutdownApplication(void)
{
    INFO_LOG("shutdown plugins, logger, and memory manager");

    if (opt_memmeasure)
    {
        outputMemstats(FALSE);
        shutdownMemInstrumentation();
    }
    shutdownMetadataLookupPlugins();

    freeOptions();
    destroyMemManager();

    return EXIT_SUCCESS;
}

void
processInput(char *input)
{
    char *result = rewriteQuery(input);
    execute(result);
}

char *
rewriteQuery(char *input)
{
    Node *parse;
    char *result = "";

    NEW_AND_ACQUIRE_MEMCONTEXT(QUERY_MEM_CONTEXT);

    TRY
    {
        parse = parseFromString(input);

        DEBUG_LOG("parser returned:\n\n<%s>", nodeToString(parse));

        result = rewriteParserOutput(parse, isRewriteOptionActivated(OPTION_OPTIMIZE_OPERATOR_MODEL));
        INFO_LOG("Rewritten SQL text from <%s>\n\n is <%s>", input, result);
        RELEASE_MEM_CONTEXT_AND_RETURN_STRING_COPY(result);
    }
    ON_EXCEPTION
    {
        // if an exception is thrown then the query memory context has been
        // destroyed and we can directly create an empty string in the callers
        // context
        DEBUG_LOG("allocated in memory context: %s", getCurMemContext()->contextName);
    }
    END_ON_EXCEPTION
    return strdup("");
}

char *
rewriteQueryFromStream (FILE *stream) {
    Node *parse;
    char *result;

    parse = parseStream(stream);
    DEBUG_LOG("parser returned:\n\n%s", nodeToString(parse));

    result = rewriteParserOutput(parse, isRewriteOptionActivated(OPTION_OPTIMIZE_OPERATOR_MODEL));
    INFO_LOG("Rewritten SQL text is <%s>", result);

    return result;
}

char *
rewriteQueryWithOptimization(char *input)
{
    Node *parse;
    char *result;

    parse = parseFromString(input);
    DEBUG_LOG("parser returned:\n\n<%s>", nodeToString(parse));

    result = rewriteParserOutput(parse, TRUE);
    INFO_LOG("Rewritten SQL text from <%s>\n\n is <%s>", input, result);

    return result;
}

char *
generatePlan(Node *oModel, boolean applyOptimizations)
{
	StringInfo result = makeStringInfo();
	Node *rewrittenTree;
	char *rewrittenSQL = NULL;
	START_TIMER("rewrite");

	rewrittenTree = provRewriteQBModel(oModel);

	if (IS_QB(rewrittenTree))
	{
	    DOT_TO_CONSOLE_WITH_MESSAGE("BEFORE REWRITE", rewrittenTree);

	    /*******************new Add for test json import jimp table***************************/
	    if(isA(rewrittenTree,List) && isA(getHeadOfListP((List *)rewrittenTree), ProjectionOperator))
	    {
	        QueryOperator *q = (QueryOperator *)getHeadOfListP((List *)rewrittenTree);
	        List *taOp = NIL;
	        findTableAccessOperator(&taOp, (QueryOperator *) q);
	        int l = LIST_LENGTH(taOp);
	        DEBUG_LOG("len %d", l);
	        if(isA(getHeadOfListP(taOp),TableAccessOperator))
	        {
	            TableAccessOperator *ta = (TableAccessOperator *) getHeadOfListP(taOp);
	            DEBUG_LOG("test %s", ta->tableName);
	            if(streq(ta->tableName,"JIMP2") || streq(ta->tableName, "JSDOC1"))
	            {
	                q = rewriteTransformationProvenanceImport(q);
	                DEBUG_LOG("Table: %s", nodeToString(ta));
	                rewrittenTree = (Node *) singleton(q);
	            }
	        }
	    }
	    /*******************new Add for test json import jimp table***************************/

	    DEBUG_LOG("provenance rewriter returned:\n\n<%s>", beatify(nodeToString(rewrittenTree)));
	    INFO_LOG("provenance rewritten query as overview:\n\n%s", operatorToOverviewString(rewrittenTree));
	    STOP_TIMER("rewrite");

	    ASSERT_BARRIER(
	            if (isA(rewrittenTree, List))
	                FOREACH(QueryOperator,o,(List *) rewrittenTree)
	                TIME_ASSERT(checkModel(o));
	            else
	                TIME_ASSERT(checkModel((QueryOperator *) rewrittenTree));
	    )

	    if(applyOptimizations)
	    {
	        START_TIMER("OptimizeModel");
	        rewrittenTree = optimizeOperatorModel(rewrittenTree);
	        INFO_LOG("after optimizing AGM graph:\n\n%s", operatorToOverviewString(rewrittenTree));
	        STOP_TIMER("OptimizeModel");
	    }
	    else
	        if (isA(rewrittenTree, List))
	            FOREACH(QueryOperator,o,(List *) rewrittenTree)
	            LC_P_VAL(o_his_cell) = materializeProjectionSequences (o);
	        else
	            rewrittenTree = (Node *) materializeProjectionSequences((QueryOperator *) rewrittenTree);

	    DOT_TO_CONSOLE_WITH_MESSAGE("AFTER OPTIMIZATIONS", rewrittenTree);
	}

	START_TIMER("SQLcodeGen");
	appendStringInfo(result, "%s\n", serializeOperatorModel(rewrittenTree));
	STOP_TIMER("SQLcodeGen");

	rewrittenSQL = result->data;
	FREE(result);

	return rewrittenSQL;

}

static char *
rewriteParserOutput (Node *parse, boolean applyOptimizations)
{
    char *rewrittenSQL = NULL;
    Node *oModel;

    START_TIMER("translation");
    oModel = translateParse(parse);
    DEBUG_LOG("translator returned:\n\n<%s>", beatify(nodeToString(oModel)));
    if (IS_OP(oModel))
    {
        INFO_LOG("translator result as overview:\n\n%s", operatorToOverviewString(oModel));
        DOT_TO_CONSOLE_WITH_MESSAGE("TRANSLATOR OUTPUT", oModel);
    }
    else if (IS_DL_NODE(oModel))
    {
        INFO_LOG("translator result as overview:\n\n%s", datalogToOverviewString(oModel));
    }
    STOP_TIMER("translation");

    ASSERT_BARRIER(
        if (IS_OP(oModel))
        {
            if (isA(oModel, List))
                FOREACH(QueryOperator,o,(List *) oModel)
                TIME_ASSERT(checkModel(o));
            else
                TIME_ASSERT(checkModel((QueryOperator *) oModel));
        }
    )

    if (getBoolOption(OPTION_COST_BASED_OPTIMIZER))
        rewrittenSQL = doCostBasedOptimization(oModel, applyOptimizations);
    else
    	rewrittenSQL = generatePlan(oModel, applyOptimizations);

    return rewrittenSQL;
}
