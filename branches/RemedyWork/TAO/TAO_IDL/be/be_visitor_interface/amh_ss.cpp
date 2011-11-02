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

be_visitor_amh_interface_ss::be_visitor_amh_interface_ss (
    be_visitor_context *ctx
  )
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
be_visitor_amh_interface_ss::visit_attribute (be_attribute *node)
{
  be_visitor_amh_operation_ss visitor (this->ctx_);
  return visitor.visit_attribute (node);
}

int
be_visitor_amh_interface_ss::visit_interface (be_interface *node)
{
  // Do not generate AMH classes for any sort of implied IDL.
  if (node->original_interface () != 0)
    {
      return 0;
    }

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

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << non_amh_name.c_str () << "*" << be_nl
      << full_skel_name
      << "::_this (void)" << be_nl
      << "{" << be_idt_nl
      << "TAO_Stub *stub = this->_create_stub ();" << be_nl_2;

  *os << "TAO_Stub_Auto_Ptr safe_stub (stub);" << be_nl
      << "::CORBA::Object_ptr tmp = CORBA::Object_ptr ();" << be_nl
      << be_nl
      << "::CORBA::Boolean _tao_opt_colloc =" << be_idt_nl
      << "stub->servant_orb_var ()->orb_core ()->"
      << "optimize_collocation_objects ();" << be_uidt_nl << be_nl
      << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "tmp," << be_nl
      << "::CORBA::Object (stub, _tao_opt_colloc, this)," << be_nl
      << "0" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << "::CORBA::Object_var obj = tmp;" << be_nl
      << "(void) safe_stub.release ();" << be_nl_2;

  *os << "typedef ::" << node->name () << " STUB_SCOPED_NAME;" << be_nl
      << "return" << be_idt_nl;

  if (!node->is_abstract ())
    {
      *os << "TAO::Narrow_Utils<STUB_SCOPED_NAME>::unchecked_narrow (";
    }
  else
    {
      *os << "TAO::AbstractBase_Narrow_Utils<STUB_SCOPED_NAME>::unchecked_narrow (";
    }
  *os << "obj.in ());" << be_nl;

  *os << be_uidt << be_uidt_nl
      << "}";

}

void
be_visitor_amh_interface_ss::dispatch_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  ACE_CString full_skel_name_holder =
    this->generate_full_skel_name (node);
  const char *full_skel_name = full_skel_name_holder.c_str ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "void" << be_nl
      << full_skel_name << "::_dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest & req," << be_nl
      << "void * context" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "this->asynchronous_upcall_dispatch (" << be_idt << be_idt_nl
      << "req," << be_nl
      << "context," << be_nl
      << "this" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}";
}

void
be_visitor_amh_interface_ss::generate_send_reply (TAO_OutStream * os)
{
  *os << be_nl_2
      << "server_request.tao_send_reply ();";
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

// ****************************************************************

class TAO_IDL_Downcast_Implementation_Worker
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  TAO_IDL_Downcast_Implementation_Worker ();

  virtual int emit (be_interface *base,
                    TAO_OutStream *os,
                    be_interface *derived);
};

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
  // buf was allocated using ACE_OS::strdup, so we must use free instead
  // of delete.
  ACE_OS::free (buf);

  *os << "if (ACE_OS::strcmp (logical_type_id, \""
      << base->repoID () << "\") == 0)" << be_idt_nl
      << "return static_cast<"
      << amh_name.c_str () << "*> (this);" << be_uidt_nl;

  return 0;
}

// ****************************************************************

class TAO_IDL_Copy_Ctor_Worker
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  TAO_IDL_Copy_Ctor_Worker (void);

  virtual int emit (be_interface *base,
                    TAO_OutStream *os,
                    be_interface *derived);
};

TAO_IDL_Copy_Ctor_Worker::TAO_IDL_Copy_Ctor_Worker (void)
{
}

int
TAO_IDL_Copy_Ctor_Worker::emit (be_interface *derived,
      TAO_OutStream *os,
      be_interface *base)
{
  if (derived == base)
    {
      return 0;
    }

  *os << "," << be_idt_nl;

  if (base->is_nested ())
    {
      be_decl *scope = 0;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();

      *os << "POA_" << scope->name () << "::AMH_"
          << base->local_name () << " (rhs)";
    }
  else
    {
      // @@ This whole thing would be more efficient if we could pass the
      // ACE_CString to compute_full_name, after all it uses that
      // internally.
      ACE_CString amh_name ("POA_");

      // @@ The following code is *NOT* exception-safe.
      char *buf = 0;
      base->compute_full_name ("AMH_", "", buf);
      amh_name += buf;
      // buf was allocated by ACE_OS::strdup, so we need to use free
      // instead of delete.
      ACE_OS::free (buf);

      *os << amh_name.c_str () << " (rhs)";
    }

  *os << be_uidt;

  return 0;
}

int
be_visitor_amh_interface_ss::generate_copy_ctor (be_interface *node,
                                                 TAO_OutStream *os)
{
  // Make sure the queues are empty.
  node->get_insert_queue ().reset ();
  node->get_del_queue ().reset ();

  // Insert ourselves in the queue.
  if (node->get_insert_queue ().enqueue_tail (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::generate_copy_ctor - "
                         "error generating entries\n"),
                        -1);
    }

  TAO_IDL_Copy_Ctor_Worker worker;
  return node->traverse_inheritance_graph (worker, os);
}

// ****************************************************************

ACE_CString
be_visitor_amh_interface_ss::generate_flat_name (be_interface *node)
{
  // @@ The following code is *NOT* exception-safe.
  char *buf = 0;
  node->compute_flat_name ("AMH_", "", buf);

  // @@ This whole thing would be more efficient if we could pass the
  // ACE_CString to compute_flat_name, after all it uses that
  // internally.
  ACE_CString result (buf);
  // buf was allocated using ACE_OS::strdup, so we must use free instead
  // of delete.
  ACE_OS::free (buf);

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
  // buf was allocated using ACE_OS::strdup, so we must use free instead
  // of delete.
  ACE_OS::free (buf);

  return result;
}
