%locations

%{
#include <stdlib.h>
#include "stdafx.h"
#include "dpd.h"
#include "flex.h"

void yyerror(const char *s);
bool isParseSegment = false;

%}

%union {
	int symbol;
	enum segmenttype segtype;
	struct comment * commentlist;
	struct protocol * protocollist;
	struct segment * segmentlist;
	struct property * propertylist;
}

%token PARSEPROTOCOL PARSESEGMENT
%token PROTOCOL SEGMENT END ENDALL
%token SEGMENT_TYPE_U8 SEGMENT_TYPE_U16 SEGMENT_TYPE_U32 SEGMENT_TYPE_I8 SEGMENT_TYPE_I16 SEGMENT_TYPE_I32 SEGMENT_TYPE_IR SEGMENT_TYPE_UR
%token SEGMENT_TYPE_FLOAT SEGMENT_TYPE_DOUBLE SEGMENT_TYPE_BOOLEAN SEGMENT_TYPE_CRC SEGMENT_TYPE_ARRAY SEGMENT_TYPE_STRING SEGMENT_TYPE_BLOCK SEGMENT_TYPE_BUFFER
%token EQUAL

%token <symbol> SWITCH CASE IF THEN ELSE DEFAULT CMP COMMENT IDENTIFIER SEGMENT_PROPERTY 
%token <symbol> VALUE_PROPERTY VALUE_INT VALUE_FLOAT VALUE_STRING VALUE_RANGE VALUE_BOOL 

%type <segtype>			segment_type
%type <commentlist>		commentlist
%type <protocollist>	protocol protocollist
%type <segmentlist>		segment segmentlist
%type <propertylist>	property propertylist ifbranch switchbranch caseitem caselist

%start firstparse

%destructor { free_commentlist($$); $$=NULL;} <commentlist>
%destructor { free_protocollist($$); $$=NULL;} <protocollist>
%destructor { free_segmentlist($$); $$=NULL;} <segmentlist>
%destructor { free_propertylist($$); $$=NULL;} <propertylist>

%%

firstparse: PARSEPROTOCOL protocollist
	| PARSESEGMENT segmentlist
;

protocollist:											{ $$ = NULL; }
	| protocollist error protocol						{ $$ = union_protocol($1, $3); }
	| protocollist protocol								{ $$ = union_protocol($1, $2); }
	| protocollist commentlist ENDALL					{ SaveProtocolList($1); free_protocollist($1); return 0; }
;

protocol:  
	  commentlist PROTOCOL IDENTIFIER segmentlist END	{ $$ = new_protocol($3, $4, $1); }
	| commentlist PROTOCOL IDENTIFIER error END			{ $$ = new_protocol($3, NULL, $1); }
;

commentlist:											{ $$ = NULL; }
	| commentlist COMMENT								{ $$ = union_comment($1, new_comment($2)); }
;

segmentlist:											{ $$ = NULL; }
	| segmentlist segment								{ $$ = union_segment($1, $2); }
	| segmentlist commentlist ENDALL					{ if(isParseSegment) { SaveSegmentList($1, -1); free_segmentlist($1); return 0; } }
;

segment: 
	commentlist SEGMENT IDENTIFIER segment_type propertylist	{ $$ = new_segment($3, $4, $5, $1); }
	| commentlist SEGMENT IDENTIFIER ifbranch					{ $$ = new_segment($3, DPDIfElse, $4, $1); }
	| commentlist SEGMENT IDENTIFIER switchbranch				{ $$ = new_segment($3, DPDSwitch, $4, $1); }
;

propertylist:													{ $$ = NULL; }
	| propertylist error property								{ $$ = union_property($1, $3); }
	| propertylist property										{ $$ = union_property($1, $2); }
;

ifbranch:
	IF IDENTIFIER CMP VALUE_INT THEN IDENTIFIER ELSE IDENTIFIER		{ $$ = new_ifproperty($1, $2, $3, v_int, $4, $5, $6, $7, $8); }
	|IF IDENTIFIER CMP VALUE_STRING THEN IDENTIFIER ELSE IDENTIFIER	{ $$ = new_ifproperty($1, $2, $3, v_str, $4, $5, $6, $7, $8); }
;

switchbranch:
	SWITCH IDENTIFIER caselist DEFAULT IDENTIFIER				{ $$ = new_switchproperty($1, $2, $3, $4, $5); }
;

caselist:
	caseitem													{ $$ = $1; }
	| caselist caseitem											{ $$ = union_property($1, $2); }
;

caseitem:
	CASE VALUE_INT IDENTIFIER									{ $$ = new_property($2, v_caseint, $3); }
	| CASE VALUE_STRING IDENTIFIER								{ $$ = new_property($2, v_casestr, $3); }
;


segment_type: SEGMENT_TYPE_U8	{ $$ = DPDUInt8; }
	| SEGMENT_TYPE_U16			{ $$ = DPDUInt16; }
	| SEGMENT_TYPE_U32			{ $$ = DPDUInt32; }
	| SEGMENT_TYPE_I8			{ $$ = DPDInt8; }
	| SEGMENT_TYPE_I16			{ $$ = DPDInt16; }
	| SEGMENT_TYPE_I32			{ $$ = DPDInt32; }
	| SEGMENT_TYPE_IR			{ $$ = DPDIntRandom; }
	| SEGMENT_TYPE_UR			{ $$ = DPDUIntRandom; }
	| SEGMENT_TYPE_DOUBLE		{ $$ = DPDDouble; }
	| SEGMENT_TYPE_FLOAT		{ $$ = DPDFloat; }
	| SEGMENT_TYPE_BOOLEAN		{ $$ = DPDBoolean; }
	| SEGMENT_TYPE_CRC			{ $$ = DPDCRC; }
	| SEGMENT_TYPE_ARRAY		{ $$ = DPDArray; }
	| SEGMENT_TYPE_STRING		{ $$ = DPDString; }
	| SEGMENT_TYPE_BLOCK		{ $$ = DPDBlock; }
	| SEGMENT_TYPE_BUFFER		{ $$ = DPDBuffer; }
;

property: SEGMENT_PROPERTY EQUAL VALUE_PROPERTY		{ $$ = new_property($1, v_property, $3); }
	| SEGMENT_PROPERTY EQUAL VALUE_INT				{ $$ = new_property($1, v_int, $3); }
	| SEGMENT_PROPERTY EQUAL VALUE_BOOL				{ $$ = new_property($1, v_bool, $3); }
	| SEGMENT_PROPERTY EQUAL VALUE_FLOAT			{ $$ = new_property($1, v_float, $3); }
	| SEGMENT_PROPERTY EQUAL VALUE_STRING			{ $$ = new_property($1, v_str, $3); }
	| SEGMENT_PROPERTY EQUAL VALUE_RANGE			{ $$ = new_property($1, v_range, $3); }
	| SEGMENT_PROPERTY EQUAL IDENTIFIER				{ $$ = new_property($1, v_range, $3); }
	| DEFAULT EQUAL VALUE_INT						{ $$ = new_property($1, v_int, $3); }
	| DEFAULT EQUAL VALUE_FLOAT						{ $$ = new_property($1, v_float, $3); }
	| DEFAULT EQUAL VALUE_STRING					{ $$ = new_property($1, v_str, $3); }
	| DEFAULT EQUAL VALUE_BOOL						{ $$ = new_property($1, v_bool, $3); }
;


%%

int first_tok = 0;

bool ParseUTF8File(char* filename)
{
	first_tok = PARSEPROTOCOL;
	isParseSegment = false;
	FILE* fs=0;
	errno_t err;

	err = fopen_s(&fs, filename, "r");
	if (err != 0)
	{
		return false;
	}
	else
	{
		yyin = fs;
		do { 
				yyparse();
			} while(!feof(yyin));
	}
	if(fs) fclose (fs);
	return true;
}

bool ParseProtocols(char* code)
{
	first_tok = PARSEPROTOCOL;
	isParseSegment = false;
	YY_BUFFER_STATE bp = yy_scan_bytes(code,(int)strlen(code));
	yy_switch_to_buffer(bp);
	yyparse();
	yy_delete_buffer(bp);
	return true;
}

bool ParseSegments(char* code)
{
	first_tok = PARSESEGMENT;
	isParseSegment = true;
	YY_BUFFER_STATE bp = yy_scan_bytes(code,(int)strlen(code));
	yy_switch_to_buffer(bp);
	yyparse();
	yy_delete_buffer(bp);
	return false;
}

void yyerror(char const * s) {
	OutError(ERROR_CODE_SYNTAX, yylval.symbol, yylval.symbol);
}

