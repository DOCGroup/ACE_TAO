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

/*
 * BE_String
 */
be_string::be_string (void)
{
}

be_string::be_string (AST_Expression *v)
  : AST_String (v),
    AST_Decl (AST_Decl::NT_string,
             new UTL_ScopedName(new Identifier("string", 1, 0, I_FALSE),
                                NULL),
             NULL)
{
  this->size_type (be_decl::VARIABLE); // we are always variable length
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

// Code generation

int
be_string::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();
  ch->indent ();
  *ch << "char *";

  return 0;
}

int
be_string::gen_client_stubs (void)
{
  return 0;
}

int
be_string::gen_server_header (void)
{
  return 0;
}

int
be_string::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int
be_string::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int
be_string::gen_server_inline (void)
{
  // nothing to be done
  return 0;
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
  *cs << this->max_size () << ", // string length" << nl;
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

// Narrowing
IMPL_NARROW_METHODS2 (be_string, AST_String, be_type)
IMPL_NARROW_FROM_DECL (be_string)
