// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Root node. This is a generic visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================
ACE_RCSID (be_visitor_root,
           root,
           "$Id$")

// Generic Root visitor
be_visitor_root::be_visitor_root (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_root::~be_visitor_root (void)
{
}

// This method must be overridden by the derived root visitors.
int
be_visitor_root::init (void)
{
  return -1;
}

int be_visitor_root::visit_root (be_root *node)
{
  // Open the appropriate output file based on what state we are in. The
  // overridden "init" method of derived classes will do the job.
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::init - "
                         "failed to initialize context\n"),
                        -1);
    }

  TAO_OutStream * const os = this->ctx_->stream ();

  int status = 0;
  be_visitor_context ctx = *this->ctx_;

  switch (this->ctx_->state ())
    {
      case TAO_CodeGen::TAO_ROOT_CS:
        {
          be_visitor_arg_traits arg_visitor ("", &ctx);
          status = node->accept (&arg_visitor);

          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_root::"
                                 "visit_root - "
                                 "failed to generate stub arg traits\n"),
                                -1);
            }
        }

        break;
      case TAO_CodeGen::TAO_ROOT_SS:
        {
          be_visitor_arg_traits arg_visitor ("S", &ctx);
          status = node->accept (&arg_visitor);

          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_root::"
                                 "visit_root - "
                                 "failed to generate skeleton arg traits\n"),
                                -1);
            }

          if (be_global->gen_thru_poa_collocation ()
              || be_global->gen_direct_collocation ())
            {
              be_visitor_arg_traits arg_visitor ("", &ctx);
              status = node->accept (&arg_visitor);

              if (status == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_visitor_root::"
                                     "visit_root - "
                                     "failed to generate collocated skeleton arg traits\n"),
                                    -1);
                }
            }

          // Generate stub/skeleton argument selection function templates
          // in skeleton.
          if (be_global->gen_thru_poa_collocation ()
              && idl_global->non_local_iface_seen_)
            {
              *os << be_nl << be_nl
                  << "// Stub/skeleton argument selection function templates."
                  << be_nl
                  << "// TAO_IDL - Generated from "
                  << __FILE__ << ":" << __LINE__;

              *os << be_nl << be_nl
                  << "namespace" << be_nl
                  << "{" << be_idt_nl;

              // Return value selection function template
              *os << "template<typename T>" << be_nl
                  << "typename TAO::SArg_Traits<T>::ret_arg_type" << be_nl
                  << "get_ret_arg (TAO_Operation_Details const * details,"
                  << be_nl
                  << "             TAO::Argument * const * skel_args)" << be_nl
                  << "{" << be_idt_nl
                  << "return" << be_idt_nl
                  << "details" << be_nl
                  << "? static_cast<typename TAO::Arg_Traits<T>::ret_val *> ("
                  << be_idt << be_idt_nl
                // return value is always the first element in the
                // TAO::Argument array
                  << "details->args ()[0])->arg ()"
                  << be_uidt << be_uidt_nl
                  << ": static_cast<typename TAO::SArg_Traits<T>::ret_val *> ("
                  << be_idt << be_idt_nl
                  << "skel_args[0])->arg ();" << be_uidt << be_uidt
                  << be_uidt << be_uidt_nl
                  << "}" << be_nl << be_nl;

              // IN argument selection function template
              *os << "template<typename T>" << be_nl
                  << "typename TAO::SArg_Traits<T>::in_arg_type" << be_nl
                  << "get_in_arg (TAO_Operation_Details const * details,"
                  << be_nl
                  << "            TAO::Argument * const * skel_args," << be_nl
                  << "            size_t i)" << be_nl
                  << "{" << be_idt_nl
                  << "return" << be_idt_nl
                  << "details" << be_nl
                  << "? static_cast<typename TAO::Arg_Traits<T>::in_arg_val *> ("
                  << be_idt << be_idt_nl
                  << "details->args ()[i])->arg ()"
                  << be_uidt << be_uidt_nl
                  << ": static_cast<typename TAO::SArg_Traits<T>::in_arg_val *> ("
                  << be_idt << be_idt_nl
                  << "skel_args[i])->arg ();" << be_uidt << be_uidt
                  << be_uidt << be_uidt_nl
                  << "}" << be_nl << be_nl;

              // INOUT argument selection function template
              *os << "template<typename T>" << be_nl
                  << "typename TAO::SArg_Traits<T>::inout_arg_type" << be_nl
                  << "get_inout_arg (TAO_Operation_Details const * details,"
                  << be_nl
                  << "               TAO::Argument * const * skel_args," << be_nl
                  << "               size_t i)" << be_nl
                  << "{" << be_idt_nl
                  << "return" << be_idt_nl
                  << "details" << be_nl
                  << "? static_cast<typename TAO::Arg_Traits<T>::inout_arg_val *> ("
                  << be_idt << be_idt_nl
                  << "details->args ()[i])->arg ()"
                  << be_uidt << be_uidt_nl
                  << ": static_cast<typename TAO::SArg_Traits<T>::inout_arg_val *> ("
                  << be_idt << be_idt_nl
                  << "skel_args[i])->arg ();" << be_uidt << be_uidt
                  << be_uidt << be_uidt_nl
                  << "}" << be_nl << be_nl;

              // OUT argument selection function template
              *os << "template<typename T>" << be_nl
                  << "typename TAO::SArg_Traits<T>::out_arg_type" << be_nl
                  << "get_out_arg (TAO_Operation_Details const * details,"
                  << be_nl
                  << "             TAO::Argument * const * skel_args," << be_nl
                  << "             size_t i)" << be_nl
                  << "{" << be_idt_nl
                  << "return" << be_idt_nl
                  << "details" << be_nl
                  << "? static_cast<typename TAO::Arg_Traits<T>::out_arg_val *> ("
                  << be_idt << be_idt_nl
                  << "details->args ()[i])->arg ()"
                  << be_uidt << be_uidt_nl
                  << ": static_cast<typename TAO::SArg_Traits<T>::out_arg_val *> ("
                  << be_idt << be_idt_nl
                  << "skel_args[i])->arg ();" << be_uidt << be_uidt
                  << be_uidt << be_uidt_nl
                  << "}" << be_uidt << be_uidt_nl;

              *os << "}" << be_uidt_nl;
            }
        }

        break;
      default:
        break;
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::visit_root - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // If we are generating the client header file, this is the place to
  // generate the proxy broker factory function pointer declarations
  // and the extern declarations for non-defined interfaces.
  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CH)
    {
      be_interface *i = 0;
      be_interface_fwd *ifwd = 0;
      size_t index = 0;

      size_t size = be_global->non_local_interfaces.size ();

      if (size > 0)
        {
          *os << be_nl << be_nl
              << "// Proxy Broker Factory function pointer declarations."
              << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__;
        }

      for (index = 0; index < size; ++index)
        {
          be_global->non_local_interfaces.dequeue_head (i);

          *os << be_nl << be_nl
              << "extern " << be_global->stub_export_macro () << be_nl
              << "TAO::Collocation_Proxy_Broker *" << be_nl
              << "(*" << i->flat_client_enclosing_scope ()
              << i->base_proxy_broker_name ()
              << "_Factory_function_pointer) ("
              << be_idt << be_idt_nl
              << "CORBA::Object_ptr obj" << be_uidt_nl
              << ");" << be_uidt;
        }

      size = be_global->non_defined_interfaces.size ();

      if (size > 0)
        {
          *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__;
        }

      for (index = 0; index < size; ++index)
        {
          be_global->non_defined_interfaces.dequeue_head (ifwd);

          *os << be_nl << be_nl;

          if (ifwd->is_valuetype ())
            {
              *os << "// External declarations for undefined valuetype"
                  << be_nl << be_nl
                  << "// " << ifwd->full_name () << be_nl;

              *os << be_global->stub_export_macro () << be_nl
                  << "void" << be_nl
                  << "tao_" << ifwd->flat_name ()
                  << "_add_ref ("
                  << be_idt << be_idt_nl
                  << ifwd->full_name () << " *" << be_uidt_nl
                  << ");" << be_uidt_nl << be_nl
                  << be_global->stub_export_macro () << be_nl
                  << "void" << be_nl
                  << "tao_" << ifwd->flat_name ()
                  << "_remove_ref (" << be_idt << be_idt_nl
                  << ifwd->full_name () << " *" << be_uidt_nl
                  << ");" << be_uidt;
            }
        }

      size = be_global->mixed_parentage_interfaces.size ();

      if (size > 0)
        {
          *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

          *os << "// Overrides of CORBA::release and CORBA::is_nil for"
              << be_nl
              << "// interfaces that inherit from both CORBA::Object" << be_nl
              << "// and CORBA::AbstractBase." << be_nl << be_nl
              << "namespace CORBA" << be_nl
              << "{" << be_idt;
        }

      for (index = 0; index < size; ++index)
        {
          be_global->mixed_parentage_interfaces.dequeue_head (i);

          *os << be_nl
              << "TAO_NAMESPACE_STORAGE_CLASS void release ("
              << i->name () << "_ptr);" << be_nl
              << "TAO_NAMESPACE_STORAGE_CLASS CORBA::Boolean is_nil ("
              << i->name () << "_ptr);";
        }

      if (size > 0)
        {
          *os << be_uidt_nl
              << "}";
        }
    }

  // Make one more pass over the entire tree and generate the OBV_ namespaces
  // and OBV_ classes.

  idl_bool obv = 1;
  status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CS);
      break;
    default:
      obv = 0;
      break;
    }

  if (obv == 1)
    {
      be_visitor_obv_module visitor (&ctx);
      status = visitor.visit_scope (node);
      obv = 0;

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "failed to generate OBV_ things\n"),
                            -1);
        }
    }

  status = 0;
  ctx = *this->ctx_;

  switch (this->ctx_->state ())
    {
      case TAO_CodeGen::TAO_ROOT_CH:
        {
          be_visitor_traits visitor (&ctx);
          status = node->accept (&visitor);

          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_root::"
                                 "visit_root - "
                                 "failed to generate traits\n"),
                                -1);
            }
        }

        break;
      default:
        break;
    }

  // The next thing we need to do is make one more pass thru the entire tree
  // and generate code for all the <<= and >>= operators for all the
  // user-defined types.

  ctx = *this->ctx_;
  status = 0;

  switch (this->ctx_->state ())
    {
      case TAO_CodeGen::TAO_ROOT_CH:
        {
          ctx.state (TAO_CodeGen::TAO_ROOT_ANY_OP_CH);

          if (be_global->any_support ())
            {
              be_visitor_root_any_op visitor (&ctx);
              status = node->accept (&visitor);
           }

          break;
        }
      case TAO_CodeGen::TAO_ROOT_CS:
        {
          ctx.state (TAO_CodeGen::TAO_ROOT_ANY_OP_CS);

          if (be_global->any_support ())
            {
              be_visitor_root_any_op visitor (&ctx);
              status = node->accept (&visitor);

              // Conditional switch to the *A.cpp stream is done
              // in the visitor constructor.
              if (be_global->gen_anyop_files ())
                {
                  (void) tao_cg->end_anyop_source ();
                }
            }

          break;
        }
      case TAO_CodeGen::TAO_ROOT_IH:
      case TAO_CodeGen::TAO_ROOT_SH:
      case TAO_CodeGen::TAO_ROOT_CI:
      case TAO_CodeGen::TAO_ROOT_IS:
      case TAO_CodeGen::TAO_ROOT_SI:
      case TAO_CodeGen::TAO_ROOT_SS:
      case TAO_CodeGen::TAO_ROOT_TIE_SH:
        break;
      default:
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_constant - "
                             "Bad context state\n"),
                            -1);
        }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                     "(%N:%l) be_visitor_root::"
                         "visit_root - "
                         "failed to generate Any operators\n"),
                        -1);
    }

  // Make another pass over the entire tree and generate the CDR operators.
  ctx = *this->ctx_;
  status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ROOT_CDR_OP_CH);
        be_visitor_root_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ROOT_CDR_OP_CS);
        be_visitor_root_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      break; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_root - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_root - "
                         "failed to generate CDR operators\n"),
                        -1);
    }

  if (be_global->gen_tmplinst ())
    {
      if (this->gen_explicit_tmplinst (node, os) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "explicit template instantiation "
                             "generation failed\n"),
                            -1);
        }
    }

  // Generate any final code such as #endifs and/or EOF newlines.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      (void) tao_cg->end_client_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      (void) tao_cg->end_server_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_IS:
      break;
    case TAO_CodeGen::TAO_ROOT_IH:
      (void) tao_cg->end_implementation_header (
          be_global->be_get_implementation_hdr_fname (0)
        );
      break;
    case TAO_CodeGen::TAO_ROOT_SI:
      if (be_global->gen_tie_classes ())
        {
          (void) tao_cg->end_server_template_inline ();
        }

      break;
    case TAO_CodeGen::TAO_ROOT_SS:
      if (be_global->gen_tie_classes ())
        {
          (void) tao_cg->end_server_template_skeletons ();
        }

      (void) tao_cg->end_server_skeletons ();
      break;
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      if (be_global->gen_tie_classes ())
        {
          (void) tao_cg->end_server_template_header ();
        }

      break;
    default:
      break;
    }

  // Make sure each file ends with a newline.
  *os << "\n";

  return 0;
}

// All common visit methods for root visitor.

int
be_visitor_root::visit_constant (be_constant *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_constant_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_constant_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_constant - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_enum (be_enum *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_enum_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_enum_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_enum_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_enum_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_enum_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_enum_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_enum - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_exception (be_exception *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_exception_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_exception_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_exception_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_exception_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_exception_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_exception_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_exception_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_exception - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

// visit an interface
int
be_visitor_root::visit_interface (be_interface *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_interface_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_interface_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_interface_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_interface_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
      {
        be_visitor_interface_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_interface_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_interface_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_interface_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_interface_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_interface_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_interface_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_interface - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == 0)
    {
      return 0;
    }
  else if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface - "
                         "failed to accept visitor\n"),
                        -1);
    }

  // Change the state depending on the kind of node strategy
  ctx.state (node->next_state (ctx.state ()));

  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface - "
                         "NUL visitor\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface - "
                         "failed to accept visitor\n"),
                        -1);
    }

  delete visitor;
  visitor = 0;

  // Do additional code generation is necessary.
  // Note, this call is delegated to the strategy connected to
  // the node.
  if (node->has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy
      ctx.state (node->next_state (ctx.state (), 1));

      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_interface - "
                             "NUL visitor\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_interface - "
                             "failed to accept visitor\n"),
                            -1);
        }

      delete visitor;
      visitor = 0;
    }

  ctx.state (TAO_CodeGen::TAO_ROOT_CH);
  return 0;
}

int
be_visitor_root::visit_interface_fwd (be_interface_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_interface_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_interface_fwd_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_interface_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_valuetype (be_valuetype *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_valuetype_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_valuetype_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_valuetype_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_valuetype_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_valuetype_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_valuetype_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_valuetype_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_valuetype_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
      {
        be_visitor_valuetype_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_valuetype_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0;    // nothing to do.
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_valuetype_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_valuetype_fwd_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_valuetype_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_valuetype_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_root::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_root::visit_component (be_component *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_component_ch visitor (&ctx);
        // This is the only context state involved in strategies.
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CH);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_component_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_component_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_component_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_component_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_component_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_component_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_component_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
      {
        be_visitor_component_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_component_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0;    // nothing to do.
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_component - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_component_fwd (be_component_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_component_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_component_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_component_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_component_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_home (be_home *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_home_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_home - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_module (be_module *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_module_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_module_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_module_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_module_any_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_module_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_module - "
                           "Bad context state\n"),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_module - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_structure (be_structure *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_structure - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_structure_fwd (be_structure_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_structure_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_structure_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_union (be_union *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_union_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_union_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_union_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_union_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_union_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_union - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_union_fwd (be_union_fwd *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
  {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_union_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
  }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_typedef (be_typedef *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_typedef - "
                         "failed to accept visitor"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::gen_explicit_tmplinst (be_root *node,
                                        TAO_OutStream *os)
{
  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CS)
    {
      // Make two more passes over the AST to generate the explicit
      // template instantiations, one for 'template class ...' and
      // one for '#pragma instantiate ...' for the client side.

      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      os->gen_ifdef_AHETI ();

      if (be_global->gen_anyop_files ())
        {
          tao_cg->anyop_source ()->gen_ifdef_AHETI ();
        }

      be_visitor_tmplinst_cs visitor (this->ctx_);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "stub explicit template instantiation failed\n"),
                            -1);
        }

      os->gen_elif_AHETI ();

      if (be_global->gen_anyop_files ())
        {
          tao_cg->anyop_source ()->gen_elif_AHETI ();
        }

      visitor.switch_mode ();

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "stub explicit template instantiation failed\n"),
                            -1);
        }

      os->gen_endif_AHETI ();

      if (be_global->gen_anyop_files ())
        {
          tao_cg->anyop_source ()->gen_endif_AHETI ();

          *tao_cg->anyop_source () << "\n";
        }
    }
  else if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS)
    {
      // Make two more passes over the AST to generate the explicit
      // template instantiations, one for 'template class ...' and
      // one for '#pragma instantiate ...' for the client side.

      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      os->gen_ifdef_AHETI ();

      be_visitor_tmplinst_ss visitor (this->ctx_);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "skel explicit template instantiation failed\n"),
                            -1);
        }

      os->gen_elif_AHETI ();

      visitor.switch_mode ();

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "skel explicit template instantiation failed\n"),
                            -1);
        }

      os->gen_endif_AHETI ();
    }

  return 0;
}
