// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_typedef.cpp
//
// = DESCRIPTION
//    Extension of class AST_Typedef that provides additional means for C++
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

be_typedef::be_typedef (void)
{
}

be_typedef::be_typedef (AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Typedef (bt, n, p),
    AST_Decl (AST_Decl::NT_typedef, n, p)
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

int
be_typedef::gen_client_header (void)
{
  be_type *bt;       // type node

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_TYPEDEF);
      cg->node (this); // pass ourselves

      bt = be_type::narrow_from_decl (this->base_type ());
      // first generate the mapping for our type
      if ((bt == NULL) || ((bt != NULL) && (bt->be_type::gen_client_header () ==
                                            -1))) 
        {
          ACE_ERROR ((LM_ERROR, "be_typedef: error generating code for base type\n"));
          return -1;
        }

      cg->pop ();
      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
}

int
be_typedef::gen_client_stubs (void)
{
  return 0;
}

int
be_typedef::gen_server_header (void)
{
  return 0;
}

int
be_typedef::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int 
be_typedef::gen_client_inline (void)
{
  be_type *bt;

  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the client inline methods for the base type
  if (bt->be_type::gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_typedef: base type inline code gen failed\n"));
      return -1;
    }
  return 0;
}

// Generates the server-side inline
int 
be_typedef::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_typedef::gen_typecode (void)
{
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type)
IMPL_NARROW_FROM_DECL (be_typedef)

