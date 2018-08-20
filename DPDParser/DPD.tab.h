/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_DPD_TAB_H_INCLUDED
# define YY_YY_DPD_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PARSEPROTOCOL = 258,
    PARSESEGMENT = 259,
    PROTOCOL = 260,
    SEGMENT = 261,
    END = 262,
    ENDALL = 263,
    SEGMENT_TYPE_U8 = 264,
    SEGMENT_TYPE_U16 = 265,
    SEGMENT_TYPE_U32 = 266,
    SEGMENT_TYPE_I8 = 267,
    SEGMENT_TYPE_I16 = 268,
    SEGMENT_TYPE_I32 = 269,
    SEGMENT_TYPE_IR = 270,
    SEGMENT_TYPE_UR = 271,
    SEGMENT_TYPE_FLOAT = 272,
    SEGMENT_TYPE_DOUBLE = 273,
    SEGMENT_TYPE_BOOLEAN = 274,
    SEGMENT_TYPE_CRC = 275,
    SEGMENT_TYPE_ARRAY = 276,
    SEGMENT_TYPE_STRING = 277,
    SEGMENT_TYPE_BLOCK = 278,
    SEGMENT_TYPE_BUFFER = 279,
    EQUAL = 280,
    SWITCH = 281,
    CASE = 282,
    IF = 283,
    THEN = 284,
    ELSE = 285,
    COMMENT = 286,
    DEFAULT = 287,
    CMP = 288,
    IDENTIFIER = 289,
    SEGMENT_PROPERTY = 290,
    VALUE_PROPERTY = 291,
    VALUE_INT = 292,
    VALUE_FLOAT = 293,
    VALUE_STRING = 294,
    VALUE_RANGE = 295,
    VALUE_BOOL = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "DPD.y" /* yacc.c:1910  */

	char* s;
	enum segmenttype segtype;
	struct comment * commentlist;
	struct protocol * protocollist;
	struct segment * segmentlist;
	struct property * propertylist;

#line 105 "DPD.tab.h" /* yacc.c:1910  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_DPD_TAB_H_INCLUDED  */
