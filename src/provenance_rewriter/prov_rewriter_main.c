/*-----------------------------------------------------------------------------
 *
 * prov_rewriter_main.c
 *		Main entry point to the provenance rewriter.
 *		
 *		AUTHOR: lord_pretzel
 *
 *		
 *
 *-----------------------------------------------------------------------------
 */

#include "provenance_rewriter/prov_rewriter.h"
#include "provenance_rewriter/prov_utility.h"
#include "provenance_rewriter/pi_cs_rewrites/pi_cs_main.h"
#include "provenance_rewriter/transformation_rewrites/transformation_prov_main.h"

#include "model/query_operator/query_operator.h"
#include "model/node/nodetype.h"
#include "model/list/list.h"

/* function declarations */
static QueryOperator *findProvenanceComputations (QueryOperator *op);
static QueryOperator *rewriteProvenanceComputation (ProvenanceComputation *op);
static QueryOperator *getUpdateForPreviousTableVersion (List *updates, char *tableName, int startPos);

/* function definitions */
List *
provRewriteQueryList (List *list)
{
    FOREACH(QueryOperator,q,list)
        q_his_cell->data.ptr_value = provRewriteQuery(q);

    return list;
}


QueryOperator *
provRewriteQuery (QueryOperator *input)
{
    return findProvenanceComputations(input);
}


QueryOperator *
findProvenanceComputations (QueryOperator *op)
{
    // is provenance computation? then rewrite
    if (isA(op, ProvenanceComputation))
        rewriteProvenanceComputation((ProvenanceComputation *) op);

    // else search for children with provenance
    FOREACH(QueryOperator, x, op->inputs)
    {
        findProvenanceComputations(x);
    }

    return op;
}

QueryOperator *
rewriteProvenanceComputation (ProvenanceComputation *op)
{
    if (LIST_LENGTH(op->op.inputs) > 1)
    {
        List *updates = op->op.inputs;

        // reverse list
        reverseList(updates);

        /*
         * Merge the individual queries for all updates into one
         */
        FOREACH(QueryOperator, u, op->op.inputs)
        {
             List *children = NULL;
             int i = 0;

             // find all table access operators
             findTableAccessVisitor(u, &children);

             FOREACH(TableAccessOperator, t, children)
             {
                 QueryOperator *op = getUpdateForPreviousTableVersion(op->op.inputs, t->tableName, i);
                 switchOperators((QueryOperator *) t, op);
                 i++;
             }
        }
//        FOREACH(TableAccessOperator, t, op->op.inputs)
//          {
//             //find the first updates U' in position j with j>i that has updated table T
//             PREP_VISIT(TableAccessOperator);
//             VISIT_OPERATOR_FIELD(t);
//             if(u != NULL)
//              {
//                t = SetOperator *node;
//              }
//          }
         
        // rewrite
//        if (isA(op, ProvenanceComputation))
//            {
//              rewriteProvenanceComputation((ProvenanceComputation *) op);
//            }
       // return
    }
    switch(op->provType)
    {
        case PROV_PI_CS:
            return rewritePI_CS(op);
        case PROV_TRANSFORMATION:
            return rewriteTransformationProvenance((QueryOperator *) op);
    }
    return NULL;
}

static QueryOperator *
getUpdateForPreviousTableVersion (List *updates, char *tableName, int startPos)
{
    QueryOperator *result = NULL;

    for(ListCell *lc = getNthOfList(updates, startPos); lc != NULL; lc = lc->next)
    {
        QueryOperator *cur = LC_P_VAL(lc);

    }

    return result;
}
