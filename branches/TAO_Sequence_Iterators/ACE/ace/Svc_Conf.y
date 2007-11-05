%{
// $Id$

#include "ace/Svc_Conf.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

#include "ace/ARGV.h"
#include "ace/Module.h"
#include "ace/Stream.h"
#include "ace/Service_Types.h"
#include "ace/OS_NS_string.h"


#include "ace/ace_wchar.h"

ACE_RCSID (ace,
           Svc_Conf_y,
           "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Prototypes.

static ACE_Module_Type *ace_get_module (ACE_Service_Type const * sr,
                                        ACE_Service_Type const * sv,
                                        int & ace_yyerrno);

static ACE_Module_Type *ace_get_module (ACE_Service_Type const * sr,
                                        ACE_TCHAR const * svc_name,
                                        int & ace_yyerrno);

#define YYDEBUG_LEXER_TEXT (yytext[yyleng] = '\0', yytext)

// Force the pretty debugging code to compile.
// #define YYDEBUG 1

ACE_END_VERSIONED_NAMESPACE_DECL

%}

%token ACE_DYNAMIC ACE_STATIC ACE_SUSPEND ACE_RESUME ACE_REMOVE ACE_USTREAM
%token ACE_MODULE_T ACE_STREAM_T ACE_SVC_OBJ_T ACE_ACTIVE ACE_INACTIVE
%token ACE_PATHNAME ACE_IDENT ACE_STRING

%start svc_config_entries

%type <ident_> ACE_IDENT ACE_STRING ACE_PATHNAME pathname parameters_opt
%type <type_> type status
%type <parse_node_> dynamic static suspend resume remove module_list stream
%type <parse_node_> stream_modules module svc_config_entry
%type <static_node_> stream_ops
%type <svc_record_> svc_location
%type <location_node_> svc_initializer

// Generate a pure (reentrant) parser -- GNU Bison only
%pure_parser

%%

svc_config_entries
  : svc_config_entries svc_config_entry
    {
      if ($2 != 0)
      {
        $2->apply (ACE_SVC_CONF_PARAM->config, ACE_SVC_CONF_PARAM->yyerrno); 
        delete $2;
      }
      ACE_SVC_CONF_PARAM->obstack.release ();
    }
  | svc_config_entries error
    {
      ACE_SVC_CONF_PARAM->obstack.release ();
    }
  | /* EMPTY */
  ;

svc_config_entry
  : dynamic
  | static
  | suspend
  | resume
  | remove
  | stream
  ;

dynamic
  : ACE_DYNAMIC svc_location parameters_opt
    {
      if ($2 != 0)
        $$ = new ACE_Dynamic_Node ($2, $3);
      else
        $$ = 0;
    }
  ;

static
  : ACE_STATIC ACE_IDENT parameters_opt
    {
      $$ = new ACE_Static_Node ($2, $3);
    }
  ;

suspend
  : ACE_SUSPEND ACE_IDENT
    {
      $$ = new ACE_Suspend_Node ($2);
    }
  ;

resume
  : ACE_RESUME ACE_IDENT
    {
      $$ = new ACE_Resume_Node ($2);
    }
  ;

remove
  : ACE_REMOVE ACE_IDENT
    {
      $$ = new ACE_Remove_Node ($2);
    }
  ;

stream
  : ACE_USTREAM stream_ops stream_modules
    {
      $$ = new ACE_Stream_Node ($2, $3);
    }
  | ACE_USTREAM ACE_IDENT { $<static_node_>$ = new ACE_Static_Node ($2); } stream_modules
    {
      $$ = new ACE_Dummy_Node ($<static_node_>3, $4);
    }
  ;

stream_ops
  : dynamic
    {
    }
  | static
    {
    }
  ;

stream_modules
  : '{'
    {
      // Initialize left context...
      $<static_node_>$ = $<static_node_>0;
    }
   module_list '}'
    {
      $$ = $3;
    }
  | /* EMPTY */ { $$ = 0; }
  ;

module_list
  : module_list module
    {
      if ($2 != 0)
        {
          $2->link ($1);
          $$ = $2;
        }
    }
  | /* EMPTY */ { $$ = 0; }
  ;

module
  : dynamic
    {
      ACE_Static_Node *svc_type = $<static_node_>1;

      if (svc_type != 0)
        {
          ACE_Static_Node *module = $<static_node_>-1;

          ACE_ARGV args (svc_type->parameters ());
          ACE_Module_Type *mt = ace_get_module (module->record (ACE_SVC_CONF_PARAM->config),
                                                svc_type->record (ACE_SVC_CONF_PARAM->config),
                                                ACE_SVC_CONF_PARAM->yyerrno);
          ACE_Stream_Type *st =
            dynamic_cast<ACE_Stream_Type *> (const_cast<ACE_Service_Type_Impl *> (module->record (ACE_SVC_CONF_PARAM->config)->type ()));

          if (!st
              || !mt
              || mt->init (args.argc (), args.argv ()) == -1
              || st->push (mt) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_LIB_TEXT ("dynamic initialization failed for Module %s\n"),
                          svc_type->name ()));
              ACE_SVC_CONF_PARAM->yyerrno++;
            }
        }
    }
  | static
    {
      ACE_Static_Node *sn = $<static_node_>-1;
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            $<static_node_>1->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

      if (((ACE_Stream_Type *) sn->record (ACE_SVC_CONF_PARAM->config)->type ())->push (mt) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("Problem with static\n")));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    }
  | suspend
    {
      ACE_Static_Node *sn = $<static_node_>-1;
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            sn->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->suspend ();
    }
  | resume
    {
      ACE_Static_Node *sn = $<static_node_>-1;
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            $<static_node_>1->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->resume ();
    }
  | remove
    {
      ACE_Static_Node *stream = $<static_node_>-1;
      ACE_Static_Node *module = $<static_node_>1;
      ACE_Module_Type *mt = ace_get_module (stream->record (ACE_SVC_CONF_PARAM->config),
                                            module->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

      ACE_Stream_Type *st =
        dynamic_cast<ACE_Stream_Type *> (const_cast<ACE_Service_Type_Impl *> (stream->record (ACE_SVC_CONF_PARAM->config)->type ()));
      if (!st || (mt != 0 && st->remove (mt) == -1))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("cannot remove Module_Type %s from STREAM_Type %s\n"),
                      module->name (),
                      stream->name ()));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    }
  ;

svc_location
  : ACE_IDENT type svc_initializer status
    {
      $$ = new ACE_Service_Type_Factory ($1, $2, $3, $4);
    }
  ;

status
  : ACE_ACTIVE
    {
      $$ = 1;
    }
  | ACE_INACTIVE
    {
      $$ = 0;
    }
  | /* EMPTY */
    {
      $$ = 1;
    }
  ;

svc_initializer
  : pathname ':' ACE_IDENT
    {
      $$ = new ACE_Object_Node ($1, $3);
    }
  | pathname ':' ACE_IDENT '(' ')'
    {
      $$ = new ACE_Function_Node ($1, $3);
    }
  | ':' ACE_IDENT '(' ')'
    {
      $$ = new ACE_Static_Function_Node ($2);
    }
  ;

type
  : ACE_MODULE_T '*'
    {
      $$ = ACE_MODULE_T;
    }
  | ACE_SVC_OBJ_T '*'
    {
      $$ = ACE_SVC_OBJ_T;
    }
  | ACE_STREAM_T '*'
    {
      $$ = ACE_STREAM_T;
    }
  ;

parameters_opt
  : ACE_STRING
  | /* EMPTY */ { $$ = 0; }
  ;

pathname
  : ACE_PATHNAME
  | ACE_IDENT
  | ACE_STRING
  ;

%%

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Prints the error string to standard output.  Cleans up the error
// messages.

void
yyerror (int yyerrno, int yylineno, char const * s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (yyerrno);
  ACE_UNUSED_ARG (yylineno);
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACE_ERROR ((LM_ERROR,
              ACE_LIB_TEXT ("ACE (%P|%t) [error %d] on line %d: %s\n"),
              yyerrno,
              yylineno,
              ACE_TEXT_CHAR_TO_TCHAR (s)));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Service_Type const * sr,
                ACE_TCHAR const * svc_name,
                int & yyerrno)
{
  ACE_Service_Type_Impl const * const type = sr->type ();
  ACE_Stream_Type const * const st =
    (sr == 0
     ? 0
     : dynamic_cast<ACE_Stream_Type const *> (type));
  ACE_Module_Type const * const mt = (st == 0 ? 0 : st->find (svc_name));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("cannot locate Module_Type %s ")
                  ACE_LIB_TEXT ("in STREAM_Type %s\n"),
                  svc_name,
                  (sr ? sr->name () : ACE_LIB_TEXT ("(nil)"))));
      ++yyerrno;
    }

  return const_cast<ACE_Module_Type *> (mt);
}

static ACE_Module_Type *
ace_get_module (ACE_Service_Type const * sr,
                ACE_Service_Type const * sv,
                int & yyerrno)
{
  ACE_Stream_Type const * const st =
    (sr == 0
     ? 0
     : static_cast<ACE_Stream_Type const *> (sr->type ()));

  ACE_Module_Type const * const mt =
    static_cast <ACE_Module_Type const *> (sv->type ());

  ACE_TCHAR const * const module_type_name =
    (mt ? mt->name () : ACE_LIB_TEXT ("(nil)"));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("cannot locate Module_Type %s or STREAM_Type %s\n"),
                  module_type_name,
                  (sr ? sr->name () : ACE_LIB_TEXT ("(nil)"))));
      ++yyerrno;
    }

  // Make sure that the Module has the same name as the
  // Module_Type object from the svc.conf file.
  ACE_Module<ACE_SYNCH> * const mp =
    static_cast<ACE_Module<ACE_SYNCH> *> (mt ? mt->object () : 0);

  if (mp && ACE_OS::strcmp (mp->name (), module_type_name) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                  module_type_name,
                  mp->name ()));
      mp->name (module_type_name);
    }

  return const_cast<ACE_Module_Type *> (mt);
}

#if defined (DEBUGGING)
// Main driver program.

int
main (int argc, char *argv[])
{
  ACE_Svc_Conf_Param param (stdin);

  // Try to reopen any filename argument to use YYIN.
  if (argc > 1 && (yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ACE_OS::fprintf (stderr, ACE_LIB_TEXT ("usage: %s [file]\n"), argv[0]), ACE_OS::exit (1);

  return ::yyparse (&param);
}
#endif /* DEBUGGING */

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* ACE_USES_CLASSIC_SVC_CONF == 1 */
