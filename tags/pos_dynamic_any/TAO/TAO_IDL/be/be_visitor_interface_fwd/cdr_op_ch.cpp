//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, cdr_op_ch, "$Id$")

// ***************************************************************************
// Forward declared interface visitor for generating CDR operator declarations 
// in the client header. Called if this node is not later defined in the file.
// ***************************************************************************

be_visitor_interface_fwd_cdr_op_ch::be_visitor_interface_fwd_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_cdr_op_ch::~be_visitor_interface_fwd_cdr_op_ch (void)
{
}

int
be_visitor_interface_fwd_cdr_op_ch::visit_interface_fwd (be_interface_fwd *node)
{
  AST_Interface *fd = node->full_definition ();

  if (!fd->is_defined ())
    {
    }

  node->cli_hdr_cdr_op_gen (1);

  return 0;
}
