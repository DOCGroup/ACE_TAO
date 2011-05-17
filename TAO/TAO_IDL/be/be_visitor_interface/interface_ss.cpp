
//=============================================================================
/**
 *  @file    interface_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the server skeletons file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "global_extern.h"
#include "ast_generator.h"
#include "ast_string.h"

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
  if (node->srv_skel_gen ()
      || node->imported ()
      || node->is_abstract ())
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

  int status =
    node->gen_operation_table (flat_name,
                               full_skel_name);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for operation ")
                         ACE_TEXT ("table failed\n")),
                        -1);
    }

  if (this->generate_proxy_classes (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for proxy classes\n")),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

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

  *os << ": TAO_ServantBase ()" << be_uidt_nl;

  // Default constructor body.
  *os << "{" << be_idt_nl
      << "this->optable_ = &tao_" << flat_name
      << "_optable;" << be_uidt_nl
      << "}" << be_nl_2;

  // find if we are at the top scope or inside some module
  *os << full_skel_name << "::"
      << local_name_prefix << node_local_name << " ("
      << "const " << local_name_prefix
      << node_local_name << "& rhs)";

  *os << be_idt_nl
      << ": TAO_Abstract_ServantBase (rhs)," << be_nl
      << "  TAO_ServantBase (rhs)";

  if (this->generate_copy_ctor (node, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT (" copy ctor generation failed\n")),
                        -1);
    }

  *os << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl_2;

  *os << full_skel_name << "::~"
      << local_name_prefix << node_local_name
      << " (void)" << be_nl;
  *os << "{" << be_nl;
  *os << "}" << be_nl;

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  // Generate code for the _is_a skeleton.
  {
    be_predefined_type rt (AST_PredefinedType::PT_boolean, 0);
    // @@ Cheat a little by placing a space before the operation name
    //    to prevent the IDL compiler from interpreting the leading
    //    underscore as an IDL escape.
    Identifier op_name (" _is_a");
    UTL_ScopedName scoped_name (&op_name, 0);
    be_operation is_a (&rt,
                       AST_Operation::OP_noflags,
                       &scoped_name,
                       node->is_local (),
                       node->is_abstract ());
    is_a.set_defined_in (node);

    ACE_CDR::ULong bound = 0UL;

    auto_ptr<AST_String> s (
      idl_global->gen ()->create_string (
        idl_global->gen ()->create_expr (bound,
                                         AST_Expression::EV_ulong)));

    Identifier arg_name ("repository_id");
    UTL_ScopedName scoped_arg_name (&arg_name, 0);
    AST_Argument *repository_id =
      idl_global->gen ()->create_argument (AST_Argument::dir_IN,
                                           s.get (),
                                           &scoped_arg_name);

    is_a.be_add_argument (repository_id);

    ACE_CString is_a_upcall_command_name =
      "_is_a_" + ACE_CString (node_local_name) + "_Upcall_Command" ;

    be_visitor_operation_upcall_command_ss upcall_command_visitor (this->ctx_);
    upcall_command_visitor.visit (&is_a,
                                  full_skel_name,
                                  is_a_upcall_command_name.c_str ());

    *os << be_nl_2
        << "void " << full_skel_name
        << "::_is_a_skel (" << be_idt << be_idt_nl
        << "TAO_ServerRequest & server_request, " << be_nl
        << "void * TAO_INTERCEPTOR (servant_upcall)," << be_nl
        << "void * servant)" << be_uidt << be_uidt_nl
        << "{" << be_idt;

    // Generate exception list.
    be_visitor_operation_exceptlist_ss exception_list (this->ctx_);
    exception_list.visit_operation (&is_a);

    be_visitor_operation_ss op_visitor (this->ctx_);

    *os << "TAO::SArg_Traits< ";

    op_visitor.gen_arg_template_param_name (&is_a,
                                            &rt,
                                            os);

    *os << ">::ret_val retval;";

    op_visitor.gen_skel_body_arglist (&is_a,
                                      os);

    *os << be_nl_2
        << "TAO::Argument * const args[] =" << be_idt_nl
        << "{" << be_idt_nl
        << "&retval," << be_nl
        << "&_tao_" << arg_name.get_string ()
        << be_uidt_nl
        << "};" << be_uidt_nl << be_nl;

    *os << "static size_t const nargs = 2;" << be_nl_2;

    // Get the right object implementation.
    *os << full_skel_name << " * const impl =" << be_idt_nl
        << "static_cast<"
        << full_skel_name << " *> (servant);"
        << be_uidt_nl;

    // Upcall_Command instantiation.
    *os << be_nl
        << is_a_upcall_command_name.c_str()
        << " command (" << be_idt_nl
        << "impl";

    if (!is_a.void_return_type ()
        || is_a.argument_count () > 0)
      {
        // server_request.operation_details () will be non-zero in the
        // thru-POA collocation case.  Use them if available.
        *os << "," << be_nl;

        if (be_global->gen_thru_poa_collocation ())
          *os << "server_request.operation_details ()," << be_nl;

        *os << "args";
      }

    *os << ");" << be_uidt_nl << be_nl;

    *os << "TAO::Upcall_Wrapper upcall_wrapper;" << be_nl
        << "upcall_wrapper.upcall (server_request" << be_nl
        << "                       , args" << be_nl
        << "                       , nargs" << be_nl
        << "                       , command"
        << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
        << "                       , servant_upcall" << be_nl
        << "                       , exceptions" << be_nl
        << "                       , nexceptions"
        << "\n#endif  /* TAO_HAS_INTERCEPTORS == 1 */" << be_nl
        << "                       );";

    this->generate_send_reply (os);

    *os << be_uidt_nl
        << "}" << be_nl_2;

    is_a.destroy ();
    rt.destroy ();
    s.get ()->destroy ();
  }

  if (!be_global->gen_minimum_corba ())
  {
    // Generate code for the _non_existent skeleton.
    be_predefined_type rt (AST_PredefinedType::PT_boolean, 0);
    // @@ Cheat a little by placing a space before the operation name
    //    to prevent the IDL compiler from interpreting the leading
    //    underscore as an IDL escape.
    Identifier op_name (" _non_existent");
    UTL_ScopedName scoped_name (&op_name, 0);
    be_operation non_existent (&rt,
                               AST_Operation::OP_noflags,
                               &scoped_name,
                               node->is_local (),
                               node->is_abstract ());
    non_existent.set_defined_in (node);

    ACE_CString non_exist_upcall_command_name =
      "_non_existent_"
      + ACE_CString (node_local_name)
      + "_Upcall_Command";

    be_visitor_operation_upcall_command_ss upcall_command_visitor (this->ctx_);
    upcall_command_visitor.visit (&non_existent,
                                  full_skel_name,
                                  non_exist_upcall_command_name.c_str ());

    *os << be_nl_2
        << "void " << full_skel_name
        << "::_non_existent_skel (" << be_idt << be_idt_nl
        << "TAO_ServerRequest & server_request, " << be_nl
        << "void * TAO_INTERCEPTOR (servant_upcall)," << be_nl
        << "void * servant)" << be_uidt << be_uidt_nl
        << "{" << be_idt;

    // Generate exception list.
    be_visitor_operation_exceptlist_ss exception_list (this->ctx_);
    exception_list.visit_operation (&non_existent);

    be_visitor_operation_ss op_visitor (this->ctx_);

    *os << "TAO::SArg_Traits< ";

    op_visitor.gen_arg_template_param_name (&non_existent,
                                            &rt,
                                            os);

    *os << ">::ret_val retval;";

    op_visitor.gen_skel_body_arglist (&non_existent,
                                      os);

    *os << be_nl_2
        << "TAO::Argument * const args[] =" << be_idt_nl
        << "{" << be_idt_nl
        << "&retval"
        << be_uidt_nl
        << "};" << be_uidt_nl << be_nl;

    *os << "static size_t const nargs = 1;" << be_nl_2;

    // Get the right object implementation.
    *os << full_skel_name << " * const impl =" << be_idt_nl
        << "static_cast<"
        << full_skel_name << " *> (servant);"
        << be_uidt_nl;

    // Upcall_Command instantiation.
    *os << be_nl
        << non_exist_upcall_command_name.c_str()
        << " command (" << be_idt_nl
        << "impl";

    if (!non_existent.void_return_type ()
        || non_existent.argument_count () > 0)
      {
        // server_request.operation_details () will be non-zero in the
        // thru-POA collocation case.  Use them if available.
        *os << "," << be_nl;

        if (be_global->gen_thru_poa_collocation ())
          *os << "server_request.operation_details ()," << be_nl;

        *os << "args";
      }

    *os << ");" << be_uidt_nl << be_nl;

    *os << "TAO::Upcall_Wrapper upcall_wrapper;" << be_nl
        << "upcall_wrapper.upcall (server_request" << be_nl
        << "                       , args" << be_nl
        << "                       , nargs" << be_nl
        << "                       , command"
        << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
        << "                       , servant_upcall" << be_nl
        << "                       , exceptions" << be_nl
        << "                       , nexceptions"
        << "\n#endif  /* TAO_HAS_INTERCEPTORS == 1 */" << be_nl
        << "                       );";

    this->generate_send_reply (os);

    *os << be_uidt_nl
        << "}" << be_nl;

    non_existent.destroy ();
    rt.destroy ();
  }

  if (!be_global->gen_minimum_corba ())
  {
    ACE_CDR::ULong bound = 0UL;

    // Generate code for the _repository_id skeleton.
    auto_ptr<AST_String> s (
      idl_global->gen ()->create_string (
        idl_global->gen ()->create_expr (bound,
                                         AST_Expression::EV_ulong)));

    // @@ Cheat a little by placing a space before the operation name
    //    to prevent the IDL compiler from interpreting the leading
    //    underscore as an IDL escape.
    Identifier op_name (" _repository_id");
    UTL_ScopedName scoped_name (&op_name, 0);
    be_operation repository_id (s.get (),
                                AST_Operation::OP_noflags,
                                &scoped_name,
                                node->is_local (),
                                node->is_abstract ());
    repository_id.set_defined_in (node);

    ACE_CString repository_id_upcall_command_name =
      "_repository_id_"
      + ACE_CString (node_local_name)
      + "_Upcall_Command" ;

    be_visitor_operation_upcall_command_ss upcall_command_visitor (this->ctx_);
    upcall_command_visitor.visit (&repository_id,
                                  full_skel_name,
                                  repository_id_upcall_command_name.c_str ());

    *os << be_nl_2
        << "void " << full_skel_name
        << "::_repository_id_skel (" << be_idt << be_idt_nl
        << "TAO_ServerRequest & server_request, " << be_nl
        << "void * TAO_INTERCEPTOR (servant_upcall)," << be_nl
        << "void * servant)" << be_uidt << be_uidt_nl
        << "{" << be_idt;

    // Generate exception list.
    be_visitor_operation_exceptlist_ss exception_list (this->ctx_);
    exception_list.visit_operation (&repository_id);

    be_visitor_operation_ss op_visitor (this->ctx_);

    *os << "TAO::SArg_Traits< ";

    op_visitor.gen_arg_template_param_name (&repository_id,
                                            s.get (),
                                            os);

    *os << ">::ret_val retval;";

    op_visitor.gen_skel_body_arglist (&repository_id,
                                      os);

    *os << be_nl_2
        << "TAO::Argument * const args[] =" << be_idt_nl
        << "{" << be_idt_nl
        << "&retval"
        << be_uidt_nl
        << "};" << be_uidt_nl << be_nl;

    *os << "static size_t const nargs = 1;" << be_nl_2;

    // Get the right object implementation.
    *os << full_skel_name << " * const impl =" << be_idt_nl
        << "static_cast<"
        << full_skel_name << " *> (servant);"
        << be_uidt_nl;

    // Upcall_Command instantiation.
    *os << be_nl
        << repository_id_upcall_command_name.c_str()
        << " command (" << be_idt_nl
        << "impl";

    if (!repository_id.void_return_type ()
        || repository_id.argument_count () > 0)
      {
        // server_request.operation_details () will be non-zero in the
        // thru-POA collocation case.  Use them if available.
        *os << "," << be_nl;

        if (be_global->gen_thru_poa_collocation ())
          *os << "server_request.operation_details ()," << be_nl;

        *os << "args";
      }

    *os << ");" << be_uidt_nl << be_nl;

    *os << "TAO::Upcall_Wrapper upcall_wrapper;" << be_nl
        << "upcall_wrapper.upcall (server_request" << be_nl
        << "                       , args" << be_nl
        << "                       , nargs" << be_nl
        << "                       , command"
        << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
        << "                       , servant_upcall" << be_nl
        << "                       , exceptions" << be_nl
        << "                       , nexceptions"
        << "\n#endif  /* TAO_HAS_INTERCEPTORS == 1 */" << be_nl
        << "                       );";

    this->generate_send_reply (os);

    *os << be_uidt_nl
        << "}";

    repository_id.destroy ();
    s.get ()->destroy ();
  }

  if (!be_global->gen_corba_e ()
      && !be_global->gen_minimum_corba ())
  {
    *os << be_nl_2;

    *os << "// TAO_IDL - Generated from " << be_nl
        << "// " << __FILE__ << ":" << __LINE__;

    *os << be_nl_2
        << "void " << full_skel_name
        << "::_interface_skel (" << be_idt << be_idt_nl
        << "TAO_ServerRequest & server_request, " << be_nl
        << "void * /* servant_upcall */," << be_nl
        << "void * servant)" << be_uidt << be_uidt_nl;
    *os << "{" << be_idt_nl;
    *os << "TAO_IFR_Client_Adapter *_tao_adapter =" << be_idt_nl
        << "ACE_Dynamic_Service<TAO_IFR_Client_Adapter>::instance ("
        << be_idt << be_idt_nl
        << "TAO_ORB_Core::ifr_client_adapter_name ()"
        << be_uidt_nl
        << ");" << be_uidt_nl << be_uidt_nl;
    *os << "if (!_tao_adapter)" << be_idt_nl
        << "{" << be_idt_nl
        << "throw ::CORBA::INTF_REPOS (::CORBA::OMGVMCID | 1, ::CORBA::COMPLETED_NO);"
        << be_uidt_nl
        << "}" << be_uidt_nl << be_nl;

    // Get the right object implementation.
    *os << full_skel_name << " * const impl =" << be_idt_nl
        << "static_cast<"
        << full_skel_name << " *> (servant);"
        << be_uidt_nl;

    *os << "::CORBA::InterfaceDef_ptr _tao_retval = impl->_get_interface ();"
        << be_nl
        << "server_request.init_reply ();" << be_nl
        << "TAO_OutputCDR &_tao_out = *server_request.outgoing ();"
        << be_nl_2
        << "::CORBA::Boolean const _tao_result =" << be_idt_nl
        << "_tao_adapter->interfacedef_cdr_insert (_tao_out, _tao_retval);"
        << be_uidt_nl << be_nl
        << "_tao_adapter->dispose (_tao_retval);"
        << be_nl_2;

    *os << "if (!_tao_result)" << be_idt_nl
        << "{" << be_idt_nl
        << "throw ::CORBA::MARSHAL ();" << be_uidt_nl
        << "}" << be_uidt;

    this->generate_send_reply (os);

    *os << be_uidt_nl
        << "}" << be_nl_2;
  }

  // Generate code for the _component skeleton, don't generate it when
  // we use CORBA/e
  if (!be_global->gen_corba_e () && !be_global->gen_minimum_corba ())
  {
    be_predefined_type rt (AST_PredefinedType::PT_object, 0);
    // @@ Cheat a little by placing a space before the operation name
    //    to prevent the IDL compiler from interpreting the leading
    //    underscore as an IDL escape.

    // Yes, _get_component()
    Identifier op_name (" _get_component");
    UTL_ScopedName scoped_name (&op_name, 0);
    be_operation get_component (&rt,
                                AST_Operation::OP_noflags,
                                &scoped_name,
                                node->is_local (),
                                node->is_abstract ());
    get_component.set_defined_in (node);

    ACE_CString get_component_upcall_command_name =
      "_get_component_"
      + ACE_CString (node_local_name)
      + "_Upcall_Command" ;

    be_visitor_operation_upcall_command_ss upcall_command_visitor (this->ctx_);
    upcall_command_visitor.visit (&get_component,
                                  full_skel_name,
                                  get_component_upcall_command_name.c_str());

    *os << be_nl_2
        << "void " << full_skel_name
        << "::_component_skel (" << be_idt << be_idt_nl
        << "TAO_ServerRequest & server_request, " << be_nl
        << "void * TAO_INTERCEPTOR (servant_upcall)," << be_nl
        << "void * servant" << be_uidt_nl
        << ")" << be_uidt_nl
        << "{" << be_idt;

    // Generate exception list.
    be_visitor_operation_exceptlist_ss exception_list (this->ctx_);
    exception_list.visit_operation (&get_component);

    be_visitor_operation_ss operation_visitor (this->ctx_);

    *os << "TAO::SArg_Traits< ";

    operation_visitor.gen_arg_template_param_name (&get_component,
                                                   &rt,
                                                   os);

    *os << ">::ret_val retval;";

    *os << be_nl_2
        << "TAO::Argument * const args[] =" << be_idt_nl
        << "{" << be_idt_nl
        << "&retval"
        << be_uidt_nl
        << "};" << be_uidt_nl << be_nl;

    *os << "static size_t const nargs = 1;" << be_nl_2;

    // Get the right object implementation.
    *os << full_skel_name << " * const impl =" << be_idt_nl
        << "static_cast<"
        << full_skel_name << " *> (servant);"
        << be_uidt_nl;

    // Upcall_Command instantiation.
    *os << be_nl
        << get_component_upcall_command_name.c_str ()
        << " command (" << be_idt_nl
        << "impl";

    if (!get_component.void_return_type ()
        || get_component.argument_count () > 0)
      {
        // server_request.operation_details () will be non-zero in the
        // thru-POA collocation case.  Use them if available.
        *os << "," << be_nl;

        if (be_global->gen_thru_poa_collocation ())
          *os << "server_request.operation_details ()," << be_nl;

        *os << "args";
      }

    *os << ");" << be_uidt_nl << be_nl;


    *os << "TAO::Upcall_Wrapper upcall_wrapper;" << be_nl
        << "upcall_wrapper.upcall (server_request" << be_nl
        << "                       , args" << be_nl
        << "                       , nargs" << be_nl
        << "                       , command"
        << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
        << "                       , servant_upcall" << be_nl
        << "                       , exceptions" << be_nl
        << "                       , nexceptions"
        << "\n#endif  /* TAO_HAS_INTERCEPTORS == 1 */" << be_nl
        << "                       );" << be_uidt_nl
        << "}";

    get_component.destroy ();
    rt.destroy ();
  }

  // Generate code for the _is_a override.

  *os << be_nl_2
      << "::CORBA::Boolean " << full_skel_name
      << "::_is_a (const char* value)" << be_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "(" << be_idt_nl;

  if (node->traverse_inheritance_graph (be_interface::is_a_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_ss::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("traversal of inhertance ")
                         ACE_TEXT ("graph failed\n")),
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
          << "\"IDL:omg.org/CORBA/AbstractBase:1.0\""
          << be_uidt_nl
          << ")";
    }

  *os << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "const char* " << full_skel_name
      << "::_interface_repository_id (void) const"
      << be_nl;
  *os << "{" << be_idt_nl;
  *os << "return \"" << node->repoID () << "\";" << be_uidt_nl;
  *os << "}";

  // Print out dispatch method.
  this->dispatch_method (node);

  this->this_method (node);

  if (be_global->gen_tie_classes () && !node->tie_skel_gen ())
    {
      // Generate the TIE class.
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_ROOT_TIE_SS);
      ctx.stream (tao_cg->server_template_skeletons ());
      be_visitor_interface_tie_ss visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_interface_ss::")
                             ACE_TEXT ("visit_interface - ")
                             ACE_TEXT ("codegen for TIE ")
                             ACE_TEXT ("class failed\n")),
                            -1);
        }

      // AMH generation reuses this visit_interface(), hence the flag.
      node->tie_skel_gen (true);
    }

  return 0;
}

int
be_visitor_interface_ss::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ss::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ss::gen_abstract_ops_helper (
  be_interface *node,
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
                             ACE_TEXT ("be_visitor_interface_ss::")
                             ACE_TEXT ("gen_abstract_ops_helper - ")
                             ACE_TEXT ("bad node in this scope\n")),
                            -1);
        }

      AST_Decl::NodeType nt = d->node_type ();

      UTL_ScopedName *item_new_name = 0;
      UTL_ScopedName *new_name = 0;

      if (AST_Decl::NT_op == nt || AST_Decl::NT_attr == nt)
        {
          ACE_NEW_RETURN (item_new_name,
                          UTL_ScopedName (d->local_name ()->copy (),
                                          0),
                          -1);

          new_name = (UTL_ScopedName *) node->name ()->copy ();
          new_name->nconc (item_new_name);
        }
      else
        {
          continue;
        }

      // We pass the node's is_abstract flag to the operation
      // constructor so we will get the right generated operation
      // body if we are regenerating an operation from an
      // abstract interface in a concrete interface or component.
      if (AST_Decl::NT_op == nt)
        {
          be_operation *op = be_operation::narrow_from_decl (d);
          UTL_ScopedName *old_name =
            (UTL_ScopedName *) op->name ()->copy ();
          op->set_name (new_name);
          op->set_defined_in (node);
          op->is_abstract (node->is_abstract ());

          be_visitor_operation_ss op_visitor (&ctx);
          op_visitor.visit_operation (op);

          op->set_name (old_name);
          op->set_defined_in (base);
          op->is_abstract (base->is_abstract ());
        }
      else if (AST_Decl::NT_attr == nt)
        {
          AST_Attribute *attr =
            AST_Attribute::narrow_from_decl (d);
          be_attribute new_attr (attr->readonly (),
                                 attr->field_type (),
                                 0,
                                 attr->is_local (),
                                 attr->is_abstract ());
          new_attr.set_defined_in (node);
          new_attr.set_name (new_name);

          UTL_ExceptList *get_exceptions =
            attr->get_get_exceptions ();

          if (0 != get_exceptions)
            {
              new_attr.be_add_get_exceptions (get_exceptions->copy ());
            }

          UTL_ExceptList *set_exceptions =
            attr->get_set_exceptions ();

          if (0 != set_exceptions)
            {
              new_attr.be_add_set_exceptions (set_exceptions->copy ());
            }

          be_visitor_attribute attr_visitor (&ctx);
          attr_visitor.visit_attribute (&new_attr);
          ctx.attribute (0);
          new_attr.destroy ();
        }
    }

  return 0;
}

void
be_visitor_interface_ss::this_method (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  // The _this () operation.
  *os << node->full_name () << " *" << be_nl
      << node->full_skel_name ()
      << "::_this (void)" << be_nl
      << "{" << be_idt_nl
      << "TAO_Stub *stub = this->_create_stub ();"
      << be_nl_2
      << "TAO_Stub_Auto_Ptr safe_stub (stub);" << be_nl;

  /* Coverity whines about an unused return value from _nil() when
     initializing tmp.  Just use zero instead. */
  *os << "::CORBA::Object_ptr tmp = CORBA::Object_ptr ();"
      << be_nl_2;

   if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
     {
       *os << "::CORBA::Boolean const _tao_opt_colloc ="
           << be_idt_nl
           << "stub->servant_orb_var ()->orb_core ()->"
           << "optimize_collocation_objects ();" << be_uidt_nl << be_nl;
     }
  *os << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "tmp," << be_nl
      << "::CORBA::Object (stub, ";

  if (be_global->gen_direct_collocation()
      || be_global->gen_thru_poa_collocation ())
    {
      *os << "_tao_opt_colloc";
    }
  else
    {
      *os << "false";
    }

  *os << ", this)," << be_nl
      << "0" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << "::CORBA::Object_var obj = tmp;" << be_nl
      << "(void) safe_stub.release ();" << be_nl_2
      << "typedef ::" << node->name () << " STUB_SCOPED_NAME;"
      << be_nl
      << "return" << be_idt_nl
      << "TAO::Narrow_Utils<STUB_SCOPED_NAME>::unchecked_narrow ("
      << be_idt << be_idt_nl
      << "obj.in ()," << be_nl;

  if (be_global->gen_direct_collocation()
      || be_global->gen_thru_poa_collocation ())
    {
      *os << node->flat_client_enclosing_scope ()
          << node->base_proxy_broker_name ()
          << "_Factory_function_pointer" << be_uidt_nl;
    }
  else
    {
      *os << "0" << be_uidt_nl;
    }

  *os << ");" << be_uidt << be_uidt << be_uidt_nl
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

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl_2;

  *os << "void " << node->full_skel_name ()
      << "::_dispatch (TAO_ServerRequest & req, void * servant_upcall)"
      << be_nl;
  *os << "{" << be_idt_nl;
  *os << "this->synchronous_upcall_dispatch (req, servant_upcall, this);"
      << be_uidt_nl;
  *os << "}";
}

int
be_visitor_interface_ss::generate_amh_classes (be_interface *node)
{
  // We have to check for any abstract ancestor until AMH is integrated
  // with abstract interfaces. If the node itself is abstract, this
  // visitor would not be created.
  if (be_global->gen_amh_classes () && !node->has_mixed_parentage ())
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

      // Do not generate strategized proxy broker for thru-POA case.
      // It isn't necessary.
      if (be_global->gen_direct_collocation ())
        {
          ctx = *this->ctx_;
          be_visitor_interface_strategized_proxy_broker_ss ispb_visitor (&ctx);

          if (node->accept (&ispb_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_interface_ss::")
                                 ACE_TEXT ("generate_proxy_classes - ")
                                 ACE_TEXT ("codegen for Base Proxy Broker ")
                                 ACE_TEXT ("class failed\n")),
                                -1);
            }
        }

      if (be_global->gen_direct_collocation()
          || be_global->gen_thru_poa_collocation ())
      {
        *os << be_nl_2;

        *os << "// TAO_IDL - Generated from" << be_nl
            << "// " << __FILE__ << ":" << __LINE__;

        // Proxy Broker Factory Function.
        *os << be_nl_2
            << "TAO::Collocation_Proxy_Broker *" << be_nl
            << node->flat_client_enclosing_scope ()
            << node->base_proxy_broker_name ()
            << "_Factory_function ( ::CORBA::Object_ptr)"
            << be_nl
            << "{" << be_idt_nl
            << "return";

        if (be_global->gen_direct_collocation ())
          {
            *os << be_idt_nl
                << "::"
                << node->full_strategized_proxy_broker_name ()
                << "::" <<"the"
                << node->strategized_proxy_broker_name ()
                << " ();" << be_uidt;
          }
        else
          {
            // @@ HACK!

            // Dummy function pointer for the thru-POA case.  It isn't
            // used to call a function but it is used to determine if
            // collocation is available.

            // @todo Change the way TAO's ORB_Core detects collocation,
            //       or at least augment it so that we don't have to
            //       resort this hack.
            *os << " reinterpret_cast<TAO::Collocation_Proxy_Broker *> (0xdead);"
                << " // Dummy";
          }

        *os << be_uidt_nl
            << "}" << be_nl_2;

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
            << "}" << be_nl_2;

        *os << "static int" << be_nl
            << node->flat_client_enclosing_scope ()
            << node->base_proxy_broker_name ()
            << "_Stub_Factory_Initializer_Scarecrow ="
            << be_idt_nl
            << node->flat_client_enclosing_scope ()
            << node->base_proxy_broker_name ()
            << "_Factory_Initializer (" << be_idt << be_idt_nl
            << "reinterpret_cast<size_t> ("
            << node->flat_client_enclosing_scope ()
            << node->base_proxy_broker_name ()
            << "_Factory_Initializer));" << << be_uidt << be_uidt << be_uidt_nl;
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
                             ACE_TEXT ("be_visitor_interface_cs::")
                             ACE_TEXT ("generate_proxy_classes - ")
                             ACE_TEXT ("codegen for Base Proxy ")
                             ACE_TEXT ("Broker class failed\n")),
                            -1);
        }
    }

  return 0;
}

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
