//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the server skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_operation,
           operation_ss,
           "$Id$")

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

be_visitor_operation_ss::be_visitor_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx),
    operation_name_ (0)
{
}

be_visitor_operation_ss::~be_visitor_operation_ss (void)
{
  delete [] this->operation_name_;
}

// Processing to be done after every element in the scope is processed.
int
be_visitor_operation_ss::post_process (be_decl *bd)
{
  // All we do here is to insert a comma and a newline.
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      *os << ",\n";
    }

  return 0;
}

int
be_visitor_operation_ss::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;

  this->ctx_->node (node);

  os->indent ();

  // If there is an argument of type "native", return immediately.
  if (node->has_native ())
    {
      return 0;
    }

  // Retrieve the operation return type.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "void " << intf->full_skel_name () << "::";

  // Check if we are an attribute node in disguise.
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
    }

  *os << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request," << be_nl
      << "void *_tao_servant," << be_nl
      << "void *_tao_servant_upcall" << be_nl
      << "ACE_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;

  // Generate the actual code for the skeleton. However, if any of the
  // argument types is "native", we do not generate any skeleton
  // last argument - is always CORBA::Environment.
  *os << "{\n" << be_idt;

  // Generate all the tables and other pre-skel info.
  if (this->gen_pre_skel_info (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_pre_skel_info failed\n"),
                        -1);
    }

  os->indent ();

  // Get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl =" << be_idt_nl
      << "ACE_static_cast (" << be_idt << be_idt_nl
      << intf->full_skel_name () << " *," << be_nl
      << "_tao_servant" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl;

  // Declare a return type variable.
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype_vardecl_ss ord_visitor (&ctx);

  // Do we have any arguments in the operation that needs marshalling?
  int flag =
    node->count_arguments_with_direction (AST_Argument::dir_INOUT |
                                          AST_Argument::dir_OUT);

  // Check if the flag is zero and for the return type.
  if (flag == 0 && node->void_return_type () == 1)
    {
      // There are no return type and argument values that needs to be
      // marshalled.
      *os << "_tao_server_request.argument_flag (0);" << be_nl;
    }

  if (bt->accept (&ord_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  // Declare variables for arguments.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);
  be_visitor_operation_argument oad_visitor (&ctx);

  if (node->accept (&oad_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  // Demarshal parameters.
  if (this->gen_demarshal_params (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }

  // Make the upcall and assign to the return val.
  ctx = *this->ctx_;
  be_visitor_operation_rettype_assign_ss ora_visitor (&ctx);

  if (bt->accept (&ora_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for retval assignment failed\n"),
                        -1);
    }

  // Make the upcall.
  *os << be_nl
      << "_tao_impl->" << node->local_name () << " (" << be_idt << be_idt_nl;
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS);
  be_visitor_operation_argument oau_visitor (&ctx);

  if (node->accept (&oau_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  // End the upcall.
  *os << be_uidt_nl << ");" << be_uidt_nl;

  if (!be_global->exception_support ())
    {
      *os << "TAO_INTERCEPTOR_CHECK;";
    }

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

  // Check if we are oneway in which case, we are done.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      // We are done. Nothing else to do, except closing the function body.
      *os << "if (_tao_server_request.response_expected ()" << be_idt
          << be_idt_nl
          << "&& !_tao_server_request.sync_with_server ())" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_server_request.init_reply ();" << be_uidt_nl
          << "}" << be_uidt;
    }

  // Marshal outgoing parameters.
  else if (this->gen_marshal_params (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_marshal_params failed\n"),
                        -1);
    }

  *os << be_nl << be_nl 
      << "// In case _tao_servant_upcall is not used in this function"
      << be_nl
      << "ACE_UNUSED_ARG (_tao_servant_upcall);" << be_uidt_nl
      << "}";;

  return 0;
}

int
be_visitor_operation_ss::visit_argument (be_argument *node)
{
  // This method is used to generate the ParamData table entry.

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the type for this argument.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";
  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "CORBA::ARG_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "CORBA::ARG_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "CORBA::ARG_OUT, ";
      break;
    }
  *os << "0}";

  return 0;
}

int
be_visitor_operation_ss::gen_pre_skel_info (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      // Instantiate a TAO_InputCDR variable.
      os->indent ();
      *os << "TAO_InputCDR &_tao_in = _tao_server_request.incoming ();\n";
    }

  return 0;
}

int
be_visitor_operation_ss::gen_demarshal_params (be_operation *node,
                                               be_type *)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      // demarshal the in and inout arguments
      *os << be_nl << be_nl << "if (!(" << be_idt << be_idt;

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      be_visitor_operation_argument_marshal oad_visitor (&ctx);

      if (node->accept (&oad_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ss::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }

      *os << be_nl << "))" << be_nl;

      // If marshaling fails, raise exception (codesetting has minor codes)
      *os << "{" << be_idt_nl << be_nl
          << "TAO_InputCDR::throw_skel_exception (errno ACE_ENV_ARG_PARAMETER);" << be_nl
          << "ACE_CHECK;" << be_uidt_nl
          << "}" << be_nl;

      *os << be_uidt << be_uidt;
    };

  return 0;
}

int
be_visitor_operation_ss::gen_marshal_params (be_operation *node,
                                             be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  // Setup parameters for marshaling and marshal them into the
  // outgoing stream.
  // The code below this is for 2way operations only.

  // We will be here only if we are 2way
  // first initialize a reply message
  *os << "_tao_server_request.init_reply ();";

  // We still need the following check because we maybe 2way and yet have no
  // parameters and a void return type.
  if (this->void_return_type (bt)
      && !this->has_param_type (node, AST_Argument::dir_INOUT)
      && !this->has_param_type (node, AST_Argument::dir_OUT))
    {
      return 0;
    }

  // Create temporary variables for the out and return parameters.
  if (!this->void_return_type (bt))
    {
      ctx = *this->ctx_;

      be_visitor_operation_rettype_post_upcall_ss visitor (&ctx);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ss::"
                             "gen_marshal_params - "
                             "codegen for return var [post upcall] failed\n"),
                            -1);
        }
    }

  // Generate any temporary variables to demarshal the arguments.
  ctx = *this->ctx_;
  be_visitor_args_post_upcall_ss vis1 (&ctx);

  if (node->accept (&vis1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_pre_stub_info - "
                         "codegen for pre args failed\n"),
                        -1);
    }

  *os << be_nl << be_nl
      << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();"
      << be_nl << be_nl;
  *os << "if (!(" << be_idt << be_idt;

  if (!this->void_return_type (bt))
    {
      // Demarshal the return val and each inout and out argument.
      ctx = *this->ctx_;
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      be_visitor_operation_rettype_marshal_ss orm_visitor (&ctx);

      if (node->accept (&orm_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ss::"
                             "gen_marshal_params - "
                             "codegen for return var failed\n"),
                            -1);
        }
    }

  if (this->has_param_type (node, AST_Argument::dir_INOUT)
      || this->has_param_type (node, AST_Argument::dir_OUT))
    {

      if (!this->void_return_type (bt))
        {
          // We have already printed the return val. SO put a &&.
          *os << " &&";
        }

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      be_visitor_operation_argument_marshal oam_visitor (&ctx);

      if (node->accept (&oam_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ss::"
                             "gen_marshal_params - "
                             "codegen for args failed\n"),
                            -1);
        }
    }

  *os << be_nl << "))" << be_nl;

  // If marshaling fails, raise exception (codesetting has minor codes)
  *os << "{" << be_idt_nl << be_nl
      << "TAO_OutputCDR::throw_skel_exception (errno ACE_ENV_ARG_PARAMETER);" << be_nl
      << "ACE_CHECK;" << be_uidt_nl
      << "}" << be_nl;

  *os << be_uidt << be_uidt;

  return 0;
}

const char *
be_visitor_operation_ss::compute_operation_name (be_operation *node)
{
  if (this->operation_name_ == 0)
    {
      size_t len = 3;           // The null termination char.

      if (this->ctx_->attribute ())
        {
          len += 5;               // "Added length for "_set_" or "_get_".
        }

      len += ACE_OS::strlen (node->local_name ()->get_string ());

      ACE_NEW_RETURN (this->operation_name_,
                      char [len],
                      0);

      ACE_OS::strcpy (this->operation_name_, "\"");

      if (this->ctx_->attribute ())
        {
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
                      node->local_name ()->get_string ());
      ACE_OS::strcat (this->operation_name_, "\"");
    }

  return this->operation_name_;
}
