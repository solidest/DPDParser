#pragma once

#define TASK_TYPE_PARSE_FILE		1	//分析文件
#define TASK_TYPE_PARSE_PROTOCOLS	2	//分析协议字符串
#define TASK_TYPE_PARSE_SEGMENTS	3	//分析字段字符串

#define TASK_STATE_NOTSTART			1	//任务未启动
#define TASK_STATE_RUNNING			2	//任务执行中
#define TASK_STATE_END				3	//任务完成

#define ERROR_CODE_SYMBOL			-1	//词法错误
#define ERROR_CODE_SYNTAX			-2	//语法错误



void OutError(int errcode, int lineno, const char *s);
bool ParseUTF8File(char* filename);
bool ParseProtocols(char* code);
bool ParseSegments(char* code);
bool ParseSemantics();
void SaveProtocolList(struct protocol * protolist);
void SaveSegmentList(struct segment * seglist, int protoid);
int GetUtf8Length(char *str, int clen);

struct comment *new_comment(char* v);
struct comment *union_comment(struct comment* list, struct comment* line);

struct protocol *new_protocol(char* pname, struct segment* seglist, struct comment* notes, int lino);
struct protocol *union_protocol(struct protocol* list, struct protocol* p);

struct segment *new_segment(char* pname, enum segmenttype stype, struct property* properlist, struct comment* notes, int lino);
struct segment *union_segment(struct segment* list, struct segment* seg);

struct property *new_switchproperty(char* switchseg, int switchlno, struct property * caselist, char* defaultvalue, int defaultlno);
struct property *new_ifproperty(char* ifid, int iflno, char* cmp, enum valuetype vtype, char* cmpvalue, int cmplino, char* thenvalue, int thenlno, char* elsevalue, int elselno);
struct property *new_property(enum valuetype vt, char* pname, char* pvalue, int lno);
struct property *union_property(struct property* list, struct property* p);

void free_commentlist(struct comment* list);
void free_protocollist(struct protocol* list);
void free_segmentlist(struct segment* list);
void free_propertylist(struct property* list);

#pragma region --Define ast Structs--

struct comment {
	char* line;
	struct comment* nextline;
};

struct protocol {
	int lineno;
	char *name;
	struct comment  *notes;
	struct protocol *next;
	struct segment *seglist;
};

struct segment {
	enum segmenttype segtype;
	int lineno;
	char *name;
	struct comment  *notes;
	struct property *properlist;
	struct segment *next;
};

struct property {
	enum valuetype vtype;
	char *name;
	char *value;
	int lineno;
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



