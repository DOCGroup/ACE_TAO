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
#include "be_visitor_operation.h"

class TAO_IDL_Downcast_Implementation_Worker
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  TAO_IDL_Downcast_Implementation_Worker ();

  virtual int emit (be_interface *base,
                    TAO_OutStream *os,
                    be_interface *derived);
};

// ****************************************************************

be_visitor_amh_interface_ss::be_visitor_amh_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface_ss (ctx)
{
}

be_visitor_amh_interface_ss::~be_visitor_amh_interface_ss (void)
{
}

int
be_visitor_amh_interface_ss::visit_operation (be_operation *node)
{
  be_visitor_amh_operation_ss visitor (this->ctx_);
  return visitor.visit_operation (node);
}

int
be_visitor_amh_interface_ss::visit_attribute (be_attribute *)
{
  ACE_DEBUG ((LM_DEBUG,
              "be_visitor_amh_interface_ss::visit_attribute - "
              "ignoring attribute, must generate code later\n"));
  return 0;
}

int
be_visitor_amh_interface_ss::visit_interface (be_interface *node)
{
  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != 0)
    return 0;

  return be_visitor_interface_ss::visit_interface (node);
}

void
be_visitor_amh_interface_ss::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // the _this () operation
  //const char *non_amh_name = node->full_name () + 4;
  ACE_CString non_amh_name = "";
  non_amh_name += node->client_enclosing_scope ();
  non_amh_name += node->local_name ();

  ACE_CString full_skel_name_holder =
    this->generate_full_skel_name (node);
  const char *full_skel_name = full_skel_name_holder.c_str ();

  *os << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl;

  *os << non_amh_name.c_str() << "*" << be_nl
      << full_skel_name
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

  *os << "return " << "::" << non_amh_name.c_str() << "::_unchecked_narrow (obj.in ());"
      << be_uidt_nl // idt = 0
      << "}" << be_nl;

}

void
be_visitor_amh_interface_ss::dispatch_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  ACE_CString full_skel_name_holder =
    this->generate_full_skel_name (node);
  const char *full_skel_name = full_skel_name_holder.c_str ();

  *os << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl;
  *os << "void" << be_nl
      << full_skel_name << "::_dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *context" << be_nl
      << "TAO_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->asynchronous_upcall_dispatch (" << be_idt << be_idt_nl
      << "req," << be_nl
      << "context," << be_nl
      << "this" << be_nl
      << "TAO_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl;
}

int
be_visitor_amh_interface_ss::generate_amh_classes (be_interface *)
{
  // No AMH classes for the AMH classes... :-)
  return 0;
}

int
be_visitor_amh_interface_ss::generate_proxy_classes (be_interface *)
{
  // No Proxy or ProxyBrokers for the AMH classes
  return 0;
}

int
be_visitor_amh_interface_ss::generate_downcast_implementation (be_interface *node,
                                                               TAO_OutStream *os)
{
  TAO_IDL_Downcast_Implementation_Worker worker;
  return node->traverse_inheritance_graph (worker, os);
}

ACE_CString
be_visitor_amh_interface_ss::generate_flat_name (be_interface *node)
{
  // @@ The following code is *NOT* exception-safe.
  char *buf = 0;
  node->compute_flat_name ("AMH_", "", buf);

  // @@ This whole thing would be more efficient if we could pass the
  // ACE_CString to compute_full_name, after all it uses that
  // internally.
  ACE_CString result (buf);
  delete[] buf;

  return result;
}

ACE_CString
be_visitor_amh_interface_ss::generate_local_name (be_interface *node)
{
  ACE_CString local_name = "AMH_";
  local_name += node->local_name ();
  return local_name;
}

ACE_CString
be_visitor_amh_interface_ss::generate_full_skel_name (be_interface *node)
{
  // @@ This whole thing would be more efficient if we could pass the
  // ACE_CString to compute_full_name, after all it uses that
  // internally.
  ACE_CString result ("POA_");

  // @@ The following code is *NOT* exception-safe.
  char *buf = 0;
  node->compute_full_name ("AMH_", "", buf);
  result += buf;
  delete[] buf;

  return result;
}

TAO_IDL_Downcast_Implementation_Worker::
TAO_IDL_Downcast_Implementation_Worker (void)
{
}

int
TAO_IDL_Downcast_Implementation_Worker::
emit (be_interface * /* derived */,
      TAO_OutStream *os,
      be_interface *base)
{
  // @@ This whole thing would be more efficient if we could pass the
  // ACE_CString to compute_full_name, after all it uses that
  // internally.
  ACE_CString amh_name ("POA_");

  // @@ The following code is *NOT* exception-safe.
  char *buf = 0;
  base->compute_full_name ("AMH_", "", buf);
  amh_name += buf;
  delete[] buf;

  *os << "if (ACE_OS::strcmp (logical_type_id, \""
      << base->repoID () << "\") == 0)" << be_idt_nl
      << "return ACE_static_cast ("
      << amh_name.c_str () << "*, this);" << be_uidt_nl;

  return 0;
}
