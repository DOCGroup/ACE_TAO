//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_interceptors_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the server stubs.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           operation_interceptors_ss, 
           "$Id$")

// ******************************************************
// primary visitor for "operation" in server header
// ******************************************************

be_visitor_operation_interceptors_ss::be_visitor_operation_interceptors_ss (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_ss::~be_visitor_operation_interceptors_ss (
    void
  )
{
}

int
be_visitor_operation_interceptors_ss::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->generate_class_declaration (os, node) == -1)
    {
      return -1;
    }

  if (this->generate_class_definition (os, node) == -1)
    {
      return -1;
    }

  return 0;
}

int
be_visitor_operation_interceptors_ss::generate_class_declaration (
    TAO_OutStream *os,
    be_operation *node
  )
{
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);

  // save the node.
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the ServerRequestInfo object per operation to
  // be used by the interecptors.

  *os << be_nl << be_nl 
      << "class TAO_ServerRequestInfo_"<< node->flat_name ();

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

  *os << " : public TAO_ServerRequestInfo" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

  *os << "TAO_ServerRequestInfo_"<< node->flat_name ();

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

  *os << " (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request," << be_nl;

  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Pass in the pointer to the Servant_Upcall.
  *os << "TAO_Object_Adapter::Servant_Upcall *tao_servant_upcall," << be_nl;

  // Get the right object implementation.
  *os << intf->full_skel_name () << " *tao_impl";

  // Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH);
  be_visitor_operation_interceptors_arglist oia_visitor (&ctx);

  if (node->accept (&oia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

 *os  << be_uidt_nl << ");" << be_uidt_nl << be_nl;

  // Here I still need to generate the other methods + private args.
  *os << "virtual Dynamic::ParameterList * arguments ("
      << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual Dynamic::ExceptionList * exceptions ("
      << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Any * result ("
      << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual char * target_most_derived_interface ("
      << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Boolean target_is_a (" << be_idt << be_idt_nl
      << "const char * id" << be_nl
      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl;

  // Store the result for later use.
  // generate the return type.
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
  // void since we cant have a private member to be of void type.
  if (!this->void_return_type (bt))
    {
      *os << be_nl << "void result (";

      ctx = *this->ctx_;
      be_visitor_operation_rettype or_visitor (&ctx);

      if (bt->accept (&or_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

      *os << " result);\n";
    }

  *os << be_uidt_nl << "private:" << be_idt_nl;
  *os << "TAO_ServerRequestInfo_" << node->flat_name ();

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

  *os << " (" << be_idt << be_idt_nl
      << "const "<< "TAO_ServerRequestInfo_"<< node->flat_name ();

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


  *os << " &" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl
      << "void operator= (" << be_idt << be_idt_nl
      << "const "
      << "TAO_ServerRequestInfo_"<< node->flat_name ();

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


  *os << " &" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << be_uidt_nl << "private:" << be_idt_nl;

  // Get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl;" << be_nl;

  // Need to generate the args as reference memebers.
  // generate the member list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing and
  // modify it to generate reference members.
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH);
  be_visitor_operation_interceptors_arglist oiia_visitor (&ctx);

  if (node->accept (&oiia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  // Generate the result data member.
  // Generate the return type.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type if its not
  // void since we cant have a private member to be of void type.
  if (!this->void_return_type (bt))
    {
      ctx = *this->ctx_;
      be_visitor_operation_rettype tor_visitor (&ctx);

      if (bt->accept (&tor_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

      *os << " _result;";
    }

  *os << be_uidt_nl << "};";

  return 0;
}

int
be_visitor_operation_interceptors_ss::generate_class_definition (
    TAO_OutStream *os,
    be_operation *node
  )
{
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);

  // Save the node.
  this->ctx_->node (node);

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // -----------------------------------------------------------------
  // Constructor
  // -----------------------------------------------------------------

  // Generate the ServerRequestInfo object definition per operation
  // to be used by the interceptors.
  *os << be_nl << be_nl 
      << "TAO_ServerRequestInfo_" << node->flat_name ();

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

  *os << "::"
      << "TAO_ServerRequestInfo_" << node->flat_name ();

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

  *os << " (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request," << be_nl
      << "TAO_Object_Adapter::Servant_Upcall *_tao_servant_upcall," << be_nl;

  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Get the right object implementation.
  *os << intf->full_skel_name () << " *tao_impl";

  // Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SS);
  be_visitor_operation_interceptors_arglist oia_visitor (&ctx);

  if (node->accept (&oia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }

  *os << be_uidt_nl << ")" << be_nl;

  // Generate the member list and set each member but before that,
  // its necessary to pass on some args to the base class.
  *os << ": TAO_ServerRequestInfo (_tao_server_request, "
      << "_tao_servant_upcall)," << be_nl
      << "  _tao_impl (tao_impl)";

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS);
  be_visitor_operation_interceptors_arglist oiai_visitor (&ctx);

  if (node->accept (&oiai_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_args_info_ss::"
                         "visit_argument- "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "{}";

  // -----------------------------------------------------------------
  // PortableInterceptor::ServerRequestInfo::arguments()
  // -----------------------------------------------------------------
  *os << be_nl << be_nl
      << "Dynamic::ParameterList *" << be_nl;

  *os << "TAO_ServerRequestInfo_"<<node->flat_name ();

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

  *os << "::arguments (" << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "// Generate the argument list on demand." << be_nl
      << "Dynamic::ParameterList *parameter_list ="  << be_idt_nl
      << "TAO_RequestInfo_Util::make_parameter_list "
      << "(ACE_ENV_SINGLE_ARG_PARAMETER);"
      << be_uidt_nl
      << "ACE_CHECK_RETURN (0);" << be_nl
      << be_nl;

  size_t parameter_count = this->count_non_out_parameters (node);

  if (node->argument_count () == 0
      || parameter_count == 0
      // Now make sure that we have some in and inout
      // parameters. Otherwise, there is nothing to be put into
      // the Dynamic::Parameterlist.
      || (!(this->has_param_type (node, AST_Argument::dir_IN))
          &&!(this->has_param_type (node, AST_Argument::dir_INOUT))))
    {
      *os << "return parameter_list;" << be_uidt_nl;
    }
  else
    {
      *os << "Dynamic::ParameterList_var safe_parameter_list = "
          << "parameter_list;";

      // Precompute the length of the Dynamic::ParameterList.  This is
      // a nice optimization since it reduces the number of additional
      // allocations to one, instead of one for each argument, in
      // addition to remove all copying that occured when growing the
      // sequence for each parameter.

      *os << be_nl
          << "parameter_list->length (" << parameter_count << ");";

      *os << be_nl << "CORBA::ULong len = 0;";

      *os << be_nl;

      // The insertion operator is different for different nodes.  We
      // change our scope to go to the argument scope to be able to
      // decide this.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST);
      be_visitor_operation_interceptors_arglist oip_visitor (&ctx);

      if (node->accept (&oip_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for argument pre invoke failed\n"),
                            -1);
        }

      *os << be_nl
          << "return safe_parameter_list._retn ();" << be_uidt_nl;
    }

  *os << "}";

  // -----------------------------------------------------------------
  // PortableInterceptor::ServerRequestInfo::exceptions()
  // -----------------------------------------------------------------
  *os << be_nl << be_nl
      << "Dynamic::ExceptionList *" << be_nl;

  *os << "TAO_ServerRequestInfo_"<<node->flat_name ();

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

  *os << "::exceptions (" << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL" << be_uidt_nl
      << ")"<< be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl;

  if (be_global->tc_support ())
    {
      *os << "// Generate the exception list on demand." << be_nl
          << "Dynamic::ExceptionList *exception_list ="  << be_idt_nl
          << "TAO_RequestInfo_Util::make_exception_list "
          << "(ACE_ENV_SINGLE_ARG_PARAMETER);"
          << be_uidt_nl
          << "ACE_CHECK_RETURN (0);" << be_nl
          << be_nl;

      if (!node->exceptions ())
        {
          *os << "return exception_list;";
        }
      else
        {
          *os << "Dynamic::ExceptionList_var safe_exception_list = "
              << "exception_list;" << be_nl;

          // We change our scope to be able to generate the exceptionlist.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST);
          be_visitor_operation_interceptors_exceptlist oie_visitor (&ctx);

          if (node->accept (&oie_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "visit_operation - "
                                 "codegen for exceptlist failed\n"),
                                -1);
            }

          *os << be_nl
              << "return safe_exception_list._retn ();";
        }
    }
  else
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw (CORBA::NO_IMPLEMENT ());" << be_nl
              << "return 0;";
        }
      else
        {
          *os << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);";
        }
    }

  *os << be_uidt_nl << "}";

  // -----------------------------------------------------------------
  // PortableInterceptor::ServerRequestInfo::result()
  // -----------------------------------------------------------------
  *os << be_nl << be_nl 
      << "CORBA::Any * " << be_nl;

  *os << "TAO_ServerRequestInfo_"<< node->flat_name ();

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

  *os << "::result (" << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL" << be_uidt_nl
      << ")"<< be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl;

  if (be_global->any_support ())
    {
      *os << "// Generate the result on demand." << be_nl;

      bt = be_type::narrow_from_decl (node->return_type ());

      if (this->void_return_type (bt))
        {
          // Return an Any with tk_void TypeCode.
          *os << "CORBA::Boolean tk_void_any = 1;" << be_nl
              << "CORBA::Any *result_any ="  << be_idt_nl
              << "TAO_RequestInfo_Util::make_any "
              << "(tk_void_any ACE_ENV_ARG_PARAMETER);"
              << be_uidt_nl
              << "ACE_CHECK_RETURN (0);" << be_nl
              << be_nl
              << "return result_any;";
        }
      else
        {
          *os << "CORBA::Boolean tk_void_any = 0;" << be_nl
              << "CORBA::Any *result_any ="  << be_idt_nl
              << "TAO_RequestInfo_Util::make_any "
              << "(tk_void_any ACE_ENV_ARG_PARAMETER);"
              << be_uidt_nl
              << "ACE_CHECK_RETURN (0);" << be_nl
              << be_nl
              << "CORBA::Any_var safe_result_any = "
              << "result_any;" << be_nl << be_nl;

          // Generate the insertion of result into Any.
          ctx = *this->ctx_;
          be_visitor_operation_interceptors_result oir_visitor (&ctx);

          if (bt->accept (&oir_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "visit_operation - "
                                 "codegen for result failed\n"),
                                -1);
            }

          *os << be_nl
              << "return safe_result_any._retn ();";
        }
    }
  else
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw (CORBA::NO_IMPLEMENT ());" << be_nl
              << "return 0;";
        }
      else
        {
          *os << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);";
        }
    }

  *os << be_uidt_nl << "}";

  // -----------------------------------------------------------------
  // PortableInterceptor::ServerRequestInfo::target_most_derived_interface()
  // -----------------------------------------------------------------
  *os << be_nl << be_nl 
      << "char *" << be_nl;

  *os << "TAO_ServerRequestInfo_"<< node->flat_name ();

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

  *os << "::target_most_derived_interface ("
      << be_idt << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_NOT_USED" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "CORBA::string_dup (this->_tao_impl->_interface_repository_id ());"
      << be_uidt << be_uidt_nl << "}";

  // -----------------------------------------------------------------
  // PortableInterceptor::ServerRequestInfo::target_is_a()
  // -----------------------------------------------------------------
  *os << be_nl << be_nl 
      << "CORBA::Boolean" << be_nl;

  *os << "TAO_ServerRequestInfo_"<< node->flat_name ();

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

  *os << "::target_is_a (" << be_idt << be_idt_nl
      << "const char * id" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl
      << "return this->_tao_impl->_is_a (id ACE_ENV_ARG_PARAMETER);"
      << be_uidt_nl << "}";

  // -----------------------------------------------------------------

  // Update the result.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type accessor if
  // its not void since we cant have a private member to be of void.
  // type.
  if (!this->void_return_type (bt))
    {
      *os << be_nl << be_nl;
      *os << "void " << be_nl;

      *os << "TAO_ServerRequestInfo_" << node->flat_name ();

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

      *os << "::"
          << "result (";

      ctx = *this->ctx_;
      be_visitor_operation_rettype or_visitor (&ctx);

      if (bt->accept (&or_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

      *os << " result)" << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "// Update the result." << be_nl
          << " this->_result = result;" << be_uidt_nl
          << "}";
    }

  return 0;
}
