// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_string.cpp
//
// = DESCRIPTION
//    Extension of class AST_Array that provides additional means for C++
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

ACE_RCSID(be, be_string, "$Id$")


/*
 * BE_String
 */
be_string::be_string (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_string::be_string (AST_Expression *v)
  : AST_String (v),
    AST_Decl (AST_Decl::NT_string,
             new UTL_ScopedName(new Identifier("string", 1, 0, I_FALSE),
                                NULL),
             NULL)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_string::be_string (AST_Expression *v, long wide)
  : AST_String (v, wide),
    AST_Decl (AST_Decl::NT_string,
              wide == 1
              ? new UTL_ScopedName(new Identifier("string",1,0,I_FALSE),
                                   NULL)
              : new UTL_ScopedName(new Identifier("wstring_t",
                                                  1,
                                                  0,
                                                  I_FALSE),
                                   NULL),
              NULL)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// overriden method
void
be_string::compute_tc_name (void)
{
  // start with the head as the CORBA namespace
  this->tc_name_ = new UTL_ScopedName (new Identifier ("CORBA", 1, 0, I_FALSE),
                                       NULL);

  this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_string", 1, 0,
                                                             I_FALSE), NULL));
}

int
be_string::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from the current indentation level
  // emit the enumeration
  *cs << "CORBA::tk_string, " << nl;
  *cs << this->max_size () << ", // string length\n";
  return 0;
}

// compute typecode size
long
be_string::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing string length
  return 4 + 4;
}

int
be_string::gen_encapsulation (void)
{
  return 0;
}

long
be_string::tc_encap_len (void)
{
  if (this->encap_len_ == -1)
    {
      this->encap_len_ = 0; // no encapsulation
    }
  return this->encap_len_;
}

int
be_string::accept (be_visitor *visitor)
{
  return visitor->visit_string (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_string, AST_String, be_type)
IMPL_NARROW_FROM_DECL (be_string)
