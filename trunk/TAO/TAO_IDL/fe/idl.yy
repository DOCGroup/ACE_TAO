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

IDL_INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
IDL_ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

IDL_INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
IDL_ANY OBLIGATION ON THE PART OF Sun OR IDL_ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IDL_IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR IDL_ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR IDL_ANY PATENTS BY
IDL_INTERFACE DEFINITION LANGUAGE CFE OR IDL_ANY PART THEREOF.

IDL_IN NO EVENT WILL SUN OR IDL_ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
IDL_ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
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
 * idl.yy - YACC grammar for IDL 1.1
 */

/* Declarations */

%{
#include "idl.h"
#include "idl_extern.h"
#include "fe_private.h"
#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern  "C" int yywrap();
#endif  // (defined(apollo) || defined(hpux)) && defined(__cplusplus)


void yyerror (const char *);
int yylex (void);
extern "C" int yywrap (void);
extern char yytext[];
extern int yyleng;
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
  FE_obv_header                 *vhval;         /* Valuetype header     */
  AST_Expression                *exval;         /* Expression value     */
  AST_UnionLabel                *ulval;         /* Union label          */
  AST_Field                     *ffval;         /* Field value          */
  AST_Field::Visibility         vival;          /* N/A, pub or priv     */
  AST_Expression::ExprType      etval;          /* Expression type      */
  AST_Argument::Direction       dival;          /* Argument direction   */
  AST_Operation::Flags          ofval;          /* Operation flags      */
  FE_Declarator                 *deval;         /* Declarator value     */
  idl_bool                      bval;           /* Boolean value        */
  long                          ival;           /* Long value           */
  unsigned long                 uival;          /* Unsigned long value  */
  double                        dval;           /* Double value         */
  float                         fval;           /* Float value          */
  char                          cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
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
                 /* OBV tokens  see OMG ptc/98-10-04 3.2.4 */
%token          IDL_ABSTRACT
%token          IDL_CUSTOM
%token          IDL_FACTORY
%token          IDL_PRIVATE
%token          IDL_PUBLIC
%token          IDL_SUPPORTS
%token          IDL_TRUNCATABLE
%token          IDL_VALUETYPE

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
%type <dcval>   param_type_spec

%type <idlist>  scoped_name
%type <slval>   opt_context at_least_one_string_literal
%type <slval>   string_literals

%type <nlval>   at_least_one_scoped_name scoped_names inheritance_spec
%type <nlval>   opt_raises supports_spec

%type <elval>   at_least_one_array_dim array_dims

%type <llval>   at_least_one_case_label case_labels

%type <dlval>   at_least_one_declarator declarators
%type <dlval>   at_least_one_simple_declarator simple_declarators

%type <ihval>   interface_header

%type <vhval>   value_header

%type <exval>   expression const_expr or_expr xor_expr and_expr shift_expr
%type <exval>   add_expr mult_expr unary_expr primary_expr literal
%type <exval>   positive_int_expr array_dim

%type <ulval>   case_label

%type <ffval>   element_spec

%type <etval>   const_type integer_type char_type boolean_type
%type <etval>   floating_pt_type any_type fixed_type signed_int
%type <etval>   unsigned_int base_type_spec octet_type

%type <dival>   direction

%type <ofval>   opt_op_attribute

%type <deval>   declarator simple_declarator complex_declarator

%type <bval>    opt_readonly, opt_truncatable

%type <idval>   interface_decl value_decl id

%type <ival>    type_dcl
%%

/*
 * Production starts here
 */
start : definitions ;

definitions
        : definitions definition
        | /* empty */
        ;

definition
        : type_dcl
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | const_dcl
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | exception
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | interface_def
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | module
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | value_def
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValuetypeDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | error
        {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
        ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

module  : IDL_MODULE
          {
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
          }
          IDENTIFIER
          {
            UTL_ScopedName *n =
                new UTL_ScopedName(new Identifier ($3), 0);
            AST_Module *m = 0;
            UTL_Scope *s = idl_global->scopes ()->top_non_null ();

            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleIDSeen);
            /*
             * Make a new module and add it to the enclosing scope
             */
            if (s != 0) {
              m = idl_global->gen ()->create_module (s, n);
              (void) s->fe_add_module (m);
            }
            /*
             * Push it on the stack
             */
            idl_global->scopes ()->push (m);
          }
          '{'
          {
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
          }
          definitions
          {
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
          }
          '}'
          {
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
            /*
             * Finished with this module - pop it from the scope stack
             */
            idl_global->scopes ()->pop ();
          }
          ;

interface_def
        : interface
        | forward
        ;

interface :
        interface_header
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Interface *i = 0;

          /*
           * Make a new interface node and add it to its enclosing scope
           */
          if (s != 0 && $1 != 0) {
            i = idl_global->gen ()->create_interface (
                                       $1->interface_name (),
                                       $1->inherits (),
                                       $1->n_inherits (),
                                       $1->inherits_flat (),
                                       $1->n_inherits_flat (),
                                       $1->is_local (),
                                       $1->is_abstract ()
                                     );
            AST_Interface::fwd_redefinition_helper (i, s);
            /*
             * Add the interface to its definition scope
             */
            (void) s->fe_add_interface (i);
          }
          /*
           * Push it on the scope stack
           */
          idl_global->scopes ()->push (i);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
        exports
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);
          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ()->pop ();
        }
        ;

interface_decl:
         IDL_INTERFACE
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
         id
         {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           $$ = $3;
         }
        ;

interface_header :
        interface_decl inheritance_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          $$ = new FE_InterfaceHeader (new UTL_ScopedName ($1, 0), $2);
        }
        |
        IDL_LOCAL interface_decl inheritance_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          $$ = new FE_Local_InterfaceHeader (new UTL_ScopedName($2, 0), $3);
        }
        |
        IDL_ABSTRACT interface_decl inheritance_spec
        {
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %s line %d\n"),
                       idl_global->filename ()->get_string (),
                       idl_global->lineno ()));
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("Sorry, I (TAO_IDL) can't handle abstract")
                       ACE_TEXT (" interfaces yet\n")));
        }
        ;

inheritance_spec
        : ':'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
          at_least_one_scoped_name
        {
          $$ = $3;
        }
        | /* EMPTY */
        {
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
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %s line %d\n"),
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
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Interface *i = 0;

          if (s != 0 && $1 != 0) {
            i = idl_global->gen ()->create_valuetype ($1->interface_name (),
                                                      $1->inherits (),
                                                      $1->n_inherits ());
            AST_Interface::fwd_redefinition_helper (i, s);
            /*
             * Add the valuetype to its definition scope
             */
            (void) s->fe_add_interface (i);
          }
          /*
           * Push it on the scope stack
           */
          idl_global->scopes ()->push (i);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
        value_elements
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);
          /*
           * Done with this interface - pop it off the scopes stack
           */
          UTL_Scope* s = idl_global->scopes ()->top ();
          AST_Interface* m = AST_Interface::narrow_from_scope (s);
          m->inherited_name_clash ();
          idl_global->scopes ()->pop ();
        }
        ;

value_abs_decl :
        IDL_ABSTRACT
        value_header
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Interface *i = 0;

          if (s != 0 && $2 != 0) {

            if ($2->n_concrete() > 0) {
              idl_global->err ()->abstract_inheritance_error ($2->interface_name ());
            }

            i = idl_global->gen()->create_valuetype ($2->interface_name (),
                                                     $2->inherits (),
                                                     $2->n_inherits ());
            i->set_abstract_valuetype ();
            AST_Interface::fwd_redefinition_helper (i, s);
            /*
             * Add the valuetype to its definition scope
             */
            (void) s->fe_add_interface (i);
          }
          /*
           * Push it on the scope stack
           */
          idl_global->scopes ()->push (i);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
        exports
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);
          /*
           * Done with this interface - pop it off the scopes stack
           */
          UTL_Scope* s = idl_global->scopes ()->top ();
          AST_Interface* m = AST_Interface::narrow_from_scope (s);
          m->inherited_name_clash ();
          idl_global->scopes ()->pop ();
        }

        ;

value_header :
        value_decl
        opt_truncatable
        inheritance_spec
        supports_spec
        {
          $$ = new FE_obv_header (new UTL_ScopedName ($1, 0), $3, $4);
        }
        ;

value_decl
        : IDL_VALUETYPE
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValuetypeSeen);
        }
        id
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValuetypeIDSeen);
           $$ = $3;
        }
        ;

opt_truncatable :
          IDL_TRUNCATABLE
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("warning in %s line %d\n"),
                        idl_global->filename ()->get_string (),
                        idl_global->lineno ()));
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("truncatable modifier not supported ")
                        ACE_TEXT ("and is ignored\n")));
            $$ = I_FALSE;
          }
        | /* EMPTY */
          {
            $$ = I_FALSE;
          }
        ;

supports_spec :
            IDL_SUPPORTS
            scoped_name
            {
              $$ = new UTL_NameList ($2, 0);
            }
        |   /* empty */
            {
              $$ = 0;
            }
        ;

value_forward_decl :
        IDL_ABSTRACT
        value_decl
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($2, 0);
          AST_InterfaceFwd *f = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ForwardDeclSeen);
          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0) {
            f = idl_global->gen()->create_valuetype_fwd (n);
            f->set_abstract_valuetype ();
            (void) s->fe_add_interface_fwd (f);
          }
        }
      |
        value_decl
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($1, 0);
          AST_InterfaceFwd *f = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ForwardDeclSeen);
          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0) {
            f = idl_global->gen ()->create_valuetype_fwd (n);
            (void) s->fe_add_interface_fwd (f);
          }
        }
        ;


value_box_decl
        : value_decl type_spec /* in this order %!?*/
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %s line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" boxes yet\n")));
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
            /* is $0 to member_i */
            $<vival>$ = AST_Field::vis_PUBLIC;
          }
          member_i
        | IDL_PRIVATE
          {
            /* is $0 to member_i */
            $<vival>$ = AST_Field::vis_PRIVATE;
          }
          member_i
        ;

exports
        : exports export
        | /* EMPTY */
        ;

export
        : type_dcl
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | const_dcl
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
        | exception
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | attribute
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | operation
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
          ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
        | error
        {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
        ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

at_least_one_scoped_name :
        scoped_name scoped_names
        {
          $$ = new UTL_NameList ($1, $2);
        }
        ;

scoped_names
        : scoped_names
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
          scoped_name
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          if ($1 == 0)
            $$ = new UTL_NameList ($4, 0);
          else {
            $1->nconc (new UTL_NameList ($4, 0));
            $$ = $1;
          }
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

scoped_name
        : id
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          $$ = new UTL_IdList($1, 0);
        }
        | IDL_SCOPE_DELIMITOR
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
          id
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          $$ = new UTL_IdList (new Identifier ($1),
                               new UTL_IdList ($3, 0));
        }
        | scoped_name
          IDL_SCOPE_DELIMITOR
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
          id
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          $1->nconc (new UTL_IdList ($4, 0));
          $$ = $1;
        }
        ;

id: IDENTIFIER
        {
            $$ = new Identifier ($1);
        }
        ;

forward :
        interface_decl
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName($1, 0);
          AST_InterfaceFwd *f = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ForwardDeclSeen);
          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0) {
            f = idl_global->gen ()->create_interface_fwd (n, 0, 0);
            (void) s->fe_add_interface_fwd (f);
          }
        }
        |
        IDL_LOCAL interface_decl
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($2, 0);
          AST_InterfaceFwd *f = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ForwardDeclSeen);
          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0) {
            f = idl_global->gen ()->create_interface_fwd (n, 1, 0);
            (void) s->fe_add_interface_fwd (f);
          }
        }
        |
        IDL_ABSTRACT interface_decl
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($2, 0);
          AST_InterfaceFwd *f = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ForwardDeclSeen);
          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0) {
            f = idl_global->gen ()->create_interface_fwd (n, 0, 1);
            (void) s->fe_add_interface_fwd (f);
          }
        }
        ;

const_dcl :
        IDL_CONST
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
        const_type
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
        id
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
        '='
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
        expression
        {
          UTL_ScopedName *n = new UTL_ScopedName ($5, 0);
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Constant *c = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);
          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope
           */
          if ($9 != 0 && s != 0) {
            if ($9->coerce ($3) == 0)
              idl_global->err ()->coercion_error ($9, $3);
            else {
              c =
                idl_global->gen ()->create_constant ($3, $9, n);
              (void) s->fe_add_constant (c);
            }
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
          $$ = AST_Expression::EV_string;
        }
        | wstring_type_spec
        {
          $$ = AST_Expression::EV_wstring;
        }
        | scoped_name
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Decl *d = 0;
          AST_PredefinedType *c = 0;
          AST_Typedef *t = 0;

          /*
           * If the constant's type is a scoped name, it must resolve
           * to a scalar constant type
           */
          if (s != 0 && (d = s->lookup_by_name ($1, I_TRUE)) != 0) {
            /*
             * Look through typedefs
             */
            while (d->node_type () == AST_Decl::NT_typedef) {
              t = AST_Typedef::narrow_from_decl (d);
              if (t == 0)
                break;
              d = t->base_type ();
            }
            if (d == 0)
              $$ = AST_Expression::EV_any;
            else if (d->node_type () == AST_Decl::NT_pre_defined) {
              c = AST_PredefinedType::narrow_from_decl (d);
              if (c != 0) {
                 $$ = idl_global->PredefinedTypeToExprType (c->pt ());
              } else {
                 $$ = AST_Expression::EV_any;
              }
            } else if (d->node_type () == AST_Decl::NT_string) {
                $$ = AST_Expression::EV_string;
            } else if (d->node_type () == AST_Decl::NT_wstring) {
                $$ = AST_Expression::EV_wstring;
            } else
              $$ = AST_Expression::EV_any;
          } else
            $$ = AST_Expression::EV_any;
        }
        ;

expression : const_expr ;

const_expr : or_expr ;

or_expr : xor_expr
        | or_expr '|' xor_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_or, 
                                                $1, 
                                                $3);
        }
        ;

xor_expr
        : and_expr
        | xor_expr '^' and_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_xor, 
                                                $1, 
                                                $3);
        }
        ;

and_expr
        : shift_expr
        | and_expr '&' shift_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_and, 
                                                $1, 
                                                $3);
        }
        ;

shift_expr
        : add_expr
        | shift_expr IDL_LEFT_SHIFT add_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_left, 
                                                $1, 
                                                $3);
        }
        | shift_expr IDL_RIGHT_SHIFT add_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_right, 
                                                $1, 
                                                $3);
        }
        ;

add_expr
        : mult_expr
        | add_expr '+' mult_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_add, 
                                                $1, 
                                                $3);
        }
        | add_expr '-' mult_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_minus, 
                                                $1, 
                                                $3);
        }
        ;

mult_expr
        : unary_expr
        | mult_expr '*' unary_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_mul, 
                                                $1, 
                                                $3);
        }
        | mult_expr '/' unary_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_div, 
                                                $1, 
                                                $3);
        }
        | mult_expr '%' unary_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_mod, 
                                                $1, 
                                                $3);
        }
        ;

unary_expr
        : primary_expr
        | '+' primary_expr
        {
          $$ = idl_global->gen ()->create_expr (AST_Expression::EC_u_plus,
                                                $2,
                                                0);
        }
        | '-' primary_expr
        {
          $$ = idl_global->gen()->create_expr (AST_Expression::EC_u_minus,
                                               $2,
                                               0);
        }
        | '~' primary_expr
        {
          $$ = idl_global->gen()->create_expr (AST_Expression::EC_bit_neg,
                                               $2,
                                               0);
        }
        ;

primary_expr
        : scoped_name
        {
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();

          AST_Decl *d = s->lookup_by_name ($1,
                                            1);

          /*
           * If the scoped name is an IDL constant, it may be used in an
           * array dim, a string bound, or a sequence bound. If so, it 
           * must be unsigned and > 0. We assign the constant's value
           * and type to the expression created here so we can check 
           * them later.
           */
          if (d != 0 && d->node_type () == AST_Decl::NT_const)
            {
              AST_Constant *c = AST_Constant::narrow_from_decl (d);
              $$ = idl_global->gen ()->create_expr (c->constant_value (),
                                                    c->et ());
            }
          else
            {
              $$ = idl_global->gen ()->create_expr ($1);
            }
        }
        | literal
        | '(' const_expr ')'
        {
          $$ = $2;
        }
        ;

literal
        : IDL_INTEGER_LITERAL
        {
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_UINTEGER_LITERAL
        {
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_STRING_LITERAL
        {
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_WSTRING_LITERAL
        {
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_CHARACTER_LITERAL
        {
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_WCHAR_LITERAL
        {
          ACE_OutputCDR::from_wchar wc ($1);
          $$ = idl_global->gen ()->create_expr (wc);
        }
        | IDL_FLOATING_PT_LITERAL
        {
          $$ = idl_global->gen ()->create_expr ($1);
        }
        | IDL_TRUETOK
        {
          $$ = idl_global->gen ()->create_expr ((idl_bool) I_TRUE,
                                                AST_Expression::EV_bool);
        }
        | IDL_FALSETOK
        {
          $$ = idl_global->gen ()->create_expr ((idl_bool) I_FALSE,
                                                AST_Expression::EV_bool);
        }
        ;

positive_int_expr :
        const_expr
        {
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
                case AST_Expression::EV_ulonglong:
                  if (ev->u.ullval == 0)
                    {
                      good_expression = 0;
                    }
                  break;
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
              $$ = 
                idl_global->gen()->create_expr ($1, 
                                                AST_Expression::EV_ulong);
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
            idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          }
          type_declarator {$$ = 0;}
        | struct_type { $$ = 0;}
        | union_type  { $$ = 0;}
        | enum_type   { $$ = 0;}
        | IDL_NATIVE simple_declarator
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Native *node = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_NativeSeen);
          /*
           * Create a node representing a Native and add it to its
           * enclosing scope
           */
          if (s != 0) {
            node = idl_global->gen ()->create_native ($2->name ());
            /*
             * Add it to its defining scope
             */
            (void) s->fe_add_native (node);
          }
        }
        ;

type_declarator :
        type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
        at_least_one_declarator
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          FE_Declarator *d = 0;
          AST_Typedef *t = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_DeclaratorsSeen);
          /*
           * Create a list of type renamings. Add them to the
           * enclosing scope
           */
          if (s != 0 && $1 != 0 && $3 != 0) {
            for (UTL_DecllistActiveIterator l ($3); !l.is_done (); l.next ()) {
              d = l.item ();
              if (d == 0)
                continue;
              AST_Type * tp = d->compose ($1);
              if (tp == 0)
                continue;
              t = idl_global->gen ()->create_typedef (tp, 
                                                      d->name (),
                                                      s->is_local (),
                                                      s->is_abstract ());
              (void) s->fe_add_typedef (t);
            }
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
          $$ = idl_global->scopes ()->bottom ()->lookup_primitive_type ($1);
        }
        | template_type_spec
        | scoped_name
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            d = s->lookup_by_name ($1, I_TRUE);
          if (d == 0)
            idl_global->err ()->lookup_error ($1);
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

at_least_one_declarator :
        declarator declarators
        {
          $$ = new UTL_DeclList ($1, $2);
        }
        ;

declarators
        : declarators
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
          declarator
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          if ($1 == 0)
            $$ = new UTL_DeclList ($4, 0);
          else {
            $1->nconc (new UTL_DeclList ($4, 0));
            $$ = $1;
          }
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

declarator
        : simple_declarator
        | complex_declarator
        ;

at_least_one_simple_declarator :
        simple_declarator simple_declarators
        {
          $$ = new UTL_DeclList ($1, $2);
        }
        ;

simple_declarators
        : simple_declarators
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
          simple_declarator
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          if ($1 == 0)
            $$ = new UTL_DeclList ($4, 0);
          else {
            $1->nconc (new UTL_DeclList ($4, 0));
            $$ = $1;
          }
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

simple_declarator :
        id
        {
          $$ = new FE_Declarator (new UTL_ScopedName ($1, 0),
                                  FE_Declarator::FD_simple, 0);
        }
        ;

complex_declarator :
        array_declarator
        {
          $$ = new FE_Declarator (new UTL_ScopedName ($1->local_name (), 
                                                      0),
                                  FE_Declarator::FD_complex,
                                  $1);
        }
        ;

integer_type
        : signed_int
        | unsigned_int
        ;

signed_int
        : IDL_LONG
        {
          $$ = AST_Expression::EV_long;
        }
        | IDL_LONG IDL_LONG
        {
          $$ = AST_Expression::EV_longlong;
        }
        | IDL_SHORT
        {
          $$ = AST_Expression::EV_short;
        }
        ;

unsigned_int
        : IDL_UNSIGNED IDL_LONG
        {
          $$ = AST_Expression::EV_ulong;
        }
        | IDL_UNSIGNED IDL_LONG IDL_LONG
        {
          $$ = AST_Expression::EV_ulonglong;
        }
        | IDL_UNSIGNED IDL_SHORT
        {
          $$ = AST_Expression::EV_ushort;
        }
        ;

floating_pt_type
        : IDL_DOUBLE
        {
          $$ = AST_Expression::EV_double;
        }
        | IDL_FLOAT
        {
          $$ = AST_Expression::EV_float;
        }
        | IDL_LONG IDL_DOUBLE
        {
          $$ = AST_Expression::EV_longdouble;
        }
        ;

fixed_type
        : IDL_FIXED
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %s line %d\n"),
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
          $$ = AST_Expression::EV_char;
        }
        | IDL_WCHAR
        {
          $$ = AST_Expression::EV_wchar;
        }
        ;

octet_type
        : IDL_OCTET
        {
          $$ = AST_Expression::EV_octet;
        }
        ;

boolean_type
        : IDL_BOOLEAN
        {
          $$ = AST_Expression::EV_bool;
        }
        ;

any_type
        : IDL_ANY
        {
          $$ = AST_Expression::EV_any;
        }
        ;

struct_type :
        IDL_STRUCT
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
        id
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($3, 0);
          AST_Structure *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          /*
           * Create a node representing a struct declaration. Add it
           * to the enclosing scope
           */
          if (s != 0) {
            d = idl_global->gen ()->create_structure (n,
                                                      s->is_local (),
                                                      s->is_abstract ());
            (void) s->fe_add_structure (d);
          }
          /*
           * Push the scope of the struct on the scopes stack
           */
          idl_global->scopes ()->push (d);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
        at_least_one_member
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);
          /*
           * Done with this struct. Pop its scope off the scopes stack
           */
          $$ = AST_Structure::narrow_from_scope (
                   idl_global->scopes ()->top_non_null ()
                 );
          idl_global->scopes ()->pop ();
        }
        ;

at_least_one_member : member members ;

members
        : members member
        | /* EMPTY */
        ;

member  :
        {
          /* is $0 to member_i */
          $<vival>$ = AST_Field::vis_NA;
        }
        member_i
        ;

member_i:
        type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
        at_least_one_declarator
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
        ';'
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);
          /*
           * Check for illegal recursive use of type
           */
          if ($1 != 0 && AST_illegal_recursive_type ($1))
            idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE, 
                                        $1);
          /*
           * Create a node representing a struct or exception member
           * Add it to the enclosing scope
           */
          else if (s != 0 && $1 != 0 && $3 != 0) {
            for (UTL_DecllistActiveIterator l ($3); !(l.is_done ()); l.next ()) {
              d = l.item ();
              if (d == 0)
                continue;
              AST_Type *tp = d->compose ($1);
              if (tp == 0)
                continue;
              /* $0 denotes Visibility, must be on yacc reduction stack */
              f = idl_global->gen ()->create_field (tp, 
                                                    d->name (), 
                                                    $<vival>0);
              (void) s->fe_add_field (f);
            }
          }
        }
        | error
        {
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
        ';'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

union_type :
        IDL_UNION
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
        id
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
        }
        IDL_SWITCH
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
        '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
        switch_type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
        ')'
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($3, 0);
          AST_Union *u = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);
          /*
           * Create a node representing a union. Add it to its enclosing
           * scope
           */
          if ($9 != 0 && s != 0) {
            AST_ConcreteType *tp = AST_ConcreteType::narrow_from_decl ($9);
            if (tp == 0) {
              idl_global->err ()->not_a_type ($9);
            } else {
              u = idl_global->gen ()->create_union (tp,
                                                    n,
                                                    s->is_local (),
                                                    s->is_abstract ());
              (void) s->fe_add_union (u);
            }
          }
          /*
           * Push the scope of the union on the scopes stack
           */
          idl_global->scopes()->push (u);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
        at_least_one_case_branch
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);
          /*
           * Done with this union. Pop its scope from the scopes stack
           */
          $$ = AST_Union::narrow_from_scope (
                   idl_global->scopes ()->top_non_null ()
                 );
          if ($$ != 0)
            {
              idl_global->scopes ()->pop ();
            }
        }
        ;

switch_type_spec :
        integer_type
        {
          $$ = idl_global->scopes ()->bottom ()->lookup_primitive_type ($1);
        }
        | char_type
        {
          /* wchars are not allowed */
          if ($1 == AST_Expression::EV_wchar)
            {
              idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
            }

          $$ = idl_global->scopes ()->bottom ()->lookup_primitive_type ($1);
        }
        | octet_type
        {
          /* octets are not allowed */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          $$ = idl_global->scopes ()->bottom ()->lookup_primitive_type ($1);
        }
        | boolean_type
        {
          $$ = idl_global->scopes ()->bottom ()->lookup_primitive_type ($1);
        }
        | enum_type
        | scoped_name
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Decl *d = 0;
          AST_PredefinedType *p = 0;
          AST_Typedef *t = 0;
          long found = I_FALSE;

          /*
           * The discriminator is a scoped name. Try to resolve to
           * one of the scalar types or to an enum. Thread through
           * typedef's to arrive at the base type at the end of the
           * chain
           */
          if (s != 0 && (d = s->lookup_by_name ($1, I_TRUE)) != 0) {
            while (!found) {
              switch (d->node_type ()) {
              case AST_Decl::NT_enum:
                $$ = d;
                found = I_TRUE;
                break;
              case AST_Decl::NT_pre_defined:
                p = AST_PredefinedType::narrow_from_decl (d);
                if (p != 0) {
                  switch (p->pt ()) {
                  case AST_PredefinedType::PT_long:
                  case AST_PredefinedType::PT_ulong:
                  case AST_PredefinedType::PT_longlong:
                  case AST_PredefinedType::PT_ulonglong:
                  case AST_PredefinedType::PT_short:
                  case AST_PredefinedType::PT_ushort:
                  case AST_PredefinedType::PT_char:
                  case AST_PredefinedType::PT_boolean:
                    $$ = p;
                    found = I_TRUE;
                    break;
                  case AST_PredefinedType::PT_wchar:
                  case AST_PredefinedType::PT_octet:
                    /* octets and wchars are not allowed */
                    idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
                    $$ = 0;
                    found = I_TRUE;
                    break;
                  default:
                    $$ = 0;
                    found = I_TRUE;
                    break;
                  }
                } else
                {
                    $$ = 0;
                    found = I_TRUE;
                }
                break;
              case AST_Decl::NT_typedef:
                t = AST_Typedef::narrow_from_decl (d);
                if (t != 0) d = t->base_type ();
                break;
              default:
                $$ = 0;
                found = I_TRUE;
                break;
              }
            }
          } else
            $$ = 0;

          if ($$ == 0)
            idl_global->err ()->lookup_error ($1);
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
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
        element_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
        ';'
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Field *f = $3;

          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);
          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0 && $1 != 0 && $3 != 0) {
              b = idl_global->gen ()->create_union_branch ($1,
                                                           f->field_type (),
                                                           f->name ());
              (void) s->fe_add_union_branch (b);
          }
        }
        | error
        {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
        ';'

        {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
        ;

at_least_one_case_label :
        case_label case_labels
        {
          $$ = new UTL_LabelList ($1, $2);
        }
        ;

case_labels
        : case_labels case_label
        {
          if ($1 == 0)
            $$ = new UTL_LabelList ($2, 0);
          else {
            $1->nconc(new UTL_LabelList ($2, 0));
            $$ = $1;
          }
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

case_label
        : IDL_DEFAULT
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
          ':'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          $$ = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
        | IDL_CASE
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
          const_expr
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
        ':'
        {
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
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
        declarator
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemDeclSeen);
          /*
           * Check for illegal recursive use of type
           */
          if ($1 != 0 && AST_illegal_recursive_type ($1))
            idl_global->err()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE, $1);
          /*
           * Create a field in a union branch
           */
          else if ($1 == 0 || $3 == 0)
            $$ = 0;
          else {
            AST_Type *tp = $3->compose ($1);
            if (tp == 0)
              $$ = 0;
            else
              $$ = idl_global->gen ()->create_field (tp,
                                                     $3->name());
          }
        }
        ;

enum_type :
        IDL_ENUM
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
        id
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName($3, 0);
          AST_Enum *e = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_EnumIDSeen);
          /*
           * Create a node representing an enum and add it to its
           * enclosing scope
           */
          if (s != 0) {
            e = idl_global->gen ()->create_enum (n,
                                                 s->is_local (),
                                                 s->is_abstract ());
            /*
             * Add it to its defining scope
             */
            (void) s->fe_add_enum (e);
          }
          /*
           * Push the enum scope on the scopes stack
           */
          idl_global->scopes ()->push (e);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
        at_least_one_enumerator
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumQsSeen);
          /*
           * Done with this enum. Pop its scope from the scopes stack
           */
          if (idl_global->scopes ()->top () == 0)
            $$ = 0;
          else {
            $$ = AST_Enum::narrow_from_scope (
                     idl_global->scopes ()->top_non_null ()
                   );
            idl_global->scopes ()->pop ();
          }
        }
        ;

at_least_one_enumerator : enumerator enumerators ;

enumerators
        : enumerators
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
          enumerator
        | /* EMPTY */
        ;

enumerator :
        IDENTIFIER
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n =
                new UTL_ScopedName (new Identifier ($1), 0);
          AST_EnumVal *e = 0;
          AST_Enum *c = 0;

          /*
           * Create a node representing one enumerator in an enum
           * Add it to the enclosing scope (the enum scope)
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_enum) {
            c = AST_Enum::narrow_from_scope (s);
            if (c != 0)
              e = idl_global->gen ()->create_enum_val (c->next_enum_val (), 
                                                       n);
            (void) s->fe_add_enum_val (e);
          }
        }
        ;

sequence_type_spec
        : seq_head
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
        positive_int_expr
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
          '>'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);
          /*
           * Remove sequence marker from scopes stack
           */
          if (idl_global->scopes ()->top() == 0)
            idl_global->scopes ()->pop ();
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          /*
           * Create a node representing a sequence
           */
          if ($4 == 0 || $4->coerce(AST_Expression::EV_ulong) == 0) {
            idl_global->err ()->coercion_error ($4, AST_Expression::EV_ulong);
            $$ = 0;
          } else if ($1 == 0) {
            $$ = 0;
          } else {
            AST_Type *tp = AST_Type::narrow_from_decl ($1);
            if (tp == 0)
              ; // Error will be caught in FE_Declarator.
            else {
              $$ = idl_global->gen ()->create_sequence ($4,
                                                        tp,
                                                        s->is_local (),
                                                        s->is_abstract ());
            }
          }
        }
        | seq_head
          '>'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);
          /*
           * Remove sequence marker from scopes stack
           */
          if (idl_global->scopes ()->top () == 0)
            idl_global->scopes ()->pop ();
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          /*
           * Create a node representing a sequence
           */
          if ($1 == 0)
            $$ = 0;
          else {
            AST_Type *tp = AST_Type::narrow_from_decl ($1);
            if (tp == 0)
              ; // Error will be caught in FE_Declarator.
            else {
              $$ =
                idl_global->gen ()->create_sequence (
                    idl_global->gen ()->create_expr ((unsigned long) 0),
                    tp,
                    s->is_local (),
                    s->is_abstract ()
                  );
            }
          }
        }
        ;

seq_head:
        IDL_SEQUENCE
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);
          /*
           * Push a sequence marker on scopes stack
           */
          idl_global->scopes ()->push (0);
        }
        '<'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
        simple_type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          $$ = $5;
        }
        ;

string_type_spec
        : string_head
          '<'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
        positive_int_expr
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
        '>'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);
          /*
           * Create a node representing a string
           */
          if ($4 == 0 || $4->coerce (AST_Expression::EV_ulong) == 0) {
            idl_global->err ()->coercion_error ($4, AST_Expression::EV_ulong);
            $$ = 0;
          } else {
            $$ = idl_global->gen ()->create_string ($4);
            /*
             * Add this AST_String to the types defined in the global scope
             */
            (void) idl_global->root ()->fe_add_string (
                                            AST_String::narrow_from_decl ($$)
                                          );
          }
        }
        | string_head
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);
          /*
           * Create a node representing a string
           */
          $$ =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr ((unsigned long) 0)
              );
          /*
           * Add this AST_String to the types defined in the global scope
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl ($$)
                                        );
        }
        ;

string_head:
        IDL_STRING
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
        ;

wstring_type_spec
        : wstring_head
          '<'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
        positive_int_expr
        {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
        '>'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);
          /*
           * Create a node representing a string
           */
          if ($4 == 0 || $4->coerce(AST_Expression::EV_ulong) == 0) {
            idl_global->err ()->coercion_error ($4, AST_Expression::EV_ulong);
            $$ = 0;
          } else {
            $$ = idl_global->gen ()->create_wstring ($4);
            /*
             * Add this AST_String to the types defined in the global scope
             */
            (void) idl_global->root ()->fe_add_string (
                                            AST_String::narrow_from_decl ($$)
                                          );
          }
        }
        | wstring_head
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);
          /*
           * Create a node representing a string
           */
          $$ =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr ((unsigned long) 0)
              );
          /*
           * Add this AST_String to the types defined in the global scope
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl ($$)
                                        );
        }
        ;

wstring_head:
        IDL_WSTRING
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
        ;

array_declarator :
        id
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
        at_least_one_array_dim
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);
          /*
           * Create a node representing an array
           */
          if ($3 != 0) {
             $$ = idl_global->gen ()->create_array (new UTL_ScopedName ($1, 
                                                                        0),
                                                    $3->length (), 
                                                    $3, 
                                                    0, 
                                                    0);
          }
        }
        ;

at_least_one_array_dim :
        array_dim array_dims
        {
          $$ = new UTL_ExprList ($1, $2);
        }
        ;

array_dims
        : array_dims array_dim
        {
          if ($1 == 0)
            $$ = new UTL_ExprList ($2, 0);
          else {
            $1->nconc (new UTL_ExprList ($2, 0));
            $$ = $1;
          }
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

array_dim :
        '['
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
        positive_int_expr
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
        ']'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);
          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers
           */
          if ($3 == 0 || $3->coerce (AST_Expression::EV_ulong) == 0) {
            idl_global->err ()->coercion_error ($3, AST_Expression::EV_ulong);
            $$ = 0;
          } else
            $$ = $3;
        }
        ;

attribute:
        opt_readonly
        IDL_ATTRIBUTE
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
        param_type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
        at_least_one_simple_declarator
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_AttrCompleted);
          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope
           */
          if (s != 0 && $4 != 0 && $6 != 0) {
            for (UTL_DecllistActiveIterator l ($6); !l.is_done (); l.next ()) {
              d = l.item ();
              if (d == 0)
                continue;
              AST_Type *tp = d->compose ($4);
              if (tp == 0)
                continue;
              a = idl_global->gen ()->create_attribute ($1,
                                                        tp,
                                                        (UTL_IdList *) d->name()->copy (),
                                                        s->is_local (),
                                                        s->is_abstract ());
              /*
               * Add one attribute to the enclosing scope
               */
              (void) s->fe_add_attribute (a);
            }
          }
        }
        ;

opt_readonly
        : IDL_READONLY
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
          $$ = I_TRUE;
        }
        | /* EMPTY */
        {
          $$ = I_FALSE;
        }
        ;

exception :
        IDL_EXCEPTION
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
        id
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n = new UTL_ScopedName ($3, 0);
          AST_Exception *e = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptIDSeen);
          /*
           * Create a node representing an exception and add it to
           * the enclosing scope
           */
          if (s != 0) {
            e = idl_global->gen ()->create_exception (n,
                                                      s->is_local (),
                                                      s->is_abstract ());
            (void) s->fe_add_exception (e);
          }
          /*
           * Push the exception scope on the scope stack
           */
          idl_global->scopes()->push (e);
        }
        '{'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
        members
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
        '}'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack
           */
          idl_global->scopes ()->pop ();
        }
        ;

operation :
        opt_op_attribute
        op_type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
        IDENTIFIER
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          UTL_ScopedName *n =
                new UTL_ScopedName (new Identifier ($4), 0);
          AST_Operation *o = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);
          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope
           */
          if (s != 0 && $2 != 0) {
            AST_Type *tp = AST_Type::narrow_from_decl ($2);
            if (tp == 0) {
              idl_global->err ()->not_a_type ($2);
            } else if (tp->node_type () == AST_Decl::NT_except) {
              idl_global->err ()->not_a_type ($2);
            } else {
              o = idl_global->gen ()->create_operation (tp,
                                                        $1,
                                                        n,
                                                        s->is_local (),
                                                        s->is_abstract ());
              (void) s->fe_add_operation (o);
            }
          }
          /*
           * Push the operation scope onto the scopes stack
           */
          idl_global->scopes()->push (o);
        }
        parameter_list
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
        opt_raises
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
        opt_context
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Operation *o = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpCompleted);
          /*
           * Add exceptions and context to the operation
           */
          if (s != 0 && s->scope_node_type() == AST_Decl::NT_op) {
            o = AST_Operation::narrow_from_scope (s);

            if ($8 != 0 && o != 0)
              (void) o->fe_add_exceptions ($8);
            if ($10 != 0)
              (void) o->fe_add_context ($10);
          }
          /*
           * Done with this operation. Pop its scope from the scopes stack
           */
          idl_global->scopes ()->pop ();
        }
        ;

opt_op_attribute
        : IDL_ONEWAY
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          $$ = AST_Operation::OP_oneway;
        }
        | IDL_IDEMPOTENT
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          $$ = AST_Operation::OP_idempotent;
        }
        | /* EMPTY */
        {
          $$ = AST_Operation::OP_noflags;
        }
        ;

op_type_spec
        : param_type_spec
        | IDL_VOID
        {
          $$ =
            idl_global->scopes ()->bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
        ;

init_decl
        : IDL_FACTORY 
        {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
        IDENTIFIER 
        {

          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          if (s->is_abstract ())
          {
            //@@ Fire error
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("error in %s line %d:\n")
                        ACE_TEXT ("Abstract valuetype can't have a ")
                        ACE_TEXT ("factory construct.\n"),
                        idl_global->filename ()->get_string (),
                        idl_global->lineno ()));
          }     
          UTL_ScopedName *n =
                new UTL_ScopedName (new Identifier ($3), 0);

          AST_Factory *factory = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an factory construct
           * and add it to its enclosing scope
           */
          if (s != 0)
          {
            factory = idl_global->gen ()->create_factory (n);
            (void) s->fe_add_factory (factory);
          }
          /*
           * Push the operation scope onto the scopes stack
           */
          idl_global->scopes ()->push (factory);
        }
        init_parameter_list
        {
          // TODO: replace parameter_list with rule that accepts only IN args

          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
          idl_global->scopes ()->pop ();
        }
        ;

init_parameter_list
        : '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
          ')'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        | '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
          at_least_one_in_parameter
          ')'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        ;

at_least_one_in_parameter : in_parameter in_parameters ;

in_parameters
        : in_parameters
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
          in_parameter
        | /* EMPTY */
        ;

in_parameter :
        IDL_IN
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
        param_type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
        declarator
        {

          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Argument *a = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);
          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope)
           */
          if ($3 != 0 && $5 != 0 && s != 0) 
          {
            AST_Type *tp = $5->compose ($3);
            if (tp != 0) 
            {
              a = idl_global->gen ()->create_argument (
                      AST_Argument::dir_IN, 
                      tp, 
                      (UTL_IdList *) $5->name ()->copy ()
                    );

              (void) s->fe_add_argument (a);
            }
          }
        }
        ;

parameter_list
        : '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
          ')'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        | '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
          at_least_one_parameter
          ')'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
        ;

at_least_one_parameter : parameter parameters ;

parameters
        : parameters
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
          parameter
        | /* EMPTY */
        ;

parameter :
        direction
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
        param_type_spec
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
        declarator
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Argument *a = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);
          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope)
           */
          if ($3 != 0 && $5 != 0 && s != 0) {
            AST_Type *tp = $5->compose ($3);
            if (tp != 0) {
              if (!s->is_local () && tp->is_local ())
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("error in %s line %d\n"),
                              idl_global->filename ()->get_string (),
                              idl_global->lineno ()));
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("Cannot use a local type as an ")
                              ACE_TEXT ("argument of a remote operation\n")));
                }
              else
                {
                  a = idl_global->gen ()->create_argument (
                                              $1, 
                                              tp, 
                                              (UTL_IdList *) $5->name ()->copy ()
                                            );
                  (void) s->fe_add_argument (a);
                }
            }
          }
        }
        ;

param_type_spec
        : base_type_spec
        {
          $$ = idl_global->scopes ()->bottom ()->lookup_primitive_type ($1);
        }
        | string_type_spec
        | wstring_type_spec
        | scoped_name
        {
          UTL_Scope *s = idl_global->scopes ()->top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            d = s->lookup_by_name ($1, I_TRUE);
          if (d == 0)
            idl_global->err ()->lookup_error ($1);
          $$ = d;
        }
        ;

direction
        : IDL_IN
        {
          $$ = AST_Argument::dir_IN;
        }
        | IDL_OUT
        {
          $$ = AST_Argument::dir_OUT;
        }
        | IDL_INOUT
        {
          $$ = AST_Argument::dir_INOUT;
        }
        ;

opt_raises
        : IDL_RAISES
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
          '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
          at_least_one_scoped_name
          ')'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          $$ = $5;
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

opt_context
        : IDL_CONTEXT
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
          '('
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
          at_least_one_string_literal
          ')'
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          $$ = $5;
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

at_least_one_string_literal :
        IDL_STRING_LITERAL string_literals
        {
          $$ = new UTL_StrList ($1, $2);
        }
        ;

string_literals
        : string_literals
          ','
        {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
          IDL_STRING_LITERAL
        {
          if ($1 == 0)
            $$ = new UTL_StrList ($4, 0);
          else {
            $1->nconc (new UTL_StrList ($4, 0));
            $$ = $1;
          }
        }
        | /* EMPTY */
        {
          $$ = 0;
        }
        ;

%%
/* programs */

/*
 * ???
 */
int
yywrap ()
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
              "%s\n",
              msg));
}
