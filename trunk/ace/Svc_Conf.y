%{
// $Id$

#include "ace/ARGV.h"
#include "ace/Svc_Conf.h"
#include "ace/Module.h"
#include "ace/Stream.h"

ACE_RCSID (ace,
           Svc_Conf_y,
           "$Id$")

// Prototypes.
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        ACE_Static_Node *svc_type);
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        const ACE_TCHAR *svc_name);

#define YYDEBUG_LEXER_TEXT (yytext[yyleng] = '\0', yytext)

// Force the pretty debugging code to compile.
// #define YYDEBUG 1

// Efficient memory allocation technique.
ACE_Obstack_T<ACE_TCHAR> *ace_obstack = 0;

// Keeps track of the number of errors encountered so far.
int yyerrno = 0;

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
        $2->apply (); delete $2;
      }
      ace_obstack->release ();
    }
  | svc_config_entries error
    {
      ace_obstack->release ();
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
          ACE_Module_Type *mt = ace_get_module (module,
                                                svc_type);
          ACE_Stream_Type *st =
            ACE_dynamic_cast (ACE_Stream_Type *,
                              ACE_const_cast (ACE_Service_Type_Impl *,
                                              module->record ()->type ()));

          if (mt->init (args.argc (), args.argv ()) == -1
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
      ACE_Module_Type *mt = ace_get_module ($<static_node_>-1, $<static_node_>1->name ());

      if (((ACE_Stream_Type *) ($<static_node_>-1)->record ()->type ())->push (mt) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("Problem with static\n")));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    }
  | suspend
    {
      ACE_Module_Type *mt = ace_get_module ($<static_node_>-1,
                                            $<static_node_>1->name ());
      if (mt != 0)
        mt->suspend ();
    }
  | resume
    {
      ACE_Module_Type *mt = ace_get_module ($<static_node_>-1,
                                            $<static_node_>1->name ());
      if (mt != 0)
        mt->resume ();
    }
  | remove
    {
      ACE_Static_Node *stream = $<static_node_>-1;
      ACE_Static_Node *module = $<static_node_>1;
      ACE_Module_Type *mt = ace_get_module (stream,
                                            module->name ());

      ACE_Stream_Type *st =
        ACE_dynamic_cast (ACE_Stream_Type *,
                          ACE_const_cast (ACE_Service_Type_Impl *,
                                          stream->record ()->type ()));
      if (mt != 0 && st->remove (mt) == -1)
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
      u_int flags
        = ACE_Service_Type::DELETE_THIS
        | ($3->dispose () == 0 ? 0 : ACE_Service_Type::DELETE_OBJ);
      ACE_Service_Object_Exterminator gobbler = 0;
      void *sym = $3->symbol (&gobbler);

      if (sym != 0)
        {
          ACE_Service_Type_Impl *stp
            = ace_create_service_type ($1,
                                       $2,
                                       sym,
                                       flags,
                                       gobbler);
          $$ = new ACE_Service_Type ($1,
                                     stp,
                                     $3->handle (),
                                     $4);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("Unable to find service: %s\n"),
                      $1));
          ++ACE_SVC_CONF_PARAM->yyerrno;
          $$ = 0;
        }
      delete $3;
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
// Prints the error string to standard output.  Cleans up the error
// messages.

void
yyerror (const ACE_TCHAR *s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACE_ERROR ((LM_ERROR,
              ACE_LIB_TEXT ("[error %d] on line %d: %s\n"),
              yyerrno,
              yylineno,
              s));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                const ACE_TCHAR *svc_name)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0
    ? 0
    : ACE_dynamic_cast (ACE_Stream_Type *,
                        ACE_const_cast (ACE_Service_Type_Impl *,
                                        type));
  ACE_Module_Type *mt = st == 0 ? 0 : st->find (svc_name);

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("cannot locate Module_Type %s in STREAM_Type %s\n"),
                  svc_name,
                  str_rec->name ()));
      yyerrno++;
    }

  return mt;
}

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                ACE_Static_Node *svc_type)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0 ? 0 : (ACE_Stream_Type *) type;
  const ACE_Service_Type *sv = svc_type->record ();
  type = sv->type ();
  ACE_Module_Type *mt = (ACE_Module_Type *) type;
  const ACE_TCHAR *module_type_name = svc_type->name ();

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("cannot locate Module_Type %s or STREAM_Type %s\n"),
                  module_type_name,
                  str_rec->name ()));
      yyerrno++;
    }

  // Make sure that the Module has the same name as the
  // Module_Type object from the svc.conf file.
  ACE_Module<ACE_SYNCH> *mp = (ACE_Module<ACE_SYNCH> *) mt->object ();

  if (ACE_OS::strcmp (mp->name (), module_type_name) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                  module_type_name,
                  mp->name ()));
      mp->name (module_type_name);
    }

  return mt;
}

ACE_Service_Type_Impl *
ace_create_service_type (const ACE_TCHAR *name,
                         int type,
                         void *symbol,
                         u_int flags,
                         ACE_Service_Object_Exterminator gobbler)
{
  ACE_Service_Type_Impl *stp = 0;

  // Note, the only place we need to put a case statement.  This is
  // also the place where we'd put the RTTI tests, if the compiler
  // actually supported them!

  switch (type)
    {
    case ACE_SVC_OBJ_T:
      ACE_NEW_RETURN (stp,
                      ACE_Service_Object_Type ((ACE_Service_Object *) symbol,
                                               name, flags,
                                               gobbler),
                      0);
      break;
    case ACE_MODULE_T:
      ACE_NEW_RETURN (stp,
                      ACE_Module_Type (symbol, name, flags),
                      0);
      break;
    case ACE_STREAM_T:
      ACE_NEW_RETURN (stp,
                      ACE_Stream_Type (symbol, name, flags),
                      0);
      break;
    default:
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("unknown case\n")));
      yyerrno++;
      break;
    }
  return stp;
}

#if defined (DEBUGGING)
// Current line number.
int yylineno = 1;

// Name given on the command-line to envoke the program.
ACE_TCHAR *program_name;

// Main driver program.

int
main (int argc, char *argv[])
{
  yyin = stdin;
  ace_obstack = new ACE_Obstack_T<ACE_TCHAR>;

  // Try to reopen any filename argument to use YYIN.
  if (argc > 1 && (yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ::fprintf (stderr, "usage: %s [file]\n", argv[0]), exit (1);

  return yyparse ();
}
#endif /* DEBUGGING */
