/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Svc_Conf.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_SVC_CONF_H
#define ACE_SVC_CONF_H

#include "ace/pre.h"

// Globally visible macros, type decls, and extern var decls for
// Service Configurator utility.

#include "ace/Obstack.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Parse_Node.h"

// The following yylex() declarations require support for reentrant
// parser generation (e.g. from GNU Bison).
#if defined (DEBUGGING)
#if defined (ACE_YY_DECL)
#undef ACE_YY_DECL
#endif /* ACE_YY_DECL */
#define ACE_YY_DECL extern "C" char *ace_yylex (ACE_YYSTYPE *ace_yylval)
#else
#define ACE_YY_DECL extern "C" int ace_yylex (ACE_YYSTYPE *ace_yylval)
#endif /* DEBUGGING */

// The following definition for the ACE_YYSTYPE must occur before
// ACE_YY_DECL is declared since ACE_YY_DECL expands to function
// prototypes that use ACE_YYSTYPE.
typedef union
{
  int type_;
  ACE_Location_Node *location_node_;
  ACE_Parse_Node *parse_node_;
  ACE_Static_Node *static_node_;
  ACE_Service_Type *svc_record_;
  ACE_TCHAR *ident_;
} ACE_YYSTYPE;

/// Maximum depth allowed when processing nested Service Configurator
/// directives.
#ifndef ACE_SERVICE_DIRECTIVE_STACK_DEPTH
const int ACE_SERVICE_DIRECTIVE_STACK_DEPTH = 8;
#endif  /* ACE_SERVICE_DIRECTIVE_STACK_DEPTH */

/// Create and push a new lexer buffer on to the buffer stack for use
/// when scanning the given file.
void ace_yy_push_buffer (FILE *file);

/// Create and push a new lexer buffer on to the buffer stack for use
/// when scanning the given directive.
void ace_yy_push_buffer (const ACE_TCHAR *directive);

/// Pop the current lexer buffer off of the buffer stack and
/// deallocate it.
void ace_yy_pop_buffer (void);

int ace_yyparse (void);
// Performs the parsing

ACE_YY_DECL;
// Performs the lexical analysis

extern FILE *ace_yyin;
// Name of input stream

void ace_yyerror (const ACE_TCHAR *);
// Error handling routine required by YACC or BISON

extern int ace_yylineno;
// Keeps track of the current line number for error-handling routine

extern int ace_yyerrno;
// Keeps track of the number of errors encountered so far

extern ACE_TCHAR *ace_yytext;
// Holds the lexeme for the current token

extern int ace_yyleng;
// Holds the length of the lexeme for the current token

extern ACE_Obstack_T<ACE_TCHAR> *ace_obstack;
// Efficient memory allocation technique

extern ACE_Service_Type_Impl *
ace_create_service_type (const ACE_TCHAR *,
                         int,
                         void *,
                         unsigned int,
                         ACE_Service_Object_Exterminator = 0);
// Factory that creates a new ACE_Service_Type_Impl.

#include "ace/post.h"

#endif /* ACE_SVC_CONF_H */
