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

#if defined (ACE_USES_CLASSIC_SVC_CONF) && (ACE_USES_CLASSIC_SVC_CONF == 1)

// Forward declarations.
struct ace_yy_buffer_state;

// The following yylex() declarations require support for reentrant
// parser generation (e.g. from GNU Bison).
#if defined (DEBUGGING)
#if defined (ACE_YY_DECL)
#undef ACE_YY_DECL
#endif /* ACE_YY_DECL */
#define ACE_YY_DECL extern "C" char *ace_yylex (ACE_YYSTYPE *ace_yylval, void *ACE_YYLEX_PARAM)
#else
#define ACE_YY_DECL extern "C" int ace_yylex (ACE_YYSTYPE *ace_yylval, void *ACE_YYLEX_PARAM)
#endif /* DEBUGGING */

extern void ace_yy_delete_buffer (ace_yy_buffer_state *buffer);

/**
 * @class ACE_Svc_Conf_Param
 *
 * @brief An instance of this object will be passed down to the
 *        yyparse() and yylex() functions.
 *
 * This class retains the state for a given parse/scan.  It primarily
 * makes it possible to hold the static object lock in the scanner
 * for as short a period of time as possible.  The resulting finer
 * grained locking prevents deadlocks from occuring when scanning a
 * `svc.conf' file and activating an ACE_Task, for example, as a
 * result of processing the directives in that file.
 */
class ACE_Svc_Conf_Param
{
public:

  enum SVC_CONF_PARAM_TYPE
    {
      /// The lexer will scan a file containing one or more directives.
      SVC_CONF_FILE,

      /// The lexer will scan a string containing a directive.
      SVC_CONF_DIRECTIVE
    };

  /// Constructor
  ACE_Svc_Conf_Param (FILE *file)
    : type (SVC_CONF_FILE),
      yyerrno (0),
      yylineno (1),
      buffer (0),
      obstack ()
  {
    source.file = file;
  }

  /// Constructor
  ACE_Svc_Conf_Param (const ACE_TCHAR *directive)
    : type (SVC_CONF_DIRECTIVE),
      yyerrno (0),
      yylineno (1),
      buffer (0),
      obstack ()
  {
    source.directive = directive;
  }

  ~ACE_Svc_Conf_Param (void)
  {
    ace_yy_delete_buffer (this->buffer);
  }

public:

  union
  {

    /// FILE stream from which directives will be scanned and parsed.
    FILE *file;

    /// String containing directive that will be scanned and parsed.
    const ACE_TCHAR *directive;

  } source;

  // Discriminant use to determine which union member to use.
  SVC_CONF_PARAM_TYPE type;

  /// Keeps track of the number of errors encountered so far.
  int yyerrno;

  /// Keeps track of the current line number for error-handling routine.
  int yylineno;

  /// Lexer buffer that corresponds to the current Service
  /// Configurator file/direct scan.
  ace_yy_buffer_state *buffer;

  /// Obstack used for efficient memory allocation when
  /// parsing/scanning a service configurator directive.
  ACE_Obstack_T<ACE_TCHAR> obstack;

};

// Parameter that is passed down to the yyparse() function, and
// eventually to yylex().
#define ACE_YYPARSE_PARAM ace_svc_conf_parameter
#define ACE_YYLEX_PARAM ACE_YYPARSE_PARAM

#define ACE_SVC_CONF_PARAM (ACE_static_cast (ACE_Svc_Conf_Param *, ACE_YYLEX_PARAM))

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

// Forward declaration
struct ace_yy_buffer_state;

/// Create and push a new lexer buffer on to the buffer stack for use
/// when scanning the given file.
void ace_yy_push_buffer (FILE *file,
                         ace_yy_buffer_state *&buffer);

/// Create and push a new lexer buffer on to the buffer stack for use
/// when scanning the given directive.
void ace_yy_push_buffer (const ACE_TCHAR *directive,
                         ace_yy_buffer_state *&buffer);

/// Pop the current lexer buffer off of the buffer stack and
/// deallocate it.
void ace_yy_pop_buffer (ace_yy_buffer_state *buf);

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
void ace_yyerror (const ACE_TCHAR *);

/// Keeps track of the current line number for error-handling routine
extern int ace_yylineno;

/// Keeps track of the number of errors encountered so far
extern int ace_yyerrno;

/// Holds the lexeme for the current token
extern ACE_TCHAR *ace_yytext;

/// Holds the length of the lexeme for the current token
extern int ace_yyleng;

#endif /* ACE_USES_CLASSIC_SVC_CONF && ACE_USES_CLASSIC_SVC_CONF == 1 */

/// Factory that creates a new ACE_Service_Type_Impl.
extern ACE_Service_Type_Impl *
ace_create_service_type (const ACE_TCHAR *,
                         int,
                         void *,
                         unsigned int,
                         ACE_Service_Object_Exterminator = 0);


#include "ace/post.h"

#endif /* ACE_SVC_CONF_H */
