
%option noyywrap yylineno HEADER-FILE="flex.h" 


EXP ([Ee][-+]?[0-9]+)
UCN (\\u[0-9a-fA-F]{4}|\\U[0-9a-fA-F]{8})
CHS (\xE9\xBE[\x80-\xA5]|(\xE4[\xB8-\xBF]|\xE9[\x80-\xBD]|[\xE5-\xE8][\x80-\xBF])[\x80-\xBF])

%{
#include "stdafx.h"
#include "dpd.h"
#include "DPD.tab.h"

int yycolumn = 1;
int utf8len = 0;

#define YY_USER_ACTION utf8len = GetUtf8Length(yytext, yyleng); yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + utf8len - 1; \
	yycolumn += utf8len;
%}
%%

%{
	extern int first_tok;

	if(first_tok!=0) {
		int holdtok = first_tok;
		first_tok = 0;
		return holdtok;
	}
%}

^("#"|([ \t]+"#")).*\n	{ yycolumn = 1; yylval.s = _strdup(yytext); return COMMENT; }
"#".*\n					{ yycolumn = 1; }


\'([^'\\]|\\['"?\\abfnrtv]|\\[0-7]{1,3}|\\[xX][0-9a-fA-F]+|{UCN}|{CHS})*\'	{ yylval.s = _strdup(yytext); return VALUE_STRING; }

"==" |
"!=" |
">=" |
"<=" |
">" |
"<"						{ yylval.s = _strdup(yytext); return CMP; }
"="						{ return EQUAL; }

"Protocol"				{ return PROTOCOL; }
"Segment"				{ return SEGMENT; }


"StandardUInt8"			{ return SEGMENT_TYPE_U8; }
"StandardUInt16"		{ return SEGMENT_TYPE_U16; }
"StandardUInt32"		{ return SEGMENT_TYPE_U32; }
"StandardInt8"			{ return SEGMENT_TYPE_I8; }
"StandardInt16"			{ return SEGMENT_TYPE_I16; }
"StandardInt32"			{ return SEGMENT_TYPE_I32; }
"RandomInt"				{ return SEGMENT_TYPE_IR; }
"RandomUInt"			{ return SEGMENT_TYPE_UR; }
"StandardDouble"		{ return SEGMENT_TYPE_DOUBLE; }
"StandardFloat"			{ return SEGMENT_TYPE_FLOAT; }
"Boolean"				{ return SEGMENT_TYPE_BOOLEAN; }
"CRC"					{ return SEGMENT_TYPE_CRC; }
"Array"					{ return SEGMENT_TYPE_ARRAY; }
"String"				{ return SEGMENT_TYPE_STRING; }
"Block"					{ return SEGMENT_TYPE_BLOCK; }
"Buffer"				{ return SEGMENT_TYPE_BUFFER; }

"ByteOrder" |
"Format" |
"Tail" |
"Code" |
"Count" |
"Child" |
"Algorithm" |
"BitCount" |
"Child" |
"Length" |
"Range"					{ yylval.s = _strdup(yytext); return SEGMENT_PROPERTY; }

"True" |
"False"					{ yylval.s = _strdup(yytext); return VALUE_BOOL; }

"Small" |
"Big" |
"Primitive" |
"Inversion" |
"Complement"			{ yylval.s = _strdup(yytext); return VALUE_PROPERTY; }



"Switch"				{ return SWITCH; }
"Case"					{ return CASE; }
"Default"				{ yylval.s = _strdup(yytext); return DEFAULT; }
"If"					{ return IF; }
"Then"					{ return THEN; }
"Else"					{ return ELSE; }

"End"					{ return END; }

0[0-7]* |
[1-9][0-9]* |
0[Xx][0-9a-fA-F]+		{  yylval.s = _strdup(yytext); return VALUE_INT; }

-?([0-9]*\.[0-9]+|[0-9]+\.){EXP}? |
[0-9]+{EXP}				{  yylval.s = _strdup(yytext); return VALUE_FLOAT; }

"("[ \t]*(0|[1-9][0-9]*)[ \t]*,[ \t]*(0|[1-9][0-9]*)[ \t]*")"	{  yylval.s = _strdup(yytext); return VALUE_RANGE; }

([a-zA-Z_]|{CHS})([a-zA-Z0-9_]|{CHS})*	{ yylval.s = _strdup(yytext); return IDENTIFIER; }

\r\n					{ yycolumn=1; }
\n						{ yycolumn=1; }
[ \t]					{ ; }

[\xEF\xBB\xBF]			{ yycolumn=1; }
<<EOF>>					{ return ENDALL; }
.						{ OutError(ERROR_CODE_SYMBOL, yylineno, yytext); }
%%
