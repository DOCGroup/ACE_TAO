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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Field
 */
be_field::be_field (void)
{
}
be_field::be_field (AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Field (ft, n, p),
    AST_Decl (AST_Decl::NT_field, n, p)
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

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// Generates the client-side header information for the field 
int 
be_field::gen_client_header (void)
{
  be_type *bt;  // the field type

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cg->node (this); // pass info thru singleton
  bt = be_type::narrow_from_decl (this->field_type ());
  if ((bt == NULL) ||
      ((bt != NULL) && (bt->be_type::gen_client_header () == -1)))
    {
      ACE_ERROR ((LM_ERROR, "be_field: error generating type\n"));
      return -1;
    }
  return 0;
}

// Generates the client-side stubs for the field
int 
be_field::gen_client_stubs (void)
{
  return 0;
}

// Generates the server-side header information for the field 
int 
be_field::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side skeletons for the field
int 
be_field::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side inline information
int 
be_field::gen_client_inline (void)
{
  // base type may need inline definitions
  be_type *bt;

  bt = be_type::narrow_from_decl (this->field_type ());
  return bt->be_type::gen_client_inline ();
}

// Generates the server-side inline
int 
be_field::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_field::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt;  // our type node
  long i, arrlen;
  long *arr;  // an array holding string names converted to array of longs

  cs = cg->outstream ();
  cg->node (this); // pass ourselves in case we are needed
  cs->indent (); // start from whatever indentation level we were at

  // generate name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("0x%x, ", arr[i]);
    }
  *cs << " // name = " << this->local_name () << "\n";

  // hand over code generation to our type node
  bt = be_type::narrow_from_decl (this->field_type ());
  return bt->be_type::gen_typecode ();
}

long
be_field::tc_encap_len (void)
{
  if (this->encap_len_ == -1)  // not computed yet
    {
      be_type *bt;
      long slen;

      this->encap_len_ = 4; // hold the size of our name
      this->encap_len_ += 4; // store the size of name
      slen = ACE_OS::strlen (this->local_name ()->get_string ()) + 1; 
      // + 1 for  NULL 
      this->encap_len_ += 4 * (slen/4 + (slen%4 ? 1:0)); // storage for the name
      
      // add to this, the size of our typecode
      bt = be_type::narrow_from_decl (this->field_type ());
      this->encap_len_ += bt->be_type::tc_encap_len ();
    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_field, AST_Field, be_decl)
IMPL_NARROW_FROM_DECL (be_field)

