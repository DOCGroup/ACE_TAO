// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_constant.cpp
//
// = DESCRIPTION
//    Extension of class AST_Constant that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Constant
 */
be_constant::be_constant (void)
{
}

be_constant::be_constant (AST_Expression::ExprType et,
			  AST_Expression *v,
                          UTL_ScopedName *n,
                          UTL_StrList *p)
  : AST_Constant (et, v, n, p),
    AST_Decl (AST_Decl::NT_const, n, p)
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// Generates the client-side header information for the constant 
int 
be_constant::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_CONSTANT_CH);

  ch = cg->client_header ();

  // if we are defined in the outermost scope, then the value is assigned
  // to us here itself, else it will be in the *.cpp file
  
  ch->indent (); // start from whatever indentation level we were at
  *ch << "static const " << this->exprtype_to_string () << " " << local_name ();
  scope = be_scope::narrow_from_scope (this->defined_in ()); // retrieve
                                                             // our scope 
  if (scope->scope_node_type () == AST_Decl::NT_root)
    {
      // We were defined at the outermost scope. So we put the value in the
      // header itself
      *ch << " = " << this->constant_value ();
    }
  *ch << ";\n\n";
  cg->pop ();
  return 0;
}

// Generates the client-side stubs for the constant
int 
be_constant::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_scope *scope;   // scope


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_CONSTANT_CS);

  cs = cg->client_stubs ();

  scope = be_scope::narrow_from_scope (this->defined_in ()); // retrieve
                                                             // our scope 
  if (scope->scope_node_type () != AST_Decl::NT_root)
    {
      // for those constants not defined in the outer most scope, they get
      // assigned to their values in the 
      cs->indent (); // start from whatever indentation level we were at
      *cs << "const " << this->exprtype_to_string () << " " << name ();
      *cs << " = " << this->constant_value ();
      *cs << ";\n\n";
    }
  cg->pop ();
  return 0;
}

// Generates the server-side header information for the constant 
int 
be_constant::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side skeletons for the constant
int 
be_constant::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side inline information
int 
be_constant::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int 
be_constant::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

char *
be_constant::exprtype_to_string (void)
{
  switch (this->et ()) 
    {
    case AST_Expression::EV_short:
      return "CORBA::Short";
    case AST_Expression::EV_ushort:
      return "CORBA::UShort";
    case AST_Expression::EV_long:
      return "CORBA::Long";
    case AST_Expression::EV_ulong:
      return "CORBA::ULong";
    case AST_Expression::EV_float:
      return "CORBA::Float";
    case AST_Expression::EV_double:
      return "CORBA::Double";
    case AST_Expression::EV_char:
      return "CORBA::Char";
    case AST_Expression::EV_octet:
      return "CORBA::Octet";
    case AST_Expression::EV_bool:
      return "CORBA::Boolean";
    case AST_Expression::EV_string:
      return "char *const";
    case AST_Expression::EV_any:
      return "CORBA::Any";
    case AST_Expression::EV_void:
      return "void";
    case AST_Expression::EV_none:
      return "none";
    }
}

// Narrowing
IMPL_NARROW_METHODS2 (be_constant, AST_Constant, be_decl)
IMPL_NARROW_FROM_DECL (be_constant)

