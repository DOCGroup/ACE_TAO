// $Id$

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

ACE_RCSID(be, be_union_branch, "$Id$")


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
}

int
be_union_branch::gen_encapsulation (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt;  // our type node
  long i, arrlen;
  ACE_UINT32 *arr;

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  // emit the case label value
  *cs << this->label ()->label_val ();
  *cs << ", // union case label (evaluated)" << nl;
  // emit name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("ACE_NTOHL (0x%x), ", arr[i]);
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
      this->encap_len_ += bt->tc_size (); // note that we add the typecode size
                                          // of the type
    }
  return this->encap_len_;
}

// compute the size type of the node in question
int
be_union_branch::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->field_type ());
  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_field::compute_size_type - "
                         "bad field type\n"), -1);
    }

  // our size type is the same as our type
  this->size_type (type->size_type ()); // as a side effect will also update
                                        // the size type of parent
  return 0;
}

int
be_union_branch::accept (be_visitor *visitor)
{
  return visitor->visit_union_branch (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_union_branch, AST_UnionBranch, be_decl)
IMPL_NARROW_FROM_DECL (be_union_branch)
