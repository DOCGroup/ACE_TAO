//=============================================================================
/**
*  @file   amh_sh.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH and AMH-RH.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

be_visitor_amh_interface_sh::be_visitor_amh_interface_sh (be_visitor_context *ctx)
  : be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_interface_sh::~be_visitor_amh_interface_sh (void)
{
}

void
be_visitor_amh_interface_sh::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  ACE_CString non_amh_name = "    ";
  non_amh_name += node->client_enclosing_scope ();
  non_amh_name += node->original_interface ()->local_name ();

  // Print out the _this() method.
  *os << "// Special _this method for AMH! \n"
      << non_amh_name.c_str () << " *_this (" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;
}



// ------------------- RH -----------------------------------------

be_visitor_amh_rh_interface_sh::be_visitor_amh_rh_interface_sh (be_visitor_context *ctx)
  : be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_rh_interface_sh::~be_visitor_amh_rh_interface_sh (void)
{
}

/*
void
be_visitor_amh_rh_interface_sh::this_method (be_interface *node)
{
// No need to generate anything
}
*/
