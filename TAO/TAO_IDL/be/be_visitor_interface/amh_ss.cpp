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
  //const char *non_amh_name = node->full_name () + 4;
  ACE_CString non_amh_name = "";
  non_amh_name += node->client_enclosing_scope ();
  non_amh_name += node->original_interface ()->local_name ();

  *os << non_amh_name.c_str() << "*" << be_nl
      << node->full_skel_name ()
      << "::_this (TAO_ENV_SINGLE_ARG_DECL)" << be_nl
      << "{" << be_idt_nl // idt = 1
      << "TAO_Stub *stub = this->_create_stub (TAO_ENV_SINGLE_ARG_PARAMETER);"
      << be_nl
      << "ACE_CHECK_RETURN (0);" << be_nl << be_nl;

  *os << "CORBA::Object_ptr tmp = CORBA::Object::_nil ();" << be_nl
      << be_nl
      << "if (stub->servant_orb_var ()->orb_core ()->optimize_collocation_objects ())"
      << be_idt_nl // idt = 2
      << "ACE_NEW_RETURN (tmp, CORBA::Object (stub, 1, this), 0);"
      << be_uidt_nl // idt = 1
      << "else"
      << be_idt_nl // idt = 2
      << "ACE_NEW_RETURN (tmp, CORBA::Object (stub, 0, this), 0);"
      << be_uidt_nl << be_nl // idt = 1
      << "CORBA::Object_var obj = tmp;" << be_nl << be_nl;

//  *os << "(void) safe_stub.release ();" << be_nl << be_nl;

  *os << "return " << "::" << non_amh_name.c_str() << "::_unchecked_narrow (obj.in ());"
      << be_uidt_nl // idt = 0
      << "}" << be_nl;

}

void
be_visitor_amh_interface_ss::dispatch_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // now the dispatch method
  *os << "void " << node->full_skel_name () <<
    "::_dispatch (TAO_ServerRequest &req, " <<
    "void *context TAO_ENV_ARG_DECL)" << be_nl;
  *os << "{" << be_idt_nl;
  // @todo ACE_TRY_ENV without check;
  *os << "this->asynchronous_upcall_dispatch" << be_idt_nl
      << " (req, context, this TAO_ENV_ARG_PARAMETER);" << be_uidt_nl;
//  *os << "this->asynchronous_upcall_reply (req);" << be_uidt_nl;
  *os << be_uidt_nl << "}" << be_nl;
}
