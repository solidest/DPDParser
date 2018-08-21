#pragma once

#define TASK_TYPE_PARSE_FILE		1	//分析文件
#define TASK_TYPE_PARSE_PROTOCOLS	2	//分析协议字符串
#define TASK_TYPE_PARSE_SEGMENTS	3	//分析字段字符串

#define TASK_STATE_READY			1	//任务已生成未启动
#define TASK_STATE_RUNNING			2	//任务执行中
#define TASK_STATE_END				3	//任务完成

#define ERROR_CODE_SYMBOL			-1	//词法错误
#define ERROR_CODE_SYNTAX			-2	//语法错误



void OutError(int errcode, int firstsymbol, int endsymbol);
bool ParseUTF8File(char* filename);
bool ParseProtocols(char* code);
bool ParseSegments(char* code);
bool ParseSemantics();
void SaveProtocolList(struct protocol * protolist);
void SaveSegmentList(struct segment * seglist, int protoid);
int SaveSymbol(const char* symbol, int lineno, int firstcol, int lastcol);
int GetUtf8Length(char *str, int clen);


struct comment *new_comment(int line);
struct comment *union_comment(struct comment* list, struct comment* line);

struct protocol *new_protocol(int name, struct segment* seglist, struct comment* notes);
struct protocol *union_protocol(struct protocol* list, struct protocol* p);

struct segment *new_segment(int name, enum segmenttype stype, struct property* properlist, struct comment* notes);
struct segment *union_segment(struct segment* list, struct segment* seg);

struct property *new_switchproperty(int switchid, int switchsegment, struct property * caselist, int defaultid, int defaultproto);
struct property *new_ifproperty(int ifid, int ifsegname, int cmp, enum valuetype vtype, int cmpvalue, int thenid, int thenproto, int elseid, int elseproto);
struct property *new_property(int name, enum valuetype vt, int value);
struct property *union_property(struct property* list, struct property* p);

void free_commentlist(struct comment* list);
void free_protocollist(struct protocol* list);
void free_segmentlist(struct segment* list);
void free_propertylist(struct property* list);

#pragma region --Define ast Structs--

struct comment {
	int line;
	struct comment* nextline;
};

struct protocol {
	int name;
	struct comment  *notes;
	struct segment *seglist;
	struct protocol *next;
};

struct segment {
	int name;
	enum segmenttype segtype;
	struct comment  *notes;
	struct property *properlist;
	struct segment *next;
};

struct property {
	int name;
	int value;
	enum valuetype vtype;
	struct property *next;
};


enum valuetype {
	v_int = 1,
	v_float,
	v_str,
	v_range,
	v_property,
	v_id,
	v_bool,
	v_caseint,
	v_casestr
};

enum segmenttype {
	DPDUInt8 = 1,
	DPDUInt16,
	DPDUInt32,
	DPDInt8,
	DPDInt16,
	DPDInt32,
	DPDIntRandom,
	DPDUIntRandom,
	DPDDouble,
	DPDFloat,
	DPDBoolean,
	DPDCRC,
	DPDArray,
	DPDString,
	DPDBlock,
	DPDBuffer,
	DPDSwitch,
	DPDIfElse
};

#pragma endregion



