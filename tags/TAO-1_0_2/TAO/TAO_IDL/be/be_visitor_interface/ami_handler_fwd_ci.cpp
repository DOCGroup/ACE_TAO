//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Ami_<Interface>_Handler node in the
//    client inline.  
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu> 
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_ami_handler_fwd, ami_handler_fwd_ci, "$Id$")


// ********************************************************************
// Visitor implementation for the Ami_Handler_Fwd type
// This one for the client inline file
// ********************************************************************

be_visitor_interface_ami_handler_fwd_ci::be_visitor_interface_ami_handler_fwd_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_ami_handler_fwd_ci::~be_visitor_interface_ami_handler_fwd_ci (void)
{
}

// visit the Ami_Handler_Fwd_ci node and its scope
int
be_visitor_interface_ami_handler_fwd_ci::visit_interface (be_interface *node)
{
  if (node->cli_inline_gen () || node->imported ())
    return 0;

  // Generate code.

  // Get the out stream.
  TAO_OutStream *os;
  os = this->ctx_->stream ();
  
  // Start from the current indentation level.
  os->indent (); // start from the current indentation level

  // generate the constructors and destructor
  *os << "ACE_INLINE" << be_nl;
  *os << node->full_name () << "::" << node->local_name () <<
    " (void) // default constructor" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->full_name () << "::" << node->local_name () <<
    " (TAO_Stub *objref, TAO_ServantBase *_tao_servant, "
      << "CORBA::Boolean _tao_collocated) // constructor" << be_nl;
  *os << "  : CORBA_Object (objref, _tao_servant, _tao_collocated)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->full_name () << "::~" << node->local_name () <<
    " (void) // destructor" << be_nl;
  *os << "{}\n\n";

  // generate the ifdefined macro for  the _var type
  os->gen_ifdef_macro (node->flat_name (), "_var");
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }
  os->gen_endif ();

  // generate the ifdefined macro for  the _out type
  os->gen_ifdef_macro (node->flat_name (), "_out");
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }
  os->gen_endif ();


  return 0;
}
