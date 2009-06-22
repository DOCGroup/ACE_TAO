%{
// $Id$
// ========================================================================
//
// = LIBRARY
//   orbsvcs / Persistent State Definition Language parser.
//
// = FILENAME
//   PSDL.yy
//
// = AUTHOR
//   Priyanka Gontla <pgontla@doc.ece.uci.edu>
//
// ========================================================================

#include "PSDL_y.h"
#include "PSDL_Node.h"
#include "PSDL_Interpreter.h"
#include "PSDL_Scope.h"
#include "PSDL_Module_Scope.h"

#define YYPARSE_PARAM lex_state
#define YYLEX_PARAM lex_state

//extern int yylex (TAO_PSDL_Node **, void *);
extern int yylex (void);

static void yyerror (const char *error)
{
  printf ("Error %s\n", error);	
}

%}

%token TAO_PSDL_SEMI TAO_PSDL_MODULE TAO_PSDL_COLON
%token TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
%token TAO_PSDL_OPEN_BRACE TAO_PSDL_CLOSE_BRACE
%token TAO_PSDL_ABSTRACT TAO_PSDL_STORAGETYPE
%token TAO_PSDL_STORAGEHOME
%token TAO_PSDL_CATALOG TAO_PSDL_PROVIDES
%token TAO_PSDL_IMPLEMENTS
%token TAO_PSDL_SCOPE TAO_PSDL_STORES TAO_PSDL_STATE
%token TAO_PSDL_AS TAO_PSDL_PRIMARY
%token TAO_PSDL_STRONG TAO_PSDL_REF TAO_PSDL_OF
%token TAO_PSDL_OPEN_ANGULAR_BRACE TAO_PSDL_CLOSE_ANGULAR_BRACE
%token TAO_PSDL_COMMA TAO_PSDL_KEY TAO_PSDL_READONLY
%token TAO_PSDL_UNION TAO_PSDL_SWITCH TAO_PSDL_CASE
%token TAO_PSDL_WCHAR TAO_PSDL_BOOLEAN TAO_PSDL_OCTET
%token TAO_PSDL_ANY TAO_PSDL_OBJECT TAO_PSDL_STRUCT
%token TAO_PSDL_DEFAULT TAO_PSDL_FLOAT TAO_PSDL_DOUBLE
%token TAO_PSDL_LONG TAO_PSDL_SHORT TAO_PSDL_UNSIGNED
%token TAO_PSDL_CHAR TAO_PSDL_IDENT TAO_PSDL_TYPEDEF
%token TAO_PSDL_NATIVE TAO_PSDL_PLUS TAO_PSDL_MINUS
%token TAO_PSDL_XOR TAO_PSDL_AND TAO_PSDL_MULT
%token TAO_PSDL_RIGHT_SHIFT TAO_PSDL_LEFT_SHIFT
%token TAO_PSDL_DIV TAO_PSDL_PERCENT TAO_PSDL_TILDA
%token TAO_PSDL_ONEWAY TAO_PSDL_CONST TAO_PSDL_EQUAL TAO_PSDL_OR
%token TAO_PSDL_VOID TAO_PSDL_IN TAO_PSDL_OUT TAO_PSDL_INOUT
%token TAO_PSDL_RAISES TAO_PSDL_CONTEXT TAO_PSDL_FIXED
%token TAO_PSDL_VALUEBASE TAO_PSDL_EXCEPTION
%token TAO_PSDL_INTERFACE
%token TAO_PSDL_LOCAL TAO_PSDL_DOUBLE_COLON TAO_PSDL_VALUETYPE
%token TAO_PSDL_CUSTOM TAO_PSDL_TRUNCATABLE TAO_PSDL_SUPPORTS
%token TAO_PSDL_STRING TAO_PSDL_WSTRING TAO_PSDL_ATTRIBUTE
%token TAO_PSDL_OPEN_SQUARE_BRACE TAO_PSDL_CLOSE_SQUARE_BRACE
%token TAO_PSDL_ENUM TAO_PSDL_SEQUENCE
%token TAO_PSDL_TRUE TAO_PSDL_FALSE
%token TAO_PSDL_INTEGER TAO_PSDL_FLOATING
%token TAO_PSDL_PRIVATE TAO_PSDL_PUBLIC TAO_PSDL_FACTORY
%token TAO_PSDL_FLOATING_PT TAO_PSDL_CHAR_LITERAL
%token TAO_PSDL_FIXED_PT

%pure_parser
%start psdl_specification

%%

psdl_specification: 	psdl_definition
		{ $$ = new TAO_PSDL_Specification ($1); }
			|
			 psdl_specification psdl_definition
		{ $$ = new TAO_PSDL_Specification ($1, $2); }
			;

psdl_definition: 	type_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			const_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			except_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			interface TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
		        psdl_module TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			catalog TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			storagehome TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			abstract_storagehome TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			storagetype TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			abstract_storagetype TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			|
			value TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Definition ($1); }
			;

psdl_module : TAO_PSDL_MODULE identifier
	       {
		 int good_module_name = TAO_PSDL_Scope::instance ()->pop_top_scope ()->add_module (TAO_PSDL_Scope::instance ()->get_identifier ());
	         if (good_module_name == -1)
		    YYABORT;
	        }
              TAO_PSDL_OPEN_CURLY_BRACE psdl_specification TAO_PSDL_CLOSE_CURLY_BRACE
	        {
		  TAO_PSDL_Scope::instance ()->set_module_scope ();
		  $$ = new TAO_PSDL_Module (TAO_PSDL_MODULE, $2, $5); }
			;

abstract_storagetype : abstract_storagetype_dcl
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Abstract_Storagetype ($1); }
			|
			abstract_storagetype_fwd_dcl
		{ $$ = new TAO_PSDL_Abstract_Storagetype ($1); }
			;

abstract_storagetype_dcl: abstract_storagetype_header TAO_PSDL_OPEN_CURLY_BRACE abstract_storagetype_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Dcl ($1, $3); }
			  |
			 abstract_storagetype_header TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Dcl ($1); }
			  ;

abstract_storagetype_fwd_dcl: TAO_PSDL_ABSTRACT TAO_PSDL_STORAGETYPE identifier
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Fwd_Dcl ($3); }
			  ;

storagetype_identifier:  TAO_PSDL_STORAGETYPE identifier
		{
		  int good_storagetype_name = TAO_PSDL_Scope::instance ()->pop_top_scope ()->add_interface (TAO_PSDL_Scope::instance ()->get_identifier ());
		  if (good_storagetype_name == -1)
		      YYABORT;

		  $$ = $2;
		}
			  ;

abstract_storagetype_header:
                        TAO_PSDL_ABSTRACT storagetype_identifier abstract_storagetype_inh_spec
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Header ($2, $3); }
			|
		        TAO_PSDL_ABSTRACT storagetype_identifier
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Header ($2); }
			;

abstract_storagetype_body:
     			abstract_storagetype_member
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Body ($1); }
			|
		   	abstract_storagetype_member abstract_storagetype_body
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Body ($1, $2); }
			;

abstract_storagetype_member:
		   	psdl_state_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Member ($1); }
			|
		   	local_op_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Member ($1); }
			;

abstract_storagetype_inh_spec:
			TAO_PSDL_COLON abstract_storagetype_name
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Inh_Spec ($2); }
			;

abstract_storagetype_name:
			scoped_name
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Name ($1); }
			|
			scoped_name TAO_PSDL_COMMA abstract_storagetype_name
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Name ($1, $3); }
			;

psdl_state_dcl : 	TAO_PSDL_READONLY TAO_PSDL_STATE psdl_state_type_spec simple_declarator
		{ $$ = new TAO_PSDL_Psdl_State_Dcl ($1, $3, $4); }
		 	|
			TAO_PSDL_STATE psdl_state_type_spec simple_declarator
		{ $$ = new TAO_PSDL_Psdl_State_Dcl ($2, $3); }
			;

psdl_state_type_spec :  base_type_spec
		{ $$ = new TAO_PSDL_Psdl_State_Type_Spec ($1); }
			|
			string_type
		{ $$ = new TAO_PSDL_Psdl_State_Type_Spec ($1); }
			|
		        wide_string_type
		{ $$ = new TAO_PSDL_Psdl_State_Type_Spec ($1); }
			|
		        abstract_storagetype_ref_type
		{ $$ = new TAO_PSDL_Psdl_State_Type_Spec ($1); }
			|
		        scoped_name
		{ $$ = new TAO_PSDL_Psdl_State_Type_Spec ($1); }
			;

abstract_storagetype_ref_type:
			TAO_PSDL_STRONG TAO_PSDL_REF TAO_PSDL_OPEN_ANGULAR_BRACE abstract_storagetype_name TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Ref_Type ($1, $4); }
			|
			TAO_PSDL_REF TAO_PSDL_OPEN_ANGULAR_BRACE abstract_storagetype_name TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Abstract_Storagetype_Ref_Type ($3); }
			;

abstract_storagehome :  abstract_storagehome_dcl
		{ $$ = new TAO_PSDL_Abstract_Storagehome ($1); }
			|
			abstract_storagehome_fwd_dcl
		{ $$ = new TAO_PSDL_Abstract_Storagehome ($1); }
			;

abstract_storagehome_fwd_dcl:
			TAO_PSDL_ABSTRACT TAO_PSDL_STORAGEHOME identifier
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Fwd_Dcl ($3); }
			;

abstract_storagehome_dcl:
			abstract_storagehome_header TAO_PSDL_OPEN_CURLY_BRACE abstract_storagehome_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Dcl ($1, $3); }
			|
			abstract_storagehome_header TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Dcl ($1); }
			;

abstract_storagehome_header:
			TAO_PSDL_ABSTRACT TAO_PSDL_STORAGEHOME identifier TAO_PSDL_OF abstract_storagetype_name abstract_storagehome_inh_spec
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Header ($3, $5, $6); }
			|
			TAO_PSDL_ABSTRACT TAO_PSDL_STORAGEHOME identifier TAO_PSDL_OF abstract_storagetype_name
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Header ($3, $5); }
			;

abstract_storagehome_body:
                        abstract_storagehome_member
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Body ($1); }
			|
			abstract_storagehome_member abstract_storagehome_body
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Body ($1, $2); }
			;

abstract_storagehome_member:
			local_op_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Member ($1); }
			|
			key_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Member ($1); }
			|
			factory_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Member ($1); }
			;

abstract_storagehome_inh_spec:
			TAO_PSDL_COLON abstract_storagehome_name
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Inh_Spec ($2); }
			;

abstract_storagehome_name:
			scoped_name
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Name ($1); }
			|
			scoped_name TAO_PSDL_COMMA abstract_storagehome_name
		{ $$ = new TAO_PSDL_Abstract_Storagehome_Name ($1, $3); }
			;

local_op_dcl : 	 	op_type_spec op_name parameter_dcls raises_expr
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4); }
			|
			op_type_spec op_name parameter_dcls
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3); }
			;

key_dcl : 		TAO_PSDL_KEY identifier TAO_PSDL_OPEN_BRACE simple_declarator_list TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Key_Dcl ($2, $4); }
			|
			TAO_PSDL_KEY identifier
		{ $$ = new TAO_PSDL_Key_Dcl ($2); }
			;

catalog : 		TAO_PSDL_CATALOG identifier catalog_inh_spec TAO_PSDL_OPEN_CURLY_BRACE catalog_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Catalog ($2, $3, $5); }
	 		|
			TAO_PSDL_CATALOG identifier TAO_PSDL_OPEN_CURLY_BRACE catalog_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Catalog ($2, $4); }
			|
			TAO_PSDL_CATALOG identifier TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Catalog ($2); }
			|
			TAO_PSDL_CATALOG identifier catalog_inh_spec TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Catalog ($2, $3); }
			;

catalog_inh_spec : 	TAO_PSDL_COLON catalog_name
		{ $$ = new TAO_PSDL_Catalog_Inh_Spec ($2); }
			;

catalog_name : 		scoped_name
		{ $$ = new TAO_PSDL_Catalog_Name ($1); }
			|
			scoped_name TAO_PSDL_COMMA catalog_name
		{ $$ = new TAO_PSDL_Catalog_Name ($1, $3); }
			;

catalog_body : 		catalog_member
		{ $$ = new TAO_PSDL_Catalog_Body ($1); }
			|
			catalog_member catalog_body
		{ $$ = new TAO_PSDL_Catalog_Body ($1, $2); }
			;

catalog_member :	provides_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Catalog_Member ($1); }
			|
		        local_op_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Catalog_Member ($1); }
			;

provides_dcl : 		TAO_PSDL_PROVIDES abstract_storagehome_name simple_declarator
		{ $$ = new TAO_PSDL_Provides_Dcl ($2, $3); }
			;

storagetype : 		storagetype_dcl
		{ $$ = new TAO_PSDL_Storagetype ($1); }
			|
		        storagetype_fwd_dcl
		{ $$ = new TAO_PSDL_Storagetype ($1); }
			;

storagetype_dcl : 	storagetype_header TAO_PSDL_OPEN_CURLY_BRACE storagetype_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Storagetype_Dcl ($1, $3); }
			|
			storagetype_header TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Storagetype_Dcl ($1); }
			;

storagetype_fwd_dcl :	TAO_PSDL_STORAGETYPE identifier
		{ $$ = new TAO_PSDL_Storagetype_Fwd_Dcl ($2); }
			;

storagetype_header : 	TAO_PSDL_STORAGETYPE identifier storagetype_inh_spec storagetype_impl_spec
		{ $$ = new TAO_PSDL_Storagetype_Header ($2, $3, $4); }
			|
			TAO_PSDL_STORAGETYPE identifier storagetype_inh_spec
		{ $$ = new TAO_PSDL_Storagetype_Header ($2, $3); }
			|
			TAO_PSDL_STORAGETYPE identifier storagetype_impl_spec
		{ $$ = new TAO_PSDL_Storagetype_Header ($2, $3); }
			;

storagetype_body : 	storagetype_member
		{ $$ = new TAO_PSDL_Storagetype_Body ($1); }
			|
			storagetype_member storagetype_body
		{ $$ = new TAO_PSDL_Storagetype_Body ($1, $2); }
			;

storagetype_member : 	psdl_state_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Storagetype_Member ($1); }
			|
		        store_directive TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Storagetype_Member ($1); }
			|
		        ref_rep_directive TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Storagetype_Member ($1); }
			;

storagetype_inh_spec :  TAO_PSDL_COLON storagetype_name
		{ $$ = new TAO_PSDL_Storagetype_Inh_Spec ($2); }
			;

storagetype_name : 	scoped_name
		{ $$ = new TAO_PSDL_Storagetype_Name ($1); }
			;

storagetype_impl_spec : TAO_PSDL_IMPLEMENTS abstract_storagetype_name
		{ $$ = new TAO_PSDL_Storagetype_Impl_Spec ($2); }
			;

storagetype_ref_type :  TAO_PSDL_REF TAO_PSDL_OPEN_ANGULAR_BRACE storagetype_name TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Storagetype_Ref_Type ($3); }
			;

storagehome_scope : 	TAO_PSDL_SCOPE storagehome_name
		{ $$ = new TAO_PSDL_Storagehome_Scope ($2); }
			;

store_directive : 	TAO_PSDL_STORES simple_declarator TAO_PSDL_AS psdl_concrete_state_type storagehome_scope
		{ $$ = new TAO_PSDL_Store_Directive ($2, $4, $5); }
			|
			TAO_PSDL_STORES simple_declarator TAO_PSDL_AS psdl_concrete_state_type
		{ $$ = new TAO_PSDL_Store_Directive ($2, $4); }
			;

psdl_concrete_state_type:
			storagetype_name
		{ $$ = new TAO_PSDL_Psdl_Concrete_State_Type ($1); }
			|
			storagetype_ref_type
		{ $$ = new TAO_PSDL_Psdl_Concrete_State_Type ($1); }
			;

ref_rep_directive : 	TAO_PSDL_REF TAO_PSDL_OPEN_BRACE simple_declarator TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Ref_Rep_Directive ($3); }
			;

storagehome : 		storagehome_header TAO_PSDL_OPEN_CURLY_BRACE storagehome_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Storagehome ($1, $3); }
			|
			storagehome_header TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Storagehome ($1); }
			;

storagehome_header : 	TAO_PSDL_STORAGEHOME identifier TAO_PSDL_OF storagetype_name storagehome_inh_spec storagehome_impl_spec
		{ $$ = new TAO_PSDL_Storagehome_Header ($2, $4, $5, $6); }
			|
			TAO_PSDL_STORAGEHOME identifier TAO_PSDL_OF storagetype_name storagehome_inh_spec
		{ $$ = new TAO_PSDL_Storagehome_Header ($2, $4, $5); }
			|
			TAO_PSDL_STORAGEHOME identifier TAO_PSDL_OF storagetype_name storagehome_impl_spec
		{ $$ = new TAO_PSDL_Storagehome_Header ($2, $4, $5); }
			|
			TAO_PSDL_STORAGEHOME identifier TAO_PSDL_OF storagetype_name
		{ $$ = new TAO_PSDL_Storagehome_Header ($2, $4); }
			;

storagehome_body : 	storagehome_member
		{ $$ = new TAO_PSDL_Storagehome_Body ($1); }
		   	|
		   	storagehome_member storagehome_body
		{ $$ = new TAO_PSDL_Storagehome_Body ($1, $2); }
			;

storagehome_member :  	key_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Storagehome_Member ($1); }
			|
		        primary_key_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Storagehome_Member ($1); }
			;

storagehome_inh_spec : TAO_PSDL_COLON storagehome_name
		{ $$ = new TAO_PSDL_Storagehome_Inh_Spec ($2); }
			;

storagehome_name : 	scoped_name
		{ $$ = new TAO_PSDL_Storagehome_Name ($1); }
			;

storagehome_impl_spec : TAO_PSDL_IMPLEMENTS abstract_storagehome_name
		{ $$ = new TAO_PSDL_Storagehome_Impl_Spec ($2); }
			;

primary_key_dcl : 	TAO_PSDL_PRIMARY TAO_PSDL_KEY identifier
		{ $$ = new TAO_PSDL_Primary_Key_Dcl ($3); }
			|
			TAO_PSDL_PRIMARY TAO_PSDL_KEY TAO_PSDL_REF
		{ $$ = new TAO_PSDL_Primary_Key_Dcl (); }
			;

identifier: 		TAO_PSDL_IDENT
		{ $$ = $1; }
			;

type_dcl : 		TAO_PSDL_TYPEDEF type_declarator
		{ $$ = new TAO_PSDL_Type_Dcl (TAO_PSDL_TYPEDEF, $2); }
			|
		    	struct_type
		{ $$ = new TAO_PSDL_Type_Dcl ($1); }
			|
			union_type
		{ $$ = new TAO_PSDL_Type_Dcl ($1); }
			|
		    	enum_type
		{ $$ = new TAO_PSDL_Type_Dcl ($1); }
			|
			TAO_PSDL_NATIVE simple_declarator
		{ $$ = new TAO_PSDL_Type_Dcl (TAO_PSDL_NATIVE, $2); }
			|
		    	constr_forward_decl
		{ $$ = new TAO_PSDL_Type_Dcl ($1); }
			;

type_declarator : 	type_spec declarators
		{ $$ = new TAO_PSDL_Type_Declarator ($1, $2); }
			;

type_spec : 		simple_type_spec
		{ $$ = new TAO_PSDL_Type_Spec ($1); }
			|
			constr_type_spec
		{ $$ = new TAO_PSDL_Type_Spec ($1); }
			;

simple_type_spec : 	base_type_spec
		{ $$ = new TAO_PSDL_Simple_Type_Spec ($1); }
			|
			template_type_spec
		{ $$ = new TAO_PSDL_Simple_Type_Spec ($1); }
			|
		        scoped_name
		{ $$ = new TAO_PSDL_Simple_Type_Spec ($1); }
			;

base_type_spec : 	floating_pt_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
		    	integer_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			char_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			wide_char_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			boolean_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			octet_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			any_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			object_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			|
			value_base_type
		{ $$ = new TAO_PSDL_Base_Type_Spec ($1); }
			;

template_type_spec : 	sequence_type
		{ $$ = new TAO_PSDL_Template_Type_Spec ($1); }
			|
			string_type
		{ $$ = new TAO_PSDL_Template_Type_Spec ($1); }
			|
			wide_string_type
		{ $$ = new TAO_PSDL_Template_Type_Spec ($1); }
			|
			fixed_pt_type
		{ $$ = new TAO_PSDL_Template_Type_Spec ($1); }
			;

constr_type_spec : 	struct_type
		{ $$ = new TAO_PSDL_Constr_Type_Spec ($1); }
			|
			union_type
		{ $$ = new TAO_PSDL_Constr_Type_Spec ($1); }
			|
			enum_type
		{ $$ = new TAO_PSDL_Constr_Type_Spec ($1); }
			;

declarators : 		declarator
		{ $$ = new TAO_PSDL_Declarators ($1); }
			|
			declarator TAO_PSDL_COMMA declarators
		{ $$ = new TAO_PSDL_Declarators ($1, $3); }
			;

declarator : 		simple_declarator
		{ $$ = new TAO_PSDL_Declarator ($1); }
			|
			complex_declarator
		{ $$ = new TAO_PSDL_Declarator ($1); }
			;

simple_declarator : 	identifier
		{ $$ = $1; }
			;

simple_declarator_list : simple_declarator
		{ $$ = new TAO_PSDL_Simple_Declarator_List ($1); }
			|
			simple_declarator_list TAO_PSDL_COMMA simple_declarator
		{ $$ = new TAO_PSDL_Simple_Declarator_List ($1, $3); }
			;

complex_declarator : array_declarator
		{ $$ = new TAO_PSDL_Complex_Declarator ($1); }
			;

floating_pt_type : 	TAO_PSDL_FLOAT
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_FLOAT); }
		   	|
		   	TAO_PSDL_DOUBLE
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_DOUBLE); }
		    	|
			TAO_PSDL_LONG TAO_PSDL_DOUBLE
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_LONG, TAO_PSDL_DOUBLE); }
			;

integer_type : 		signed_int
		{ $$ = $1; }
			|
			unsigned_int
		{ $$ = $1; }
			;

signed_int : 		signed_short_int
		{ $$ = $1; }
			|
			signed_long_int
		{ $$ = $1; }
			|
			signed_longlong_int
		{ $$ = $1; }
			;

signed_short_int : 	TAO_PSDL_SHORT
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_SHORT); }
			;

signed_long_int : 	TAO_PSDL_LONG
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_LONG); }
			;

signed_longlong_int : 	TAO_PSDL_LONG TAO_PSDL_LONG
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_LONG, TAO_PSDL_LONG); }
			;

unsigned_int : 		unsigned_short_int
		{ $$ = new TAO_PSDL_Predefined_Type ($1); }
			|
			unsigned_long_int
		{ $$ = new TAO_PSDL_Predefined_Type ($1); }
			|
			unsigned_longlong_int
		{ $$ = new TAO_PSDL_Predefined_Type ($1); }
			;

unsigned_short_int : 	TAO_PSDL_UNSIGNED TAO_PSDL_SHORT
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_UNSIGNED, TAO_PSDL_SHORT); }
			;

unsigned_long_int : TAO_PSDL_UNSIGNED TAO_PSDL_LONG
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_UNSIGNED, TAO_PSDL_LONG); }
			;

unsigned_longlong_int : TAO_PSDL_UNSIGNED TAO_PSDL_LONG TAO_PSDL_LONG
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_UNSIGNED, TAO_PSDL_LONG, TAO_PSDL_LONG); }
			;

char_type : 		TAO_PSDL_CHAR
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_CHAR); }
			;

wide_char_type : 	TAO_PSDL_WCHAR
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_WCHAR); }
			;

boolean_type : 		TAO_PSDL_BOOLEAN
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_BOOLEAN); }
			;

octet_type : 		TAO_PSDL_OCTET
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_OCTET); }
			;

any_type : 		TAO_PSDL_ANY
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_ANY); }
			;

object_type : 		TAO_PSDL_OBJECT
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_OBJECT); }
			;

struct_type : 		TAO_PSDL_STRUCT identifier
		{
		  int good_struct_name = TAO_PSDL_Scope::instance ()->pop_top_scope ()->add_struct (TAO_PSDL_Scope::instance ()->get_identifier ());

		  if (good_struct_name == -1)
			YYABORT;
		}
			TAO_PSDL_OPEN_CURLY_BRACE member_list TAO_PSDL_CLOSE_CURLY_BRACE
		{
  		  TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_STRUCT, $2, $5); }
			;

member_list : 		member
		{ $$ = new TAO_PSDL_Member_List ($1); }
			|
			member member_list
		{ $$ = new TAO_PSDL_Member_List ($1, $2); }
			;

member : 		type_spec declarators TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Member ($1, $2); }
			;

union_type : 		TAO_PSDL_UNION identifier TAO_PSDL_SWITCH TAO_PSDL_OPEN_BRACE switch_type_spec TAO_PSDL_CLOSE_BRACE TAO_PSDL_OPEN_CURLY_BRACE switch_body TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type ($2, $5, $8); }
			;

switch_type_spec : 	integer_type
		{ $$ = new TAO_PSDL_Switch_Type_Spec ($1); }
			|
			char_type
		{ $$ = new TAO_PSDL_Switch_Type_Spec ($1); }
			|
			boolean_type
		{ $$ = new TAO_PSDL_Switch_Type_Spec ($1); }
			|
			enum_type
		{ $$ = new TAO_PSDL_Switch_Type_Spec ($1); }
			|
			scoped_name
		{ $$ = new TAO_PSDL_Switch_Type_Spec ($1); }
			;

switch_body : 		case
		{ $$ = new TAO_PSDL_Switch_Body ($1); }
			|
			case switch_body
		{ $$ = new TAO_PSDL_Switch_Body ($1, $2); }
			;

case : 			case_label element_spec TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Case ($1, $2); }
			;

case_label : 		TAO_PSDL_CASE const_exp TAO_PSDL_COLON
		{ $$ = new TAO_PSDL_Case_Label ($1, $2); }
			|
			TAO_PSDL_DEFAULT TAO_PSDL_COLON
		{ $$ = new TAO_PSDL_Case_Label ($1); }
			|
			TAO_PSDL_CASE const_exp TAO_PSDL_COLON case_label
		{ $$ = new TAO_PSDL_Case_Label ($1, $2, $4); }
			|
			TAO_PSDL_DEFAULT TAO_PSDL_COLON case_label
		{ $$ = new TAO_PSDL_Case_Label ($1, $3); }
			;

element_spec : 		type_spec declarator
		{ $$ = new TAO_PSDL_Element_Spec ($1, $2); }
			;

const_dcl : 		TAO_PSDL_CONST const_type identifier TAO_PSDL_EQUAL const_exp
		{ $$ = new TAO_PSDL_Const_Dcl ($2, $3, $5); }
			;

const_type : 		integer_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			char_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			wide_char_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
		    	boolean_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			floating_pt_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			string_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			wide_string_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			fixed_pt_const_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			scoped_name
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			|
			octet_type
		{ $$ = new TAO_PSDL_Predefined_Type($1); }
			;

const_exp : 		or_expr
		{ $$ = new TAO_PSDL_Const_Exp ($1); }
			;

or_expr : 		xor_expr
		{ $$ = new TAO_PSDL_Or_Expr ($1); }
			|
			or_expr TAO_PSDL_OR xor_expr
		{ $$ = new TAO_PSDL_Or_Expr ($1, $3); }
			;

xor_expr :		and_expr
		{ $$ = new TAO_PSDL_Xor_Expr ($1); }
			|
			xor_expr TAO_PSDL_XOR and_expr
		{ $$ = new TAO_PSDL_Xor_Expr ($1, $3); }
			;

and_expr : 		shift_expr
		{ $$ = new TAO_PSDL_And_Expr ($1); }
			|
			and_expr TAO_PSDL_AND shift_expr
		{ $$ = new TAO_PSDL_And_Expr ($1, $3); }
			;

shift_expr : 		add_expr
		{ $$ = new TAO_PSDL_Shift_Expr ($1); }
			|
			shift_expr TAO_PSDL_RIGHT_SHIFT add_expr
		{ $$ = new TAO_PSDL_Shift_Expr ($1, $2, $3); }
			|
			shift_expr TAO_PSDL_LEFT_SHIFT add_expr
		{ $$ = new TAO_PSDL_Shift_Expr ($1, $2, $3); }
			;

add_expr : 		mult_expr
		{ $$ = new TAO_PSDL_Add_Expr ($1); }
			|
			add_expr TAO_PSDL_PLUS mult_expr
		{ $$ = new TAO_PSDL_Add_Expr ($1, $2, $3); }
			|
			add_expr TAO_PSDL_MINUS mult_expr
		{ $$ = new TAO_PSDL_Add_Expr ($1, $2, $3); }
			;

mult_expr : 		unary_expr
		{ $$ = new TAO_PSDL_Mult_Expr ($1); }
			|
			mult_expr TAO_PSDL_MULT unary_expr
		{ $$ = new TAO_PSDL_Mult_Expr ($1, $3); }
			|
			mult_expr TAO_PSDL_DIV unary_expr
		{ $$ = new TAO_PSDL_Mult_Expr ($1, $3); }
			|
			mult_expr TAO_PSDL_PERCENT unary_expr
		{ $$ = new TAO_PSDL_Mult_Expr ($1, $3); }
			;

unary_expr : 		unary_operator primary_expr
		{ $$ = new TAO_PSDL_Unary_Expr ($1, $2); }
			|
			primary_expr
		{ $$ = new TAO_PSDL_Unary_Expr ($1); }
			;

unary_operator : 	TAO_PSDL_MINUS
		{ $$ = $1; }
			|
			TAO_PSDL_PLUS
		{ $$ = $1; }
			|
			TAO_PSDL_TILDA
		{ $$ = $1; }
			;

primary_expr : 		scoped_name
		{ $$ = new TAO_PSDL_Primary_Expr ($1); }
			|
			literal
		{ $$ = new TAO_PSDL_Primary_Expr ($1); }
			|
			TAO_PSDL_OPEN_BRACE const_exp TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Primary_Expr ($2); }
			;

except_header : 	TAO_PSDL_EXCEPTION identifier
		{
		  int good_except_name = TAO_PSDL_Scope::instance ()->pop_top_scope ()->add_exception (TAO_PSDL_Scope::instance ()->get_identifier ());
	          if (good_except_name == -1)
		    YYABORT;
	    	  $$ = $2;
		}
			;

except_dcl : 		except_header TAO_PSDL_OPEN_CURLY_BRACE member_list TAO_PSDL_CLOSE_CURLY_BRACE
		{
		  TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Except_Dcl ($1, $3);
		}
			|
			except_header TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{
		  TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Except_Dcl ($1);
		}
			;

op_name :		identifier
		{
		  int good_op_dcl_name = TAO_PSDL_Scope::instance ()->pop_top_scope ()->add_op_dcl (TAO_PSDL_Scope::instance ()->get_identifier ());

	          if (good_op_dcl_name == -1)
		    YYABORT;
	
		  $$ = $1;
		}
			;

op_dcl : 		op_attribute op_type_spec op_name parameter_dcls raises_expr context_expr
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4, $5, $6); }
			|
			op_type_spec op_name parameter_dcls raises_expr context_expr
		{ TAO_PSDL_Scope::instance ()->set_scope (); 
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4, $5); }
			|
			 op_attribute op_type_spec op_name parameter_dcls raises_expr
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4, $5); }
			|
			 op_attribute op_type_spec op_name parameter_dcls context_expr
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4, $5); }
			|
			op_type_spec op_name parameter_dcls raises_expr
		{ TAO_PSDL_Scope::instance ()->set_scope ();
                  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4); }
			|
			op_type_spec op_name parameter_dcls context_expr
		{ TAO_PSDL_Scope::instance ()->set_scope ();
		  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3, $4); }
			|
			op_type_spec op_name parameter_dcls
		{ TAO_PSDL_Scope::instance ()->set_scope ();
                  $$ = new TAO_PSDL_Op_Dcl ($1, $2, $3); }
			;

op_attribute : 		TAO_PSDL_ONEWAY
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_ONEWAY); }
			;

op_type_spec : 		param_type_spec
		{ $$ = new TAO_PSDL_Op_Type_Spec ($1); }
			|
		 	TAO_PSDL_VOID
		{ $$ = new TAO_PSDL_Op_Type_Spec (TAO_PSDL_VOID); }
			;

parameter_dcls : 	TAO_PSDL_OPEN_BRACE param_dcl TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Parameter_Dcls ($2); }
			|
			TAO_PSDL_OPEN_BRACE TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Parameter_Dcls (); }
			;

param_dcl : 		param_attribute param_type_spec simple_declarator
		{ $$ = new TAO_PSDL_Param_Dcl ($1, $2, $3); }
			|
			param_attribute param_type_spec simple_declarator TAO_PSDL_COMMA param_dcl
		{ $$ = new TAO_PSDL_Param_Dcl ($1, $2, $3, $5); }
			;

param_attribute : 	TAO_PSDL_IN
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_IN); }
			|
			TAO_PSDL_OUT
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_OUT); }
			|
			TAO_PSDL_INOUT
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_INOUT); }
			;

raises_expr : 		TAO_PSDL_RAISES TAO_PSDL_OPEN_BRACE scoped_name TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Raises_Expr ($3); }
			|
			TAO_PSDL_RAISES TAO_PSDL_OPEN_BRACE scoped_name TAO_PSDL_COMMA comma_scoped_name TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Raises_Expr ($3, $4); }
			;

context_expr : 		TAO_PSDL_CONTEXT TAO_PSDL_OPEN_BRACE string_literal TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Context_Expr ($3); }
			;

param_type_spec : 	base_type_spec
		{ $$ = new TAO_PSDL_Param_Type_Spec ($1); }
			|
			string_type
		{ $$ = new TAO_PSDL_Param_Type_Spec ($1); }
			|
			wide_string_type
		{ $$ = new TAO_PSDL_Param_Type_Spec ($1); }
			|
			scoped_name
		{ $$ = new TAO_PSDL_Param_Type_Spec ($1); }
			;

fixed_pt_type : 	TAO_PSDL_FIXED TAO_PSDL_OPEN_ANGULAR_BRACE positive_int_const TAO_PSDL_COMMA positive_int_const TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type ($3, $5); }
			;

fixed_pt_const_type : 	TAO_PSDL_FIXED
		{ $$ = $1; }
			;

value_base_type : 	TAO_PSDL_VALUEBASE
		{ $$ = $1; }
			;

constr_forward_decl : 	TAO_PSDL_STRUCT identifier
		{ $$ = new TAO_PSDL_Constr_Forward_Decl ($1, $2); }
			|
			TAO_PSDL_UNION identifier
		{ $$ = new TAO_PSDL_Constr_Forward_Decl ($1, $2); }
			;

interface : 		interface_dcl
		{ $$ = new TAO_PSDL_Interface ($1); }
			|
			forward_dcl
		{ $$ = new TAO_PSDL_Interface ($1); }
			;

interface_dcl : 	interface_header TAO_PSDL_OPEN_CURLY_BRACE interface_body TAO_PSDL_CLOSE_CURLY_BRACE
		{
		  TAO_PSDL_Scope::instance ()->set_interface_scope ();
		  $$ = new TAO_PSDL_Interface_Dcl ($1, $3);
		}
			|
			interface_header TAO_PSDL_OPEN_CURLY_BRACE TAO_PSDL_CLOSE_CURLY_BRACE
		{
		  TAO_PSDL_Scope::instance ()->set_interface_scope ();
		  $$ = new TAO_PSDL_Interface_Dcl ($1);
		}
			;

forward_dcl : 		TAO_PSDL_ABSTRACT TAO_PSDL_INTERFACE identifier
		{ $$ = new TAO_PSDL_Forward_Dcl (TAO_PSDL_ABSTRACT, $3); }
			|
			TAO_PSDL_LOCAL TAO_PSDL_INTERFACE identifier
		{ $$ = new TAO_PSDL_Forward_Dcl (TAO_PSDL_LOCAL, $3); }
			|
			TAO_PSDL_INTERFACE identifier
		{ $$ = new TAO_PSDL_Forward_Dcl ($2); }
			;

interface_identifier:    TAO_PSDL_INTERFACE identifier
		{
		  int good_interface_name = TAO_PSDL_Scope::instance ()->pop_top_scope ()->add_interface (TAO_PSDL_Scope::instance ()->get_identifier ());
		  if (good_interface_name == -1)
		      YYABORT;

		  $$ = $2;
		}
			;

interface_header : 	TAO_PSDL_ABSTRACT interface_identifier
		{ $$ = new TAO_PSDL_Interface_Header ($1, $2); }
			|
			TAO_PSDL_LOCAL interface_identifier
		{ $$ = new TAO_PSDL_Interface_Header ($1, $2); }
			|
			interface_identifier
		{ $$ = new TAO_PSDL_Interface_Header ($1); }
			|
	 		TAO_PSDL_ABSTRACT interface_identifier interface_inheritance_spec
		{ $$ = new TAO_PSDL_Interface_Header ($1, $2, $3); }
			|
			TAO_PSDL_LOCAL interface_identifier interface_inheritance_spec
		{ $$ = new TAO_PSDL_Interface_Header ($1, $2, $3); }
			|
			interface_identifier interface_inheritance_spec
		{ $$ = new TAO_PSDL_Interface_Header ($2, $1); }
			;

interface_body : 	export
		{ $$ = new TAO_PSDL_Interface_Body ($1); }
			|
			export interface_body
		{ $$ = new TAO_PSDL_Interface_Body ($1, $2); }
			;

export : 		type_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Export_Dcl ($1); }
			|
			const_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Export_Dcl ($1); }
			|
			except_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Export_Dcl ($1); }
			|
			attr_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Export_Dcl ($1); }
			|
			op_dcl TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Export_Dcl ($1); }
			;

interface_inheritance_spec:
			TAO_PSDL_COLON interface_name
		{ $$ = new TAO_PSDL_Interface_Inheritance_Spec ($2); }
			;

interface_name : 	scoped_name
		{ $$ = new TAO_PSDL_Interface_Name ($1); }
			|
			scoped_name TAO_PSDL_COMMA interface_name
		{ $$ = new TAO_PSDL_Interface_Name ($1, $2); }
			;

scoped_name : 		identifier TAO_PSDL_DOUBLE_COLON scoped_name
		{ $$ = new TAO_PSDL_Scoped_Name ($1, $3); }
			|
			identifier
		{ $$ = $1; }
			;

comma_scoped_name: 	identifier TAO_PSDL_COMMA comma_scoped_name
		{ $$ = new TAO_PSDL_Scoped_Name ($1, $3); }
			|
			identifier
		{ $$ = $1; }
			;

value  :  		value_dcl
		{ $$ = new TAO_PSDL_Value ($1); }
			|
			value_abs_dcl
		{ $$ = new TAO_PSDL_Value ($1); }
			|
			value_box_dcl
		{ $$ = new TAO_PSDL_Value ($1); }
			|
			value_forward_dcl
		{ $$ = new TAO_PSDL_Value ($1); }
			;

value_common_base:  	TAO_PSDL_VALUETYPE identifier
		{ $$ = new TAO_PSDL_Value_Common_Base ($2); }
			;

value_forward_dcl:	TAO_PSDL_ABSTRACT value_common_base
		{ $$ = new TAO_PSDL_Value_Forward_Dcl ($1, $2); }
			|
			value_common_base
		{ $$ = new TAO_PSDL_Value_Forward_Dcl ($1); }
			;

value_box_dcl  : 	value_common_base type_spec
		{ $$ = new TAO_PSDL_Value_Box_Dcl ($1, $2); }
			;

value_abs_dcl : 	TAO_PSDL_ABSTRACT value_common_base value_inheritance_spec TAO_PSDL_OPEN_CURLY_BRACE export TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Value_Abs_Dcl ($2, $3, $5); }
			|
			TAO_PSDL_ABSTRACT value_common_base TAO_PSDL_OPEN_CURLY_BRACE export TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Value_Abs_Dcl ($2, $4); }
			;

value_dcl   : value_header TAO_PSDL_OPEN_CURLY_BRACE value_element TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Value_Dcl ($1, $3); }
			;

value_header  : 	TAO_PSDL_CUSTOM value_common_base value_inheritance_spec
		{ $$ = new TAO_PSDL_Value_Header ($1, $2, $3); }
			|
			value_common_base value_inheritance_spec
		{ $$ = new TAO_PSDL_Value_Header ($1, $2); }
			|
			TAO_PSDL_CUSTOM value_common_base
		{ $$ = new TAO_PSDL_Value_Header ($1, $2); }
			;

value_inheritance_spec  :  	TAO_PSDL_COLON TAO_PSDL_TRUNCATABLE value_name TAO_PSDL_SUPPORTS interface_name
		{ $$ = new TAO_PSDL_Value_Inheritance_Spec ($2, $3, $5); }
				|
				TAO_PSDL_COLON value_name TAO_PSDL_SUPPORTS interface_name
		{ $$ = new TAO_PSDL_Value_Inheritance_Spec ($2, $4); }
				|
				TAO_PSDL_SUPPORTS interface_name
		{ $$ = new TAO_PSDL_Value_Inheritance_Spec ($2); }
				|
				TAO_PSDL_COLON TAO_PSDL_TRUNCATABLE value_name
		{ $$ = new TAO_PSDL_Value_Inheritance_Spec ($2, $3); }
				|
				TAO_PSDL_COLON value_name
		{ $$ = new TAO_PSDL_Value_Inheritance_Spec ($2); }
				;


string_type: 			TAO_PSDL_STRING TAO_PSDL_OPEN_ANGULAR_BRACE positive_int_const TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_STRING, $3); }
				|
				TAO_PSDL_STRING
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_STRING); }
				;

wide_string_type : 		TAO_PSDL_WSTRING TAO_PSDL_OPEN_ANGULAR_BRACE positive_int_const TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type ($3); }
				|
				TAO_PSDL_WSTRING
		{ $$ = $1; }
				;

array_declarator : 		identifier fixed_array_size
		{ $$ = new TAO_PSDL_Array_Declarator ($1, $2); }
				;

fixed_array_size : 		TAO_PSDL_OPEN_SQUARE_BRACE positive_int_const TAO_PSDL_CLOSE_SQUARE_BRACE
		{ $$ = new TAO_PSDL_Fixed_Array_Size ($2); }
				;

attr_dcl : 			TAO_PSDL_READONLY TAO_PSDL_ATTRIBUTE param_type_spec simple_declarator
		{ $$ = new TAO_PSDL_Attr_Dcl (TAO_PSDL_READONLY, $3, $4); }
				|
				TAO_PSDL_ATTRIBUTE param_type_spec simple_declarator
		{ $$ = new TAO_PSDL_Attr_Dcl ($2, $3); }
				;

enum_type : 			TAO_PSDL_ENUM identifier TAO_PSDL_OPEN_CURLY_BRACE enumerator TAO_PSDL_CLOSE_CURLY_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_ENUM, $2, $4); }
				;

enumerator : 			identifier
		{ $$ = new TAO_PSDL_Enumerator ($1); }
				|
				identifier TAO_PSDL_COMMA enumerator
		{ $$ = new TAO_PSDL_Enumerator ($1, $3); }
				;

sequence_type : 		TAO_PSDL_SEQUENCE TAO_PSDL_OPEN_ANGULAR_BRACE simple_type_spec TAO_PSDL_COMMA positive_int_const TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_SEQUENCE, $3, $5); }
				|
				TAO_PSDL_SEQUENCE TAO_PSDL_OPEN_ANGULAR_BRACE simple_type_spec TAO_PSDL_CLOSE_ANGULAR_BRACE
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_SEQUENCE, $3); }
				;

literal : 			integer_literal
		{ $$ = $1; }
				|
			 	string_literal
		{ $$ = $1; }
				|
				wide_string_literal
		{ $$ = $1; }
				|
				character_literal
		{ $$ = $1; }
				|
				wide_character_literal
		{ $$ = $1; }
				|
				fixed_pt_literal
		{ $$ = $1; }
				|
				floating_pt_literal
		{ $$ = $1; }
				|
				boolean_literal
		{ $$ = $1; }
				;

boolean_literal : 		TAO_PSDL_TRUE
		{ $$ = $1; }
				|
				TAO_PSDL_FALSE
		{ $$ = $1; }
				;

integer_literal: 		TAO_PSDL_INTEGER
		{ $$ = $1; }
				;

floating_pt_literal: 		TAO_PSDL_FLOATING_PT
		{ $$ = $1; }
				;

character_literal: 		TAO_PSDL_CHAR_LITERAL
		{ $$ = $1; }
				;

wide_character_literal: 	TAO_PSDL_WCHAR
		{ $$ = $1; }
				;

string_literal: 		TAO_PSDL_STRING
		{ $$ = $1; }
				;

wide_string_literal: 		TAO_PSDL_WSTRING
		{ $$ = $1; }
				;

fixed_pt_literal: 		TAO_PSDL_FIXED_PT
		{ $$ = $1; }
				;

value_name: 			scoped_name
		{ $$ = new TAO_PSDL_Value_Name ($1); }
				|
				scoped_name TAO_PSDL_COMMA value_name
		{ $$ = new TAO_PSDL_Value_Name ($1, $3); }
				;

value_element : 		export
		{ $$ = new TAO_PSDL_Value_Element ($1); }
				|
				state_member
		{ $$ = new TAO_PSDL_Value_Element ($1); }
				|
				init_dcl
		{ $$ = new TAO_PSDL_Value_Element ($1); }
				;

state_member : 			TAO_PSDL_PUBLIC type_spec declarators TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_State_Member ($1, $2, $3); }
				|
				TAO_PSDL_PRIVATE type_spec declarators TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_State_Member ($1, $2, $3); }
				;

init_dcl : 			TAO_PSDL_FACTORY identifier TAO_PSDL_OPEN_BRACE init_param_decls TAO_PSDL_CLOSE_BRACE TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Init_Dcl ($2, $4); }
				|
				TAO_PSDL_FACTORY identifier TAO_PSDL_OPEN_BRACE  TAO_PSDL_CLOSE_BRACE TAO_PSDL_SEMI
		{ $$ = new TAO_PSDL_Init_Dcl ($2); }
				;

init_param_decls: 		init_param_decl
		{ $$ = new TAO_PSDL_Init_Param_Decls ($1); }
				|
				init_param_decl TAO_PSDL_COMMA init_param_decls
		{ $$ = new TAO_PSDL_Init_Param_Decls ($1, $2); }
				;

init_param_decl : 		init_param_attribute param_type_spec simple_declarator
		{ $$ = new TAO_PSDL_Init_Param_Decl ($1, $2, $3); }
				;

init_param_attribute : 		TAO_PSDL_IN
		{ $$ = new TAO_PSDL_Predefined_Type (TAO_PSDL_IN); }
				;

positive_int_const : 		const_exp
		{ $$ = new TAO_PSDL_Positive_Int_Const ($1); }
				;

factory_dcl :			 TAO_PSDL_FACTORY identifier factory_parameters
		{ $$ = new TAO_PSDL_Factory_Dcl ($2, $3); }
				;

factory_parameters: 		 TAO_PSDL_OPEN_BRACE simple_declarator TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Factory_Parameters ($2); }
				 |
				 TAO_PSDL_OPEN_BRACE TAO_PSDL_CLOSE_BRACE
		{ $$ = new TAO_PSDL_Factory_Parameters (); }
				 ;
