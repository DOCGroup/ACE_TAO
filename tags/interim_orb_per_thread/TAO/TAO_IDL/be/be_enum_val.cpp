// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_enum_val.cpp
//
// = DESCRIPTION
//    Extension of class AST_EnumVal that provides additional means for C++
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

be_enum_val::be_enum_val (void)
{
}

be_enum_val::be_enum_val (unsigned long v, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Constant (AST_Expression::EV_ulong,
                  AST_Decl::NT_enum_val,
                  new AST_Expression(v),
                  n, 
                  p),
    AST_Decl (AST_Decl::NT_enum_val, n, p)
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

// NOTE: The IDL front end does a strange thing of putting the enum_vals in the
// Enum decl scope as well as the scope that encloses the ENUM decl. Since we
// took care of generating the client header declarations for all the
// enum_vals, we do not do anything in any of the methods below. However, we
// have to provide them so that whenever be_scope finds an enum_val in a scope
// that is not an ENUM, it just ignores it.

// Generates the client-side header information for the enum val 
int 
be_enum_val::gen_client_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side stubs for the enum val
int 
be_enum_val::gen_client_stubs (void)
{
  return 0;
}

// Generates the server-side header information for the enum val 
int 
be_enum_val::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side skeletons for the enum val
int 
be_enum_val::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side inline information
int 
be_enum_val::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int 
be_enum_val::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_enum_val::gen_typecode (void)
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
  return 0;
}

long
be_enum_val::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      long slen;

      this->encap_len_ = 4; // store the size of name
      slen = ACE_OS::strlen (this->local_name ()->get_string ()) + 1; 
      // + 1 for  NULL 
      this->encap_len_ += 4 * (slen/4 + (slen%4 ? 1:0)); // storage for the name
    }

  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_enum_val, AST_EnumVal, be_decl)
IMPL_NARROW_FROM_DECL (be_enum_val)

