/*-----------------------------------------------------------------------------
 *
 * set.c
 *			  
 *		
 *		AUTHOR: lord_pretzel
 *
 *		
 *
 *-----------------------------------------------------------------------------
 */

#include "log/logger.h"
#include "mem_manager/mem_mgr.h"
#include "model/node/nodetype.h"
#include "model/set/set.h"
#include "uthash.h"

static SetElem *getSetElem(Set *set, void *key);

static boolean intCmp (void *a, void *b);
static void *intCpy (void *a);

static boolean ptrCmp (void *a, void *b);
static void *ptrCpy (void *a);

static boolean strCmp (void *a, void *b);
static void *strCpy (void *a);

#define CREATE_SAME_TYPE_SET(s) newSet(s->setType, s->typelen, s->eq, s->cpy)


Set *
newSet(SetType set, int typelen, boolean (*eq) (void *, void *), void * (*cpy) (void *))
{
    Set *result = makeNode(Set);

    result->setType = set;
    result->elem = NULL;
    result->typelen = typelen;

    if (set == SET_TYPE_INT)
    {
        result->eq = intCmp;
        result->cpy = intCpy;
    }
    else if (set == SET_TYPE_POINTER)
    {
        result->eq = ptrCmp;
        result->cpy = ptrCpy;
    }
    else if (set == SET_TYPE_STRING)
    {
        result->eq = strCmp;
        result->cpy = strCpy;
    }
    else
    {
        result->eq = eq;
        result->cpy = cpy;
    }

    return result;
}

Set *
makeSet(SetType set, int typelen, boolean (*eq) (void *, void *), void * (*cpy) (void *), ...)
{
    Set *result = newSet(set, typelen, eq, cpy);
    va_list args;
    void * arg;

    va_start(args, cpy);

    while((arg = va_arg(args,void *)) != NULL)
        addToSet(result, arg);

    va_end(args);

    return result;
}

Set *
makeSetInt(int elem, ...)
{
    Set *result = INTSET();
    va_list args;
    int arg;

    va_start(args, elem);

    for(arg = elem; arg >= 0; arg = va_arg(args,int))
    {
        INFO_LOG("add int %d to set", arg);
        addIntToSet(result, arg);
    }

    va_end(args);

    return result;
}


boolean
hasSetElem (Set *set, void *_el)
{
    return getSetElem(set, _el) != NULL;
}

static SetElem *
getSetElem(Set *set, void *key)
{
    SetElem *result, *s;

    if (set->setType == SET_TYPE_POINTER)
        HASH_FIND_PTR(set->elem, &key, result);
    else if (set->setType == SET_TYPE_STRING)
        HASH_FIND_STR(set->elem, key, result);
    else
        HASH_FIND_CMP(hh,set->elem, key, set->typelen, result, set->eq);

    for(s=set->elem; s != NULL; s=s->hh.next) {
        INFO_LOG("key and value %d with hv %u keyptr %p", s->data, s->hh.hashv, s->hh.key);
    }

    return result;
}

boolean
hasSetIntElem (Set *set, int _el)
{
    SetElem *result = NULL, *s;

    HASH_FIND(hh,set->elem, &_el, sizeof(int), result);

    for(s=set->elem; s != NULL; s=s->hh.next) {
        INFO_LOG("key and value %d with hv %u keyptr %d", *((int *) s->data), s->hh.hashv, *((int *) s->hh.key));
    }

    return result != NULL;
}

boolean
addToSet (Set *set, void *elem)
{
    SetElem *setEl;

    if (hasSetElem(set, elem))
        return FALSE;

    setEl = NEW(SetElem);
    setEl->data = elem;

    if (set->setType == SET_TYPE_POINTER)
        HASH_ADD_PTR(set->elem, data, setEl);
    else if (set->setType == SET_TYPE_STRING)
        HASH_ADD_KEYPTR(hh, set->elem, setEl->data, strlen((char *) elem), setEl);
    else
        HASH_ADD_KEYPTR(hh, set->elem, setEl->data, set->typelen, setEl);

    return TRUE;
}

boolean
addIntToSet (Set *set, int elem)
{
    SetElem *setEl;

    if (hasSetIntElem(set, elem))
        return FALSE;

    setEl = NEW(SetElem);
    setEl->data = NEW(int);
    *((int *) setEl->data) = elem;

    HASH_ADD_KEYPTR(hh, set->elem, setEl->data, sizeof(int), setEl);

    return TRUE;
}

void
removeSetElem (Set *set, void *elem)
{
    SetElem *e;

    e = getSetElem(set,elem);

    if (e != NULL)
    {
        HASH_DEL(set->elem, e);
        if (set->setType == SET_TYPE_NODE) // && set->type != T_IntSet)
            deepFree(e->data);
        else if (set->setType == SET_TYPE_STRING)
            FREE(e->data);

        FREE(e);
    }
}

void
removeSetIntElem (Set *set, int elem)
{
    SetElem *e;

    HASH_FIND(hh, set->elem, &elem, sizeof(int), e);
    if (e != NULL)
    {
        HASH_DEL(set->elem, e);
        FREE(e);
    }
}

Set *
unionSets (Set *left, Set *right)
{
    Set *result;
    SetElem *s;

    assert(left->setType == right->setType);
    assert(left->cpy && right->cpy);

    result = CREATE_SAME_TYPE_SET(left);

    for(s = left->elem; s != NULL; s = s->hh.next)
        addToSet(result, left->cpy(s->data));
    for(s = right->elem; s != NULL; s = s->hh.next)
        if (!hasSetElem(result, s->data))
            addToSet(result, right->cpy(s->data));

    return result;
}

Set *
intersectSets (Set *left, Set *right)
{
    Set *result;
    SetElem *s;

    assert(left->setType == right->setType);
    assert(left->cpy && right->cpy);

    result = CREATE_SAME_TYPE_SET(left);

    for(s = left->elem; s != NULL; s = s->hh.next)
        if (hasSetElem(right, s->data))
            addToSet(result, left->cpy(s->data));

    return result;
}

int
setSize (Set *set)
{
    if (set == NULL)
        return 0;
    return HASH_COUNT(set->elem);
}

static boolean
intCmp (void *a, void *b)
{
    int c = *((int *) a);
    int d = *((int *) b);

    return c == d;
}

static void *
intCpy (void *a)
{
    int *result = NEW(int);
    *result = *((int *) a);

    return (void *) result;
}

static boolean
ptrCmp (void *a, void *b)
{
    return a == b;
}

static void *
ptrCpy (void *a)
{
    return a;
}

static boolean
strCmp (void *a, void *b)
{
    return strcmp(a,b) == 0;
}

static void *
strCpy (void *a)
{
    char *in = (char *) a;

    return strdup(in);
}