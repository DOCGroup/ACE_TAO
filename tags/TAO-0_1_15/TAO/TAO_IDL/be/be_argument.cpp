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
}


// All the methods here are very similar. The state is set by the be_operation
// method which then invokes its scope which inturn calls one of these methods
// with the state that was set.


int
be_argument::gen_client_header (void)
{
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_argument::gen_client_header - "
                         "error generating type\n"),
                        -1);
    }
  return 0;
}

int
be_argument::gen_client_stubs (void)
{
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_argument::gen_client_stubs - "
                         "error generating type\n"),
                        -1);
    }
  return 0;
}

int
be_argument::gen_server_header (void)
{
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_argument::gen_server_header - "
                         "error generating type\n"),
                        -1);
    }
  return 0;
}

int
be_argument::gen_server_skeletons (void)
{
  be_type *bt;  // the field type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_argument::gen_server_skeletons - "
                         "error generating type\n"),
                        -1);
    }
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

int
be_argument::accept (be_visitor *visitor)
{
  return visitor->visit_argument (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_argument, AST_Argument, be_decl)
IMPL_NARROW_FROM_DECL (be_argument)
