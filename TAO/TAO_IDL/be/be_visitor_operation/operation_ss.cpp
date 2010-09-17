
//=============================================================================
/**
 *  @file    operation_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Operation in the server skeleton
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

be_visitor_operation_ss::be_visitor_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ss::~be_visitor_operation_ss (void)
{
}

int
be_visitor_operation_ss::visit_operation (be_operation * node)
{
  /// No server-side code generation for these implied IDL nodes.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

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
                         ACE_TEXT ("be_visitor_operation_ss::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("Bad return type\n")),
                        -1);
    }

  return this->gen_skel_operation_body (node, bt);
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
      *os << "::CORBA::ARG_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "::CORBA::ARG_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "::CORBA::ARG_OUT, ";
      break;
    }
  *os << "0}";

  return 0;
}

int
be_visitor_operation_ss::gen_pre_skel_info (be_operation * node)
{
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_exceptlist_ss visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_visitor_operation_ss::"
                         "gen_pre_skel_info - "
                         "Exception TypeCode list generation error\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation_ss::gen_skel_operation_body (be_operation * node,
                                                  be_type * return_type)
{
  TAO_OutStream * const os = this->ctx_->stream ();

  // We need the interface node in which this operation was
  // defined.  However, if this operation node was an attribute node
  // in disguise, we get this information from the context.
  UTL_Scope *s = this->ctx_->attribute ()
                 ? this->ctx_->attribute ()->defined_in ()
                 : node->defined_in ();

  be_interface *intf = be_interface::narrow_from_scope (s);

  if (intf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_operation_ss::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("bad interface scope\n")),
                        -1);
    }

  ACE_CString upcall_command_name =
    this->ctx_->port_prefix ()
    + ACE_CString (node->local_name ()->get_string ()) + "_"
    + ACE_CString (intf->local_name ());

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        {
          upcall_command_name = "_set_" + upcall_command_name;
        }
      else
        {
          upcall_command_name = "_get_" + upcall_command_name;
        }
    }

  // Generate the local class encapsulating the actual servant upcall
  // command/invocation.
  be_visitor_operation_upcall_command_ss upcall_command_visitor (this->ctx_);
  upcall_command_visitor.visit (node,
                                intf->full_skel_name (),
                                upcall_command_name.c_str ());

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

  *os << this->ctx_->port_prefix ().c_str () << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest & server_request," << be_nl
      << "void * TAO_INTERCEPTOR (servant_upcall)," << be_nl
      << "void * servant)" << be_uidt << be_uidt_nl;

  // Generate the actual code for the skeleton. However, if any of the
  // argument types is "native", we do not generate any skeleton
  // last argument - is always CORBA::Environment.
  *os << "{" << be_idt;

  // Generate all the tables and other pre-skel info.
  if (this->gen_pre_skel_info (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_pre_skel_info failed\n"),
                        -1);
    }

  // Declare return type helper class.

  *os << "TAO::SArg_Traits< ";

  this->gen_arg_template_param_name (node,
                                     return_type,
                                     os);

  *os << ">::ret_val retval;";

  // Declare the argument helper classes.
  this->gen_skel_body_arglist (node, os);

  *os << be_nl << be_nl
      << "TAO::Argument * const args[] =" << be_idt_nl
      << "{" << be_idt_nl
      << "&retval";

  for (UTL_ScopeActiveIterator arg_list_iter (node, UTL_Scope::IK_decls);
       ! arg_list_iter.is_done ();
       arg_list_iter.next ())
    {
      AST_Argument * const arg =
        AST_Argument::narrow_from_decl (arg_list_iter.item ());

      *os << "," << be_nl
          << "&_tao_" << arg->local_name ();
    }

  *os << be_uidt_nl
      << "};" << be_uidt_nl << be_nl;

  *os << "static size_t const nargs = "
      << (node->argument_count () + 1) << ";" << be_nl << be_nl;

  // Get the right object implementation.
  *os << intf->full_skel_name () << " * const impl =" << be_idt_nl
      << "static_cast<"
      << intf->full_skel_name () << " *> (servant);" << be_uidt << be_uidt_nl;

  // Upcall_Command instantiation.
  *os << be_idt_nl
      << upcall_command_name.c_str()
      << " command (" << be_idt_nl
      << "impl";

  if (!node->void_return_type () || node->argument_count () > 0)
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
      << "}" << be_nl << be_nl;

  return 0;
}

void
be_visitor_operation_ss::gen_skel_body_arglist (be_operation * node,
                                                TAO_OutStream * os)
{
  for (UTL_ScopeActiveIterator arg_decl_iter (node, UTL_Scope::IK_decls);
       ! arg_decl_iter.is_done ();
       arg_decl_iter.next ())
    {
      AST_Argument * const arg =
        AST_Argument::narrow_from_decl (arg_decl_iter.item ());

      *os << be_nl
          << "TAO::SArg_Traits< ";

      this->gen_arg_template_param_name (arg,
                                         arg->field_type (),
                                         os);

      *os << ">::";

      switch (arg->direction ())
        {
          case AST_Argument::dir_IN:
            *os << "in";
            break;
          case AST_Argument::dir_INOUT:
            *os << "inout";
            break;
          case AST_Argument::dir_OUT:
            *os << "out";
          default:
            break;
        }

      *os << "_arg_val _tao_" << arg->local_name () << ";";
    }
}
