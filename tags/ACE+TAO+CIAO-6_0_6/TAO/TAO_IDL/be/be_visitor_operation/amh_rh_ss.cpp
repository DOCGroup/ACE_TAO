//=============================================================================
/**
 *  @file   amh_rh_ss.cpp
 *
 *  $Id$
 *
 *  Creates code for AMH-RH operations.
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
//=============================================================================

be_visitor_amh_rh_operation_ss::be_visitor_amh_rh_operation_ss (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_rh_operation_ss::~be_visitor_amh_rh_operation_ss (void)
{
}

int
be_visitor_amh_rh_operation_ss::visit_operation (be_operation *node)
{
  // Nothing to be done for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  /// These are not for the server side.
  if (node->is_sendc_ami ())
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();

  UTL_Scope *s =
    this->ctx_->attribute ()
      ? this->ctx_->attribute ()->defined_in ()
      : node->defined_in ();

  be_interface *intf = be_interface::narrow_from_scope (s);

  if (intf == 0)
    {
      be_porttype *pt = be_porttype::narrow_from_scope (s);

      if (pt == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_amh_rh_operation_sh::")
                             ACE_TEXT ("visit_operation - ")
                             ACE_TEXT ("bad scope\n")),
                            -1);
        }
      else
        {
          intf = this->ctx_->interface ();
        }
    }

  char *buf = 0;
  intf->compute_full_name ("TAO_", "", buf);
  ACE_CString response_handler_implementation_name ("POA_");
  response_handler_implementation_name += buf;
  // buf was allocated by ACE_OS::strdup, so we need to use free instead
  // of delete.
  ACE_OS::free (buf);
  buf = 0;

  // Step 1 : Generate return type: always void
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "void" << be_nl
      << response_handler_implementation_name.c_str () << "::";

  // Check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        {
          *os << "_set_";
        }
      else
        {
          *os << "_get_";
        }
    }

  *os << this->ctx_->port_prefix ().c_str ()
      << node->local_name ();

  // Step 2 : Generate the params of the method
  be_visitor_context ctx (*this->ctx_);

  // Set the substate because response handler operations without
  // parameters don't use the environment parameter in the body.
  ctx.sub_state (TAO_CodeGen::TAO_AMH_RESPONSE_HANDLER_OPERATION);

  be_visitor_operation_arglist visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_rh_operation_ss::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  int is_an_exception_reply = 0;

  // Find out if the operation is one of the *_excep() operations, the
  // conditions are:
  // 1) The local_name ends in _excep()
  // 2) There is exactly one argument
  // 3) The argument takes an implied valuetype generated from the
  //    original interface
  // 4) The implied valuetype ends in ExceptionHolder
  const char *last_underbar = ACE_OS::strrchr (node->full_name (), '_');

  if (last_underbar != 0
      && ACE_OS::strcmp (last_underbar, "_excep") == 0)
    {
      if (node->nmembers () == 1)
        {
          UTL_ScopeActiveIterator i (node,
                                     UTL_Scope::IK_decls);

          if (!i.is_done ())
            {
              be_argument *argument =
                be_argument::narrow_from_decl (i.item ());
              be_valuetype *vt =
                be_valuetype::narrow_from_decl (argument->field_type ());

              if (vt != 0
                  && vt->original_interface () == intf->original_interface ())
                {
                  const char *last_E =
                    ACE_OS::strrchr (vt->full_name (), 'E');

                  if (last_E != 0
                      && ACE_OS::strcmp (last_E, "ExceptionHolder") == 0)
                    {
                      is_an_exception_reply = 1;
                    }
                }
            }
        }
    }

  if (is_an_exception_reply)
    {
      // Remove the trailing '_excep' from the operation name, we know
      // there is one from the checks above...
      ACE_CString operation_name (node->local_name ()->get_string ());
      ACE_CString::size_type const idx = operation_name.rfind ('_');
      ACE_ASSERT (idx != ACE_CString::npos);
      operation_name[idx] = '\0';

      *os << be_nl << "{" << be_idt_nl
          << "try" << be_nl
          << "{" << be_idt_nl
          << "holder->raise_" << operation_name.c_str ()
          << " ();" << be_uidt_nl
          << "}" << be_nl
          << "catch ( ::CORBA::Exception& ex)"
          << be_nl
          << "{" << be_idt_nl
          << "this->_tao_rh_send_exception (ex);" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}";
    }
  else
    {
      // Step 3: Generate actual code for the method
      *os << be_nl << "{" << be_idt_nl
          << "this->_tao_rh_init_reply ();" << be_nl_2;

      this->marshal_params (node);

      *os << be_nl
          << "this->_tao_rh_send_reply ();" << be_uidt_nl
          << "}";
    }

  return 0;
}


int
be_visitor_amh_rh_operation_ss::marshal_params (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled out.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      // marshal the in and inout arguments
      *os << "if (!(" << be_idt << be_idt_nl;

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      be_visitor_operation_argument_invoke visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_amh_rh_operation_ss::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }

      *os << be_uidt_nl << "))" << be_nl
          << "{" << be_idt_nl;

      // If marshaling fails, raise exception.
      if (this->gen_raise_exception ("::CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) gen_raise_exception failed\n"),
                            -1);
        }

      *os << be_uidt_nl
          << "}" << be_uidt_nl;
    }

  return 0;
}
