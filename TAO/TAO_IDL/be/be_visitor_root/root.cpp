//
// $Id$
//

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

  // If we are generating the client header file, this is the place to
  // generate the proxy broker factory function pointer declarations
  // and the extern declarations for non-defined interfaces.
  if (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_CH)
    {
      TAO_OutStream *os = this->ctx_->stream ();

      size_t size = be_global->non_local_interfaces.size ();
      be_interface *i = 0;
      be_interface_fwd *ifwd = 0;
      size_t index = 0;

      if (size > 0)
        {
          *os << "// Proxy Broker Factory function pointer declarations."
              << be_nl << be_nl;
          *os << be_nl << "// TAO_IDL - Generated from "
              << __FILE__ << ":" << __LINE__ << be_nl << be_nl;
        }

      for (index = 0; index < size; ++index)
        {
          be_global->non_local_interfaces.dequeue_head (i);

          *os << "extern " << be_global->stub_export_macro () << be_nl
              << i->full_base_proxy_broker_name () << " *" << be_nl
              << "(*" << i->flat_client_enclosing_scope ()
              << i->base_proxy_broker_name ()
              << "_Factory_function_pointer) ("
              << be_idt << be_idt_nl
              << "CORBA::Object_ptr obj" << be_uidt_nl
              << ");" << be_uidt_nl << be_nl;
        }

      size = be_global->non_defined_interfaces.size ();

      if (size > 0)
        {
          *os << be_nl << "// TAO_IDL - Generated from "
              << __FILE__ << ":" << __LINE__ << be_nl << be_nl;
        }

      for (index = 0; index < size; ++index)
        {
          be_global->non_defined_interfaces.dequeue_head (ifwd);

          *os << "// External declarations for undefined interface" << be_nl
              << "// " << ifwd->full_name () << be_nl;

          *os << be_global->stub_export_macro () << be_nl
              << ifwd->full_name () << "_ptr" << be_nl
              << "tao_" << ifwd->flat_name ()
              << "_duplicate ("
              << be_idt << be_idt_nl
              << ifwd->full_name  () << "_ptr" << be_uidt_nl
              << ");" << be_uidt_nl
              << be_global->stub_export_macro () << be_nl
              << "void" << be_nl
              << "tao_" << ifwd->flat_name ()
              << "_release (" << be_idt << be_idt_nl
              << ifwd->full_name () << "_ptr" << be_uidt_nl
              << ");" << be_uidt_nl
              << be_global->stub_export_macro () << be_nl
              << ifwd->full_name () << "_ptr" << be_nl
              << "tao_" << ifwd->flat_name ()
              << "_nil (" << be_idt << be_idt_nl
              << "void" << be_uidt_nl
              << ");" << be_uidt_nl
              << be_global->stub_export_macro () << be_nl
              << ifwd->full_name () << "_ptr" << be_nl
              << "tao_" << ifwd->flat_name ()
              << "_narrow (" << be_idt << be_idt_nl
              << "CORBA::Object *" << be_nl
              << "ACE_ENV_ARG_DECL_NOT_USED" << be_uidt_nl
              << ");" << be_uidt_nl
              << be_global->stub_export_macro () << be_nl
              << "CORBA::Object *" << be_nl
              << "tao_" << ifwd->flat_name ()
              << "_upcast (" << be_idt << be_idt_nl
              << "void *" << be_uidt_nl
              << ");" << be_uidt_nl << be_nl;
        }
    }

  be_visitor_context ctx (*this->ctx_);

  // Make one more pass over the entire tree and generate the OBV_ namespaces
  // and OBV_ classes.

  idl_bool obv = 0;
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        obv = 1;
        ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CH);
        be_visitor_obv_module visitor (&ctx);
        status = visitor.visit_scope (node);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        obv = 1;
        ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CI);
        be_visitor_obv_module visitor (&ctx);
        status = visitor.visit_scope (node);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        obv = 1;
        ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CS);
        be_visitor_obv_module visitor (&ctx);
        status = visitor.visit_scope (node);
        break;
      }
    default:
      break;
    }

  if (obv == 1 && status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_root - "
                         "failed to generate OBV_ things\n"),  
                        -1);
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


  // Make one more pass over the entire tree and generate the CDR operators.
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
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      return 0; // nothing to be done
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
                         "failed to generate CDR operators\n"),  
                        -1);
    }

  // Generate any final code such as #endifs.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      (void) tao_cg->end_client_header ();
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
        ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
        be_visitor_constant_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
        be_visitor_constant_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
      return 0; // nothing to be done
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
        ctx.state (TAO_CodeGen::TAO_ENUM_CH);
        be_visitor_enum_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CS);
        be_visitor_enum_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
        be_visitor_enum_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
        be_visitor_enum_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
        be_visitor_enum_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
        be_visitor_enum_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_enum - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
        be_visitor_exception_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
        be_visitor_exception_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
        be_visitor_exception_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
        be_visitor_exception_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
        be_visitor_exception_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
        be_visitor_exception_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI);
        be_visitor_exception_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
        be_visitor_exception_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_exception - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CI);
        be_visitor_interface_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CS);
        be_visitor_interface_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_SH);
        be_visitor_interface_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_IH);
        be_visitor_interface_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_SI);
        be_visitor_interface_si visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_SS);
        be_visitor_interface_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_IS);
        be_visitor_interface_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH);
        be_visitor_interface_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS);
        be_visitor_interface_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH);
        be_visitor_interface_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI);
        be_visitor_interface_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS);
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
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CH);
        be_visitor_interface_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CI);
        be_visitor_interface_fwd_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CI);
        be_visitor_interface_fwd_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_ANY_OP_CH);
        be_visitor_interface_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CH);
        be_visitor_interface_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_interface_fwd - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CH);
        be_visitor_valuetype_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CI);
        be_visitor_valuetype_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CS);
        be_visitor_valuetype_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH);
        be_visitor_valuetype_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI);
        be_visitor_valuetype_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS);
        be_visitor_valuetype_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH);
        be_visitor_valuetype_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS);
        be_visitor_valuetype_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      return 0;    // nothing to do.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_valuetype - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CH);
        be_visitor_valuetype_fwd_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CI);
        be_visitor_valuetype_fwd_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CH);
        be_visitor_valuetype_fwd_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CI);
        be_visitor_valuetype_fwd_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_valuetype_fwd - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_MODULE_CH);
        be_visitor_module_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_CI);
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_CS);
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_SH);
        be_visitor_module_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SI:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_SI);
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_SS);
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_IS);
        be_visitor_module visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_IH);
        be_visitor_module_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_ANY_OP_CH);
        be_visitor_module_any_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_ANY_OP_CS);
        be_visitor_module_any_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_CDR_OP_CH);
        be_visitor_module_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_CDR_OP_CI);
        be_visitor_module_cdr_op visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_MODULE_CDR_OP_CS);
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
        ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_structure - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_UNION_CH);
        be_visitor_union_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CI);
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CS);
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
        be_visitor_union_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
        be_visitor_union_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
        be_visitor_union_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
        be_visitor_union_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_union - "
                           "Bad context state\n"), 
                          -1);
      }
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
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
        be_visitor_typedef_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
        be_visitor_typedef_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
        be_visitor_typedef_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
        be_visitor_typedef_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
        be_visitor_typedef_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
        be_visitor_typedef_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
        be_visitor_typedef_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
        be_visitor_typedef_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_typedef - "
                           "Bad context state\n"), 
                          -1);
      }
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
