%{
// $Id$
#define ACE_BUILD_DLL
#include "ace/ARGV.h"
#include "ace/Svc_Conf.h"
#include "ace/Module.h"
#include "ace/Stream.h"

ACE_RCSID(ace, Svc_Conf_y, "$Id$")

// Prototypes.
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        ACE_Static_Node *svc_type);
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        const char *svc_name);

#define YYDEBUG_LEXER_TEXT (yytext[yyleng] = '\0', yytext)
// Force the pretty debugging code to compile.
#define YYDEBUG 1

// Efficient memory allocation technique.
ACE_Obstack *ace_obstack;

%}
%token ACE_DYNAMIC ACE_STATIC ACE_SUSPEND ACE_RESUME ACE_REMOVE ACE_USTREAM
%token ACE_MODULE_T ACE_STREAM_T ACE_SVC_OBJ_T ACE_ACTIVE ACE_INACTIVE
%token ACE_PATHNAME ACE_IDENT ACE_STRING
%token ACE_LPAREN ACE_RPAREN ACE_LBRACE ACE_RBRACE ACE_STAR ACE_COLON

%start svc_config_entries

%type <ident_> ACE_IDENT ACE_STRING ACE_PATHNAME pathname parameters_opt
%type <type_> type status
%type <parse_node_> dynamic static suspend resume remove module_list stream
%type <parse_node_> stream_modules module svc_config_entry
%type <static_node_> stream_ops
%type <svc_record_> svc_location
%type <location_node_> svc_initializer

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
  : ACE_LBRACE
    {
      // Initialize left context...
      $<static_node_>$ = $<static_node_>0;
    }
   module_list ACE_RBRACE
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

          ACE_ARGV args (ASYS_WIDE_STRING (svc_type->parameters ()));
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
                          ASYS_TEXT ("dynamic initialization failed for Module %s\n"),
                          ASYS_WIDE_STRING (svc_type->name ())));
              yyerrno++;
            }
        }
    }
  | static
    {
      ACE_Module_Type *mt = ace_get_module ($<static_node_>-1, $<static_node_>1->name ());

      if (((ACE_Stream_Type *) ($<static_node_>-1)->record ()->type ())->push (mt) == -1)
        yyerrno++;
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
                      ASYS_TEXT ("cannot remove Module_Type %s from STREAM_Type %s\n"),
                      ASYS_WIDE_STRING (module->name ()),
                      ASYS_WIDE_STRING (stream->name ())));
          yyerrno++;
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
            = ace_create_service_type (ASYS_WIDE_STRING ($1),
                                       $2,
                                       sym,
                                       flags,
                                       gobbler);
          $$ = new ACE_Service_Type (ASYS_WIDE_STRING ($1),
                                     stp,
                                     $3->handle (),
                                     $4);
        }
      else
        {
          ++yyerrno;
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
  : pathname ACE_COLON ACE_IDENT
    {
      $$ = new ACE_Object_Node ($1, $3);
    }
  | pathname ACE_COLON ACE_IDENT ACE_LPAREN ACE_RPAREN
    {
      $$ = new ACE_Function_Node ($1, $3);
    }
  | ACE_COLON ACE_IDENT ACE_LPAREN ACE_RPAREN
    {
      $$ = new ACE_Static_Function_Node ($2);
    }
  ;

type
  : ACE_MODULE_T ACE_STAR
    {
      $$ = ACE_MODULE_T;
    }
  | ACE_SVC_OBJ_T ACE_STAR
    {
      $$ = ACE_SVC_OBJ_T;
    }
  | ACE_STREAM_T ACE_STAR
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
  ;

%%
// Prints the error string to standard output.  Cleans up the error
// messages.

void
yyerror (const char *s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("[error %d] on line %d: %s\n"),
              ++yyerrno,
              yylineno,
              ASYS_WIDE_STRING (s)));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                const char *svc_name)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0
    ? 0
    : ACE_dynamic_cast (ACE_Stream_Type *,
                        ACE_const_cast (ACE_Service_Type_Impl *,
                                        type));
  ACE_Module_Type *mt = st == 0 ? 0 : st->find (ASYS_WIDE_STRING (svc_name));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("cannot locate Module_Type %s in STREAM_Type %s\n"),
                  ASYS_WIDE_STRING (svc_name),
                  ASYS_WIDE_STRING (str_rec->name ())));
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
  const char *module_type_name = svc_type->name ();
  const ASYS_TCHAR *wname = ASYS_WIDE_STRING (module_type_name);

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("cannot locate Module_Type %s or STREAM_Type %s\n"),
                  wname,
                  ASYS_WIDE_STRING (str_rec->name ())));
      yyerrno++;
    }

  // Make sure that the Module has the same name as the
  // Module_Type object from the svc.conf file.
  ACE_Module<ACE_SYNCH> *mp = (ACE_Module<ACE_SYNCH> *) mt->object ();

  if (ACE_OS::strcmp (mp->name (), wname) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                  wname,
                  mp->name ()));
      mp->name (wname);
    }

  return mt;
}

ACE_Service_Type_Impl *
ace_create_service_type (const ASYS_TCHAR *name,
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
                                               ASYS_WIDE_STRING (name), flags,
                                               gobbler),
                      0);
      break;
    case ACE_MODULE_T:
      ACE_NEW_RETURN (stp,
                      ACE_Module_Type (symbol, ASYS_WIDE_STRING (name), flags),
                      0);
      break;
    case ACE_STREAM_T:
      ACE_NEW_RETURN (stp,
                      ACE_Stream_Type (symbol, ASYS_WIDE_STRING (name), flags),
                      0);
      break;
    default:
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("unknown case\n")));
      yyerrno++;
      break;
    }
  return stp;
}

#if defined (DEBUGGING)
// Current line number.
int yylineno = 1;

// Name given on the command-line to envoke the program.
char *program_name;

// Main driver program.

int
main (int argc, char *argv[])
{
  yyin = stdin;
  ace_obstack = new ACE_Obstack;

  // Try to reopen any filename argument to use YYIN.
  if (argc > 1 && (yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ::fprintf (stderr, "usage: %s [file]\n", argv[0]), exit (1);

  return yyparse ();
}
#endif /* DEBUGGING */
