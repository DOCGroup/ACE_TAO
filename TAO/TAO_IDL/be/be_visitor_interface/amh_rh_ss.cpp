//=============================================================================
/**
*  @file   amh_rh_ss.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH-ResponseHandler that generates code for 
*  RH in the skeleton source files.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

be_visitor_amh_rh_interface_ss::be_visitor_amh_rh_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface_ss (ctx)
{
}

be_visitor_amh_rh_interface_ss::~be_visitor_amh_rh_interface_ss (void)
{
}

int
be_visitor_amh_rh_interface_ss::visit_interface (be_interface *node)
{
  if (node->srv_skel_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  
  // Generate the skeleton class name.
  // this is AMH_intf_ResponseHandler
  ACE_CString rh_base_class_name = node->local_name (); 
  // and this is TAO_AMH_intf_ResponseHandler
  ACE_CString rh_skel_class_name =  "TAO_";
  rh_skel_class_name += rh_base_class_name.c_str ();
  
  // constructor
  *os << "// skeleton constructor\n";
  // find if we are at the top scope or inside some module
  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::POA_" << node->local_name ()
          << " (void)\n";
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::" << rh_skel_class_name.c_str()
          << " (void)\n";
    }

  if (node->traverse_inheritance_graph
      (be_interface::copy_ctor_helper, os) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "be_visitor_interface_ss::visit_interface - "
                       " copy ctor generation failed\n"), -1);
  *os << "  TAO_ServantBase (rhs)" << be_uidt_nl
      << "{}" << be_nl << be_nl;

  *os << "// skeleton destructor" << be_nl;

  if (!node->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *os << node->full_skel_name () << "::~POA_" << node->local_name () <<
        " (void)" << be_nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *os << node->full_skel_name () << "::~" << rh_skel_class_name.c_str() <<
        " (void)" << be_nl;
    }
  *os << "{" << be_nl;
  *os << "}\n\n";


  // generate code for elements in the scope (e.g., operations)
  // We'll rely on the base class (be_visitor_scope) to do the 
  // right thing for us.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << "\n\n";

  return 0;
}
