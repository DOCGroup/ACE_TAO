//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server skeletons file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface,
           interface_ss,
           "$Id$")

// ************************************************************
// Interface visitor for server skeletons.
// ************************************************************

be_visitor_interface_ss::be_visitor_interface_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ss::~be_visitor_interface_ss (void)
{
}

int
be_visitor_interface_ss::visit_interface (be_interface *node)
{
  if (node->srv_skel_gen () || node->imported () || node->is_abstract ())
    {
      return 0;
    }

  if (node->is_local ())
    {
      if (this->is_amh_rh_node (node))
        {
          // Create amh_rh_visitors.
          be_visitor_amh_rh_interface_ss amh_rh_ss_intf (this->ctx_);
          amh_rh_ss_intf.visit_interface (node);
        }

      return 0;
    }

  if (this->generate_amh_classes (node) == -1)
    {
      return -1;
    }

  ACE_CString full_skel_name_holder =
    this->generate_full_skel_name (node);

  const char *full_skel_name = full_skel_name_holder.c_str ();

  ACE_CString flat_name_holder =
    this->generate_flat_name (node);

  const char *flat_name = flat_name_holder.c_str ();

  int status = node->gen_operation_table (flat_name,
                                          full_skel_name);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for operation table failed\n"),
                        -1);
    }

  if (this->generate_proxy_classes (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for proxy classes\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Find if we are at the top scope or inside some module,
  // pre-compute the prefix that must be added to the local name in
  // each case.
  const char *local_name_prefix = "";

  if (!node->is_nested ())
    {
      local_name_prefix = "POA_";
    }

  ACE_CString node_local_name_holder =
    this->generate_local_name (node);

  const char *node_local_name = node_local_name_holder.c_str ();

  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name
      << " (void)" << be_idt_nl;

  // Initialize optable in base member initializer list.
  *os << ": TAO_ServantBase (&tao_"
      << flat_name << "_optable)" << be_uidt_nl;

  // Default constructor body.
  *os << "{" << be_nl
      << "}" << be_nl << be_nl;

  // find if we are at the top scope or inside some module
  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name << " ("
      << "const " << local_name_prefix << node_local_name << "& rhs)";

  *os << be_idt_nl
      << ": TAO_Abstract_ServantBase (rhs)," << be_nl
      << "  TAO_ServantBase (rhs)";

  if (this->generate_copy_ctor (node, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::visit_interface - "
                         " copy ctor generation failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl << be_nl;

  *os << full_skel_name << "::~"
      << local_name_prefix << node_local_name
      << " (void)" << be_nl;
  *os << "{" << be_nl;
  *os << "}";

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate code for the _is_a skeleton.
  *os << "void " << full_skel_name
      << "::_is_a_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_servant," << be_nl
      << "void * /* Servant_Upcall */" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;

//   {
//     Identifier arg_name (ACE_OS::strdup ("type_id"));
//     UTL_ScopedName arg_scoped_name (&arg_name, 0);
//     AST_Argument arg (AST_Argument::dir_in,
//                       AST_Decl::NT_pre_defined,
//                       &arg_scoped_name);


//     AST_Type rt (AST_Decl::NT_op, 0);
//     Identifier op_name (ACE_OS::strdup ("_is_a"));
//     UTL_ScopedName scoped_name (&op_name, 0);
//     be_operation is_a (&rt,
//                        AST_Operation::OP_noflags,
//                        &scoped_name,
//                        node->is_local (),
//                        node->is_abstract ());
//     is_a.set_defined_in (node);

//     be_visitor_operation_upcall_command_ss upcall_command_visitor (this->ctx_);
//     upcall_command_visitor.visit_operation (&is_a);
//   }

  *os << "TAO_InputCDR &_tao_in = _tao_server_request.incoming ();"
  << be_nl << be_nl;

  *os << full_skel_name << " *_tao_impl =" << be_idt_nl
      << "static_cast<" << full_skel_name
      << " *> (_tao_servant);" << be_uidt_nl << be_nl;

  *os << "CORBA::Boolean _tao_retval = 0;" << be_nl;
  *os << "CORBA::String_var value;" << be_nl << be_nl;

  *os << "if (!(_tao_in >> value.out ()))" << be_idt_nl
      << "{" << be_idt_nl;

  if (be_global->use_raw_throw ())
    {
      *os << "throw CORBA::MARSHAL ();" ;
    }
  else
    {
      *os << "ACE_THROW (CORBA::MARSHAL ());";
    }

  *os << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  *os << "_tao_retval = _tao_impl->_is_a (value.in () ACE_ENV_ARG_PARAMETER);"
      << be_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;
  *os << "_tao_server_request.init_reply ();" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();"
      << be_nl << be_nl;

  *os << "if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))"
      << be_idt_nl
      << "{" << be_idt_nl;

  if (be_global->use_raw_throw ())
    {
      *os << "throw CORBA::MARSHAL ();";
    }
  else
    {
      *os << "ACE_THROW (CORBA::MARSHAL ());";
    }

  *os << be_uidt_nl
      << "}" << be_uidt;

  this->generate_send_reply (os);

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;


  // Generate code for the _non_existent skeleton.
  *os << "void " << full_skel_name
      << "::_non_existent_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_servant," << be_nl
      << "void * /* Servant_Upcall */" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << full_skel_name << " *_tao_impl =" << be_idt_nl
      << "static_cast<" << full_skel_name
      << " *> (_tao_servant);" << be_uidt_nl << be_nl;

  *os << "CORBA::Boolean _tao_retval =" << be_idt_nl
      << "_tao_impl->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);"
      << be_uidt_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;

  *os << "_tao_server_request.init_reply ();" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();"
      << be_nl << be_nl;

  *os << "if (!(_tao_out << CORBA::Any::from_boolean (_tao_retval)))"
      << be_idt_nl
      << "{" << be_idt_nl;

  if (be_global->use_raw_throw ())
    {
      *os << "throw CORBA::MARSHAL ();";
    }
  else
    {
      *os << "ACE_THROW (CORBA::MARSHAL ());";
    }

  *os << be_uidt_nl
      << "}" << be_uidt;

  this->generate_send_reply (os);

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate code for the _interface skeleton.
  *os << "void " << full_skel_name
      << "::_interface_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_servant," << be_nl
      << "void * /* Servant_Upcall */" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << "TAO_IFR_Client_Adapter *_tao_adapter =" << be_idt_nl
      << "ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance ("
      << be_idt << be_idt_nl
      << "TAO_ORB_Core::ifr_client_adapter_name ()" << be_uidt_nl
      << ");" << be_uidt_nl << be_uidt_nl;
  *os << "if (_tao_adapter == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_THROW (CORBA::INTF_REPOS ());" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;
  *os << full_skel_name << " *_tao_impl =" << be_idt_nl
      << "static_cast<" << full_skel_name
      << " *> (_tao_servant);" << be_uidt_nl << be_nl;

  *os << "CORBA::InterfaceDef_ptr _tao_retval = " << be_idt_nl
      << "_tao_impl->_get_interface (ACE_ENV_SINGLE_ARG_PARAMETER);"
      << be_uidt_nl
      << "ACE_CHECK;" << be_nl << be_nl
      << "_tao_server_request.init_reply ();" << be_nl
      << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();"
      << be_nl << be_nl
      << "CORBA::Boolean _tao_result =" << be_idt_nl
      << "_tao_adapter->interfacedef_cdr_insert (" << be_idt << be_idt_nl
      << "_tao_out," << be_nl
      << "_tao_retval" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl << be_nl
      << "_tao_adapter->dispose (_tao_retval);" << be_nl << be_nl;

  *os << "if (_tao_result == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl
      << "}" << be_uidt;

  this->generate_send_reply (os);

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate code for the _component skeleton.
  *os << "void " << full_skel_name
      << "::_component_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /* Servant_Upcall */" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << full_skel_name << " *_tao_impl =" << be_idt_nl
      << "static_cast<" << full_skel_name
      << " *> (_tao_object_reference);" << be_uidt_nl << be_nl;

  *os << "CORBA::Object_var _tao_retval =" << be_idt_nl
      << "_tao_impl->_get_component (ACE_ENV_SINGLE_ARG_PARAMETER);"
      << be_uidt_nl;
  *os << "ACE_CHECK;" << be_nl << be_nl;

  *os << "_tao_server_request.init_reply ();" << be_nl;
  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();"
      << be_nl << be_nl;

  *os << "if (!(_tao_out << _tao_retval.in ()))" << be_idt_nl
      << "{" << be_idt_nl;

  if (be_global->use_raw_throw ())
    {
      *os << "throw CORBA::MARSHAL ();";
    }
  else
    {
      *os << "ACE_THROW (CORBA::MARSHAL ());";
    }

  *os << be_uidt_nl
      << "}" << be_uidt;

  this->generate_send_reply (os);

  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate code for the _is_a override.

  *os << "CORBA::Boolean " << full_skel_name
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* value" << be_nl
      << "ACE_ENV_ARG_DECL_NOT_USED" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "(" << be_idt_nl;

  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  *os << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
      << "value," << be_nl
      << "\"IDL:omg.org/CORBA/Object:1.0\"" << be_uidt_nl
      << ")";

  if (node->has_mixed_parentage ())
    {
      *os << " ||" << be_uidt_nl
          << "!ACE_OS::strcmp (" << be_idt << be_idt_nl
          << "(char *)value," << be_nl
          << "\"IDL:omg.org/CORBA/AbstractBase:1.0\"" << be_uidt_nl
          << ")";
    }

  *os << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

//   // the downcast method.
//   *os << "void* " << full_skel_name
//       << "::_downcast (" << be_idt << be_idt_nl
//       << "const char* logical_type_id" << be_uidt_nl
//       << ")" << be_uidt_nl
//       << "{" << be_idt_nl;

//   if (this->generate_downcast_implementation (node, os) == -1)
//     {
//       ACE_ERROR_RETURN ((LM_ERROR,
//                          "be_visitor_interface_ss::"
//                          "visit_interface - "
//                          "traverse for downcast implementation failed\n"),
//                         -1);
//     }

//   *os << "if (ACE_OS::strcmp (logical_type_id," << be_nl
//       << "                    \"IDL:omg.org/CORBA/Object:1.0\") == 0)"
//       << be_idt_nl
//       << "{" << be_idt_nl
//       << "return static_cast<PortableServer::Servant> (this);"
//       << be_uidt_nl
//       << "}" << be_uidt_nl << be_nl;

//   *os << "return 0;" << be_uidt_nl
//       << "}" << be_nl << be_nl;

  *os << "const char* " << full_skel_name
      << "::_interface_repository_id (void) const"
      << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return \"" << node->repoID () << "\";" << be_uidt_nl;
  *os << "}";

  // Print out dispatch method.
  this->dispatch_method (node);

  this->this_method (node);

  return 0;
}

int
be_visitor_interface_ss::gen_abstract_ops_helper (be_interface *node,
                                                  be_interface *base,
                                                  TAO_OutStream *os)
{
  if (!base->is_abstract ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_ROOT_SS);

  for (UTL_ScopeActiveIterator si (base, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ss::"
                             "gen_abstract_ops_helper - "
                             "bad node in this scope\n"),
                            -1);
        }

      UTL_ScopedName *item_new_name  = 0;
      ACE_NEW_RETURN (item_new_name,
                      UTL_ScopedName (d->local_name ()->copy (),
                                      0),
                      -1);

      UTL_ScopedName *base = (UTL_ScopedName *)node->name ()->copy ();
      base->nconc (item_new_name);

      if (d->node_type () == AST_Decl::NT_op)
        {
          // We pass the node's is_abstract flag to the operation
          // constructor so we will get the right generated operation
          // body if we are regenerating an operation from an
          // abstract interface in a concrete interface or component.
          AST_Operation *op = AST_Operation::narrow_from_decl (d);
          be_operation new_op (op->return_type (),
                               op->flags (),
                               0,
                               op->is_local (),
                               node->is_abstract ());
          new_op.set_defined_in (node);
          be_visitor_interface::add_abstract_op_args (op,
                                                      new_op);
          new_op.set_name (base);
          be_visitor_operation_ss op_visitor (&ctx);
          op_visitor.visit_operation (&new_op);
        }
      else if (d->node_type () == AST_Decl::NT_attr)
        {
          AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 0,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);
          new_attr.set_name (base);
          new_attr.be_add_get_exceptions (attr->get_get_exceptions ());
          new_attr.be_add_set_exceptions (attr->get_set_exceptions ());
          be_visitor_attribute attr_visitor (&ctx);
          attr_visitor.visit_attribute (&new_attr);
          ctx.attribute (0);
        }

      base->destroy ();
      delete base;
      base = 0;
    }

  return 0;
}

void
be_visitor_interface_ss::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // the _this () operation.
  *os << node->full_name () << " *" << be_nl
      << node->full_skel_name ()
      << "::_this (ACE_ENV_SINGLE_ARG_DECL)" << be_nl
      << "{" << be_idt_nl
      << "TAO_Stub *stub = this->_create_stub (ACE_ENV_SINGLE_ARG_PARAMETER);"
      << be_nl
      << "ACE_CHECK_RETURN (0);" << be_nl << be_nl
      << "TAO_Stub_Auto_Ptr safe_stub (stub);" << be_nl;

  *os << "CORBA::Object_ptr tmp = CORBA::Object::_nil ();"
      << be_nl << be_nl
      << "CORBA::Boolean _tao_opt_colloc =" << be_idt_nl
      << "stub->servant_orb_var ()->orb_core ()->"
      << "optimize_collocation_objects ();" << be_uidt_nl << be_nl
      << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "tmp," << be_nl
      << "CORBA::Object (stub, _tao_opt_colloc, this)," << be_nl
      << "0" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << "CORBA::Object_var obj = tmp;" << be_nl
      << "(void) safe_stub.release ();" << be_nl << be_nl
      << "typedef ::" << node->name () << " STUB_SCOPED_NAME;" << be_nl
      << "return" << be_idt_nl
      << "TAO::Narrow_Utils<STUB_SCOPED_NAME>::unchecked_narrow ("
      << be_idt << be_idt_nl
      << "obj.in ()," << be_nl
      << node->flat_client_enclosing_scope ()
      << node->base_proxy_broker_name ()
      << "_Factory_function_pointer" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}";
}

void
be_visitor_interface_ss::generate_send_reply (TAO_OutStream *)
{
  // no-op for regular interfaces
}

void
be_visitor_interface_ss::dispatch_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "void " << node->full_skel_name ()
      << "::_dispatch (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &req," << be_nl
      << "void *servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;
  *os << "{" << be_idt_nl;
  *os << "this->synchronous_upcall_dispatch (req," << be_nl
      << "                                   servant_upcall" << be_nl
      << "                                   ACE_ENV_ARG_PARAMETER);"
      << be_uidt_nl;
  *os << "}";
}

int
be_visitor_interface_ss::generate_amh_classes (be_interface *node)
{
   if (be_global->gen_amh_classes ())
    {
      be_visitor_amh_interface_ss amh_intf (this->ctx_);
      return amh_intf.visit_interface (node);
    }

  return 0;
}

int
be_visitor_interface_ss::generate_proxy_classes (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx = *this->ctx_;

  // Strategized Proxy Broker Implementation.
  if (be_global->gen_thru_poa_collocation ()
      || be_global->gen_direct_collocation ())
    {
      ctx = *this->ctx_;
      be_visitor_interface_strategized_proxy_broker_ss ispb_visitor (&ctx);

      if (node->accept (&ispb_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss::"
                             "generate_proxy_classes - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }

      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      // Proxy Broker Factory Function.
      *os << be_nl << be_nl
          << "TAO::Collocation_Proxy_Broker *" << be_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function (CORBA::Object_ptr)" << be_nl
          << "{" << be_idt_nl
          << "return" << be_idt_nl
          << "::"
          << node->full_strategized_proxy_broker_name ()
          << "::" <<"the"
          << node->strategized_proxy_broker_name ()
          << " ();" << be_uidt << be_uidt_nl
          << "}" << be_nl << be_nl;

      // Proxy Broker Function Pointer Initializer.
      *os << "int" << be_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_Initializer (size_t)" << be_nl
          << "{" << be_idt_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer = "
          << be_idt_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function;"
          << be_uidt_nl
          << be_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_nl << be_nl;

      *os << "static int" << be_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Stub_Factory_Initializer_Scarecrow =" << be_idt_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_Initializer (" << be_idt << be_idt_nl
          << "reinterpret_cast<" << be_idt << be_idt_nl
          << "size_t> (" << be_nl
          << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_Initializer" << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl << be_nl;
    }


  // Proxy Impl Implementations.
  if (be_global->gen_thru_poa_collocation ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS);
      be_visitor_interface_thru_poa_proxy_impl_ss itppi_visitor (&ctx);

      if (node->accept (&itppi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "generate_proxy_classes - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }
    }

  if (be_global->gen_direct_collocation ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS);
      be_visitor_interface_direct_proxy_impl_ss idpi_visitor (&ctx);

      if (node->accept (&idpi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_cs::"
                             "generate_proxy_classes - "
                             "codegen for Base Proxy Broker class failed\n"),
                            -1);
        }
    }

  return 0;
}

// int
// be_visitor_interface_ss::generate_downcast_implementation (be_interface *node,
//                                                            TAO_OutStream *os)
// {
//   return node->traverse_inheritance_graph (be_interface::downcast_helper,
//                                            os);
// }

int
be_visitor_interface_ss::generate_copy_ctor (be_interface *node,
                                             TAO_OutStream *os)
{
  return node->traverse_inheritance_graph (be_interface::copy_ctor_helper,
                                           os);
}

ACE_CString
be_visitor_interface_ss::generate_flat_name (be_interface *node)
{
  return ACE_CString (node->flat_name ());
}

ACE_CString
be_visitor_interface_ss::generate_local_name (be_interface *node)
{
  return ACE_CString (node->local_name ());
}

ACE_CString
be_visitor_interface_ss::generate_full_skel_name  (be_interface *node)
{
  return ACE_CString (node->full_skel_name ());
}
