//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for an Interface in the client
//    header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, any_op_ch, "$Id$")


// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_interface_any_op_ch::be_visitor_interface_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_ch::~be_visitor_interface_any_op_ch (void)
{
}

int
be_visitor_interface_any_op_ch::visit_interface (be_interface *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = tao_cg->client_header ();

  // generate the Any <<= and >>= operator declarations
  os->indent ();
  *os << "// Any operators for interface " << node->name () << be_nl;
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, " << node->name ()
      << " _ptr); // copying version" << be_nl;
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "_ptr*); // noncopying version" << be_nl;
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}
