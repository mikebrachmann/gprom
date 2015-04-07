/*-----------------------------------------------------------------------------
 *
 * parser_dl.c
 *			  
 *		
 *		AUTHOR: lord_pretzel
 *
 *		
 *
 *-----------------------------------------------------------------------------
 */

#include "common.h"
#include "log/logger.h"
#include "mem_manager/mem_mgr.h"
#include "parser/parser.h"
#include "parser/parse_internal_jp.h"
#include "parser/parser_jp.h"
#include "jp_parser.tab.h"
#include "instrumentation/timing_instrumentation.h"

static Node *parseInternaljp (void);

Node *
parseStreamjp (FILE *stream)
{
    jpin = stream;

    return parseInternaljp();
}

Node *
parseFromStringjp (char *input)
{
    INFO_LOG("parse JSON PATH:\n%s", input);
    jpSetupStringInput(input);

    return parseInternaljp();
}

static Node *
parseInternaljp (void) //TODO make copyObject work first
{
    Node *result;
    START_TIMER("module - parser");

    NEW_AND_ACQUIRE_MEMCONTEXT("PARSER_CONTEXT");

    // parse
    int rc = jpparse();
    if (rc)
    {
        FATAL_LOG("parse error!");
        return NULL;
    }

    STOP_TIMER("module - parser");

    if(jpParseResult != NIL)
    DEBUG_LOG("Json Path model generated by parser is:\n%s\n\n%s",
            nodeToString(jpParseResult),
            beatify(nodeToString(jpParseResult)));

    // create copy of parse result in parent context
    FREE_MEM_CONTEXT_AND_RETURN_COPY(Node,jpParseResult);
}

