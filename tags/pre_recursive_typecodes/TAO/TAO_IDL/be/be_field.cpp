// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_field.cpp
//
// = DESCRIPTION
//    Extension of class AST_Field that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_field, "$Id$")

/*
 * BE_Field
 */
be_field::be_field (void)
{
}

be_field::be_field (AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p, Visibility vis)
  : AST_Field (ft, n, p, vis),
    AST_Decl (AST_Decl::NT_field, n, p)
{
}

int
be_field::gen_encapsulation (void)
{
  TAO_OutStream *cs; // output stream
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt;  // our type node
  long i, arrlen;
  ACE_UINT32 *arr;

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  // generate name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("ACE_NTOHL (0x%x), ", arr[i]);
    }
  *cs << " // name = " << this->local_name () << "\n";

  // hand over code generation to our type node
  bt = be_type::narrow_from_decl (this->field_type ());
  return bt->gen_typecode ();
}

long
be_field::tc_encap_len (void)
{
  if (this->encap_len_ == -1)  // not computed yet
    {
      be_type *bt;

      // struct member is represented as the "name" followed by the typecode

      this->encap_len_ = this->name_encap_len (); // for name

      // add to this, the size of our typecode
      bt = be_type::narrow_from_decl (this->field_type ());
      this->encap_len_ += bt->tc_size (); // note that we must add typecode
                                          // size of the type
    }
  return this->encap_len_;
}

// compute the size type of the node in question
int
be_field::compute_size_type (void)
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
be_field::accept (be_visitor *visitor)
{
  return visitor->visit_field (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_field, AST_Field, be_decl)
IMPL_NARROW_FROM_DECL (be_field)
