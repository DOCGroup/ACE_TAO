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

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::visit_root - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

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
              << i->full_base_proxy_broker_name () << " *" << be_nl
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
              << "TAO_NAMESPACE CORBA" << be_nl
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
              << "}" << be_nl
              << "TAO_NAMESPACE_CLOSE";
        }
    }

  be_visitor_context ctx (*this->ctx_);

  // Make one more pass over the entire tree and generate the OBV_ namespaces
  // and OBV_ classes.

  idl_bool obv = 1;
  int status = 0;

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

  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CH)
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
          }

        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      (void) tao_cg->end_implementation_header (
          be_global->be_get_implementation_hdr_fname (0)
        );
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      (void) tao_cg->end_server_header ();
      return 0;
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_IS:
      break;
    case TAO_CodeGen::TAO_ROOT_SI:
      if (be_global->gen_tie_classes ())
        {
          (void) tao_cg->end_server_template_inline ();
        }

      *os << "\n\n";

      return 0;
    case TAO_CodeGen::TAO_ROOT_SS:
      if (be_global->gen_tie_classes ())
        {
          (void) tao_cg->end_server_template_skeletons ();
        }

      (void) tao_cg->end_server_skeletons ();
      return 0;
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      if (be_global->gen_tie_classes ())
        {
          (void) tao_cg->end_server_template_header ();
        }

      return 0;
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
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ROOT_CDR_OP_CI);
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
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      return 0; // nothing to be done
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

  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CS)
    {
      // Make two more passes over the AST to generate the explicit
      // template instantiations, one for 'template class ...' and
      // one for '#pragma instantiate ...'.

      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      os->gen_ifdef_AHETI ();

      be_visitor_tmplinst visitor (this->ctx_);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "explicit template instantiation failed\n"),
                            -1);
        }

      os->gen_elif_AHETI ();

      visitor.switch_mode ();

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "explicit template instantiation failed\n"),
                            -1);
        }

      os->gen_endif_AHETI ();
    }

  // Generate any final code such as #endifs and/or EOF newlines.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      (void) tao_cg->end_client_header ();
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_CS:
      *os << "\n\n";
      break;
    default:
      break;
    }

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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_enum_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_exception_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_interface_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_interface_fwd_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_valuetype_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_valuetype_fwd_cdr_op_ci visitor (&ctx);
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
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        be_visitor_eventtype_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_eventtype_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_eventtype_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_eventtype_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_eventtype_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_eventtype_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_eventtype_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_eventtype_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_eventtype_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
      {
        be_visitor_eventtype_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_eventtype_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0;    // nothing to do.
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_eventtype - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_root::visit_eventtype_fwd (be_eventtype_fwd *node)
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
        be_visitor_eventtype_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_eventtype_fwd_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_eventtype_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_eventtype_fwd_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_eventtype_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_component_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_component_fwd_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_structure_cdr_op_ci visitor (&ctx);
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_union_cdr_op_ci visitor (&ctx);
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
      return 0; // nothing to be done
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
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
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
