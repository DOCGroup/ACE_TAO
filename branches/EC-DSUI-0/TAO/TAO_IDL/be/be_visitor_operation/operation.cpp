//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_operation,
           operation,
           "$Id$")

// ************************************************************
// Generic Operation visitor
// ************************************************************

be_visitor_operation::be_visitor_operation (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    operation_name_ (0)
{
}

be_visitor_operation::~be_visitor_operation (void)
{
  delete [] operation_name_;
}

// Is the operation return type void?
int
be_visitor_operation::void_return_type (be_type *bt)
{
  if (bt->node_type () == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType::PredefinedType pdt =
        be_predefined_type::narrow_from_decl (bt)->pt ();

      if (pdt == AST_PredefinedType::PT_void)
        {
          return 1;
        }
    }

  return 0;
}

int
be_visitor_operation::has_param_type (be_operation *node,
                                      AST_Argument::Direction dir)
{
  return (node->count_arguments_with_direction (dir) != 0);
}

size_t
be_visitor_operation::count_non_out_parameters (be_operation *node)
{
  // @@ Once the valuetype issue discussed below is fixed we can
  //    replace this routine with:
  //
  // return node->count_arguments_with_direction (AST_Argument::dir_IN
  //                                              | AST_Argument::dir_INOUT);
  //
  size_t count = 0;

  // Initialize an iterator to iterate thru our scope.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      be_argument *bd =
        be_argument::narrow_from_decl (si.item ());

      // We do not generate insertion operators for valuetypes
      // yet. Do not include them in the count.
      be_valuetype *vt =
        be_valuetype::narrow_from_decl (bd->field_type ());

      if (bd && (bd->direction () != AST_Argument::dir_OUT) && !vt)
        {
          ++count;
        }
    }

  return count;
}

int
be_visitor_operation::is_amh_exception_holder (be_interface *node)
{
  int is_an_amh_exception_holder = 0;
  const char *amh_underbar = "AMH_";
  const char *node_name = node->local_name ();

  if( amh_underbar[0] == node_name[0] &&
      amh_underbar[1] == node_name[1] &&
      amh_underbar[2] == node_name[2] &&
      amh_underbar[3] == node_name[3]
      ) // node name starts with "AMH_"
    {
      //ACE_DEBUG ((LM_DEBUG, "Passed first test of amh_excepholder \n"));
      const char *last_E = ACE_OS::strrchr (node->full_name (), 'E');
      if (last_E != 0
          && ACE_OS::strcmp (last_E, "ExceptionHolder") == 0)
        {
          //ACE_DEBUG ((LM_DEBUG, "be_visitor_operation: Passed second test of amh_excepholder \n"));
          is_an_amh_exception_holder = 1;
        }
    }

  return is_an_amh_exception_holder;
}

// Method to generate the throw specs for exceptions that are thrown by the
// operation.
int
be_visitor_operation::gen_throw_spec (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  const char *throw_spec_open = "throw (";
  const char *throw_spec_close = ")";

  if (!be_global->use_raw_throw ())
    {
      throw_spec_open = "ACE_THROW_SPEC ((";
      throw_spec_close = "))";
    }

  UTL_Scope *scope = node->defined_in ();
  be_interface *iface = be_interface::narrow_from_scope (scope);

  /***************************************************************************/
  // 2.6
  // Generate the Right Throw Spec if it is an AMH ExceptionHolder
  /***************************************************************************/
  // Check if this is (IF and it's not VT) or (it is an AMH ExceptionHolder).
  if (iface != 0)
    {
      int is_amh_exception_holder = this->is_amh_exception_holder (iface);
      AST_Decl::NodeType nt = iface->node_type ();

      if (nt != AST_Decl::NT_valuetype || is_amh_exception_holder)
        {
          *os << be_nl << throw_spec_open;
          *os << be_idt_nl << "CORBA::SystemException";

          if (node->exceptions ())
            {
              // Initialize an iterator to iterate thru the exception list.
              for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
                   !ei.is_done ();
                   ei.next ())
                {
                  be_exception *excp =
                    be_exception::narrow_from_decl (ei.item ());

                  if (excp == 0)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                                         "(%N:%l) be_visitor_operation"
                                         "gen_throw_spec - "
                                         "bad exception node\n"),
                                        -1);

                    }

                  *os << be_nl << ", ";
                  *os << excp->name ();
                }
            }

          *os << be_uidt_nl << throw_spec_close << be_uidt;
        }
    }
  /*******************************************************************************/
  return 0;
}

int
be_visitor_operation::gen_environment_decl (int argument_emitted,
                                            be_operation *node)
{
  // Generate the CORBA::Environment parameter for the alternative mapping.
  if (be_global->exception_support ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Use ACE_ENV_SINGLE_ARG_DECL or ACE_ENV_ARG_DECL depending on
  // whether the operation node has parameters.
  const char *env_decl = "ACE_ENV_SINGLE_ARG_DECL";

  if (this->ctx_->sub_state ()
        == TAO_CodeGen::TAO_AMH_RESPONSE_HANDLER_OPERATION
      && node->argument_count () == 0)
    {
      // Response handler operations don't use the environment arg
      // unless there are other args in the operation.
      env_decl = "ACE_ENV_SINGLE_ARG_DECL";
      this->ctx_->sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);
    }
  else if (argument_emitted || node->argument_count () > 0)
    {
      env_decl = "ACE_ENV_ARG_DECL";
    }

  TAO_CodeGen::CG_STATE cgs = this->ctx_->state ();

  if (node->argument_count () > 0
      || cgs == TAO_CodeGen::TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH
      || cgs == TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH
      || cgs == TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS)
    {
      *os << be_nl;
    }

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      // Last argument is always CORBA::Environment.
      *os << env_decl << "_WITH_DEFAULTS";
      break;
    default:
      *os << env_decl;
      break;
    }

  return 0;
}

// Method that returns the appropriate CORBA::Environment variable.
const char *
be_visitor_operation::gen_environment_var (void)
{
  static const char *ace_try_env_decl = "ACE_DECLARE_NEW_CORBA_ENV;";
  static const char *null_env_decl = "";

  // Check if we are generating stubs/skeletons for
  // true C++ exception support.
  if (be_global->exception_support ())
    {
      return ace_try_env_decl;
    }
  else
    {
      return null_env_decl;
    }
}

int
be_visitor_operation::gen_raise_exception (be_type *return_type,
                                           const char *exception_name,
                                           const char *exception_arguments)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (be_global->use_raw_throw ())
    {
      *os << "throw "
          << exception_name << "(" << exception_arguments << ");\n";
      return 0;
    }

  int is_void =
    return_type == 0 || this->void_return_type (return_type);

  if (is_void)
    {
      *os << "ACE_THROW (";
    }
  else
    {
      *os << "ACE_THROW_RETURN (";
    }

  *os << exception_name << " (" << exception_arguments << ")";

  if (is_void)
    {
      *os << ");";

      return 0;
    }

  *os << ",";

  // Non-void return type.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_raise_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");";

  return 0;
}

int
be_visitor_operation::gen_check_exception (be_type *return_type)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (return_type == 0 || this->void_return_type (return_type))
    {
      *os << "ACE_CHECK;" << be_nl;
      return 0;
    }

  // Non-void return type....
  *os << "ACE_CHECK_RETURN (";
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_check_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");" << be_nl;
  return 0;
}

int
be_visitor_operation::gen_check_interceptor_exception (be_type *return_type)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (return_type == 0 || this->void_return_type (return_type))
    {
      *os << "TAO_INTERCEPTOR_CHECK;\n";
      return 0;
    }

  // Non-void return type.
  *os << "TAO_INTERCEPTOR_CHECK_RETURN (";
  be_visitor_context ctx (*this->ctx_);
  be_visitor_operation_rettype_return_cs visitor (&ctx);

  if (return_type->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation::"
                         "gen_check_exception - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << ");\n";
  return 0;
}

int
be_visitor_operation::gen_stub_operation_body (
    be_operation *node,
    be_type *return_type
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;
  const char *target = "_collocated_tao_target_";

  if (node->defined_in ()->is_abstract ())
    {
      target = "this";
    }

  *os << be_nl << "{" << be_idt_nl;

  const char *env = this->gen_environment_var ();

  if (ACE_OS::strcmp ("", env) != 0)
    {
      *os << env << be_nl;
    }

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always ACE_ENV_ARG_PARAMETER
  if (!node->has_native ())
    {
      // native type does not exist.

      // Generate any "pre" stub information such as tables or declarations
      // This is a template method and the actual work will be done by the
      // derived class
      if (this->gen_pre_stub_info (node) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "gen_pre_stub_info failed\n"
            ),
            -1
          );
        }
    }

  // Declare return type.
  ctx = *this->ctx_;
  be_visitor_operation_rettype_vardecl_cs rd_visitor (&ctx);

  if (return_type->accept (&rd_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  if (node->void_return_type () == 0)
    {
      *os << be_nl;
    }

  if (node->has_native ()) // native exists => no stub
    {
      if (this->gen_raise_exception (return_type,
                                     "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "codegen for return var failed\n"
            ),
            -1
          );
        }
    }
  else
    {
      // Generate code that retrieves the underlying stub object and then
      // invokes do_static_call on it.
      *os << "TAO_Stub *istub = " << target << "->_stubobj ();"
          << be_nl << be_nl
          << "if (istub == 0)" << be_idt_nl
          << "{" << be_idt_nl;

      // If the stub object was bad, then we raise a system exception.
      if (this->gen_raise_exception (return_type, "CORBA::INTERNAL", "") == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "codegen for checking exception failed\n"
            ),
            -1
          );
        }

      *os << be_uidt_nl << "}" << be_uidt_nl;

      // Do any pre marshal and invoke processing with return type. This
      // includes allocating memory, initialization.
      ctx = *this->ctx_;
      be_visitor_operation_rettype_pre_invoke_cs rpi_visitor (&ctx);

      if (return_type->accept (&rpi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "codegen for retval pre invoke failed\n"
            ),
            -1
          );
        }

      // Do any pre marshal and invoke stuff with arguments.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_PRE_INVOKE_CS);
      be_visitor_operation_argument api_visitor (&ctx);

      if (node->accept (&api_visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "codegen for argument pre invoke failed\n"
            ),
            -1
          );
        }

      // Generate the code for marshaling in the parameters and transmitting
      // them.
      if (this->gen_marshal_and_invoke (node, return_type) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "codegen for marshal and invoke failed\n"
            ),
            -1
          );

        }

      if (!this->void_return_type (return_type))
        {
          AST_Decl::NodeType nt = return_type->node_type ();

          if (nt == AST_Decl::NT_typedef)
            {
              AST_Typedef *td = AST_Typedef::narrow_from_decl (return_type);
              AST_Type *t = td->primitive_base_type ();
              nt = t->node_type ();
            }

          *os << be_nl << be_nl;

          // Now generate the normal successful return statement.
          if (return_type->size_type () == AST_Type::VARIABLE
              || nt == AST_Decl::NT_array)
            {
              *os << "return _tao_retval._retn ();";
            }
          else
            {
              *os << "return _tao_retval;";
            }
        }
    } // end of if (!native)

  *os << be_uidt_nl << "}";

  return 0;
}

int
be_visitor_operation::gen_pre_stub_info (
    be_operation *node
  )
{
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_exceptlist_cs visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_cs::"
                         "gen_pre_stub_info - "
                         "Exceptionlist generation error\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation::gen_marshal_and_invoke (
    be_operation *node,
    be_type *bt
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;
  const char *target = "_collocated_tao_target_";

  if (node->defined_in ()->is_abstract ())
    {
      target = "this";
    }


  os->indent ();

  *os << be_nl;

  // Create the GIOP_Invocation and grab the outgoing CDR stream.
  switch (node->flags ())
    {
    case AST_Operation::OP_oneway:
      *os << "TAO_GIOP_Oneway_Invocation _tao_call ";
      break;
    default:
      *os << "TAO_GIOP_Twoway_Invocation _tao_call ";
    }

  // Do we have "_set_" or "_get_" prepended?
  size_t ext = this->ctx_->attribute () ? 5 : 0;

  // Do we have any arguments in the operation that needs marshalling
  int flag =
    node->count_arguments_with_direction (AST_Argument::dir_IN
                                          | AST_Argument::dir_INOUT);

  *os << "(" << be_idt << be_idt_nl
      << "istub," << be_nl
      << this->compute_operation_name (node)
      << "," << be_nl
      << ACE_OS::strlen (node->original_local_name ()->get_string ()) + ext
      << "," << be_nl
      << flag
      << "," <<be_nl
      << "istub->orb_core ()" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << be_nl
      << "int _invoke_status;" << be_nl;

  // Generate code to obtain the client request interceptors from the
  // ORB.
  *os << "\n#if (TAO_HAS_INTERCEPTORS == 1)" << be_nl;
  *os << "TAO_ClientRequestInterceptor_Adapter _tao_vfr ("
      << be_idt << be_idt_nl
      << "istub->orb_core ()->client_request_interceptors ()," << be_nl
      << "&_tao_call," << be_nl
      << "_invoke_status"
      << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << "\n#endif  /* TAO_HAS_INTERCEPTORS */" << be_nl;

  // The connection retry loop.
  *os << be_nl
      << "for (;;)" << be_idt_nl
      << "{" << be_idt_nl
      << "_invoke_status = TAO_INVOKE_EXCEPTION;" << be_nl;


  *os << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl;

  *os << "TAO_ClientRequestInfo_" << node->flat_name ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << " _tao_ri (" << be_idt << be_idt_nl
      << "&_tao_call," << be_nl
      << target;

  // Generate the formal argument fields which are passed
  // to the RequestInfo object.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS);
  be_visitor_operation_interceptors_arglist iia_visitor (&ctx);

  if (node->accept (&iia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "visit_operation - "
                         "codegen for arglist failed\n"),
                        -1);
    }

  *os << be_uidt_nl << ");" << be_uidt_nl;

  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << "\n#endif /* TAO_HAS_INTERCEPTORS */" << be_nl;

 // Prepare the request header.
  *os << be_nl << "CORBA::Short _tao_response_flag = ";

  switch (node->flags ())
    {
    case AST_Operation::OP_oneway:
      *os << "_tao_call.sync_scope ();" << be_nl
          << "TAO_INTERCEPTOR (_tao_ri.response_expected (0));" << be_nl;
      break;
    default:
      *os << "TAO_TWOWAY_RESPONSE_FLAG;" << be_nl
          << "TAO_INTERCEPTOR (_tao_ri.response_expected (1));" << be_nl;
    }

  *os << "\n#if TAO_HAS_INTERCEPTORS == 1" << be_nl;

  *os << be_nl << "ACE_TRY" << be_idt_nl
      << "{" << be_idt_nl;

  // Invoke send_request() interception point.
  // This is done before the Invocation::start() call so that a
  // connection can be avoided if send_request() throws an exception,
  // i.e. this is an optimization.
  *os << "_tao_vfr.send_request (" << be_idt << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_nl;

  // _invoke_status is potentially set (via a reference) in
  // TAO_ClientRequestInterceptor_Adapter::send_request() so check its
  // value.  If a location forward or transport retry occured, then we
  // need to restart the loop so that the ClientRequestInfo object is
  // setup for the re-issued request.
  //
  // Note that since we're invoking the send_request() interception
  // point before the Invocation::start() call, i.e. before a
  // connection is ever made, we're actually able to forward requests
  // for oneways via the PortableInterceptor::ForwardRequest exception
  // too!
  *os  << be_nl
       << "if (_invoke_status == TAO_INVOKE_RESTART)" << be_idt_nl
       << "{" << be_idt_nl
       << "_tao_call.restart_flag (1);" << be_uidt_nl
       << "}" << be_uidt_nl
       << "else" << be_idt_nl
       << "{" << be_idt_nl;

  *os << "\n#endif /* TAO_HAS_INTERCEPTORS */" << be_nl;


  *os << be_nl
      << "_tao_call.start (ACE_ENV_SINGLE_ARG_PARAMETER);" << be_nl;

  // Check if there is an exception.
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

 *os << be_nl
      << "_tao_call.prepare_header (" << be_idt << be_idt_nl
      << "ACE_static_cast (CORBA::Octet, _tao_response_flag)" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl;

  // Check if there is an exception.
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in.
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
      be_visitor_operation_argument_invoke oai_visitor (&ctx);

      if (node->accept (&oai_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for return var in "
                             "do_static_call failed\n"),
                            -1);
        }

      *os << be_nl
          << "))" << be_uidt_nl
          << "{" << be_idt_nl;

      // If marshaling fails, raise exception (codesetting has various minors)
      *os << "TAO_OutputCDR::throw_stub_exception (errno "
          << "ACE_ENV_ARG_PARAMETER); "
          << be_nl;

      if (this->gen_check_interceptor_exception (bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for checking exception failed\n"),
                            -1);
        }

      *os << be_uidt_nl;
      *os << "}" << be_uidt_nl << be_nl;
    }
  else
    {
      *os << be_nl << be_nl;
    }

  *os << "_invoke_status =" << be_idt_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    {
      // Oneway operation.
      *os << "_tao_call.invoke (ACE_ENV_SINGLE_ARG_PARAMETER);";
    }
  else
    {
      if (node->exceptions ())
        {
          *os << "_tao_call.invoke (_tao_" << node->flat_name ()
              << "_exceptiondata, "
              << node->exceptions ()->length ()
              << " ACE_ENV_ARG_PARAMETER);";
        }
      else
        {
          *os << "_tao_call.invoke (0, 0 ACE_ENV_ARG_PARAMETER);";
        }
    }

  *os << be_uidt_nl;

  // Check if there is an exception.
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << be_nl
      << "if (_invoke_status == TAO_INVOKE_EXCEPTION)" << be_idt_nl
      << "{" << be_idt_nl;

  // Unlisted user exception received by client.
  int status = this->gen_raise_interceptor_exception (
                         bt,
                         "CORBA::UNKNOWN",
                         "CORBA::OMGVMCID | 1, CORBA::COMPLETED_YES"
                       );

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and invoke - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "}" << be_uidt_nl;

  // Note that we no longer turn this code generation off if it's a
  // one way operation since the sync scope policy may actually allow
  // things such as LOCATION_FORWARD replies to be propagated back to
  // the client (e.g. SYNC_WITH_TARGET).

  *os << "else if (_invoke_status == TAO_INVOKE_RESTART)" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_call.restart_flag (1);" << be_nl
      << "TAO_INTERCEPTOR (" << be_idt << be_idt_nl
      << "_tao_ri.reply_status (_invoke_status);" << be_nl
      << "_tao_vfr.receive_other (" << be_idt << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt;

  // If we reach here, we are ready to proceed.
  // the code below this is for twoway operations only.

  if (!this->void_return_type (bt)
      || this->has_param_type (node, AST_Argument::dir_INOUT)
      || this->has_param_type (node, AST_Argument::dir_OUT))

    {
      // Do any post_invoke stuff that might be necessary.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_POST_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      be_visitor_operation_argument oapi_visitor (&ctx);

      if (node->accept (&oapi_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for args in post do_static_call\n"),
                            -1);
        }

      // Generate any temporary variables to demarshal the arguments.
      be_visitor_args_decl vis1 (&ctx);

      if (node->accept (&vis1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "gen_pre_stub_info - "
                             "codegen for pre args failed\n"),
                            -1);
        }

      if (!this->void_return_type (bt))
        {
          // Generate any temporary variables to demarshal the return value.
          be_visitor_operation_rettype_post_invoke_cs vis2 (&ctx);

          if (bt->accept (&vis2) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "gen_pre_stub_info - "
                                 "codegen rettype [post invoke] failed\n"),
                                -1);
            }
        }

      // Check if there was a user exception, else demarshal the
      // return val (if any) and parameters (if any) that came with
      // the response message.
      *os << be_nl << be_nl
          << "TAO_InputCDR &_tao_in = _tao_call.inp_stream ();"
          << be_nl ;

      //  Added so codeset translators may be used to decode reply
      *os << "_tao_call.transport()->assign_translators (&_tao_in,0);"
          << be_nl << be_nl;

      // reply
      *os << "if (!(" << be_idt << be_idt;

      if (!this->void_return_type (bt))
        {
          // Demarshal the return value.
          ctx = *this->ctx_;
          ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
          be_visitor_operation_rettype_marshal_ss ori_visitor (&ctx);

          if (node->accept (&ori_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "gen_marshal_and_invoke - "
                                 "codegen for return var failed\n"),
                                -1);
            }
        }

      if (this->has_param_type (node, AST_Argument::dir_INOUT)
          || this->has_param_type (node, AST_Argument::dir_OUT))
        {
          if (!this->void_return_type (bt))
            {
              *os << " &&" << be_nl;
            }

          // Demarshal each out and inout argument.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
          ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
          be_visitor_operation_argument_invoke oai_visitor (&ctx);

          if (node->accept (&oai_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "gen_marshal_and_invoke - "
                                 "codegen for return var failed\n"),
                                -1);
            }
        }

      *os << be_nl
          << "))" << be_uidt_nl
          << "{" << be_idt_nl;

      // If marshaling fails, raise exception (codesetting has various minors)
      *os << "TAO_InputCDR::throw_stub_exception (errno "
          << "ACE_ENV_ARG_PARAMETER); "
          << be_nl;
      if (this->gen_check_interceptor_exception (bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for checking exception failed\n"),
                            -1);
        }

      *os << be_nl;

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "gen_marshal_and invoke - "
                             "codegen for return var failed\n"),
                            -1);
        }

      *os << be_uidt_nl << "}" << be_uidt_nl;
    }

  *os << "\n#if TAO_HAS_INTERCEPTORS == 1"  << be_nl;

  // Populate the ClientRequestInfo object with result, if any, of the
  // invocation.
  if (!this->void_return_type (bt))
    {
      // Here's what we are going to do to have a uniform way of getting the
      // return value updated for the Request Info:
      // declare a operation_retval type object and assign the
      // _tao_retval._retn () to it.
      // We pass this to the result updation method (note: it hasnt
      // got destroyed)
      // We then put it back into the original _tao_retval
      // object.
      // And finally the _retn () is returned from the operation w.o
      // causing any problems.

      // Generate the return type mapping (same as in the header file)
      ctx = *this->ctx_;
      be_visitor_operation_rettype oro_visitor (&ctx);

      if (bt->accept (&oro_visitor) == -1)
        {
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_remote_proxy_impl_cs::"
              "visit_operation - "
              "codegen for return type failed\n"
            ),
            -1
          );
        }

      if (bt->size_type () == AST_Type::VARIABLE
          || bt->node_type () == AST_Decl::NT_array)
        {
          *os << " _tao_retval_info =" << be_idt_nl
              << "_tao_retval._retn ();" << be_uidt_nl
              << "_tao_ri.result (_tao_retval_info);" << be_nl
              << "_tao_retval = _tao_retval_info;" << be_nl;
        }
      else
        {
          *os << " _tao_retval_info =" << be_idt_nl
              << "_tao_retval;" << be_uidt_nl
              << "_tao_ri.result (_tao_retval_info);" << be_nl;
        }
    }

  // Oneway operations don't have receive_reply() interception since
  // once the request goes over the wire, its the end of the story!
  // However, we still need to call an ending interception point
  // (receive_other()) to satisfy the General Flow Rules.
  if (node->flags () != AST_Operation::OP_oneway)
    {
      // If we get this far (in the generated code) then we
      // successfully completed the request, i.e. no connection retry
      // will occur, nor will a LOCATION_FORWARD.
      // Invoke receive_reply() interception point.
      *os << be_nl
          << "_tao_ri.reply_status (_invoke_status);" << be_nl
          << "_tao_vfr.receive_reply (" << be_idt << be_idt_nl
          << "&_tao_ri" << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt_nl;
    }
  else if (node->flags () == AST_Operation::OP_oneway)
    {
      // Invoke receive_other() interception point.
      *os << be_nl
          << "_tao_ri.reply_status (_invoke_status);" << be_nl
          << "_tao_vfr.receive_other (" << be_idt_nl
          << "&_tao_ri" << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_nl;
    }

  *os << "ACE_TRY_CHECK;"
      << be_uidt_nl;

  *os << "\n#endif  /* TAO_HAS_INTERCEPTORS */" << be_nl;

  *os << "}" << be_uidt << be_uidt_nl;  // End inner "else" block.

  // Note that we do NOT catch the PortableInterceptor::ForwardRequest
  // exception here.  It is caught in the
  // TAO_ClientRequestInterceptor_Adapter class.  This is necessary to
  // prevent applications from being able to throw the exception in an
  // effort to get an easy (but illegal) way to forward a request.

  *os << "\n#if TAO_HAS_INTERCEPTORS == 1"  << be_nl
      << "}" << be_uidt << be_uidt_nl // End outer "else" block.
      << "}" << be_uidt_nl
      << "ACE_CATCHANY" << be_idt_nl
      << "{" << be_idt_nl;

  // Update the exception field of the ClientRequestInfo.
  *os << "_tao_ri.exception (&ACE_ANY_EXCEPTION);"<< be_nl;

  *os << "_tao_vfr.receive_exception (" << be_idt << be_idt_nl
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

  *os << "}" << be_uidt_nl;

  // Convert non-CORBA C++ exceptions to CORBA::UNKNOWN.
  *os << "\n# if defined (ACE_HAS_EXCEPTIONS) \\\n"
      << "     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)" << be_nl
      << "ACE_CATCHALL" << be_idt_nl
      << "{" << be_idt_nl
      << "CORBA::UNKNOWN ex;" << be_nl
      << be_nl
      << "_tao_ri.exception (&ex);"<< be_nl
      << "_tao_vfr.receive_exception (" << be_idt << be_idt_nl
      << "&_tao_ri" << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_nl;

  // The receive_exception() interception point may have thrown a
  // PortableInterceptor::ForwardRequest exception.  In that event,
  // the connection retry loop must be restarted so do not throw the
  // CORBA::UNKNOWN exception to convert the unhandled C++ exception.
  *os << be_nl
      << "const PortableInterceptor::ReplyStatus _tao_status =" << be_idt_nl
      << "_tao_ri.reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_nl;

  *os << be_nl
      << "if (_tao_status == PortableInterceptor::SYSTEM_EXCEPTION)"
      << be_idt_nl;

  if (be_global->use_raw_throw ())
    {
      *os << "throw ";
    }
  else
    {
      *os << "ACE_TRY_THROW ";
    }

  *os << "(ex);" << be_uidt << be_uidt_nl
      << "}" << be_uidt
      << "\n# endif  /* ACE_HAS_EXCEPTIONS"
      << " && ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */" << be_nl << be_nl;

  *os << "ACE_ENDTRY;" << be_nl;

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
      << "if (_tao_status != PortableInterceptor::LOCATION_FORWARD" << be_nl
      << "    && _tao_status != PortableInterceptor::TRANSPORT_RETRY)"
      << be_idt_nl;
  // Continue to below break statement."
  *os << "\n#endif  /* TAO_HAS_INTERCEPTORS */" << be_nl;

  *os << be_nl
      << "if (_invoke_status != TAO_INVOKE_RESTART)" << be_idt_nl
      << "break;" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt;

  return 0;
}

int
be_visitor_operation::gen_raise_interceptor_exception (
    be_type *bt,
    const char *excep,
    const char *completion_status
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->void_return_type (bt))
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw " << excep << "(" << completion_status << ");";
        }
      else
        {
          *os << "TAO_INTERCEPTOR_THROW (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")" << be_uidt << be_uidt_nl
              << ");" << be_uidt;
        }
    }
  else
    {
      if (bt->size_type () == AST_Type::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")," << be_uidt_nl
              <<  "0" << be_uidt_nl
              << ");" << be_uidt;
        }
      else
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << be_idt << be_idt_nl
              << excep << " (" << be_idt << be_idt_nl
              << completion_status << be_uidt_nl
              << ")," << be_uidt_nl
              <<  "_tao_retval" << be_uidt_nl
              << ");" << be_uidt;
        }
    }

  return 0;
}

const char*
be_visitor_operation::compute_operation_name (
    be_operation *node
  )
{
  if (this->operation_name_ == 0)
    {
      // Length for two double quotes and the null termination char.
      size_t len = 3;

      if (this->ctx_->attribute ())
        {
          // "Added length for "_set_" or "_get_".
          len += 5;
        }

      len += ACE_OS::strlen (node->original_local_name ()->get_string ());

      ACE_NEW_RETURN (this->operation_name_,
                      char [len],
                      0);

      ACE_OS::strcpy (this->operation_name_, "\"");

      if (this->ctx_->attribute ())
        {
          // Now check if we are a "get" or "set" operation.
          if (node->nmembers () == 1)
            {
              ACE_OS::strcat (this->operation_name_, "_set_");
            }
          else
            {
              ACE_OS::strcat (this->operation_name_, "_get_");
            }
        }

      ACE_OS::strcat (this->operation_name_,
                      node->original_local_name ()->get_string ());
      ACE_OS::strcat (this->operation_name_, "\"");
    }

  return this->operation_name_;
}
