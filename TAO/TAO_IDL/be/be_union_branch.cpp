// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_union_branch.cpp
//
// = DESCRIPTION
//    Extension of class AST_UnionBranch that provides additional means for C++
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
 * BE_UnionBranch
 */
be_union_branch::be_union_branch (void)
{
}

be_union_branch::be_union_branch (AST_UnionLabel *lab, AST_Type *ft,
                                  UTL_ScopedName *n, UTL_StrList *p)
  : AST_UnionBranch (lab, ft, n, p),
    AST_Field (AST_Decl::NT_union_branch, ft, n, p),
    AST_Decl (AST_Decl::NT_union_branch, n, p)
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

// Generates the client-side header information for the union_branch 
int 
be_union_branch::gen_client_header (void)
{
  be_type *bt;  // the union_branch type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s   = cg->make_state ();
  if (!s || !bt || (s->gen_code (bt, this) ==  -1))
    {
      ACE_ERROR ((LM_ERROR, "be_union_branch: error generating type\n"));
      return -1;
    }
  return 0;
}

// Generates the client-side stubs for the union_branch
int 
be_union_branch::gen_client_stubs (void)
{
  return 0;
}

// Generates the server-side header information for the union_branch 
int 
be_union_branch::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side skeletons for the union_branch
int 
be_union_branch::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side inline information
int 
be_union_branch::gen_client_inline (void)
{
  be_type *bt;  // the union_branch type
  be_state *s;  // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  bt = be_type::narrow_from_decl (this->field_type ());
  s = cg->make_state ();
  if (!s || !bt || (s->gen_code (bt, this) ==  -1))
    {
      ACE_ERROR ((LM_ERROR, 
        "be_union_branch: error generating impl of access methods\n"));
      return -1;
    }
  return 0;
}

// Generates the server-side inline
int 
be_union_branch::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_union_branch::gen_encapsulation (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt;  // our type node
  long i, arrlen;
  long *arr;  // an array holding string names converted to array of longs

  cs = cg->client_stubs ();
  cg->node (this); // pass ourselves in case we are needed
  cs->indent (); // start from whatever indentation level we were at

  // emit the case label value
  *cs << this->label ()->label_val ();
  *cs << ", // union case label (evaluated)" << nl;
  // emit name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("0x%x, ", arr[i]);
    }
  *cs << " // name = " << this->local_name () << "\n";

  // hand over code generation to our type node
  bt = be_type::narrow_from_decl (this->field_type ());
  if (!bt)
    return -1;
  return bt->gen_typecode ();
}

long
be_union_branch::tc_encap_len (void)
{
  if (this->encap_len_ == -1)
    {
      be_type *bt;

      this->encap_len_ = 4; // case label;
      this->encap_len_ += this->name_encap_len (); // for name
      bt = be_type::narrow_from_decl (this->field_type ());
      if (!bt)
        {
          ACE_ERROR ((LM_ERROR, "be_union_branch: bad field type\n"));
          return -1;
        }
      this->encap_len_ += bt->tc_encap_len ();
    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_union_branch, AST_UnionBranch, be_decl)
IMPL_NARROW_FROM_DECL (be_union_branch)

