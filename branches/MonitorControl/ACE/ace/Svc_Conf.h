// -*- C++ -*-

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

#include /**/ "ace/pre.h"

// Globally visible macros, type decls, and extern var decls for
// Service Configurator utility.

#include "ace/Obstack.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Config.h"
#include "ace/Parse_Node.h"

#include "ace/Svc_Conf_Param.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

// The following yylex() declarations require support for reentrant
// parser generation (e.g. from GNU Bison).
#define ACE_YY_DECL extern "C" int ace_yylex (ACE_YYSTYPE *ace_yylval, void *ACE_YYLEX_PARAM)

// Forward declarations
class ACE_Location_Node;
class ACE_Parse_Node;
class ACE_Static_Node;
class ACE_Service_Type_Factory;

// The following definition for the ACE_YYSTYPE must occur before
// ACE_YY_DECL is declared since ACE_YY_DECL expands to function
// prototypes that use ACE_YYSTYPE.
typedef union
{
  int type_;
  ACE_Location_Node *location_node_;
  ACE_Parse_Node *parse_node_;
  ACE_Static_Node *static_node_;
  ACE_Service_Type_Factory *svc_record_;
  ACE_TCHAR *ident_;
} ACE_YYSTYPE;

// Forward declaration
struct ace_yy_buffer_state;

/// Performs the parsing
#ifdef ACE_YYPARSE_PARAM
int ace_yyparse (void *);
#else
int ace_yyparse (void);
#endif

/// Performs the lexical analysis
ACE_YY_DECL;

/// Name of input stream
extern FILE *ace_yyin;

/// Error handling routine required by YACC or BISON
void ace_yyerror (int yyerrno, int yylineno, char const *);

/// Holds the lexeme for the current token
extern ACE_TCHAR *ace_yytext;

/// Holds the length of the lexeme for the current token
extern int ace_yyleng;

#endif /* ACE_USES_CLASSIC_SVC_CONF == 1 */

/// Factory that creates a new ACE_Service_Type_Impl.
extern ACE_Service_Type_Impl *
ace_create_service_type (const ACE_TCHAR *,
                         int,
                         void *,
                         unsigned int,
                         ACE_Service_Object_Exterminator = 0);


ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_SVC_CONF_H */
