// $Id$

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

ACE_RCSID(be, be_enum_val, "$Id$")

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
}

int
be_enum_val::gen_encapsulation (void)
{
  TAO_OutStream *cs; // output stream
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
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
  return 0;
}

long
be_enum_val::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      this->encap_len_ = this->name_encap_len (); // for name
    }

  return this->encap_len_;
}

int
be_enum_val::accept (be_visitor *visitor)
{
  return visitor->visit_enum_val (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_enum_val, AST_EnumVal, be_decl)
IMPL_NARROW_FROM_DECL (be_enum_val)
