//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_constant.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Constant
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_constant.h"

// ********************************************************************
// Visitor implementation for the Constant type
// This one for the client header file
// ********************************************************************

be_visitor_constant_ch::be_visitor_constant_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_ch::~be_visitor_constant_ch (void)
{
}

// visit the Constant_Ch node and its scope
int
be_visitor_constant_ch::visit_constant (be_constant *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      // if we are defined in the outermost scope, then the value is assigned
      // to us here itself, else it will be in the *.cpp file

      os->indent (); // start from whatever indentation level we were at
      *os << "static const " << node->exprtype_to_string ()
          << " " << node->local_name ();
      if (!node->is_nested ())
        {
          // We were defined at the outermost scope. So we put the value in the
          // header itself
          *os << " = " << node->constant_value ();
        }
      *os << ";\n\n";
      node->cli_hdr_gen (I_TRUE);
    }
  return 0;
}

// ********************************************************************
// Visitor implementation for the Constant type
// This one for the client stubs file
// ********************************************************************

be_visitor_constant_cs::be_visitor_constant_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_constant_cs::~be_visitor_constant_cs (void)
{
}

// visit the Constant_cs node and its scope
int
be_visitor_constant_cs::visit_constant (be_constant *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_stub_gen () && !node->imported ())
    {
      if (node->is_nested ())
        {
          // for those constants not defined in the outer most scope, they get
          // assigned to their values in the impl file
          os->indent (); // start from whatever indentation level we were at
          *os << "const " << node->exprtype_to_string () << " " << node->name ()
              << " = " << node->constant_value () << ";\n\n";
        }
      node->cli_stub_gen (I_TRUE);
    }
  return 0;
}
