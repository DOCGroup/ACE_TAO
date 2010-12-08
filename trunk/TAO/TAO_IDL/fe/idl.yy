/* $Id$ */

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in whichlong
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

/*
 * idl.yy - YACC grammar for IDL 3.x
 */

/* Declarations */

%{
#include "utl_identifier.h"
#include "utl_err.h"
#include "utl_string.h"
#include "utl_strlist.h"
#include "utl_namelist.h"
#include "utl_exprlist.h"
#include "utl_labellist.h"
#include "utl_decllist.h"

#include "global_extern.h"
#include "nr_extern.h"

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_field.h"
#include "ast_expression.h"
#include "ast_operation.h"
#include "ast_generator.h"
#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_template_module_ref.h"
#include "ast_typedef.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_porttype.h"
#include "ast_connector.h"
#include "ast_uses.h"
#include "ast_constant.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_structure_fwd.h"
#include "ast_extern.h"
#include "ast_enum.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_factory.h"
#include "ast_finder.h"
#include "ast_exception.h"
#include "ast_param_holder.h"
#include "ast_visitor_tmpl_module_inst.h"
#include "ast_visitor_tmpl_module_ref.h"
#include "ast_visitor_context.h"

#include "fe_declarator.h"
#include "fe_interface_header.h"
#include "fe_obv_header.h"
#include "fe_component_header.h"
#include "fe_home_header.h"
#include "fe_utils.h"

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern  "C" int yywrap();
#endif  // (defined(apollo) || defined(hpux)) && defined(__cplusplus)


void yyerror (const char *);
int yylex (void);
extern "C" int yywrap (void);
extern char yytext[];
extern int yyleng;
AST_Enum *tao_enum_constant_decl = 0;
AST_Expression::ExprType t_param_const_type = AST_Expression::EV_none;
#define YYDEBUG_LEXER_TEXT (yytext[yyleng] = '\0', yytext)
// Force the pretty debugging code to compile.
#define YYDEBUG 1
%}

/*
 * Declare the type of values in the grammar
 */

%union {
  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
  FE_OBVHeader                  *vhval;         /* Valuetype header     */
  FE_ComponentHeader            *chval;         /* Component header     */
  FE_HomeHeader                 *hhval;         /* Home header          */
  AST_Expression                *exval;         /* Expression value     */
  AST_UnionLabel                *ulval;         /* Union label          */
  AST_Field                     *ffval;         /* Field value          */
  AST_Field::Visibility         vival;          /* N/A, pub or priv     */
  AST_Expression::ExprType      etval;          /* Expression type      */
  AST_Argument::Direction       dival;          /* Argument direction   */
  AST_Operation::Flags          ofval;          /* Operation flags      */
  FE_Declarator                 *deval;         /* Declarator value     */
  ACE_CDR::Boolean              bval;           /* Boolean value        */
  ACE_CDR::LongLong             ival;           /* Long Long value      */
  ACE_CDR::ULongLong            uival;          /* Unsigned long long   */
  ACE_CDR::Double               dval;           /* Double value         */
  ACE_CDR::Float                fval;           /* Float value          */
  ACE_CDR::Char                 cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
  AST_Decl::NodeType            ntval;          /* Node type value      */
  FE_Utils::T_Param_Info        *pival;         /* Template interface param */
  FE_Utils::T_PARAMLIST_INFO    *plval;         /* List of template params */
  FE_Utils::T_ARGLIST           *alval;         /* List of template args */
}

/*
 * Token types: These are returned by the lexer
 */

%token <strval> IDENTIFIER

%token          IDL_CONST
%token          IDL_MODULE
%token          IDL_INTERFACE
%token          IDL_TYPEDEF
%token          IDL_LONG
%token          IDL_SHORT
%token          IDL_UNSIGNED
%token          IDL_DOUBLE
%token          IDL_FLOAT
%token          IDL_CHAR
%token          IDL_WCHAR
%token          IDL_OCTET
%token          IDL_BOOLEAN
%token          IDL_FIXED
%token          IDL_ANY
%token          IDL_OBJECT
%token          IDL_STRUCT
%token          IDL_UNION
%token          IDL_SWITCH
%token          IDL_ENUM
%token          IDL_SEQUENCE
%token          IDL_STRING
%token          IDL_WSTRING
%token          IDL_EXCEPTION
%token          IDL_CASE
%token          IDL_DEFAULT
%token          IDL_READONLY
%token          IDL_ATTRIBUTE
%token          IDL_ONEWAY
%token          IDL_IDEMPOTENT
%token          IDL_VOID
%token          IDL_IN
%token          IDL_OUT
%token          IDL_INOUT
%token          IDL_RAISES
%token          IDL_CONTEXT
%token          IDL_NATIVE
%token          IDL_LOCAL
                /* OBV tokens */
%token          IDL_ABSTRACT
%token          IDL_CUSTOM
%token          IDL_FACTORY
%token          IDL_PRIVATE
%token          IDL_PUBLIC
%token          IDL_SUPPORTS
%token          IDL_TRUNCATABLE
%token          IDL_VALUETYPE
                /* Component tokens */
%token          IDL_COMPONENT
%token          IDL_CONSUMES
%token          IDL_EMITS
%token          IDL_EVENTTYPE
%token          IDL_FINDER
%token          IDL_GETRAISES
%token          IDL_HOME
%token          IDL_IMPORT
%token          IDL_MULTIPLE
%token          IDL_PRIMARYKEY
%token          IDL_PROVIDES
%token          IDL_PUBLISHES
%token          IDL_SETRAISES
%token          IDL_TYPEID
%token          IDL_TYPEPREFIX
%token          IDL_USES
%token          IDL_MANAGES
                /* Extended ports tokens  */
%token          IDL_TYPENAME
%token          IDL_PORT
%token          IDL_MIRRORPORT
%token          IDL_PORTTYPE
%token          IDL_CONNECTOR
%token          IDL_ALIAS

%token <ival>   IDL_INTEGER_LITERAL
%token <uival>  IDL_UINTEGER_LITERAL
%token <sval>   IDL_STRING_LITERAL
%token <cval>   IDL_CHARACTER_LITERAL
%token <dval>   IDL_FLOATING_PT_LITERAL
%token          IDL_TRUETOK
%token          IDL_FALSETOK

%token <strval> IDL_SCOPE_DELIMITOR
%token          IDL_LEFT_SHIFT
%token          IDL_RIGHT_SHIFT
%token <wcval>  IDL_WCHAR_LITERAL
%token <wsval>  IDL_WSTRING_LITERAL

/*
 * These are production names:
 */

%type <dcval>   type_spec simple_type_spec constructed_type_spec
%type <dcval>   template_type_spec sequence_type_spec string_type_spec
%type <dcval>   struct_type enum_type switch_type_spec union_type
%type <dcval>   array_declarator op_type_spec seq_head wstring_type_spec
%type <dcval>   param_type_spec type_dcl type_declarator actual_parameter

%type <idlist>  scoped_name interface_type component_inheritance_spec
%type <idlist>  home_inheritance_spec primary_key_spec module_header
%type <idlist>  template_module_header

%type <slval>   opt_context at_least_one_string_literal
%type <slval>   string_literals formal_parameter_names
%type <slval>   at_least_one_formal_parameter_name

%type <nlval>   at_least_one_scoped_name scoped_names inheritance_spec
%type <nlval>   opt_raises opt_getraises opt_setraises supports_spec

%type <elval>   at_least_one_array_dim array_dims

%type <llval>   at_least_one_case_label case_labels

%type <dlval>   at_least_one_declarator declarators
%type <dlval>   at_least_one_simple_declarator simple_declarators

%type <ihval>   interface_header

%type <vhval>   value_header event_rest_of_header

%type <chval>   component_header

%type <hhval>   home_header

%type <exval>   expression const_expr or_expr xor_expr and_expr shift_expr
%type <exval>   add_expr mult_expr unary_expr primary_expr literal
%type <exval>   positive_int_expr array_dim

%type <ulval>   case_label

%type <ffval>   element_spec

%type <etval>   const_type integer_type char_type boolean_type
%type <etval>   floating_pt_type fixed_type any_type signed_int
%type <etval>   unsigned_int base_type_spec octet_type object_type

%type <dival>   direction

%type <ofval>   opt_op_attribute

%type <deval>   declarator simple_declarator complex_declarator

%type <bval>    opt_truncatable opt_multiple uses_opt_multiple

%type <idval>   interface_decl value_decl union_decl struct_decl id
%type <idval>   event_header event_plain_header event_custom_header
%type <idval>   event_abs_header

%type <ntval>   formal_parameter_type

%type <pival>   formal_parameter

%type <plval>   formal_parameters at_least_one_formal_parameter

%type <sval>    formal_parameter_name

%type <alval>   actual_parameters at_least_one_actual_parameter
%%

/*
 * Production starts here.
 */
start : definitions ;

definitions
        : definitions definition
        | /* EMPTY */
        ;

definition
        : fixed_definition
        {
        }
        | template_module ';'
        {
        }
        | template_module_inst ';'
        {
        }
        ;

at_least_one_definition
        : definitions definition
        ;

fixed_definition
        : type_dcl
        {
// fixed_definition : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | typeid_dcl
        {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | typeprefix_dcl
        {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | const_dcl
        {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | exception
        {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
          ';'
        {
//      ';'
        idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | interface_def
        {
//      | interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | module
        {
//      | module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | value_def
        {
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | component
        {
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | home_decl
        {
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | event
        {
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | porttype_decl
        {
//      | porttype_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | connector_decl
        {
//      | connector_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | error
        {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
        ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

module_header
        : IDL_MODULE
        {
// module_header  : IDL_MODULE
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
        scoped_name
        {
          $<idlist>$ = $3;
        }
        ;

module
        : module_header
        {
// module : module_header
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleIDSeen);

          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a parse error with
          // the appropriate message.
          if ($1->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }

          AST_Module *m = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Make a new module and add it to the enclosing scope
           */
          if (s != 0)
            {
              m = idl_global->gen ()->create_module (s,
                                                     $1);
              (void) s->fe_add_module (m);
            }

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (m);
        }
        '{'
        {
//      '{'
        idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
        at_least_one_definition
        {
//      at_least_one_definition
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

template_module_header
        : module_header '<'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
        ;

template_module
        : template_module_header
        {
// template_module : template_module_header
          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a syntax error with
          // the appropriate message.
          if ($1->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }
        }
        at_least_one_formal_parameter
        {
          if (FE_Utils::duplicate_param_id ($3))
            {
              idl_global->err ()->duplicate_param_id (
                $1);

              return 1;
            }
        }
        '>'
        {
//        '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleParamsSeen);

          AST_Template_Module *tm =
            idl_global->gen ()->create_template_module ($1,
                                                        $3);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Module *m = s->fe_add_module (tm);

          // We've probably tried to reopen a template module,
          // going further will cause a crash.
          if (m == 0)
            {
              return 1;
            }

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (tm);

          // Store these for reference as we parse the scope
          // of the template module.
          idl_global->current_params ($3);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
        at_least_one_tpl_definition
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleQsSeen);

          /*
           * Finished with this module - pop it from the scope stack.
           */
          idl_global->scopes ().pop ();

          // Clear the pointer so scoped name lookup will know
          // that we are no longer in a template module scope.
          idl_global->current_params (0);
        }
        ;

at_least_one_tpl_definition
        : tpl_definition tpl_definitions
        ;

tpl_definitions
        : tpl_definitions tpl_definition
        | /* EMPTY */
        ;

tpl_definition
        : fixed_definition
        | template_module_ref ';'
        ;

template_module_ref
        : IDL_ALIAS scoped_name
        {
// template_module_ref : IDL_ALIAS scoped_name
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
        '<' at_least_one_formal_parameter_name '>'
        {
//        '<' at_least_one_formal_parameter_name '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
        id
        {
//        id
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ($2);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
              return 1;
            }

          AST_Template_Module *ref =
            AST_Template_Module::narrow_from_decl (d);

          if (ref == 0)
            {
              idl_global->err ()->template_module_expected (d);
              return 1;
            }

          bool refs_match =
            ref->match_param_refs ($5,
                                   s);

          if (! refs_match)
            {
              // Error message is already output.
              return 1;
            }

          UTL_ScopedName sn ($8, 0);

          AST_Template_Module_Ref *tmr =
            idl_global->gen ()->create_template_module_ref (
              &sn,
              ref,
              $5);

          (void) s->fe_add_template_module_ref (tmr);

          sn.destroy ();
          $2->destroy ();
          delete $2;
          $2 = 0;

          ast_visitor_context ctx;
          ctx.template_params (ref->template_params ());
          ast_visitor_tmpl_module_ref v (&ctx);

          // The implied IDL resulting from this reference is
          // created here, in the template module scope. Upon
          // instantiation of the enclosing template module, the
          // visitor copies this implied IDL to the instantiated
          // module scope. The extra copy is less than ideal, but
          // otherwise we have ugly lookup issues when the
          // referenced template module's contents are referenced
          // using the aliased scoped name.
          if (v.visit_template_module_ref (tmr) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("visit_template_module_ref")
                          ACE_TEXT (" failed\n")));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }
        }
        ;

template_module_inst
        : template_module_header
        {
// template_module_inst : template_module_header
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
        at_least_one_actual_parameter '>'
        {
//        at_least_one_actual_parameter '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
        id
        {
//        id
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName *sn = $1;
          AST_Template_Module *ref = 0;
          AST_Decl *d = s->lookup_by_name (sn);

          if (d == 0)
            {
              idl_global->err ()->lookup_error (sn);
              return 1;
            }
          else
            {
              ref = AST_Template_Module::narrow_from_decl (d);

              if (ref == 0)
                {
                  idl_global->err ()->template_module_expected (d);
                  return 1;
                }
            }

          sn->destroy ();
          delete sn;
          sn = 0;
          $1 = 0;

          if (! ref->match_arg_names ($3))
            {
              return 1;
            }

          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ($6,
                                           0),
                          1);

          AST_Template_Module_Inst *tmi =
            idl_global->gen ()->create_template_module_inst (
              sn,
              ref,
              $3);

          (void) s->fe_add_template_module_inst (tmi);

          ast_visitor_context ctx;
          ctx.template_args ($3);
          ast_visitor_tmpl_module_inst v (&ctx);

          if (v.visit_template_module_inst (tmi) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("visit_template_module_inst")
                          ACE_TEXT (" failed\n")));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }
        }
        ;

interface_def
        : interface
        | interface_forward
        ;

interface :
        interface_header
        {
// interface : interface_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Interface *i = 0;

          /*
           * Make a new interface node and add it to its enclosing scope.
           */
          if (s != 0 && $1 != 0)
            {
              i =
                idl_global->gen ()->create_interface (
                                        $1->name (),
                                        $1->inherits (),
                                        $1->n_inherits (),
                                        $1->inherits_flat (),
                                        $1->n_inherits_flat (),
                                        $1->is_local (),
                                        $1->is_abstract ()
                                      );
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the interface to its definition scope.
               */
              (void) s->fe_add_interface (i);

              // This FE_InterfaceHeader class isn't destroyed with the AST.
              $1->destroy ();
              delete $1;
              $1 = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (i);
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
        exports
        {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
        ;

interface_decl:
         IDL_INTERFACE
         {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
         id
         {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           $$ = $3;
         }
        ;

interface_header :
        interface_decl inheritance_spec
        {
// interface_header : interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          if ($2 != 0 && $2->truncatable ())
            {
              idl_global->err ()->syntax_error (
                                      IDL_GlobalData::PS_InheritColonSeen
                                    );
            }

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ($1, 0),
                          1);
          ACE_NEW_RETURN ($$,
                          FE_InterfaceHeader (n,
                                              $2,
                                              false,
                                              false,
                                              true),
                          1);

          if (0 != $2)
            {
              $2->destroy ();
              delete $2;
              $2 = 0;
            }
        }
        |
        IDL_LOCAL interface_decl inheritance_spec
        {
//      | IDL_LOCAL interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ($2, 0),
                          1);
          ACE_NEW_RETURN ($$,
                          FE_InterfaceHeader (n,
                                              $3,
                                              true,
                                              false,
                                              true),
                          1);

          if (0 != $3)
            {
              $3->destroy ();
              delete $3;
              $3 = 0;
            }
        }
        |
        IDL_ABSTRACT interface_decl inheritance_spec
        {
//      | IDL_ABSTRACT interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ($2, 0),
                          1);
          ACE_NEW_RETURN ($$,
                          FE_InterfaceHeader (n,
                                              $3,
                                              false,
                                              true,
                                              true),
                          1);

          if (0 != $3)
            {
              $3->destroy ();
              delete $3;
              $3 = 0;
            }
        }
        ;

inheritance_spec
        : ':' opt_truncatable
        {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
          at_least_one_scoped_name
        {
//      at_least_one_scoped_name
          $4->truncatable ($2);
          $$ = $4;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = 0;
        }
        ;

value_def
        : valuetype
        | value_abs_decl
        | value_forward_decl
        | value_box_decl
        ;

valuetype
        : IDL_CUSTOM value_concrete_decl
        {
// valuetype : IDL_CUSTOM value_concrete_decl
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %C line %d\n"),
                       idl_global->filename ()->get_string (),
                       idl_global->lineno ()));
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                       ACE_TEXT (" custom yet\n")));
        }
        | value_concrete_decl
        ;

value_concrete_decl :
        value_header
        {
// value_concrete_decl : value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && $1 != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                  $1->name (),
                  $1->inherits (),
                  $1->n_inherits (),
                  $1->inherits_concrete (),
                  $1->inherits_flat (),
                  $1->n_inherits_flat (),
                  $1->supports (),
                  $1->n_supports (),
                  $1->supports_concrete (),
                  false,
                  $1->truncatable (),
                  false);

              i = AST_Interface::narrow_from_decl (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = AST_ValueType::narrow_from_decl (i);
              (void) s->fe_add_valuetype (v);

              // FE_OBVHeader is not automatically destroyed in the AST
              $1->destroy ();
              delete $1;
              $1 = 0;
            }

          /*
           * Push it on the scope stack
           */
          idl_global->scopes ().push (v);
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
        value_elements
        {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          AST_ValueType *vt =
            AST_ValueType::narrow_from_scope (
                idl_global->scopes ().top_non_null ()
              );

          if (vt != 0 && vt->will_have_factory ())
            {
              idl_global->valuefactory_seen_ = true;
            }

          /*
           * Done with this value type - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
        ;

value_abs_decl :
        IDL_ABSTRACT
        value_header
        {
// value_abs_decl : IDL_ABSTRACT value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && $2 != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    $2->name (),
                    $2->inherits (),
                    $2->n_inherits (),
                    $2->inherits_concrete (),
                    $2->inherits_flat (),
                    $2->n_inherits_flat (),
                    $2->supports (),
                    $2->n_supports (),
                    $2->supports_concrete (),
                    true,
                    false,
                    false
                  );
              i = AST_Interface::narrow_from_decl (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = AST_ValueType::narrow_from_decl (i);
              (void) s->fe_add_valuetype (v);

              // FE_OBVHeader is not automatically destroyed in the AST
              $2->destroy ();
              delete $2;
              $2 = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (v);
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
        exports
        {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

value_header :
        value_decl
        inheritance_spec
        {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
        supports_spec
        {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ($1, 0),
                          1);
          ACE_NEW_RETURN ($$,
                          FE_OBVHeader (sn,
                                        $2,
                                        $4,
                                        ($2 != 0
                                           ? $2->truncatable ()
                                           : false)),
                          1);

          if (0 != $4)
            {
              $4->destroy ();
              delete $4;
              $4 = 0;
            }

          if (0 != $2)
            {
              $2->destroy ();
              delete $2;
              $2 = 0;
            }
        }
        ;

value_decl
        : IDL_VALUETYPE
        {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          $$ = $3;
        }
        ;

opt_truncatable :
        IDL_TRUNCATABLE
        {
// opt_truncatable : IDL_TRUNCATABLE
          $$ = true;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = false;
        }
        ;

supports_spec :
        IDL_SUPPORTS
        at_least_one_scoped_name
        {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          $$ = $2;
        }
        |   /* EMPTY */
        {
/*      |    EMPTY */
          $$ = 0;
        }
        ;

value_forward_decl :
        IDL_ABSTRACT
        value_decl
        {
// value_forward_decl : IDL_ABSTRACT value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);
          AST_ValueTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_valuetype_fwd (&n,
                                                            true);
              (void) s->fe_add_valuetype_fwd (f);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        |
        value_decl
        {
//      | value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);
          AST_ValueTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_valuetype_fwd (&n,
                                                            false);
              (void) s->fe_add_valuetype_fwd (f);
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        ;

value_box_decl
        : value_decl type_spec /* in this order %!?*/
        {
// value_box_decl : value_decl type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueBoxDeclSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);

          if (s != 0 && $2 != 0)
            {
              /*
              * Get the type_spec associated with the valuebox
              */
              AST_Type *tp = 0;
              AST_Typedef *td
                = AST_Typedef::narrow_from_decl ($2);

              if (td == 0)
                {
                  tp = AST_Type::narrow_from_decl ($2);
                }
              else
                {
                  tp = td->primitive_base_type ();
                }

              if (tp == 0)
                {
                  // The <type_spec> given is a valid type
                  idl_global->err ()->not_a_type ($2);
                }
              else
                {
                  AST_Decl::NodeType nt = tp->node_type ();

                  if (nt == AST_Decl::NT_valuetype
                      || nt == AST_Decl::NT_eventtype)
                    {
                      // valuetype is not allowed as <type_spec>
                      // for boxed value
                      idl_global->err ()->error0 (
                          UTL_Error::EIDL_ILLEGAL_BOXED_TYPE
                        );
                    }
                  else
                    {
                      /*
                      * Add the valuebox to its definition scope
                      */
                      AST_ValueBox *vb =
                        idl_global->gen ()->create_valuebox (&n,
                                                             tp);
                      (void) s->fe_add_valuebox (vb);
                    }
                }
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        ;

value_elements
        : value_elements value_element
        | /* EMPTY */
        ;

value_element
        : state_member
        | export
        | init_decl
          ';'
        ;

state_member
        : IDL_PUBLIC
        {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          $<vival>$ = AST_Field::vis_PUBLIC;
        }
        member_i
        {
//        member_i
        }
        | IDL_PRIVATE
        {
//        IDL_PRIVATE
          /* is $0 to member_i */
          $<vival>$ = AST_Field::vis_PRIVATE;
        }
        member_i
        {
//        member_i
        }
        ;

exports
        : exports export
        | /* EMPTY */
        ;

export
        : type_dcl
        {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | typeid_dcl
        {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | typeprefix_dcl
        {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | const_dcl
        {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
        | exception
        {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | attribute
        {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | operation
        {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | error
        {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
        ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

at_least_one_scoped_name :
        scoped_name scoped_names
        {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ($<nlval>$,
                          UTL_NameList ($1,
                                        $2),
                          1);
        }
        ;

scoped_names
        : scoped_names
          ','
        {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
        scoped_name
        {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          UTL_NameList *nl = 0;
          ACE_NEW_RETURN (nl,
                          UTL_NameList ($4,
                                        0),
                          1);

          if ($1 == 0)
            {
              $<nlval>$ = nl;
            }
          else
            {
              $1->nconc (nl);
              $<nlval>$ = $1;
            }
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $<nlval>$ = 0;
        }
        ;

scoped_name
        : id
        {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ($$,
                          UTL_IdList ($1,
                                      0),
                          1);
        }
        | IDL_SCOPE_DELIMITOR
        {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier ($1),
                          1);
          ACE::strdelete ($1);
          $1 = 0;
          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ($3,
                                      0),
                          1);
          ACE_NEW_RETURN ($<idlist>$,
                          UTL_IdList (id,
                                      sn),
                          1);
        }
        | scoped_name
          IDL_SCOPE_DELIMITOR
        {
//      | scoped_name IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ($2);
          $2 = 0;
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ($4,
                                      0),
                          1);
          $1->nconc (sn);
          $<idlist>$ = $1;
        }
        ;

id : IDENTIFIER
        {
// id: IDENTIFIER
          ACE_NEW_RETURN ($$,
                          Identifier ($1),
                          1);
          ACE::strdelete ($1);
          $1 = 0;
        }
        ;

interface_forward :
        interface_decl
        {
// interface_forward : interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);

          if (ACE_OS::strcmp ($1->get_string (),
                              "TypeCode") == 0
              && !idl_global->in_main_file ())
            {
              AST_PredefinedType *pdt =
                idl_global->gen ()->create_predefined_type (
                                        AST_PredefinedType::PT_pseudo,
                                        &n
                                      );

              s->add_to_scope (pdt);

              $1->destroy ();
              delete $1;
              $1 = 0;

              break;
            }

          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            0,
                                                            0);
              (void) s->fe_add_interface_fwd (f);
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        |
        IDL_LOCAL interface_decl
        {
//      | IDL_LOCAL interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);
          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_InterfaceForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            1,
                                                            0);
              (void) s->fe_add_interface_fwd (f);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        |
        IDL_ABSTRACT interface_decl
        {
//      | IDL_ABSTRACT interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);
          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_InterfaceForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            0,
                                                            1);
              (void) s->fe_add_interface_fwd (f);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        ;

const_dcl :
        IDL_CONST
        {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
        const_type
        {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
        '='
        {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
        expression
        {
//      expression
          UTL_ScopedName n ($5,
                            0);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Constant *c = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);

          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope.
           */
          if ($9 != 0 && s != 0)
            {
              AST_Param_Holder *param_holder =
                $9->param_holder ();

              AST_Expression::AST_ExprValue *result =
                $9->check_and_coerce ($3,
                                      tao_enum_constant_decl);
              tao_enum_constant_decl = 0;

              // If the expression is a template parameter place
              // holder, 'result' will be 0, but it's ok.
              if (result == 0 && param_holder == 0)
                {
                  idl_global->err ()->coercion_error ($9,
                                                      $3);
                  $9->destroy ();
                  delete $9;
                  $9 = 0;
                }
              else
                {
                  AST_Expression::ExprType et =
                    $3;

                  if (param_holder != 0
                      && et != param_holder->info ()->const_type_)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());
                    }
                  else
                    {
                      c =
                        idl_global->gen ()->create_constant (
                          $3,
                          $9,
                          &n);

                      (void) s->fe_add_constant (c);
                    }
                }

              $5->destroy ();
              delete $5;
              $5 = 0;

              delete result;
              result = 0;
            }
        }
        ;

const_type
        : integer_type
        | char_type
        | octet_type
        | boolean_type
        | floating_pt_type
        | fixed_type
        | string_type_spec
        {
// const_type
//      : integer_type
//      | char_type
//      | octet_type
//      | boolean_type
//      | floating_pt_type
//      | fixed_type
//      | string_type_spec
          $$ = AST_Expression::EV_string;
        }
        | wstring_type_spec
        {
//      | wstring_type_spec
          $$ = AST_Expression::EV_wstring;
        }
        | scoped_name
        {
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_PredefinedType *c = 0;
          AST_Typedef *t = 0;
          UTL_ScopedName *sn = $1;

          /*
           * If the constant's type is a scoped name, it must resolve
           * to a scalar constant type
           */
          AST_Decl *d =
            s->lookup_by_name (sn);

          if (s != 0 && d != 0)
            {
              /*
               * Look through typedefs.
               */
              while (d->node_type () == AST_Decl::NT_typedef)
                {
                  t = AST_Typedef::narrow_from_decl (d);

                  if (t == 0)
                    {
                      break;
                    }

                  d = t->base_type ();
                }

              if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  c = AST_PredefinedType::narrow_from_decl (d);

                  $<etval>$ = FE_Utils::PredefinedTypeToExprType (c->pt ());
                }
              else if (d->node_type () == AST_Decl::NT_string)
                {
                  $<etval>$ = AST_Expression::EV_string;
                }
              else if (d->node_type () == AST_Decl::NT_wstring)
                {
                  $<etval>$ = AST_Expression::EV_wstring;
                }
              else if (d->node_type () == AST_Decl::NT_enum)
                {
                  $<etval>$ = AST_Expression::EV_enum;
                  tao_enum_constant_decl =
                    AST_Enum::narrow_from_decl (d);
                }
              else
                {
                  idl_global->err ()->constant_expected (sn, d);
                }
            }
          else
            {
              idl_global->err ()->lookup_error (sn);
            }

          sn->destroy ();
          delete sn;
          sn = 0;
          $1 = 0;
        }
        ;

expression : const_expr ;

const_expr : or_expr ;

or_expr : xor_expr
        | or_expr '|' xor_expr
        {
// or_expr : xor_expr
//      | or_expr '|' xor_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    $1,
                                    $3
                                  );
        }
        ;

xor_expr
        : and_expr
        | xor_expr '^' and_expr
        {
// xor_expr : and_expr
//      | xor_expr '^' and_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    $1,
                                    $3
                                  );
        }
        ;

and_expr
        : shift_expr
        | and_expr '&' shift_expr
        {
// and_expr : shift_expr | and_expr '&' shift_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    $1,
                                    $3
                                  );
        }
        ;

shift_expr
        : add_expr
        | shift_expr IDL_LEFT_SHIFT add_expr
        {
// shift_expr : add_expr | shift_expr IDL_LEFT_SHIFT add_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    $1,
                                    $3
                                  );
        }
        | shift_expr IDL_RIGHT_SHIFT add_expr
        {
//      | shift_expr IDL_RIGHT_SHIFT add_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    $1,
                                    $3
                                  );
        }
        ;

add_expr
        : mult_expr
        | add_expr '+' mult_expr
        {
// add_expr : mult_expr | add_expr '+' mult_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    $1,
                                    $3
                                  );
        }
        | add_expr '-' mult_expr
        {
//      | add_expr '-' mult_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    $1,
                                    $3
                                  );
        }
        ;

mult_expr
        : unary_expr
        | mult_expr '*' unary_expr
        {
// mult_expr : unary_expr | mult_expr '*' unary_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    $1,
                                    $3
                                  );
        }
        | mult_expr '/' unary_expr
        {
//      | mult_expr '/' unary_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    $1,
                                    $3
                                  );
        }
        | mult_expr '%' unary_expr
        {
//      | mult_expr '%' unary_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    $1,
                                    $3
                                  );
        }
        ;

unary_expr
        : primary_expr
        | '+' primary_expr
        {
// unary_expr : primary_expr | '+' primary_expr
          $$ =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    $2,
                                    0
                                  );
        }
        | '-' primary_expr
        {
//      | '-' primary_expr
          $$ =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   $2,
                                   0
                                 );
        }
        | '~' primary_expr
        {
//      | '~' primary_expr
          $$ =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   $2,
                                   0
                                 );
        }
        ;

primary_expr
        : scoped_name
        {
// primary_expr : scoped_name
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ($1);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($1);
              return 1;
            }
          else if (d->node_type () == AST_Decl::NT_const)
            {
              /*
               * If the scoped name is an IDL constant, it
               * may be used in an array dim, a string
               * bound, or a sequence bound. If so, it
               * must be unsigned and > 0. We assign the
               * constant's value and type to the
               * expression created here so we can check
               * them later.
               */
              AST_Constant *c =
                AST_Constant::narrow_from_decl (d);

              $<exval>$ =
                idl_global->gen ()->create_expr (
                  c->constant_value (),
                  c->et ());
            }
          else
            {
              // An AST_Expression owns the scoped name
              // passed in this constructor, so we copy it
              // and destroy it below no matter which case
              // is followed.
              $<exval>$ =
                idl_global->gen ()->create_expr (
                  $1->copy ());
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        | literal
        | '(' const_expr ')'
        {
//      | literal
//      | '(' const_expr ')'
          $$ = $2;
        }
        ;

literal
        : IDL_INTEGER_LITERAL
        {
// literal : IDL_INTEGER_LITERAL
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_UINTEGER_LITERAL
        {
//      | IDL_UINTEGER_LITERAL
          $$ =
            idl_global->gen ()->create_expr ($1);
        }
        | IDL_STRING_LITERAL
        {
//      | IDL_STRING_LITERAL
          $$ = idl_global->gen ()->create_expr ($1);
          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        | IDL_WSTRING_LITERAL
        {
//      | IDL_WSTRING_LITERAL
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_CHARACTER_LITERAL
        {
//      | IDL_CHARACTER_LITERAL
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_WCHAR_LITERAL
        {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ($1);
          $$ = idl_global->gen ()->create_expr (wc);
        }
        | IDL_FLOATING_PT_LITERAL
        {
//      | IDL_FLOATING_PT_LITERAL
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_TRUETOK
        {
//      | IDL_TRUETOK
          $$ = idl_global->gen ()->create_expr (true);
        }
        | IDL_FALSETOK
        {
//      | IDL_FALSETOK
          $$ = idl_global->gen ()->create_expr (false);
        }
        ;

positive_int_expr :
        const_expr
        {
// positive_int_expr : const_expr
          int good_expression = 1;
          $1->evaluate (AST_Expression::EK_positive_int);
          AST_Expression::AST_ExprValue *ev = $1->ev ();

          /*
           * If const_expr is an enum value (AST_EnumVal inherits from
           * AST_Constant), the AST_ExprValue will probably not be set,
           * but there's no need to check anyway
           */
          if (ev != 0)
            {
              switch (ev->et)
              {
                case AST_Expression::EV_ushort:
                  if (ev->u.usval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_ulong:
                  if (ev->u.ulval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
#if !defined (ACE_LACKS_LONGLONG_T)
                case AST_Expression::EV_ulonglong:
                  if (ev->u.ullval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
#endif
                case AST_Expression::EV_octet:
                  if (ev->u.oval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_bool:
                  if (ev->u.bval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                default:
                  good_expression = 0;
                  break;
              }
            }

          if (good_expression)
            {
              $$ = $1;
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
        ;

type_dcl
        : IDL_TYPEDEF
        {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
        type_declarator
        {
//      type_declarator
          $$ = 0;
        }
        | struct_type
        {
//      | struct_type
          $$ = 0;
        }
        | union_type
        {
//      | union_type
          $$ = 0;
        }
        | enum_type
        {
//      | enum_type
          $$ = 0;
        }
        | IDL_NATIVE simple_declarator
        {
//      | IDL_NATIVE simple_declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Native *node = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_NativeSeen);

          /*
           * Create a node representing a Native and add it to its
           * enclosing scope
           */
          if (s != 0)
            {
              node =
                idl_global->gen ()->create_native (
                                        $2->name ()
                                      );
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_native (node);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        | constructed_forward_type_spec
        {
//      | constructed_forward_type_spec
          $$ = 0;
        }
        ;

type_declarator :
        type_spec
        {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
        at_least_one_declarator
        {
//      at_least_one_declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Typedef *t = 0;
          unsigned long index = 0UL;
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclaratorsSeen);

          /*
           * Create a list of type renamings. Add them to the
           * enclosing scope
           */
          if (s != 0 && $1 != 0 && $3 != 0)
            {
              for (UTL_DecllistActiveIterator l ($3);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type * tp = d->compose ($1);

                  if (tp == 0)
                    {
                      continue;
                    }

                  if (AST_Decl::NT_except == tp->node_type ())
                    {
                      idl_global->err ()->not_a_type (tp);
                      continue;
                    }

                  t = idl_global->gen ()->create_typedef (tp,
                                                          d->name (),
                                                          s->is_local (),
                                                          s->is_abstract ());

                  // If the base type is a sequence or array, the typedef
                  // constructor sets owns_base_type_ to true. But if
                  // there is a comma-separated list of such typedefs,
                  // the base type can be destroyed only once. In all
                  // other cases, the line below has no effect.
                  if (index++ > 0)
                    {
                      t->owns_base_type (false);
                    }

                  (void) s->fe_add_typedef (t);
                  idl_global->in_typedef (false);
                }

              // This FE_Declarator class isn't destroyed with the AST.
              $3->destroy ();
              delete $3;
              $3 = 0;
            }
        }
        ;

type_spec
        : simple_type_spec
        | constructed_type_spec
        ;

simple_type_spec
        : base_type_spec
        {
// simple_type_spec : base_type_spec
          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 $1
                                               );
        }
        | template_type_spec
        | scoped_name
        {
//      | template_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d =
                s->lookup_by_name ($1);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($1);
            }
          else
            {
              d->last_referenced_as ($1);
            }


          $$ = d;
        }
        ;

base_type_spec
        : integer_type
        | floating_pt_type
        | char_type
        | boolean_type
        | octet_type
        | fixed_type
        | any_type
        | object_type
        ;

template_type_spec
        : sequence_type_spec
        | string_type_spec
        | wstring_type_spec
        ;

constructed_type_spec
        : struct_type
        | union_type
        | enum_type
        ;

constructed_forward_type_spec
        : struct_forward_type
        | union_forward_type
        ;

at_least_one_declarator :
        declarator declarators
        {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ($$,
                          UTL_DeclList ($1,
                                        $2),
                          1);
        }
        ;

declarators
        : declarators
          ','
        {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
        declarator
        {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ($4,
                                        0),
                          1);

          if ($1 == 0)
            {
              $<dlval>$ = dl;
            }
          else
            {
              $1->nconc (dl);
              $<dlval>$ = $1;
            }
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $<dlval>$ = 0;
        }
        ;

declarator
        : simple_declarator
        | complex_declarator
        ;

at_least_one_simple_declarator :
        simple_declarator simple_declarators
        {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ($<dlval>$,
                          UTL_DeclList ($1,
                                        $2),
                          1);
        }
        ;

simple_declarators
        : simple_declarators
          ','
        {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
        simple_declarator
        {
//      simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ($4,
                                        0),
                          1);

          if ($1 == 0)
            {
              $<dlval>$ = dl;
            }
          else
            {
              $1->nconc (dl);
              $<dlval>$ = $1;
            }
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $<dlval>$ = 0;
        }
        ;

simple_declarator :
        id
        {
// simple_declarator : id
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ($1,
                                          0),
                          1);
          ACE_NEW_RETURN ($<deval>$,
                          FE_Declarator (sn,
                                         FE_Declarator::FD_simple,
                                         0),
                          1);
        }
        ;

complex_declarator :
        array_declarator
        {
// complex_declarator : array_declarator
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (
                              $1->local_name ()->copy (),
                              0
                            ),
                          1);
          ACE_NEW_RETURN ($<deval>$,
                          FE_Declarator (sn,
                                         FE_Declarator::FD_complex,
                                         $1),
                          1);
        }
        ;

integer_type
        : signed_int
        | unsigned_int
        ;

signed_int
        : IDL_LONG
        {
// signed_int : IDL_LONG
          $$ = AST_Expression::EV_long;
        }
        | IDL_LONG IDL_LONG
        {
//      | IDL_LONG IDL_LONG
          $$ = AST_Expression::EV_longlong;
        }
        | IDL_SHORT
        {
//      | IDL_SHORT
          $$ = AST_Expression::EV_short;
        }
        ;

unsigned_int
        : IDL_UNSIGNED IDL_LONG
        {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          $$ = AST_Expression::EV_ulong;
        }
        | IDL_UNSIGNED IDL_LONG IDL_LONG
        {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          $$ = AST_Expression::EV_ulonglong;
        }
        | IDL_UNSIGNED IDL_SHORT
        {
//      | IDL_UNSIGNED IDL_SHORT
          $$ = AST_Expression::EV_ushort;
        }
        ;

floating_pt_type
        : IDL_DOUBLE
        {
// floating_pt_type : IDL_DOUBLE
          $$ = AST_Expression::EV_double;
        }
        | IDL_FLOAT
        {
//      | IDL_FLOAT
          $$ = AST_Expression::EV_float;
        }
        | IDL_LONG IDL_DOUBLE
        {
//      | IDL_LONG IDL_DOUBLE
          $$ = AST_Expression::EV_longdouble;
        }
        ;

fixed_type
        : IDL_FIXED
        {
// fixed_type : IDL_FIXED
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %C line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" fixed types yet\n")));
        }
        ;

char_type
        : IDL_CHAR
        {
// char_type : IDL_CHAR
          $$ = AST_Expression::EV_char;
        }
        | IDL_WCHAR
        {
//      | IDL_WCHAR
          $$ = AST_Expression::EV_wchar;
        }
        ;

octet_type
        : IDL_OCTET
        {
// octet_type : IDL_OCTET
          $$ = AST_Expression::EV_octet;
        }
        ;

boolean_type
        : IDL_BOOLEAN
        {
// boolean_type : IDL_BOOLEAN
          $$ = AST_Expression::EV_bool;
        }
        ;

any_type
        : IDL_ANY
        {
// any_type : IDL_ANY
          $$ = AST_Expression::EV_any;
        }
        ;

object_type
        : IDL_OBJECT
        {
// object_type : IDL_OBJECT
          $$ = AST_Expression::EV_object;
        }
        ;

struct_decl :
        IDL_STRUCT
        {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          $$ = $3;
        }
        ;


struct_type
        : struct_decl
        {
// struct_type : struct_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);
          AST_Structure *d = 0;

          /*
           * Create a node representing a struct declaration. Add it
           * to the enclosing scope
           */
          if (s != 0)
            {
              d =
                idl_global->gen ()->create_structure (
                                        &n,
                                        s->is_local (),
                                        s->is_abstract ()
                                      );
              AST_Structure::fwd_redefinition_helper (d,
                                                      s);
              (void) s->fe_add_structure (d);
            }

          /*
           * Push the scope of the struct on the scopes stack.
           */
          idl_global->scopes ().push (d);

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
        at_least_one_member
        {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          $$ = AST_Structure::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
        ;

at_least_one_member : member members ;

members
        : members member
        | /* EMPTY */
        ;

member  :
        {
// member  :
          /* is $0 to member_i */
          $<vival>$ = AST_Field::vis_NA;
        }
        member_i
        ;

member_i :
        type_spec
        {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
        at_least_one_declarator
        {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
          ';'
        {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);

          /*
           * Check for illegal recursive use of type.
           */
          if ($1 != 0
              && AST_illegal_recursive_type ($1))
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                          $1);
            }
          /*
           * Create a node representing a struct or exception member
           * Add it to the enclosing scope.
           */
          else if (s != 0
                   && $1 != 0
                   && $3 != 0)
            {
              for (UTL_DecllistActiveIterator l ($3);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ($1);

                  if (tp == 0)
                    {
                      continue;
                    }

                  /* $0 denotes Visibility, must be on yacc reduction stack. */
                  f =
                    idl_global->gen ()->create_field (
                                            tp,
                                            d->name (),
                                            $<vival>0
                                          );
                  (void) s->fe_add_field (f);
                }
            }

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        | error
        {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

union_decl :
        IDL_UNION
        {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          $$ = $3;
        }
        ;

union_type
        : union_decl IDL_SWITCH
        {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
        '('
        {
//      '('
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);
          AST_Union *u = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);

          /*
           * Create a node representing an empty union. Add it to its enclosing
           * scope.
           */
          if (s != 0)
            {
              u = idl_global->gen ()->create_union (0,
                                                    &n,
                                                    s->is_local (),
                                                    s->is_abstract ());

              AST_Structure *st = AST_Structure::narrow_from_decl (u);
              AST_Structure::fwd_redefinition_helper (st,
                                                      s);
              u = AST_Union::narrow_from_decl (st);
              (void) s->fe_add_union (u);
            }

          /*
           * Push the scope of the union on the scopes stack
           */
          idl_global->scopes ().push (u);

          /*
           * Don't delete $1 yet; we'll need it a bit later.
           */
        }
        switch_type_spec
        {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
        ')'
        {
//      ')'
          /*
           * The top of the scopes must an empty union we added after we
           * encountered 'union <id> switch ('. Now we are ready to add a
           * correct one. Temporarily remove the top so that we setup the
           * correct union in a right scope. But first save pragma prefix
           * since UTL_ScopeStack::pop() removes it.
           */
          char *prefix = 0;
          idl_global->pragma_prefixes ().top (prefix);
          prefix = ACE::strnew (prefix);
          UTL_Scope *top = idl_global->scopes ().top_non_null ();
          idl_global->scopes ().pop ();

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union.
           */
          if ($6 != 0
              && s != 0)
            {
              AST_ConcreteType *tp =
                AST_ConcreteType::narrow_from_decl ($6);

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ($6);
                }
              else
                {
                  /* Create a union with a correct discriminator. */
                  AST_Union *u = 0;
                  u = idl_global->gen ()->create_union (tp,
                                                        &n,
                                                        s->is_local (),
                                                        s->is_abstract ());
                  /* Narrow the enclosing scope. */
                  AST_Union *e = AST_Union::narrow_from_scope (top);

                  e->redefine (u);

                  u->destroy ();
                  delete u;
                }
            }

          /*
           * Restore the top.
           */
          idl_global->scopes ().push (top);
          idl_global->pragma_prefixes ().push (prefix);

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
        at_least_one_case_branch
        {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);

          /*
           * Done with this union. Pop its scope from the scopes stack.
           */
          $$ = AST_Union::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );

          if ($$ != 0)
            {
              idl_global->scopes ().pop ();
            }
        }
        ;

switch_type_spec :
        integer_type
        {
// switch_type_spec : integer_type
          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 $1
                                               );
        }
        | char_type
        {
//      | char_type
          /* wchars are not allowed. */
          if ($1 == AST_Expression::EV_wchar)
            {
              idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
            }

          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 $1
                                               );
        }
        | octet_type
        {
//      | octet_type
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 $1
                                               );
        }
        | boolean_type
        {
//      | boolean_type
          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 $1
                                               );
        }
        | enum_type
        | scoped_name
        {
//      | enum_type
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          AST_PredefinedType *p = 0;
          AST_Typedef *t = 0;
          bool found = false;

          /*
           * The discriminator is a scoped name. Try to resolve to
           * one of the scalar types or to an enum. Thread through
           * typedef's to arrive at the base type at the end of the
           * chain.
           */
          d =
            s->lookup_by_name ($1);

          if (s != 0 && d != 0)
            {
              while (!found)
                {
                  switch (d->node_type ())
                  {
                    case AST_Decl::NT_enum:
                      $$ = d;
                      found = true;
                      break;
                    case AST_Decl::NT_pre_defined:
                      p = AST_PredefinedType::narrow_from_decl (d);

                      if (p != 0)
                        {
                          switch (p->pt ())
                          {
                            case AST_PredefinedType::PT_long:
                            case AST_PredefinedType::PT_ulong:
                            case AST_PredefinedType::PT_longlong:
                            case AST_PredefinedType::PT_ulonglong:
                            case AST_PredefinedType::PT_short:
                            case AST_PredefinedType::PT_ushort:
                            case AST_PredefinedType::PT_char:
                            case AST_PredefinedType::PT_boolean:
                              $$ = p;
                              found = true;
                              break;
                            case AST_PredefinedType::PT_wchar:
                            case AST_PredefinedType::PT_octet:
                              /* octets and wchars are not allowed */
                              idl_global->err ()->error0 (
                                  UTL_Error::EIDL_DISC_TYPE
                                );
                              $$ = 0;
                              found = true;
                              break;
                            default:
                              $$ = 0;
                              found = true;
                              break;
                          }
                        }
                      else
                        {
                          $$ = 0;
                          found = true;
                        }

                      break;
                    case AST_Decl::NT_typedef:
                      t = AST_Typedef::narrow_from_decl (d);

                      if (t != 0)
                        {
                          d = t->base_type ();
                        }

                      break;
                    default:
                      $$ = 0;
                      found = true;
                      break;
                  }
                }
            }
          else
            {
              $$ = 0;
            }

          if ($$ == 0)
            {
              idl_global->err ()->lookup_error ($1);

              $1->destroy ();
              delete $1;
              $1 = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        ;

at_least_one_case_branch : case_branch case_branches ;

case_branches
        : case_branches case_branch
        | /* empty */
        ;

case_branch :
        at_least_one_case_label
        {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
        element_spec
        {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
          ';'
        {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Field *f = $3;
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);

          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0
              &&  $1 != 0
              && $3 != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        $1,
                                        f->field_type (),
                                        f->name ()
                                      );
              (void) s->fe_add_union_branch (b);

              // f has passed its field type to the union branch,
              // but the rest still needs to be cleaned up.
              f->AST_Decl::destroy ();
              delete f;
              f = 0;
            }
        }
        | error
        {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
          ';'
        {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

at_least_one_case_label :
        case_label case_labels
        {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ($<llval>$,
                          UTL_LabelList ($1,
                                         $2),
                          1);
        }
        ;

case_labels
        : case_labels case_label
        {
// case_labels : case_labels case_label
          UTL_LabelList *ll = 0;
          ACE_NEW_RETURN (ll,
                          UTL_LabelList ($2,
                                         0),
                          1);

          if ($1 == 0)
            {
              $<llval>$ = ll;
            }
          else
            {
              $1->nconc (ll);
              $<llval>$ = $1;
            }
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $<llval>$ = 0;
        }
        ;

case_label
        : IDL_DEFAULT
        {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
          ':'
        {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          $$ = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
        | IDL_CASE
        {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
          const_expr
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
          ':'
        {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          $$ = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      $3
                                    );
        }
        ;

element_spec :
        type_spec
        {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
        declarator
        {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemDeclSeen);

          /*
           * Check for illegal recursive use of type
           */
          if ($1 != 0
              && AST_illegal_recursive_type ($1))
            {
              idl_global->err()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                         $1);

              $$ = 0;
            }
          /*
           * Create a field in a union branch
           */
          else if ($1 == 0
                   || $3 == 0)
            {
              $$ = 0;
            }
          else
            {
              AST_Type *tp = $3->compose ($1);

              if (tp == 0)
                {
                  $$ = 0;
                }
              else
                {
                  $$ = idl_global->gen ()->create_field (
                                               tp,
                                               $3->name ()
                                             );
                }

              $3->destroy ();
              delete $3;
              $3 = 0;
            }
        }
        ;

struct_forward_type
        : struct_decl
        {
// struct_forward_type : struct_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);
          AST_StructureFwd *d = 0;

          /*
           * Create a node representing a forward declaration of a struct.
           */
          if (s != 0)
            {
              d = idl_global->gen ()->create_structure_fwd (&n);
              (void) s->fe_add_structure_fwd (d);
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        ;

union_forward_type
        : union_decl
        {
// union_forward_type : union_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($1,
                            0);
          AST_UnionFwd *u = 0;

          /*
           * Create a node representing a forward declaration of a union.
           */
          if (s != 0)
            {
              u = idl_global->gen ()->create_union_fwd (&n);
              (void) s->fe_add_union_fwd (u);
            }

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        ;

enum_type :
        IDL_ENUM
        {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
        id
        {
//      id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($3,
                            0);
          AST_Enum *e = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumIDSeen);

          /*
           * Create a node representing an enum and add it to its
           * enclosing scope.
           */
          if (s != 0)
            {
              e = idl_global->gen ()->create_enum (&n,
                                                   s->is_local (),
                                                   s->is_abstract ());
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_enum (e);
            }

          /*
           * Push the enum scope on the scopes stack.
           */
          idl_global->scopes ().push (e);

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
        at_least_one_enumerator
        {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumQsSeen);

          /*
           * Done with this enum. Pop its scope from the scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
              $$ = 0;
            }
          else
            {
              $$ = AST_Enum::narrow_from_scope (
                       idl_global->scopes ().top_non_null ()
                     );
              idl_global->scopes ().pop ();
            }
        }
        ;

at_least_one_enumerator : enumerator enumerators ;

enumerators
        : enumerators
          ','
        {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
          enumerator
        | /* EMPTY */
        ;

enumerator :
        IDENTIFIER
        {
// enumerator : IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ($1);
          ACE::strdelete ($1);
          $1 = 0;
          UTL_ScopedName n (&id,
                            0);
          AST_EnumVal *e = 0;
          AST_Enum *c = 0;

          /*
           * Create a node representing one enumerator in an enum
           * Add it to the enclosing scope (the enum scope)
           */
          if (s != 0
              && s->scope_node_type () == AST_Decl::NT_enum)
            {
              c = AST_Enum::narrow_from_scope (s);

              if (c != 0)
                {
                  e = idl_global->gen ()->create_enum_val (
                                              c->next_enum_val (),
                                              &n
                                            );
                }

              (void) s->fe_add_enum_val (e);
            }
        }
        ;

sequence_type_spec
        : seq_head
          ','
        {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
        positive_int_expr
        {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
          '>'
        {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          /*
           * Remove sequence marker from scopes stack.
           */
          if (idl_global->scopes ().top() == 0)
            {
              idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a sequence
           */
          AST_Expression::AST_ExprValue *ev = 0;
          AST_Param_Holder *param_holder = 0;

          if ($4 != 0)
            {
              param_holder =
                $4->param_holder ();

              ev = $4->coerce (AST_Expression::EV_ulong);
            }

          // If the expression corresponds to a template parameter,
          // it's ok for the coercion to fail at this point. We check
          // for a type mismatch below.
          if (0 == $4
              || (0 == ev && 0 == param_holder))
            {
              idl_global->err ()->coercion_error ($4,
                                                  AST_Expression::EV_ulong);
              $<dcval>$ = 0;
            }
          else if (0 == $1)
            {
              $<dcval>$ = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl ($1);

              if (0 == tp)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id,
                                     0);

                  $$ =
                    idl_global->gen ()->create_sequence (
                                            $4,
                                            tp,
                                            &sn,
                                            s->is_local (),
                                            s->is_abstract ()
                                          );

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete ev;
          ev = 0;
        }
        | seq_head
          '>'
        {
//      | seq_head '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          /*
           * Remove sequence marker from scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
             idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a sequence.
           */
          if ($1 == 0)
            {
              $$ = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl ($1);

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id, 0);
                  ACE_CDR::ULong bound = 0UL;

                  $$ =
                    idl_global->gen ()->create_sequence (
                        idl_global->gen ()->create_expr (
                                                bound,
                                                AST_Expression::EV_ulong
                                              ),
                        tp,
                        &sn,
                        s->is_local (),
                        s->is_abstract ()
                      );

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }
        }
        ;

seq_head :
        IDL_SEQUENCE
        {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
        '<'
        {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
        simple_type_spec
        {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          $$ = $5;
        }
        ;

string_type_spec
        : string_head
          '<'
        {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
        positive_int_expr
        {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
        '>'
        {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if ($4 != 0)
            {
              ev = $4->coerce (AST_Expression::EV_ulong);
            }

          if (0 == $4 || 0 == ev)
            {
              idl_global->err ()->coercion_error ($4,
                                                  AST_Expression::EV_ulong);
              $$ = 0;
            }
          else
            {
              $$ = idl_global->gen ()->create_string ($4);
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl (
                                                  $$
                                                )
                                            );

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete ev;
          ev = 0;
        }
        | string_head
        {
//      | string_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);
          /*
           * Create a node representing a string.
           */
          ACE_CDR::ULong bound = 0UL;

          $$ =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl (
                                              $$
                                            )
                                        );
        }
        ;

string_head :
        IDL_STRING
        {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
        ;

wstring_type_spec
        : wstring_head
          '<'
        {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
        positive_int_expr
        {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
        '>'
        {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          if ($4 == 0
              || $4->coerce (AST_Expression::EV_ulong) == 0)
            {
              idl_global->err ()->coercion_error ($4,
                                                  AST_Expression::EV_ulong);
              $$ = 0;
            }
          else
            {
              $$ = idl_global->gen ()->create_wstring ($4);
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl ($$)
                                            );

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }
        }
        | wstring_head
        {
//      | wstring_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a wstring.
           */
          ACE_CDR::ULong bound = 0UL;

          $$ =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl ($$)
                                        );
        }
        ;

wstring_head :
        IDL_WSTRING
        {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
        ;

array_declarator :
        id
        {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
        at_least_one_array_dim
        {
//      at_least_one_array_dim
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);

          /*
           * Create a node representing an array.
           */
          if ($3 != 0)
            {
              UTL_ScopedName sn ($1,
                                 0);
              $$ =
                idl_global->gen ()->create_array (
                                        &sn,
                                        $3->length (),
                                        $3,
                                        0,
                                        0
                                      );

              $3->destroy ();
              delete $3;
              $3 = 0;

              sn.destroy ();

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }
        }
        ;

at_least_one_array_dim :
        array_dim array_dims
        {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ($<elval>$,
                          UTL_ExprList ($1,
                                        $2),
                          1);
        }
        ;

array_dims
        : array_dims array_dim
        {
// array_dims : array_dims array_dim
          UTL_ExprList *el = 0;
          ACE_NEW_RETURN (el,
                          UTL_ExprList ($2,
                                        0),
                          1);

          if ($1 == 0)
            {
              $<elval>$ = el;
            }
          else
            {
              $1->nconc (el);
              $<elval>$ = $1;
            }
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $<elval>$ = 0;
        }
        ;

array_dim :
        '['
        {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
        positive_int_expr
        {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
        ']'
        {
//      ']'
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);

          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers.
           */
          AST_Expression::AST_ExprValue *ev = 0;
          AST_Param_Holder *param_holder = 0;

          if ($3 != 0)
            {
              param_holder =
                $3->param_holder ();

              ev =
                $3->coerce (AST_Expression::EV_ulong);
            }

          if (0 == $3
              || (ev == 0 && param_holder == 0))
            {
              idl_global->err ()->coercion_error ($3,
                                                  AST_Expression::EV_ulong);
              $$ = 0;
            }
          else
            {
              if (param_holder != 0)
                {
                  AST_Expression::ExprType et =
                    param_holder->info ()->const_type_;

                  // If the bound expression represents a
                  // template parameter, it must be a const
                  // and of type unsigned long.
                  if (et != AST_Expression::EV_ulong)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());

                      delete ev;
                      ev = 0;
                      return 1;
                    }
                }

              $$ = $3;
            }

          delete ev;
          ev = 0;
        }
        ;

attribute
        : attribute_readonly
        | attribute_readwrite
        ;

attribute_readonly :
        IDL_READONLY
        {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
        IDL_ATTRIBUTE
        {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
        param_type_spec
        {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
        at_least_one_simple_declarator
        {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
        opt_raises
        {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0
              && $5 != 0
              && $7 != 0)
            {
              for (UTL_DecllistActiveIterator l ($7);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ($5);

                  if (tp == 0)
                    {
                      continue;
                    }

                  a =
                    idl_global->gen ()->create_attribute (
                                            true,
                                            tp,
                                            d->name (),
                                            s->is_local (),
                                            s->is_abstract ()
                                          );

                  if ($9 != 0)
                    {
                      (void) a->fe_add_get_exceptions ($9);

                      $9->destroy ();
                      delete $9;
                      $9 = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          $7->destroy ();
          delete $7;
          $7 = 0;
        }
        ;

attribute_readwrite :
        IDL_ATTRIBUTE
        {
// attribute_readwrite : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
        param_type_spec
        {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
        at_least_one_simple_declarator
        {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
        opt_getraises
        {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
        opt_setraises
        {
//      opt_setraises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0
              && $3 != 0
              && $5 != 0)
            {
              for (UTL_DecllistActiveIterator l ($5);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ($3);

                  if (tp == 0)
                    {
                      continue;
                    }

                  a =
                    idl_global->gen ()->create_attribute (
                                            false,
                                            tp,
                                            d->name (),
                                            s->is_local (),
                                            s->is_abstract ()
                                          );

                  if ($7 != 0)
                    {
                      (void) a->fe_add_get_exceptions ($7);

                      $7->destroy ();
                      delete $7;
                      $7 = 0;
                    }

                  if ($9 != 0)
                    {
                      (void) a->fe_add_set_exceptions ($9);

                      $9->destroy ();
                      delete $9;
                      $9 = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          $5->destroy ();
          delete $5;
          $5 = 0;
        }
        ;

exception :
        IDL_EXCEPTION
        {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
        id
        {
//      id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($3,
                            0);
          AST_Exception *e = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptIDSeen);

          /*
           * Create a node representing an exception and add it to
           * the enclosing scope.
           */
          if (s != 0)
            {
              e = idl_global->gen ()->create_exception (&n,
                                                        s->is_local (),
                                                        s->is_abstract ());
              (void) s->fe_add_exception (e);
            }

          /*
           * Push the exception scope on the scope stack.
           */
          idl_global->scopes ().push (e);

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        '{'
        {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
        members
        {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

operation :
        opt_op_attribute
        op_type_spec
        {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
        IDENTIFIER
        {
//      IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ($4);
          ACE::strdelete ($4);
          $4 = 0;

          UTL_ScopedName n (&id, 0);
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (s != 0 && $2 != 0)
            {
              AST_Type *tp =
                AST_Type::narrow_from_decl ($2);

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ($2);
                }
              else if (tp->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type ($2);
                }
              else
                {
                  AST_Decl *d = ScopeAsDecl (s);
                  AST_Decl::NodeType nt = d->node_type ();
                  bool local =
                    s->is_local ()
                    || nt == AST_Decl::NT_valuetype
                    || nt == AST_Decl::NT_eventtype;

                  o =
                    idl_global->gen ()->create_operation (
                      tp,
                      $1,
                      &n,
                      local,
                      s->is_abstract ());

                  if (!local && tp->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (tp, o);
                      o->destroy ();
                      delete o;
                      o = 0;
                    }
                  else
                    {
                      (void) s->fe_add_operation (o);
                    }
                }
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
        parameter_list
        {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
        opt_raises
        {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
        opt_context
        {
//      opt_context
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpCompleted);

          /*
           * Add exceptions and context to the operation.
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_op)
            {
              o = AST_Operation::narrow_from_scope (s);

              if ($8 != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions ($8);
                }

              if ($10 != 0)
                {
                  (void) o->fe_add_context ($10);
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

opt_op_attribute
        : IDL_ONEWAY
        {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          $$ = AST_Operation::OP_oneway;
        }
        | IDL_IDEMPOTENT
        {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          $$ = AST_Operation::OP_idempotent;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = AST_Operation::OP_noflags;
        }
        ;

op_type_spec
        : param_type_spec
        | IDL_VOID
        {
// op_type_spec : param_type_spec | IDL_VOID
          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
        ;

init_decl
        : IDL_FACTORY
        {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
        IDENTIFIER
        {
//      IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (s->is_abstract ())
            {
              //@@ Fire error
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("error in %C line %d:\n")
                          ACE_TEXT ("Abstract valuetype can't have a ")
                          ACE_TEXT ("factory construct.\n"),
                          idl_global->filename ()->get_string (),
                          idl_global->lineno ()));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }

          Identifier id ($3);
          ACE::strdelete ($3);
          $3 = 0;

          UTL_ScopedName n (&id,
                            0);
          AST_Factory *factory = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an factory construct
           * and add it to its enclosing scope
           */
          if (s != 0)
            {
              factory = idl_global->gen ()->create_factory (&n);
              (void) s->fe_add_factory (factory);
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (factory);
        }
        init_parameter_list
        {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
        opt_raises
        {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if ($7 != 0)
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = AST_Factory::narrow_from_scope (s);
              (void) f->fe_add_exceptions ($7);
            }

          idl_global->scopes ().pop ();
        }
        ;

init_parameter_list
        : '('
        {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
        ')'
        {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        | '('
        {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
        at_least_one_in_parameter
        ')'
        {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        ;

at_least_one_in_parameter : in_parameter in_parameters ;

in_parameters
        : in_parameters
          ','
        {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
        in_parameter
        | /* EMPTY */
        ;

in_parameter :
        IDL_IN
        {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
        param_type_spec
        {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
        declarator
        {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ($3 != 0
              && $5 != 0 &&
              s != 0)
            {
              AST_Type *tp = $5->compose ($3);

              if (tp != 0)
                {
                  a = idl_global->gen ()->create_argument (
                          AST_Argument::dir_IN,
                          tp,
                          $5->name ()
                        );

                  (void) s->fe_add_argument (a);
                }
            }

          $5->destroy ();
          delete $5;
          $5 = 0;
        }
        ;

parameter_list
        : '('
        {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
        ')'
        {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        | '('
        {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
        at_least_one_parameter
        ')'
        {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        ;

at_least_one_parameter : parameter parameters ;

parameters
        : parameters
          ','
        {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
        parameter
        | /* EMPTY */
        ;

parameter :
        direction
        {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
        param_type_spec
        {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
        declarator
        {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ($3 != 0
              && $5 != 0
              && s != 0)
            {
              AST_Type *tp = $5->compose ($3);

              if (tp != 0)
                {
                  if (!s->is_local () && tp->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (tp, s);
                    }
                  else
                    {
                      a =
                        idl_global->gen ()->create_argument (
                            $1,
                            tp,
                            $5->name ()
                          );
                      (void) s->fe_add_argument (a);
                    }
                }
            }

          $5->destroy ();
          delete $5;
          $5 = 0;
        }
        ;

param_type_spec
        : base_type_spec
        {
// param_type_spec : base_type_spec
          $$ =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 $1
                                               );
        }
        | string_type_spec
        | wstring_type_spec
        | scoped_name
        {
//      | string_type_spec
//      | wstring_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          UTL_ScopedName *n = $1;

          if (s != 0)
            {
              d = s->lookup_by_name (n, false, false);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error (n);
              $1->destroy ();
              $1 = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }
          else
            {
              d->last_referenced_as ($1);
              AST_Decl::NodeType nt = d->node_type ();
              AST_Type *t = AST_Type::narrow_from_decl (d);
              AST_Typedef *td = 0;
              bool can_be_undefined = false;

              if (nt == AST_Decl::NT_typedef)
                {
                  // This code block ensures that a sequence of
                  // as-yet-undefined struct or union isn't used
                  // as a return type or argument.
                  td = AST_Typedef::narrow_from_decl (d);
                  AST_Type *pbt = td->primitive_base_type ();

                  if (pbt->node_type () == AST_Decl::NT_sequence)
                    {
                      t = pbt;
                      AST_Sequence *seq_type =
                        AST_Sequence::narrow_from_decl (pbt);
                      AST_Type *elem_type =
                        seq_type->base_type ();
                      AST_Decl::NodeType elem_nt =
                        elem_type->node_type ();

                      if (elem_nt == AST_Decl::NT_typedef)
                        {
                          AST_Typedef *elem_td =
                            AST_Typedef::narrow_from_decl (elem_type);
                          elem_type = elem_td->primitive_base_type ();
                          elem_nt = elem_type->node_type ();
                        }

                      if (elem_nt == AST_Decl::NT_interface
                          || elem_nt == AST_Decl::NT_interface_fwd
                          || elem_nt == AST_Decl::NT_valuetype
                          || elem_nt == AST_Decl::NT_valuetype_fwd
                          || elem_nt == AST_Decl::NT_component
                          || elem_nt == AST_Decl::NT_component_fwd)
                        {
                          can_be_undefined = true;
                        }
                    }

                  if (! t->is_defined () && ! can_be_undefined)
                    {
                      idl_global->err ()->error1 (
                        UTL_Error::EIDL_ILLEGAL_ADD,
                        (nt == AST_Decl::NT_typedef ? td : t));

                      /* If we don't return here, we'll crash later.*/
                      return 1;
                    }
                }
              else
                {
                  // For forward declared structs and unions, we
                  // want the full definition, but we need to
                  // check that it's been fully defined.
                  AST_StructureFwd *fwd =
                    AST_StructureFwd::narrow_from_decl (d);

                  if (fwd != 0)
                    {
                      t = fwd->full_definition ();

                      if (! t->is_defined ())
                        {
                          idl_global->err ()->error1 (
                            UTL_Error::EIDL_ILLEGAL_ADD,
                            t);

                          /* If we don't return here, we'll crash later.*/
                          return 1;
                        }

                      d = t;
                    }
                }
            }

          $$ = d;
        }
        ;

direction
        : IDL_IN
        {
// direction : IDL_IN
          $$ = AST_Argument::dir_IN;
        }
        | IDL_OUT
        {
//      | IDL_OUT
          $$ = AST_Argument::dir_OUT;
        }
        | IDL_INOUT
        {
//      | IDL_INOUT
          $$ = AST_Argument::dir_INOUT;
        }
        ;

opt_raises
        : IDL_RAISES
        {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
          '('
        {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
          at_least_one_scoped_name
          ')'
        {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          $$ = $5;
        }
        | /* EMPTY */
        {
          $$ = 0;
/*      |  EMPTY */
        }
        ;

opt_getraises
        : IDL_GETRAISES
        {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
          '('
        {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
          at_least_one_scoped_name
          ')'
        {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          $$ = $5;
        }
        | /* EMPTY */
        {
          $$ = 0;
/*      |  EMPTY */
        }
        ;

opt_setraises
        : IDL_SETRAISES
        {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
          '('
        {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
          at_least_one_scoped_name
          ')'
        {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          $$ = $5;
        }
        | /* EMPTY */
        {
          $$ = 0;
/*      |  EMPTY */
        }
        ;

opt_context
        : IDL_CONTEXT
        {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
        '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
        at_least_one_string_literal
        ')'
        {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          $$ = $5;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = 0;
        }
        ;

at_least_one_string_literal :
        IDL_STRING_LITERAL string_literals
        {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ($<slval>$,
                          UTL_StrList ($1,
                                       $2),
                          1);
        }
        ;

string_literals
        : string_literals
          ','
        {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
        IDL_STRING_LITERAL
        {
//      IDL_STRING_LITERAL
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ($4,
                                       0),
                          1);

          if ($1 == 0)
            {
              $<slval>$ = sl;
            }
          else
            {
              $1->nconc (sl);
              $<slval>$ = $1;
            }
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = 0;
        }
        ;

typeid_dcl
        : IDL_TYPEID scoped_name IDL_STRING_LITERAL
        {
// typeid_dcl : IDL_TYPEID scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ($2);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
            }
          else
            {
              d->set_id_with_typeid (
                     $3->get_string ()
                   );
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

typeprefix_dcl
        : IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL
        {
// typeprefix_dcl : IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = ScopeAsDecl (s);

          // If we are in a module, we want to avoid a lookup unless the
          // typeprefix is to be applied to some other scope, since we
          // might get a previous opening of the module, and the prefix
          // of this opening would never get checked or set.
          if (d->name ()->compare ($2) != 0)
            {
              d =
                s->lookup_by_name ($2);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
            }
          else
            {
              d->set_prefix_with_typeprefix (
                     $3->get_string ()
                   );
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

component
        : component_decl
        | component_forward_decl
        ;

component_forward_decl :
        IDL_COMPONENT
        id
        {
// component_forward_decl : IDL_COMPONENT id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);
          AST_ComponentFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_ComponentForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of a
           * component. Store it in the enclosing scope.
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_component_fwd (&n);
              (void) s->fe_add_component_fwd (f);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        ;

component_decl :
        component_header
        {
// component_decl : component_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Component *c = 0;

          /*
           * Make a new component node and add it to the enclosing scope.
           */
          if (s != 0 && $1 != 0)
            {
              c =
                idl_global->gen ()->create_component (
                                        $1->name (),
                                        $1->base_component (),
                                        $1->supports (),
                                        $1->n_supports (),
                                        $1->supports_flat (),
                                        $1->n_supports_flat ()
                                      );
              AST_Interface *i = AST_Interface::narrow_from_decl (c);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the component to its definition scope.
               */
              c = AST_Component::narrow_from_decl (i);
              (void) s->fe_add_component (c);

              // This FE_ComponentHeader class isn't destroyed with the AST.
              $1->destroy ();
              delete $1;
              $1 = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (c);
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
        component_exports
        {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

component_header :
        IDL_COMPONENT
        id
        {
//      component_header: IDL_COMPONENT id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
        component_inheritance_spec
        {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
        supports_spec
        {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ($2,
                                          0),
                          1);
          ACE_NEW_RETURN ($<chval>$,
                          FE_ComponentHeader (n,
                                              $4,
                                              $6,
                                              false),
                          1);

          if (0 != $6)
            {
              $6->destroy ();
              delete $6;
              $6 = 0;
            }

          if (0 != $4)
            {
              $4->destroy ();
              delete $4;
              $4 = 0;
            }
        }
        ;

component_inheritance_spec
        : ':'
        {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
          scoped_name
        {
//      scoped_name
          $<idlist>$ = $3;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = 0;
        }
        ;

component_exports
        : component_exports component_export
        | /* EMPTY */
        ;

component_export
        : provides_decl
        {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | uses_decl
        {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | emits_decl
        {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | publishes_decl
        {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | consumes_decl
        {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | attribute
        {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | extended_port_decl
        {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        ;

provides_decl : IDL_PROVIDES interface_type id
        {
// provides_decl : IDL_PROVIDES interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ($2, true, false);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
          else
            {
              int compare = 0;
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_interface:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_interface)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  case AST_Decl::NT_pre_defined:
                    // Nothing else but CORBA::Object can have
                    // this identifier.
                    compare =
                      ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object");

                    // Simple provides port must use IDL interface
                    // or CORBA::Object.
                    if (compare != 0)
                      {
                        idl_global->err ()->interface_expected (d);
                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->interface_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *port_interface_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ($3);

              UTL_ScopedName sn ($3,
                                 0);

              AST_Provides *p =
                idl_global->gen ()->create_provides (&sn,
                                                     port_interface_type);

              (void) s->fe_add_provides (p);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

interface_type
        : scoped_name
        {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          $$ = $1;
        }
        | IDL_OBJECT
        {
//      | IDL_OBJECT
          Identifier *corba_id = 0;

          ACE_NEW_RETURN (corba_id,
                          Identifier ("Object"),
                          1);

          UTL_IdList *conc_name = 0;
          ACE_NEW_RETURN (conc_name,
                          UTL_IdList (corba_id,
                                      0),
                          1);

          ACE_NEW_RETURN (corba_id,
                          Identifier ("CORBA"),
                          1);

          ACE_NEW_RETURN ($<idlist>$,
                          UTL_IdList (corba_id,
                                      conc_name),
                          1);
        }
        ;

uses_decl : uses_opt_multiple interface_type id
        {
// uses_decl : uses_opt_multiple interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ($2, true, false);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
          else
            {
              int compare = 0;
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_interface:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_interface)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  case AST_Decl::NT_pre_defined:
                    // Nothing else but CORBA::Object can have
                    // this identifier.
                    compare =
                      ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object");

                    // Simple provides port must use IDL interface
                    // or CORBA::Object.
                    if (compare != 0)
                      {
                        idl_global->err ()->interface_expected (d);
                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->interface_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *port_interface_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ($3);

              UTL_ScopedName sn ($3,
                                 0);

              AST_Uses *u =
                idl_global->gen ()->create_uses (&sn,
                                                 port_interface_type,
                                                 $1);

              (void) s->fe_add_uses (u);

              AST_Component *c =
                AST_Component::narrow_from_scope (s);

              if (c != 0
                  && u->is_multiple ()
                  && !idl_global->using_ifr_backend ()
                  && !idl_global->ignore_idl3 ()
                  && nt != AST_Decl::NT_param_holder)
                {
                  // These datatypes must be created in the
                  // front end so they can be looked up
                  // when compiling the generated executor IDL.
                  FE_Utils::create_uses_multiple_stuff (c, u);
                }
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

uses_opt_multiple
        : IDL_USES opt_multiple
        {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          $$ = $2;
        }
        ;

opt_multiple
        : IDL_MULTIPLE
        {
// opt_multiple : IDL_MULTIPLE
          $$ = true;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = false;
        }
        ;

emits_decl : IDL_EMITS scoped_name id
        {
// emits_decl : IDL_EMITS scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ($2, true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ($3);

              UTL_ScopedName sn ($3,
                                 0);

              AST_Emits *e =
                idl_global->gen ()->create_emits (&sn,
                                                  event_type);

              (void) s->fe_add_emits (e);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

publishes_decl : IDL_PUBLISHES scoped_name id
        {
// publishes_decl : IDL_PUBLISHES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ($2, true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ($3);

              UTL_ScopedName sn ($3,
                                 0);

              AST_Publishes *p =
                idl_global->gen ()->create_publishes (&sn,
                                                      event_type);

              (void) s->fe_add_publishes (p);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

consumes_decl : IDL_CONSUMES scoped_name id
        {
// consumes_decl : IDL_CONSUMES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ($2, true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();

              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;

                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());

                        so_far_so_good = false;
                      }

                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              FE_Utils::original_local_name ($3);

              UTL_ScopedName sn ($3,
                                 0);

              AST_Consumes *c =
                idl_global->gen ()->create_consumes (&sn,
                                                     event_type);

              (void) s->fe_add_consumes (c);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;

          $3->destroy ();
          delete $3;
          $3 = 0;
       }
        ;

home_decl :
        home_header
        {
// home_decl : home_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Home *h = 0;

          /*
           * Make a new home node and add it to the enclosing scope.
           */
          if (s != 0 && $1 != 0)
            {
              h =
                idl_global->gen ()->create_home (
                                        $1->name (),
                                        $1->base_home (),
                                        $1->managed_component (),
                                        $1->primary_key (),
                                        $1->supports (),
                                        $1->n_supports (),
                                        $1->supports_flat (),
                                        $1->n_supports_flat ()
                                      );
              /*
               * Add the home to its definition scope.
               */
              (void) s->fe_add_home (h);

              // This FE_HomeHeader class isn't destroyed with the AST.
              $1->destroy ();
              delete $1;
              $1 = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (h);
        }
        home_body
        {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

home_header :
        IDL_HOME
        {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
        id
        {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
        home_inheritance_spec
        {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
        supports_spec
        {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
        IDL_MANAGES
        {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
        scoped_name
        {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
        primary_key_spec
        {
//      primary_key_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_PrimaryKeySpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component home.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ($3, 0),
                          1);

          ACE_NEW_RETURN ($<hhval>$,
                          FE_HomeHeader (n,
                                         $5,
                                         $7,
                                         $11,
                                         $13),
                          1);

          $11->destroy ();
          delete $11;
          $11 = 0;

          if (0 != $5)
            {
              $5->destroy ();
              delete $5;
              $5 = 0;
            }

          if (0 != $13)
            {
              $13->destroy ();
              delete $13;
              $13 = 0;
            }

          if (0 != $7)
            {
              $7->destroy ();
              delete $7;
              $7 = 0;
            }
        }
        ;

home_inheritance_spec
        : ':'
        {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
          scoped_name
        {
//      scoped_name
          $<idlist>$ = $3;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = 0;
        }
        ;

primary_key_spec
        : IDL_PRIMARYKEY
          scoped_name
        {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          $$ = $2;
        }
        | /* EMPTY */
        {
/*      |  EMPTY */
          $$ = 0;
        }
        ;

home_body :
        '{'
        {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
        home_exports
        {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
        ;

home_exports
        : home_exports home_export
        | /* EMPTY */
        ;

home_export
        : export
        | factory_decl
        {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
          ';'
        {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | finder_decl
        {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
          ';'
        {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        ;

factory_decl :
        IDL_FACTORY
        id
        {
// factory_decl : IDL_FACTORY id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a factory operation
           * and add it to the enclosing scope.
           */
          AST_Factory *f = idl_global->gen ()->create_factory (&n);
          (void) s->fe_add_factory (f);

          $2->destroy ();
          delete $2;
          $2 = 0;

          /*
           * Push the factory scope onto the scopes stack.
           */
          idl_global->scopes ().push (f);
        }
        init_parameter_list
        {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
        opt_raises
        {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the factory.
           */
          if ($6 != 0)
            {
              (void) s->fe_add_exceptions ($6);
            }

          /*
           * Done with this factory. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

finder_decl :
        IDL_FINDER
        id
        {
// finder_decl : IDL_FINDER id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);

          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a home finder
           * and add it to the enclosing scope.
           */
          AST_Finder *f =
            idl_global->gen ()->create_finder (&n);

          (void) s->fe_add_finder (f);


          $2->destroy ();
          delete $2;
          $2 = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (f);
        }
        init_parameter_list
        {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
        opt_raises
        {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the finder.
           */
          if ($6 != 0)
            {
              (void) s->fe_add_exceptions ($6);
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

event
        : event_decl
        | event_abs_decl
        | event_forward_decl
        ;

event_forward_decl
        : event_abs_forward_decl
        | event_concrete_forward_decl
        ;

event_concrete_forward_decl :
        IDL_EVENTTYPE
        id
        {
// event_concrete_forward_decl : IDL_EVENTTYPE id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($2,
                            0);
          AST_EventTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * eventtype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_eventtype_fwd (&n,
                                                            false);
              (void) s->fe_add_valuetype_fwd (f);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        ;

event_abs_forward_decl :
        IDL_ABSTRACT
        IDL_EVENTTYPE
        id
        {
// event_abs_forward_decl : IDL_ABSTRACT IDL_EVENTTYPE id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ($3,
                            0);
          AST_EventTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * eventtype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_eventtype_fwd (&n,
                                                            true);
              (void) s->fe_add_valuetype_fwd (f);
            }

          $3->destroy ();
          delete $3;
          $3 = 0;
        }
        ;

event_abs_decl :
        event_abs_header
        event_rest_of_header
        {
// event_abs_decl : event_abs_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && $1 != 0)
            {
              UTL_ScopedName sn ($1,
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    $2->inherits (),
                    $2->n_inherits (),
                    $2->inherits_concrete (),
                    $2->inherits_flat (),
                    $2->n_inherits_flat (),
                    $2->supports (),
                    $2->n_supports (),
                    $2->supports_concrete (),
                    true,
                    false,
                    false
                  );
              i = AST_Interface::narrow_from_decl (e);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the eventetype to its definition scope
               */
              e = AST_EventType::narrow_from_decl (i);
              (void) s->fe_add_eventtype (e);
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);

          $1->destroy ();
          delete $1;
          $1 = 0;
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
        exports
        {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

event_abs_header :
        IDL_ABSTRACT
        IDL_EVENTTYPE
        id
        {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          $$ = $3;
        }
        ;

event_custom_header :
        IDL_CUSTOM
        IDL_EVENTTYPE
        id
        {
// event_custom_header : IDL_CUSTOM IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %C line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" custom yet\n")));
          $$ = 0;
        }
        ;

event_plain_header :
        IDL_EVENTTYPE
        id
        {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          $$ = $2;
        }
        ;

event_rest_of_header :
        inheritance_spec
        {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
        supports_spec
        {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          ACE_NEW_RETURN ($<vhval>$,
                          FE_OBVHeader (
                            0,
                            $1,
                            $3,
                            $1
                              ? $1->truncatable ()
                              : false,
                            true),
                          1);

          if (0 != $3)
            {
              $3->destroy ();
              delete $3;
              $3 = 0;
            }

          if (0 != $1)
            {
              $1->destroy ();
              delete $1;
              $1 = 0;
            }
        }
        ;

event_decl :
        event_header
        event_rest_of_header
        {
// event_decl : event_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && $1 != 0)
            {
              // We create the scoped name here instead of with the
              // FE_EventHeader because there is a token for it only here.
              UTL_ScopedName sn ($1,
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    $2->inherits (),
                    $2->n_inherits (),
                    $2->inherits_concrete (),
                    $2->inherits_flat (),
                    $2->n_inherits_flat (),
                    $2->supports (),
                    $2->n_supports (),
                    $2->supports_concrete (),
                    false,
                    $2->truncatable (),
                    false
                  );
              i = AST_Interface::narrow_from_decl (e);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the eventetype to its definition scope
               */
              e = AST_EventType::narrow_from_decl (i);
              (void) s->fe_add_eventtype (e);

              // FE_EventHeader is not automatically destroyed in the AST
              $2->destroy ();
              delete $2;
              $2 = 0;

              sn.destroy ();
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);
        }
        '{'
        {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
        value_elements
        {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
        '}'
        {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
        ;

event_header
        : event_custom_header
        {
// event_header : event_custom_header
          $$ = $1;
        }
        | event_plain_header
        {
// event_header : event_plain_header
          $$ = $1;
        }
        ;

formal_parameter_type
        : IDL_TYPENAME
        {
// formal_parameter_type : IDL_TYPENAME
          $<ntval>$ = AST_Decl::NT_type;
        }
        | IDL_STRUCT
        {
//        IDL_STRUCT
          $<ntval>$ = AST_Decl::NT_struct;
        }
        | IDL_UNION
        {
//        IDL_UNION
          $<ntval>$ = AST_Decl::NT_union;
        }
        | IDL_EVENTTYPE
        {
//        IDL_EVENTTYPE
          $<ntval>$ = AST_Decl::NT_eventtype;
        }
        | IDL_SEQUENCE
        {
//        IDL_SEQUENCE
          $<ntval>$ = AST_Decl::NT_sequence;
        }
        | IDL_INTERFACE
        {
//        IDL_INTERFACE
          $<ntval>$ = AST_Decl::NT_interface;
        }
        | IDL_VALUETYPE
        {
//        IDL_VALUETYPE
          $<ntval>$ = AST_Decl::NT_valuetype;
        }
        | IDL_ENUM
        {
//        IDL_ENUM
          $<ntval>$ = AST_Decl::NT_enum;
        }
        | IDL_EXCEPTION
        {
//        IDL_EXCEPTION
          $<ntval>$ = AST_Decl::NT_except;
        }
        | IDL_CONST const_type
        {
//        IDL_CONST const_type
          $<ntval>$ = AST_Decl::NT_const;
          t_param_const_type = $2;
        }
        ;

at_least_one_formal_parameter
        : formal_parameter formal_parameters
        {
// at_least_one_formal_parameter : formal_parameter formal_parameters
          if ($2 == 0)
            {
              ACE_NEW_RETURN ($2,
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          $2->enqueue_head (*$1);
          delete $1;
          $1 = 0;

          // The param added above is always the last one parsed,
          // so we check for matches between sequence<T> & T here.
          ACE_CString bad_id =
            FE_Utils::check_for_seq_of_param (
              $2);

          if (!bad_id.empty ())
            {
              delete $2;
              $2 = 0;

              idl_global->err ()->mismatch_seq_of_param (bad_id.c_str ());
              return 1;
            }

          $<plval>$ = $2;
        }
        ;

formal_parameters
        : formal_parameters ','
        {
// formal_parameters : formal_parameters ','
          // Maybe add a new parse state to set here.
        }
        formal_parameter
        {
//        formal_parameter
          if ($1 == 0)
            {
              ACE_NEW_RETURN ($1,
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          $1->enqueue_tail (*$4);
          $<plval>$ = $1;

          delete $4;
          $4 = 0;
        }
        | /* EMPTY */
        {
//        /* EMPTY */
          $<plval>$ = 0;
        }
        ;

formal_parameter
        : formal_parameter_type IDENTIFIER
        {
// formal_parameter : formal_parameter_type IDENTIFIER

          ACE_NEW_RETURN ($<pival>$,
                          FE_Utils::T_Param_Info,
                          1);

          AST_Decl::NodeType nt = $1;

          $<pival>$->type_ = nt;
          $<pival>$->name_ = $2;
          ACE::strdelete ($2);
          $2 = 0;

          if (nt == AST_Decl::NT_const)
            {
              $<pival>$->const_type_ = t_param_const_type;
              $<pival>$->enum_const_type_decl_ =
                tao_enum_constant_decl;

              // Reset these values.
              t_param_const_type = AST_Expression::EV_none;
              tao_enum_constant_decl = 0;
            }
        }
        | IDL_SEQUENCE '<' IDENTIFIER '>' IDENTIFIER
        {
          ACE_NEW_RETURN ($<pival>$,
                          FE_Utils::T_Param_Info,
                          1);

          $<pival>$->type_ = AST_Decl::NT_sequence;
          $<pival>$->seq_param_ref_  = $3;
          $<pival>$->name_ += $5;

          ACE::strdelete ($3);
          $3 = 0;
          ACE::strdelete ($5);
          $5 = 0;
        }
        ;

at_least_one_formal_parameter_name
        : formal_parameter_name formal_parameter_names
        {
// at_least_one_formal_parameter_name : formal_parameter_name formal_parameter_names
          ACE_NEW_RETURN ($<slval>$,
                          UTL_StrList ($1,
                                       $2),
                          1);
        }
        ;

formal_parameter_names
        : formal_parameter_names ',' formal_parameter_name
        {
// formal_parameter_names : formal_parameter_names ',' formal_parameter_name
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ($3,
                                       0),
                          1);

          if ($1 == 0)
            {
              $<slval>$ = sl;
            }
          else
            {
              $1->nconc (sl);
              $<slval>$ = $1;
            }
        }
        | /* EMPTY */
        {
//        /* EMPTY */
          $<slval>$ = 0;
        }
        ;

formal_parameter_name
        : IDENTIFIER
        {
// formal_parameter_name : IDENTIFIER
          ACE_NEW_RETURN ($$,
                          UTL_String ($1, true),
                          1);
        }
        ;

porttype_decl
        : IDL_PORTTYPE
        {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
        IDENTIFIER
        {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          Identifier id ($3);
          ACE::strdelete ($3);
          $3 = 0;

          UTL_ScopedName sn (&id, 0);
          AST_PortType *p =
            idl_global->gen ()->create_porttype (&sn);

          (void) s->fe_add_porttype (p);

          // Push it on the scopes stack.
          idl_global->scopes ().push (p);
        }
        '{'
        {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
        at_least_one_port_export
        {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
        '}'
        {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
        ;

at_least_one_port_export
        : port_export port_exports
        {
// at_least_one_port_export : port_export port_exports
        }
        ;

port_exports
        : port_exports port_export
        {
// port_exports : port_exports port_export
        }
        | /* EMPTY */
        {
//        | /* EMPTY */
        }
        ;

port_export
        : provides_decl
        {
// port_export : provides_decl
        }
        ';'
        {
//        ';'
        }
        | uses_decl
        {
//        | uses_decl
        }
        ';'
        {
//        ';'
        }
        | attribute
        {
//        | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
        ';'
        {
//        ';'
        }
        ;

extended_port_decl
        : IDL_PORT scoped_name IDENTIFIER
        {
// extended_port_decl : IDL_PORT scoped_name IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ($2);
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
          else
            {
              pt = AST_PortType::narrow_from_decl (d);

              if (pt == 0)
                {
                  idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                              d);
                  so_far_so_good = false;
                }
            }

          if (so_far_so_good)
            {
              Identifier id ($3);
              ACE::strdelete ($3);
              $3 = 0;

              UTL_ScopedName sn (&id,
                                 0);

              AST_Extended_Port *ep =
                idl_global->gen ()->create_extended_port (
                  &sn,
                  pt);

              (void) s->fe_add_extended_port (ep);

              // Create (in the AST) the struct(s) and sequence(s)
              // needed for multiplex uses ports, if any.
              for (UTL_ScopeActiveIterator i (pt, UTL_Scope::IK_decls);
                   !i.is_done ();
                   i.next ())
                {
                  d = i.item ();

                  AST_Uses *u = AST_Uses::narrow_from_decl (d);

                  if (u != 0 && u->is_multiple ())
                    {
                      AST_Component *c =
                        AST_Component::narrow_from_scope (s);

                      FE_Utils::create_uses_multiple_stuff (
                        c,
                        u,
                        id.get_string ());
                    }
                }
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        | IDL_MIRRORPORT scoped_name IDENTIFIER
        {
//        | IDL_MIRRORPORT scoped_name IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_MirrorPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ($2);
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ($2);
              so_far_so_good = false;
            }
           else
             {
               pt = AST_PortType::narrow_from_decl (d);

               if (pt == 0)
                 {
                   idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                               d);
                   so_far_so_good = false;
                 }
             }

          if (so_far_so_good)
            {
              Identifier id ($3);
              ACE::strdelete ($3);
              $3 = 0;

              UTL_ScopedName sn (&id,
                                 0);

              AST_Mirror_Port *mp =
                idl_global->gen ()->create_mirror_port (
                  &sn,
                  pt);

              (void) s->fe_add_mirror_port (mp);
            }

          $2->destroy ();
          delete $2;
          $2 = 0;
        }
        ;

at_least_one_actual_parameter
        : actual_parameter actual_parameters
        {
// at_least_one_actual_parameter : actual_parameter actual_parameters
          if ($2 == 0)
            {
              ACE_NEW_RETURN ($2,
                              FE_Utils::T_ARGLIST,
                              1);
            }

          $2->enqueue_head ($1);
          $<alval>$ = $2;
        }
        ;

actual_parameters
        : actual_parameters ',' actual_parameter
        {
// actual_parameters : actual_parameters ',' actual_parameter
          if ($1 == 0)
            {
              ACE_NEW_RETURN ($1,
                              FE_Utils::T_ARGLIST,
                              1);
            }

          $1->enqueue_tail ($3);
          $<alval>$ = $1;
        }
        | /* EMPTY */
        {
//         | /* EMPTY */
          $<alval>$ = 0;
        }
        ;

actual_parameter
        : expression
        {
// actual_parameter : expression
          // To avoid grammar conflicts with this LALR(1) parser,
          // we take advantage of the fact that an expression can
          // be a scoped name. At that lower level, we create an
          // expression containing the scoped name, and at a
          // higher lever, deduce that it's not supposed to be
          // a constant and look up the type to add to the template
          // arg list.
          AST_Expression *ex = $1;
          UTL_ScopedName *sn = ex->n ();
          AST_Decl *d = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (sn != 0)
            {
              d = s->lookup_by_name (sn);

              if (d == 0)
                {
                  idl_global->err ()->lookup_error (sn);
                  return 1;
                }
              else
                {
                  AST_Decl::NodeType nt = d->node_type ();

                  if (nt == AST_Decl::NT_enum_val)
                    {
                      $1->evaluate (
                        AST_Expression::EK_const);

                      $<dcval>$ =
                        idl_global->gen ()->create_constant (
                          $1->ev ()->et,
                          $1,
                          sn);
                    }
                  else
                    {
                      $<dcval>$ = d;
                    }
                }
            }
          else
            {
              $1->evaluate (
                AST_Expression::EK_const);

              $<dcval>$ =
                idl_global->gen ()->create_constant (
                  $1->ev ()->et,
                  $1,
                  0);
            }
        }
        ;

connector_decl
        : connector_header connector_body
        ;

connector_header
        : IDL_CONNECTOR
        {
// connector_header : IDL_CONNECTOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
        IDENTIFIER
        {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
        component_inheritance_spec
        {
//        component_inheritance_spec
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Connector *parent = 0;
          bool so_far_so_good = true;

          Identifier id ($3);
          ACE::strdelete ($3);
          $3 = 0;

          UTL_ScopedName sn (&id, 0);

          if ($5 != 0)
            {
              AST_Decl *d =
                s->lookup_by_name ($5);

              if (d == 0)
                {
                  idl_global->err ()->lookup_error ($5);
                  so_far_so_good = false;
                }

              parent =
                AST_Connector::narrow_from_decl (d);

              if (parent == 0)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_CONNECTOR_EXPECTED,
                    d);

                  so_far_so_good = false;
                }

              $5->destroy ();
              delete $5;
              $5 = 0;
            }

          if (so_far_so_good)
            {
              AST_Connector *c =
                idl_global->gen ()->create_connector (&sn,
                                                      parent);

              (void) s->fe_add_connector (c);

              // Push it on the scopes stack.
              idl_global->scopes ().push (c);
           }
        }
        ;

connector_body
        : '{'
        {
// connector_body " '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
        connector_exports
        {
//        connector_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
        '}'
        {
//        '}
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
        ;

connector_exports
        : connector_exports connector_export
        | /* EMPTY */
        ;

connector_export
        : provides_decl
        {
// connector_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | uses_decl
        {
//        | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | attribute
        {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | extended_port_decl
        {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
          ';'
        {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        ;

%%
/* programs */

/*
 * ???
 */
int
yywrap (void)
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 */
void
yyerror (const char *msg)
{
  ACE_ERROR ((LM_ERROR,
              "%C\n",
              msg));
}
