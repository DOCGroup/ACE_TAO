/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Svc_Conf.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SVC_CONF_H)
#define ACE_SVC_CONF_H

// Globally visible macros, type decls, and extern var decls for
// Service Configurator utility.

#include "ace/Obstack.h"
#include "ace/Service_Config.h"
#include "ace/Parse_Node.h"
   
#if defined (DEBUGGING)
#if defined (YY_DECL)
#undef YY_DECL
#endif /* YY_DECL */
#define YY_DECL extern "C" char *ace_yylex (void)
#else
#define YY_DECL extern "C" int ace_yylex (void)
#endif /* DEBUGGING */

void ace_yyrestart (FILE *);
// Restart input file parsing 

int ace_yyparse (void);
// Performs the parsing 

YY_DECL;
// Performs the lexical analysis 

extern FILE *ace_yyin;
// Name of input stream 

void ace_yyerror (char *);
// Error handling routine required by YACC or BISON 

extern int ace_yylineno;
// Keeps track of the current line number for error-handling routine 

extern int ace_yyerrno;
// Keeps track of the number of errors encountered so far 

extern char *ace_yytext;
// Holds the lexeme for the current token 

extern int ace_yyleng;
// Holds the length of the lexeme for the current token 

extern ACE_Obstack *ace_obstack;
// Efficient memory allocation technique 

ACE_Service_Type_Impl *ace_create_service_type (const char *, int, const void *, unsigned int);
// Factory that creates a new ACE_Service_Type_Impl. 

typedef union 
{
  int type_;
  ACE_Location_Node *location_node_;
  ACE_Parse_Node *parse_node_;
  ACE_Static_Node *static_node_;
  ACE_Service_Type *svc_record_;
  char *ident_; 
} YYSTYPE;
extern YYSTYPE ace_yylval;
#endif /* ACE_SVC_CONF_H */

