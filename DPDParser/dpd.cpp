
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include "dpd.h"
#include "DpdParserDB.h"

DpdParserDB* g_pDb;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_pDb = new DpdParserDB();
			return true;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			delete g_pDb;
			return true;
		break;
	}
	return TRUE;
}

//启动taskid指定的分析任务，分析完成返回true，启动失败返回false
extern "C" bool __declspec(dllexport) StartParse(int taskid)
{
	bool result = false;
	if(!g_pDb->LoadTask(taskid)) return false;
	g_pDb->UpdateTaskState(TASK_STATE_RUNNING);
	switch (g_pDb->getTaskId())
	{
		case TASK_TYPE_PARSE_FILE:
			result = ParseUTF8File(g_pDb->getTaskCode());
			break;
		case TASK_TYPE_PARSE_PROTOCOLS:
			result = ParseProtocols(g_pDb->getTaskCode());
			break;
		case TASK_TYPE_PARSE_SEGMENTS:
			result = ParseSegments(g_pDb->getTaskCode());
			break;
		default:
			result = false;
	}
	if (result) result = ParseSemantics();
	g_pDb->UpdateTaskState(TASK_STATE_END);

	return result;
}

//记录协议分析结果
void SaveProtocolList(struct protocol * proto)
{
	while (proto)
	{
		int pid = g_pDb->SaveProtocol(proto);
		SaveSegmentList(proto->seglist, pid);
		proto = proto->next;
	}
}

//记录字段分析结果
void  SaveSegmentList(struct segment * seg, int protocolid)
{
	while (seg)
	{
		int segid = g_pDb->SaveSegment(seg, protocolid);
		struct property * proper = seg->properlist;
		while (proper)
		{
			g_pDb->SaveProperty(proper, segid);
			proper = proper->next;
		}
		seg = seg->next;
	}
}

//保存符号 返回符号id
int SaveSymbol(const char* symbol, int lineno, int firstcol, int lastcol)
{
	return g_pDb->SaveSymbol(symbol, lineno, firstcol, lastcol);
}


//语义分析
bool  ParseSemantics()
{
	return false;
}

//记录错误信息
void OutError(int errcode, int firstsymbol, int endsymbol)
{
	g_pDb->SaveError(errcode, firstsymbol, endsymbol);
}

#pragma region --For ast Struct--

void free_commentlist(struct comment* list)
{
	struct comment* next = NULL;

	while (list)
	{
		next = list->nextline;
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
		free(list);
		list = next;
	}

}


struct protocol *new_protocol(int name, struct segment* seglist, struct comment* notes)
{
	struct protocol * ret = (struct protocol*)malloc(sizeof(struct protocol));
	ret->name = name;
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

struct segment *new_segment(int name, enum segmenttype stype, struct property* properlist, struct comment* notes)
{
	struct segment * ret = (struct segment*)malloc(sizeof(struct segment));
	ret->name = name;
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

struct property *new_property(int name, enum valuetype vt, int value)
{
	struct property* r = (struct property*)malloc(sizeof(struct property));
	r->vtype = vt;
	r->name = name;
	r->value = value;
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


struct property *new_ifproperty(int ifid, int ifsegname, int cmp, enum valuetype vtype, int cmpvalue, int thenid, int thenproto, int elseid, int elseproto)
{
	struct property* ret = new_property(ifid,v_id,  ifsegname);
	ret = union_property(ret, new_property(cmp, vtype, cmpvalue));
	ret = union_property(ret, new_property(thenid,v_id, thenproto));
	ret = union_property(ret, new_property(elseid,v_id, elseproto));
	return ret;
}

struct property *new_switchproperty(int switchid, int switchsegment, struct property * caselist, int defaultid, int defaultproto)
{
	struct property* ret = new_property(switchid, v_id, switchsegment);
	ret = union_property(ret, caselist);
	ret = union_property(ret, new_property(defaultid, v_id, defaultproto));
	return ret;
}

struct comment *new_comment(int line)
{
	struct comment* r = (struct comment*)malloc(sizeof(struct comment));
	r->line = line;
	r->nextline = NULL;
	return r;
}

struct comment *union_comment(struct comment* list, struct comment* line)
{
	if (list == NULL)
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
#pragma endregion

#pragma region --Helper--

unsigned char utf8_look_for_table[] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};



#define UTFLEN(x)  utf8_look_for_table[(x)]


//计算str字符数目
int GetUtf8Length(char *str, int clen)
{
	int len = 0;
	for (char *ptr = str;
		*ptr != 0 && len < clen;
		len++, ptr += UTFLEN((unsigned char)*ptr));
	return len;
}



//get子串
char* SubUtfString(char *str, unsigned int start, unsigned int end)
{
	unsigned int len = GetUtf8Length(str, (int)strlen(str));
	if (start >= len) return NULL;
	if (end > len) end = len;
	char *sptr = str;
	for (unsigned int i = 0; i < start; ++i, sptr += UTFLEN((unsigned char)*sptr));
	char *eptr = sptr;
	for (unsigned int i = start; i < end; ++i, eptr += UTFLEN((unsigned char)*eptr));
	int retLen = (int)(eptr - sptr);
	char *retStr = (char*)malloc(retLen + 1);
	memcpy(retStr, sptr, retLen);
	retStr[retLen] = 0;
	return retStr;
}

#pragma endregion

