
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "dpd.h"


void free_protocol(struct protocol* list)
{


}


struct protocol *new_protocol(char* pname, struct segment* seglist, struct comment* notes, int lino)
{
	struct protocol * ret=NULL;
	return ret;
}

struct protocol *union_protocol(struct protocol* list, struct protocol* p)
{
	struct protocol * ret=NULL;
	return ret;
}

struct segment *new_segment(char* pname, enum segmenttype stype, struct property* seglist, struct comment* notes, int lino)
{
	struct segment* ret = NULL;
	return ret;
}

struct segment *union_segment(struct segment* list, struct segment* seg)
{
	return NULL;
}

struct property *new_property(enum valuetype vt, char* pname, char* pvalue, int lno)
{
	struct property* r = (struct property*)malloc(sizeof(struct property));
	r->vtype = vt;
	r->name = pname;
	r->value = pvalue;
	r->lineno = lno;
	r->next = NULL;
	return r;
}

//IF IDENTIFIER CMP cmpvalue THEN IDENTIFIER ELSE IDENTIFIER
struct property *new_ifproperty(char* ifid, int iflno, char* cmp, enum valuetype vtype, char* cmpvalue, int cmplino, char* thenvalue, int thenlno, char* elsevalue, int elselno)
{

	struct property* ret = new_property(v_id, _strdup("If"), ifid, iflno);
	ret = union_property(ret, new_property(vtype, cmp, cmpvalue, cmplino));
	ret = union_property(ret, new_property(v_id, _strdup("Then"), thenvalue, thenlno));
	ret = union_property(ret, new_property(v_id, _strdup("Else"), elsevalue, elselno));
	return ret;
}

//SWITCH IDENTIFIER caselist DEFAULT IDENTIFIER
struct property *new_switchproperty(char* switchseg, int switchlno, struct property * caselist, char* defaultvalue, int defaultlno)
{
	struct property* ret = new_property(v_id, _strdup("Switch"), switchseg, switchlno);
	ret = union_property(ret, caselist);
	ret = union_property(ret, new_property(v_id, _strdup("Default"), defaultvalue, defaultlno));
	return ret;
}


struct property *union_property(struct property* list, struct property* p)
{
	if (list == NULL)
	{
		return p;
	}
	struct property* n = list;
	while (n->next)
	{
		n = n->next;
	}
	n->next = p;
	return list;
}

struct comment *new_comment(char* v)
{
	struct comment* r = (struct comment*)malloc(sizeof(struct comment));
	r->line = v;
	r->nextline = NULL;
	return r;
}

struct comment *union_comment(struct comment* list, struct comment* line)
{
	if (list==NULL)
	{
		return line;
	}
	struct comment* n = list;
	while (n->nextline)
	{
		n = n->nextline;
	}
	n->nextline = line;
	return list;

}