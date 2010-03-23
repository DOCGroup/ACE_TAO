// $Id$

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

// Generic Root visitor
be_visitor_root::be_visitor_root (be_visitor_context *ctx)
  : be_visitor_module (ctx)
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

  // The SI and SS cases are caught in BE_produce()
  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SH
      && !be_global->gen_skel_files ())
    {
      return 0;
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
        }

        break;
      case TAO_CodeGen::TAO_ROOT_EX_IDL:
        {
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

          if (be_global->gen_direct_collocation() || be_global->gen_thru_poa_collocation ())
            {
              *os << be_nl << be_nl
                  << "extern " << be_global->stub_export_macro () << be_nl
                  << "TAO::Collocation_Proxy_Broker *" << be_nl
                  << "(*" << i->flat_client_enclosing_scope ()
                  << i->base_proxy_broker_name ()
                  << "_Factory_function_pointer) ("
                  << be_idt << be_idt_nl
                  << "::CORBA::Object_ptr obj);" << be_uidt << be_uidt;
            }
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

          *os << be_global->core_versioning_begin () << be_nl;

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
              << "TAO_NAMESPACE_STORAGE_CLASS ::CORBA::Boolean is_nil ("
              << i->name () << "_ptr);";
        }

      if (size > 0)
        {
          *os << be_uidt_nl
              << "}";

          *os << be_global->core_versioning_end () << be_nl;
        }
    }

  // Make one more pass over the entire tree and generate the OBV_ namespaces
  // and OBV_ classes.

  bool obv = true;
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
      obv = false;
      break;
    }

  if (obv == true)
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
          be_visitor_traits traits_visitor (&ctx);
          status = node->accept (&traits_visitor);

          if (status == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_root::"
                                 "visit_root - "
                                 "failed to generate traits\n"),
                                -1);
            }

          if (be_global->gen_template_export ())
            {
              be_visitor_template_export export_visitor (&ctx);
              status = node->accept (&export_visitor);

              if (status == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_visitor_root::"
                                     "visit_root - "
                                     "failed to export templates\n"),
                                    -1);
                }
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

          // Conditional switch to the *A.h stream is done
          // in the visitor constructor.
          if (be_global->gen_anyop_files ())
            {
              (void) tao_cg->end_anyop_header ();
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
            }

          // Conditional switch to the *A.cpp stream is done
          // in the visitor constructor.
          if (be_global->gen_anyop_files ())
            {
              (void) tao_cg->end_anyop_source ();
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
      case TAO_CodeGen::TAO_ROOT_SVH:
      case TAO_CodeGen::TAO_ROOT_SVS:
      case TAO_CodeGen::TAO_ROOT_EXH:
      case TAO_CodeGen::TAO_ROOT_EXS:
      case TAO_CodeGen::TAO_ROOT_CNH:
      case TAO_CodeGen::TAO_ROOT_CNS:
      case TAO_CodeGen::TAO_ROOT_EX_IDL:
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
    case TAO_CodeGen::TAO_ROOT_SVH:
    case TAO_CodeGen::TAO_ROOT_SVS:
    case TAO_CodeGen::TAO_ROOT_EXH:
    case TAO_CodeGen::TAO_ROOT_EXS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
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

  if (be_global->gen_dcps_type_support ())
    {
  // Make another pass over the entire tree and generate the
  // Serializer operators.
  ctx = *this->ctx_;
  status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH);
        be_visitor_root_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS);
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
    case TAO_CodeGen::TAO_ROOT_SVH:
    case TAO_CodeGen::TAO_ROOT_SVS:
    case TAO_CodeGen::TAO_ROOT_EXH:
    case TAO_CodeGen::TAO_ROOT_EXS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
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
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_root - "
                         "failed to generate CDR operators\n"),
                        -1);
    }

  // Generate any final code such as #endifs and/or EOF newlines.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      (void) tao_cg->end_client_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      (void) tao_cg->end_client_inline ();
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      (void) tao_cg->end_client_stubs ();
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
      (void) tao_cg->end_server_inline ();
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
    case TAO_CodeGen::TAO_ROOT_SVH:
      (void) tao_cg->end_ciao_svnt_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_SVS:
      (void) tao_cg->end_ciao_svnt_source ();
      break;
    case TAO_CodeGen::TAO_ROOT_EXH:
      (void) tao_cg->end_ciao_exec_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_EXS:
      (void) tao_cg->end_ciao_exec_source ();
      break;
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
      (void) tao_cg->end_ciao_exec_idl ();
      break;
    case TAO_CodeGen::TAO_ROOT_CNH:
      (void) tao_cg->end_ciao_conn_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_CNS:
      (void) tao_cg->end_ciao_conn_source ();
      break;
    default:
      break;
    }

  // Make sure each file ends with a newline.
  *os << "\n";

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
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SVH:
    case TAO_CodeGen::TAO_ROOT_SVS:
    case TAO_CodeGen::TAO_ROOT_EXH:
    case TAO_CodeGen::TAO_ROOT_EXS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        be_visitor_module_serializer_op visitor (&ctx);
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
