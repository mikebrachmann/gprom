#ifndef NODETYPE_H
#define NODETYPE_H

#include "common.h"

typedef enum NodeTag {
    T_Invalid=0,

    /*tags for value nodes*/
    T_Value = 200,
    T_Integer,
    T_Float,
    T_String,
    T_BitString,
    T_Null,
    
    /* lists */
    T_List,
    T_IntList,

    /* expression nodes */
    T_Constant,
    T_AttributeReference,
    T_FunctionCall,
    T_Operator,

    /* query block model nodes */
    T_QueryBlock,
    T_SelectItem,
    T_FromItem,
    T_Distinct
} NodeTag;

typedef struct Node{
    NodeTag type;
} Node;


/*stringinfo provides the string data type*/

typedef struct StringInfoData
{
    char *data;
    int  len;
    int maxlen;
    int cursor;

}StringInfoData;

typedef StringInfoData *StringInfo;


/*------------------------------------------------------------------
*makeStringInfo
*The function is create an empty StringInfoData and return a pointer.
*-------------------------------------------------------------------*/
extern StringInfo makeStringInfo(void);

/*------------------------------------------------------------------
*initStringInfo
*The function is init a StringInfoData.
*-------------------------------------------------------------------*/
extern void initStringInfo(StringInfo str);

/*------------------------------------------------------------------
*resetStringInfo
*The function is clear the current content of StringInfo.
*-------------------------------------------------------------------*/
extern void resetStringInfo(StringInfo str);

/*------------------------------------------------------------------
*appendStringInfoString
*The function is append a string to str.
*-------------------------------------------------------------------*/
extern void appendStringInfoString(StringInfo str, const char *s);

/*------------------------------------------------------------------
*appendStringInfoChar
*The function is append byte to str.
*-------------------------------------------------------------------*/
extern void appendStringInfoChar(StringInfo str, char ch);

/*------------------------------------------------------------------
*appendBinaryStringInfo
*The function is append binary data to a StringInfo.
*-------------------------------------------------------------------*/
extern void appendBinaryStringInfo(StringInfo str, const char *data, int datalen);

/*------------------------------------------------------------------
*enlargeStringInfo
*The function is StringInfo's buffer can hold the "needed" bytes.
*-------------------------------------------------------------------*/
extern void enlargeStringInfo(StringInfo str, int needed);

#define nodeTag(nodeptr) (((Node*)(nodeptr))->type)
#define newNode(size, tag)
#define makeNode()  ((type*)newNode(sizeof(type),T_type)
#define nodeSetTag(nodeptr,t) (((Node*)(nodeptr))->type = (t))
#define isA(nodeptr, type)  (nodeTag(nodeptr)) == T_type)

/*extern declaration */
extern char *nodeToString(void *obj);
extern void outNode(/* TODO */ void *obj);

/*readfun.c*/
extern void *stringToNode(char *str);

/*copyfun.c*/
extern void *copyObject(void *obj);

/*equalfun.c*/
extern boolean equal(void *a, void *b);


#endif /*NODETYPE_H*/
