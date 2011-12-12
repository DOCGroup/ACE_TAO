
//=============================================================================
/**
 *  @file    component.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Components. This is a generic visitor.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ******************************************************
// Generic Component visitor
// ******************************************************

be_visitor_component::be_visitor_component (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_component::~be_visitor_component (void)
{
}

int
be_visitor_component::visit_attribute (be_attribute *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
      break;
    default:
      return 0; // nothing to be done
    }

  // Same visitor for all the above cases where an action is taken.
  be_visitor_attribute visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_component::visit_operation (be_operation *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified ased on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
    // These first two cases may have the context state changed
    // by a strategy, so we use the visitor factory below.
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
        be_visitor_operation_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
        be_visitor_operation_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_operation_ih visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_operation_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_operation_is visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SVH:
    case TAO_CodeGen::TAO_ROOT_SVS:
    case TAO_CodeGen::TAO_ROOT_EXH:
    case TAO_CodeGen::TAO_ROOT_EXS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("be_visitor_component::")
                           ACE_TEXT ("visit_operation - ")
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
                         ACE_TEXT ("be_visitor_component::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_component::visit_structure (be_structure *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
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
                         ACE_TEXT ("be_visitor_component::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_component::visit_typedef (be_typedef *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
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
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_SVH:
    case TAO_CodeGen::TAO_ROOT_SVS:
    case TAO_CodeGen::TAO_ROOT_SVTH:
    case TAO_CodeGen::TAO_ROOT_SVTS:
    case TAO_CodeGen::TAO_ROOT_EXH:
    case TAO_CodeGen::TAO_ROOT_EXS:
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
    case TAO_CodeGen::TAO_ROOT_EX_IDL:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("be_visitor_component::")
                           ACE_TEXT ("visit_typedef - ")
                           ACE_TEXT ("Bad context state\n")),
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component::")
                         ACE_TEXT ("visit_typedef - ")
                         ACE_TEXT ("failed to accept visitor\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_component::visit_extended_port (
  be_extended_port *node)
{
  this->ctx_->port_prefix () =
    node->local_name ()->get_string ();
  this->ctx_->port_prefix () += '_';

  /// If the port visit traverses any attributes defined in the
  /// original porttype, this is a way for visitors down the
  /// line to tell what scope we are actually in.
  this->ctx_->interface (
    be_interface::narrow_from_scope (node->defined_in ()));

  be_porttype *pt = node->port_type ();

  if (this->visit_scope (pt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component::")
                         ACE_TEXT ("visit_extended_port - ")
                         ACE_TEXT ("visit_scope () failed\n")),
                        -1);
    }

  this->ctx_->port_prefix () = "";

  return 0;
}

int
be_visitor_component::visit_mirror_port (be_mirror_port *node)
{
  this->ctx_->port_prefix () =
    node->local_name ()->get_string ();
  this->ctx_->port_prefix () += '_';

  /// If the port visit traverses any attributes defined in the
  /// original porttype, this is a way for visitors down the
  /// line to tell what scope we are actually in.
  this->ctx_->interface (
    be_interface::narrow_from_scope (node->defined_in ()));

  be_porttype *pt = node->port_type ();

  if (this->visit_scope (pt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component::")
                         ACE_TEXT ("visit_extended_port - ")
                         ACE_TEXT ("visit_scope () failed\n")),
                        -1);
    }

  this->ctx_->port_prefix () = "";

  return 0;
}


