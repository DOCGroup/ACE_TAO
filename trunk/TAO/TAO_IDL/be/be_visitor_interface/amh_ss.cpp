//=============================================================================
/**
 *  @file   amh_ss.cpp
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

be_visitor_amh_interface_ss::be_visitor_amh_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface_ss (ctx)
{
}

be_visitor_amh_interface_ss::~be_visitor_amh_interface_ss (void)
{
}

void 
be_visitor_amh_interface_ss::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  
  // the _this () operation
  const char *non_amh_name = node->full_name () + 4;

  *os << non_amh_name << "*" << be_nl  
      << node->full_skel_name ()
      << "::_this (CORBA_Environment &ACE_TRY_ENV)" << be_nl
      << "{" << be_idt_nl // idt = 1
      << "TAO_Stub *stub = this->_create_stub (ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK_RETURN (0);" << be_nl << be_nl;
}

void 
be_visitor_amh_interface_ss::dispatch_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  
  // now the dispatch method
  *os << "void " << node->full_skel_name () <<
    "::_dispatch (TAO_ServerRequest &req, " <<
    "void *context, CORBA::Environment &ACE_TRY_ENV)" << be_nl;
  *os << "{" << be_idt_nl;
  // @todo ACE_TRY_ENV without check;
  *os << "this->asynchronous_upcall_dispatch (req, context, this, ACE_TRY_ENV);" << be_nl;
  *os << "this->asynchronous_upcall_reply (req);" << be_uidt_nl;
  *os << "}" << be_nl;
}
