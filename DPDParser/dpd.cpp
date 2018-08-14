
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "dpd.h"


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