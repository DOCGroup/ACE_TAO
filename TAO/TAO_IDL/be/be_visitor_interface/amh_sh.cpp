//=============================================================================
/**
 *  @file   amh_sh.cpp
 *
 *  $Id$
 *
 *  Specialized interface visitor for AMH.
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
  
  const char *non_amh_name = node->full_name () + 4;

  // Print out the _this() method.
  *os << "::" << non_amh_name << " *_this (" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;
}
