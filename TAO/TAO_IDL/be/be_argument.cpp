// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_argument.cpp
//
// = DESCRIPTION
//    Extension of class AST_Argument that provides additional means for C++
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
 * BE_Argument
 */

be_argument::be_argument (void)
{
}

be_argument::be_argument (AST_Argument::Direction d, AST_Type *ft,
                          UTL_ScopedName *n, UTL_StrList *p)
  : AST_Argument (d, ft, n, p),
    AST_Field (AST_Decl::NT_argument, ft, n, p),
    AST_Decl (AST_Decl::NT_argument, n, p)
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

int
be_argument::gen_client_header (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ARGUMENT_CH); // generating code for arguments

  os = cg->client_header ();
  cg->outstream (os);

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_argument: error generating type\n"));
      return -1;
    }
  *os << " " << this->local_name () << ", ";
  cg->pop ();
  return 0;
}

int
be_argument::gen_client_stubs (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ARGUMENT_CS); // generating code for arguments

  os = cg->client_stubs ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_argument: error generating type\n"));
      return -1;
    }
  *os << " " << this->local_name () << ", ";
  cg->pop ();
  return 0;
}

int
be_argument::gen_server_header (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ARGUMENT_SH); // generating code for arguments

  os = cg->server_header ();
  cg->outstream (os);

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_argument: error generating type\n"));
      return -1;
    }
  *os << " " << this->local_name () << ", ";
  cg->pop ();
  return 0;
}

int
be_argument::gen_server_skeletons (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ARGUMENT_SS); // generating code for arguments

  os = cg->server_skeletons ();
  cg->outstream (os);

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_argument: error generating type\n"));
      return -1;
    }
  *os << " " << this->local_name () << ", ";
  cg->pop ();
  return 0;
}

// Generates the client-side inline information
int 
be_argument::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int 
be_argument::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_argument, AST_Argument, be_decl)
IMPL_NARROW_FROM_DECL (be_argument)

