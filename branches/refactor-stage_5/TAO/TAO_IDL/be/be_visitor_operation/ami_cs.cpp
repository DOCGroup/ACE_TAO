//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale,
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//    Michael Kircher
//
// ============================================================================

ACE_RCSID (be_visitor_operation,
           operation_ami_cs,
           "$Id$")

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_cs::be_visitor_operation_ami_cs (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_cs::~be_visitor_operation_ami_cs (void)
{
}

// Processing to be done after every element in the scope is
// processed.
int
be_visitor_operation_ami_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      *os << ",\n";
    }

  return 0;
}

int
be_visitor_operation_ami_cs::visit_operation (be_operation *node)
{
  // No sendc method for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  be_visitor_context ctx;
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type mapping. Return type is simply void.
  *os << be_nl << be_nl
      << "void" << be_nl;

  // Generate the operation name.

  // Grab the scope name.
  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  if (parent == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_operation - "
                         "scope name is nil\n"),
                        -1);
    }

  // Generate the scope::operation name.
  *os << parent->full_name ()
      << "::sendc_";

    // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "set_";
        }
      else
        {
          *os << "get_";
        }
    }

  *os << node->local_name ()->get_string ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist oa_visitor (&ctx);

  if (node->arguments ()->accept (&oa_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always ACE_ENV_ARG_PARAMETER
  *os << be_nl << "{" << be_idt;

  *os << this->gen_environment_var () << be_nl;

  if (node->has_native ()) // native exists => no stub
    {
      be_predefined_type bpt (AST_PredefinedType::PT_void,
                              0);

      int status = this->gen_raise_exception (&bpt,
                                              "CORBA::MARSHAL",
                                              "");

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "codegen for return var failed\n"),
                            -1);
        }
    }
  else
    {
      *os << "if (!this->is_evaluated ())" << be_idt_nl
          << "{" << be_idt_nl
          << "ACE_NESTED_CLASS (CORBA, Object)::tao_object_initialize (this);"
          << be_uidt_nl
          << "}" << be_uidt_nl << be_nl;

      *os << "if (this->the_TAO_" << parent->local_name () 
          << "_Proxy_Broker_ == 0)" << be_idt_nl
          << "{" << be_idt_nl
          << parent->flat_name () << "_setup_collocation (" 
          << be_idt << be_idt_nl
          << "this->ACE_NESTED_CLASS (CORBA, Object)::_is_collocated ()" 
          << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl;
    }

  const char *lname = node->local_name ()->get_string ();
  long opname_len = ACE_OS::strlen (lname);
  ACE_CString opname;
  int nargs = node->argument_count ();
  idl_bool add_retval = I_FALSE;
  idl_bool add_comma = I_FALSE;

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Declare return type helper class.

      *os << be_nl
          << "TAO::Arg_Traits<";

      this->gen_arg_template_param_name (bt,
                                         os);

      *os << ">::ret_val _tao_retval;";

      // If we are a attribute node, add the length of the operation
      // name.
      opname_len += 5;

      // Count the return value.
      nargs++;

      add_retval = I_TRUE;

      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          opname = "_set_";
          add_comma = I_TRUE;
        }
      else
        {
          opname = "_get_";
        }
    }

  opname += lname;

  // Declare the argument helper classes.
  this->gen_stub_body_arglist (node, os, I_TRUE);

  *os << be_nl << be_nl
      << "TAO::Argument *_tao_signature [] =" << be_idt_nl
      << "{" << be_idt;

  if (add_retval)
    {
      *os << be_nl
          << "&_tao_retval";
    }

  if (add_comma)
    {
      *os << ",";
    }

  AST_Argument *arg = 0;
  int args_left = node->argument_count ();

  for (UTL_ScopeActiveIterator arg_list_iter (node, UTL_Scope::IK_decls);
       ! arg_list_iter.is_done ();
       arg_list_iter.next ())
    {
      arg = AST_Argument::narrow_from_decl (arg_list_iter.item ());

      if (arg->direction () == AST_Argument::dir_OUT)
        {
          nargs--;
          args_left--;
          continue;
        }

      *os << be_nl
          << "&_tao_" << arg->local_name ();

      if (--args_left)
        {
          *os << ",";
        }
    }

  *os << be_uidt_nl 
      << "}" << be_uidt;

  be_interface *intf = be_interface::narrow_from_decl (parent);

  *os << be_nl << be_nl
      << "TAO::Asynch_Invocation_Adapter _tao_call (" << be_idt << be_idt_nl
      << "this," << be_nl
      << "_tao_signature," << be_nl
      << nargs << "," << be_nl
      << "\"" << opname.fast_rep () << "\"," << be_nl
      << opname_len << "," << be_nl
      << "this->the" << intf->base_proxy_broker_name () << "_"
      << be_uidt_nl
      << ");" << be_uidt;

  *os << be_nl << be_nl
      << "_tao_call.invoke (" << be_idt << be_idt_nl
      << "ami_handler," << be_nl
      << "&";

  if (parent->is_nested ())
    {
      be_decl *gparent =
        be_scope::narrow_from_scope (parent->defined_in ())->decl ();

      *os << gparent->name () << "::";
    }

  *os << "AMI_" << parent->local_name () << "Handler::"
      << opname.fast_rep () + (this->ctx_->attribute () != 0) 
      << "_reply_stub" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_CHECK;";

  *os << be_uidt_nl
      << "}";

  return 0;
}

// This method is used to generate the ParamData table entry.
int
be_visitor_operation_ami_cs::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // Retrieve the type for this argument.
  bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";

  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "PARAM_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "PARAM_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "PARAM_OUT, ";
      break;
    }

  *os << "0}";

  return 0;
}

int
be_visitor_operation_ami_cs::gen_pre_stub_info (be_operation *node,
                                                be_type *bt)
{
  // Nothing to be done here, we do not through any exceptions,
  // besides system exceptions, so we do not need an user exception table.
  ACE_UNUSED_ARG (node);
  ACE_UNUSED_ARG (bt);

  return 0;
}

int
be_visitor_operation_ami_cs::gen_marshal_and_invoke (be_operation *node,
                                                     be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  os->indent ();

  // Check the flags just once, and assert, getting in this state
  // indicates a severe bug in the IDL compiler.
  ACE_ASSERT (node->flags () != AST_Operation::OP_oneway
              || !"Cannot generate AMI calls for oneways");

  *os << "TAO_GIOP_Twoway_Asynch_Invocation _tao_call("
      << be_idt << be_idt_nl
      << "istub," << be_nl;
  *os << "\"";

  size_t ext = 0;

  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          *os << "_set_";
        }
      else
        {
          *os << "_get_";
        }

      ext += 5;
    }

  // Do we have any arguments in the operation that needs marshalling
  int flag =
    node->count_arguments_with_direction (AST_Argument::dir_IN
                                          | AST_Argument::dir_INOUT) != 0;

  *os << node->local_name ()
      << "\"," << be_nl
      << ACE_OS::strlen (node->original_local_name ()->get_string ()) + ext
      << "," << be_nl
      << flag
      << ","<< be_nl
      << "istub->orb_core ()," << be_nl;

  // Next argument is the reply handler skeleton for this method.

  // Get the interface.
  be_decl *interface =
    be_interface::narrow_from_scope (node->defined_in ())->decl ();

  {
    char *full_name = 0;

    interface->compute_full_name ("AMI_",
                                  "Handler",
                                  full_name);

    *os << "&" << full_name << "::";

  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation
      if (node->nmembers () == 1)
        {
          *os << "set_";
        }
      else
        {
          *os << "get_";
        }
    }

    *os << node->local_name () << "_reply_stub," << be_nl;

    delete full_name;
  }


  // Next argument is the ami handler passed in for this method.
  *os << "ami_handler" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << "\n" << be_nl;

  *os << "int _invoke_status;\n"
      << "\n#if (TAO_HAS_INTERCEPTORS == 1)" << be_nl
      << "TAO_ClientRequestInterceptor_Adapter _tao_vfr ("
      << be_idt << be_idt_nl
      << "istub->orb_core ()->client_request_interceptors ()," << be_nl
      << "&_tao_call," << be_nl
      << "_invoke_status"
      << be_uidt_nl
      << ");" << be_uidt_nl
      << "\n#endif  /* TAO_HAS_INTERCEPTORS */\n" << be_nl;

  *os << "for (;;)" << be_nl
      << "{" << be_idt_nl
      << "_invoke_status = TAO_INVOKE_EXCEPTION;" << be_nl;

  *os << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
      << "TAO_AMI_ClientRequestInfo_i _tao_ri (" << be_idt << be_idt_nl
      << "&_tao_call," << be_nl
      << "this" << be_uidt_nl
      << ");" << be_uidt_nl;
  *os << "\n#endif /* TAO_HAS_INTERCEPTORS */\n" << be_nl;

  *os << "CORBA::Short _tao_response_flag = TAO_TWOWAY_RESPONSE_FLAG;\n";

  *os << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
      << "_tao_ri.response_expected(0);" << be_nl
      << "ACE_TRY" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_vfr.send_request (" << be_idt << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_TRY_CHECK;\n" << be_nl
      << "if (_invoke_status == TAO_INVOKE_RESTART)" << be_nl
      << "{" << be_idt_nl
      << "_tao_call.restart_flag (1);" << be_nl
      << "continue;" << be_uidt_nl
      << "}\n"
      << "#endif /* TAO_HAS_INTERCEPTORS */\n" << be_nl;

  *os << "_tao_call.start (ACE_ENV_SINGLE_ARG_PARAMETER);" << be_nl;

  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << be_nl
      << "_tao_call.prepare_header (" << be_idt << be_idt_nl
      << "ACE_static_cast (CORBA::Octet, _tao_response_flag)" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl << ");"
      << be_uidt_nl;

  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  // Now make sure that we have some in and inout
  // parameters. Otherwise, there is nothing to be marshaled in.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      *os << be_nl
          << "TAO_OutputCDR &_tao_out = _tao_call.out_stream ();"
          << be_nl << be_nl
          << "if (!(" << be_idt << be_idt_nl;

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      be_visitor_operation_argument_invoke visitor (&ctx);

      if (node->marshaling ()->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_ami_cs::"
              "gen_marshal_and_invoke - "
              "codegen for return var in do_static_call failed\n"
            ),
            -1
          );
        }

      *os << be_uidt_nl
          << "))" << be_nl;

      // If marshaling fails, raise exception.
      int status = this->gen_raise_interceptor_exception (
          bt, "CORBA::MARSHAL", "");

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "gen_marshal_and invoke - "
                             "codegen for return var failed\n"),
                            -1);
        }

      *os << be_uidt;
    }

  *os << be_nl << be_nl
      << "_invoke_status =" << be_idt_nl
      << "_tao_call.invoke (ACE_ENV_SINGLE_ARG_PARAMETER);"
      << be_uidt_nl;

  // Check if there is an exception.
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << be_nl
      << "if (_invoke_status == TAO_INVOKE_RESTART)" << be_nl
      << "{" << be_idt
      << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
      << "_tao_ri.reply_status (_invoke_status);" << be_nl
      << "_tao_vfr.receive_other (" << be_idt << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_TRY_CHECK;"
      << "\n#else" << be_nl
      << "_tao_call.restart_flag (1);"
      << "\n#endif  /* TAO_HAS_INTERCEPTORS */" << be_nl
      << "continue;" << be_uidt_nl
      << "}"<< be_nl
      << "else if (_invoke_status != TAO_INVOKE_OK)" << be_nl
      << "{" << be_idt_nl;

  int status =
    this->gen_raise_interceptor_exception (
      bt,
      "CORBA::UNKNOWN",
      "TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES");

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "gen_marshal_and invoke - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}\n";

  *os << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl
      << "_tao_ri.reply_status (_invoke_status);" << be_nl
      << "_tao_vfr.receive_other (" << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "ACE_CATCHANY" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_ri.exception (&ACE_ANY_EXCEPTION);" << be_nl
      << "_tao_vfr.receive_exception (" << be_idt << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_nl;
  // The receive_exception() interception point may have thrown a
  // PortableInterceptor::ForwardRequest exception.  In that event,
  // the connection retry loop must be restarted so do not rethrow the
  // caught exception.
  *os << be_nl
      << "const PortableInterceptor::ReplyStatus _tao_status =" << be_idt_nl
      << "_tao_ri.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_nl;

  *os << be_nl
      << "if (_tao_status == PortableInterceptor::SYSTEM_EXCEPTION" << be_nl
      << "    || _tao_status == PortableInterceptor::USER_EXCEPTION)"
      << be_idt_nl
      << "{" << be_idt_nl;

  if (be_global->use_raw_throw ())
    {
      *os << "throw;";
    }
  else
    {
      *os << "ACE_RE_THROW;";
    }

  *os << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl;

  *os << "}" << be_uidt_nl
      << "ACE_ENDTRY;" << be_nl;

  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }
  // The receive_exception() or receive_other() interception point may
  // have thrown a PortableInterceptor::ForwardRequest exception.  In
  // that event, the connection retry loop must be restarted.  Note
  // that the _invoke_status variable is not set by the interceptor
  // support code, so we must explicitly check the status in the
  // ClientRequestInfo object.
  *os << be_nl
      << "const PortableInterceptor::ReplyStatus _tao_status =" << be_idt_nl
      << "_tao_ri.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);" << be_uidt_nl;

  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << be_nl
      << "if (_tao_status == PortableInterceptor::LOCATION_FORWARD" << be_nl
      << "    || _tao_status == PortableInterceptor::TRANSPORT_RETRY)"
      << be_idt_nl
      << "{" << be_idt_nl
      << "continue;" << be_uidt_nl
      << "}" << be_uidt_nl;

  *os << "\n#endif  /* TAO_HAS_INTERCEPTORS */\n" << be_nl;

  *os << "break;" << be_uidt_nl
      << "}\n" << be_nl;

  // Return type is void and we are going to worry about OUT or INOUT
  // parameters. Return from here.
  return 0;
}
