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
    PROTOCOL = 258,
    SEGMENT = 259,
    SEGMENT_TYPE_U8 = 260,
    SEGMENT_TYPE_U16 = 261,
    SEGMENT_TYPE_U32 = 262,
    SEGMENT_TYPE_I8 = 263,
    SEGMENT_TYPE_I16 = 264,
    SEGMENT_TYPE_I32 = 265,
    SEGMENT_TYPE_RI = 266,
    SEGMENT_TYPE_DOUBLE = 267,
    SEGMENT_TYPE_FLOAT = 268,
    SEGMENT_TYPE_BOOLEAN = 269,
    SEGMENT_TYPE_CRC = 270,
    SEGMENT_TYPE_ARRAY = 271,
    SEGMENT_TYPE_STRING = 272,
    SEGMENT_TYPE_BLOCK = 273,
    SEGMENT_TYPE_BUFFER = 274,
    EQUAL = 275,
    CMP = 276,
    SEGMENT_PROPERTY = 277,
    VALUE_PROPERTY = 278,
    VALUE_INT = 279,
    VALUE_FLOAT = 280,
    VALUE_STRING = 281,
    VALUE_RANGE = 282,
    DEFAULT = 283,
    SWITCH = 284,
    CASE = 285,
    IDENTIFIER = 286,
    COMMENT = 287,
    END = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "DPD.y" /* yacc.c:1910  */

	int lineno;
	char* propertyname;
	char* propertyvalue;
	char* id;
	enum segmenttype stype;
	struct comment* commentlist;
	struct protocol *protocollist;
	struct segment *segmentlist;
	struct property *propertylist;

#line 100 "DPD.tab.h" /* yacc.c:1910  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_DPD_TAB_H_INCLUDED  */
