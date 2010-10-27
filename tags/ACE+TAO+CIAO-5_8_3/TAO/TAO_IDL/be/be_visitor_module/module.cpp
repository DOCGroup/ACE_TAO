
//=============================================================================
/**
 *  @file    module.cpp
 *
 *  $Id$
 *
 *  Generic visitor generating code for Module
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ******************************************************
// Generic Module visitor
// ******************************************************

be_visitor_module::be_visitor_module (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_module::~be_visitor_module (void)
{
}

int
be_visitor_module::visit_module (be_module *node)
{
  if (node->nmembers () == 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_EMPTY_MODULE,
                                  node);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::visit_module - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_constant (be_constant *node)
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
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_constant - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_native (be_native *node)
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
        be_visitor_native_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::::")
                         ACE_TEXT ("visit_native - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_enum (be_enum *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
      {
        be_visitor_enum_serializer_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        be_visitor_enum_serializer_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_enum - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_exception (be_exception *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        return 0; // nothing to be done for DCPS
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_exception - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_interface (be_interface *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      {
        ctx.state (TAO_CodeGen::TAO_INTERFACE_CH);
        be_visitor_interface_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
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
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
      {
        be_visitor_interface_ex_idl visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_interface_svh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_interface_svs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
    case TAO_CodeGen::TAO_ROOT_EXH:
    case TAO_CodeGen::TAO_ROOT_EXS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
      {
        // Nothing to be done for these cases.
        return 0;
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("be_visitor_module::")
                           ACE_TEXT ("visit_interface - ")
                           ACE_TEXT ("Bad context state\n")),
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
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  ctx.state (TAO_CodeGen::TAO_ROOT_CH);
  return 0;
}

int
be_visitor_module::visit_interface_fwd (be_interface_fwd *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
      {
        return 0; // nothing to be done for DCPS
       break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_interface_fwd - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_valuebox (be_valuebox *node)
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
        be_visitor_valuebox_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CI:
      {
        be_visitor_valuebox_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        be_visitor_valuebox_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      {
        be_visitor_valuebox_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      {
        be_visitor_valuebox_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      {
        be_visitor_valuebox_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      {
        be_visitor_valuebox_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        break;
      }
    default:
      return 0;    // nothing to do.
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_valuebox - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_valuetype (be_valuetype *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        return 0; // nothing to be done for DCPS
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
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_valuetype - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_valuetype_fwd (be_valuetype_fwd *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
      {
        return 0; // nothing to be done for DCPS
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_valuetype_fwd - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_module::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_module::visit_component (be_component *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    // These cases all behave like interfaces.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
      return this->visit_interface (node);

    // Nothing for these with components.
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
      break;
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_component_svh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_component_svs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
      {
        be_visitor_component_exh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXS:
      {
        be_visitor_component_exs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
      {
        be_visitor_component_ex_idl visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("be_visitor_module::")
                           ACE_TEXT ("visit_component - ")
                           ACE_TEXT ("Bad context state\n")),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_component_fwd (be_component_fwd *node)
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
        be_visitor_component_fwd_any_op_ch visitor (&ctx);
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
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_component_fwd - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_home (be_home *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_home_svh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_home_svs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
      {
        be_visitor_home_ex_idl visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
      {
        be_visitor_home_exh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXS:
      {
        be_visitor_home_exs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_home - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_connector (be_connector *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
      case TAO_CodeGen::TAO_ROOT_CNH:
        {
          if (node->dds_connector ())
            {
              be_visitor_connector_dds_exh visitor (&ctx);
              status = node->accept (&visitor);
              break;
            }
          else if (node->ami_connector ())
            {
              be_visitor_connector_ami_exh visitor (&ctx);
              status = node->accept (&visitor);
              break;
            }
        }
      case TAO_CodeGen::TAO_ROOT_CNS:
        {
          if (node->dds_connector ())
            {
              be_visitor_connector_dds_exs visitor (&ctx);
              status = node->accept (&visitor);
              break;
            }
          else if (node->ami_connector ())
            {
              be_visitor_connector_ami_exs visitor (&ctx);
              status = node->accept (&visitor);
              break;
            }
        }
      // Skip these contexts, the connector impl is
      // generated in a separate pass, using the context states
      // above.
      case TAO_CodeGen::TAO_ROOT_EXH:
      case TAO_CodeGen::TAO_ROOT_EXS:
        break;
      default:
        // In all other cases, same as component.
        return this->visit_component (node);
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_structure (be_structure *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
      {
        be_visitor_structure_serializer_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        be_visitor_structure_serializer_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_structure_fwd (be_structure_fwd *node)
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
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_structure_fwd - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_union (be_union *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
      {
        be_visitor_union_serializer_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        be_visitor_union_serializer_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_union - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_union_fwd (be_union_fwd *node)
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
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_union_fwd - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_module::visit_typedef (be_typedef *node)
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
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CH:
      {
        be_visitor_typedef_serializer_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SERIALIZER_OP_CS:
      {
        be_visitor_typedef_serializer_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_module::")
                         ACE_TEXT ("visit_typedef - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}
