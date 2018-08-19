
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "dpd.h"


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void free_commentlist(struct comment* list)
{
	struct comment* next = NULL;

	while (list)
	{
		next = list->nextline;
		free(list->line);
		free(list);
		list = next;
	}
}
void free_protocollist(struct protocol* list)
{
	struct protocol* next = NULL;
	while (list)
	{
		next = list->next;
		free_segmentlist(list->seglist);
		free_commentlist(list->notes);
		free(list->name);
		free(list);
		list = next;
	}

}
void free_segmentlist(struct segment* list)
{
	struct segment* next = NULL;
	while (list)
	{
		next = list->next;
		free_propertylist(list->properlist);
		free_commentlist(list->notes);
		free(list->name);
		free(list);
		list = next;
	}

}

void free_propertylist(struct property* list)
{
	struct property* next = NULL;
	while (list)
	{
		next = list->next;
		free( list->name );
		free( list->value );
		free( list );
		list = next;
	}

}


struct protocol *new_protocol(char* pname, struct segment* seglist, struct comment* notes, int lino)
{
	struct protocol * ret= (struct protocol*)malloc(sizeof(struct protocol));
	ret->name = pname;
	ret->lineno = lino;
	ret->next = NULL;
	ret->notes = notes;
	ret->seglist = seglist;
	return ret;
}

struct protocol *union_protocol(struct protocol* list, struct protocol* p)
{
	if (!list)
	{
		return p;
	}
	struct protocol* n = list;
	while (n->next)
	{
		n = n->next;
	}
	n->next = p;
	return list;
}

struct segment *new_segment(char* pname, enum segmenttype stype, struct property* properlist, struct comment* notes, int lino)
{
	struct segment * ret = (struct segment*)malloc(sizeof(struct segment));
	ret->lineno = lino;
	ret->name = pname;
	ret->next = NULL;
	ret->notes = notes;
	ret->properlist = properlist;
	ret->segtype = stype;
	return ret;
}

struct segment *union_segment(struct segment* list, struct segment* seg)
{
	if (!list)
	{
		return seg;
	}
	struct segment* n = list;
	while (n->next)
	{
		n = n->next;
	}
	n->next = seg;
	return list;
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