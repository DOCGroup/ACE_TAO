
//=============================================================================
/**
 *  @file    interface.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces. This is a generic visitor.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ******************************************************
// Generic Interface visitor
// ******************************************************

be_visitor_interface::be_visitor_interface (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_interface::~be_visitor_interface (void)
{
}

// This method must be overridden by the derived interface visitors.
int
be_visitor_interface::visit_interface (be_interface *)
{
  return -1;
}

int
be_visitor_interface::visit_scope (be_scope *node)
{
  if (this->be_visitor_scope::visit_scope (node) == -1)
    {
      return -1;
    }

  be_interface *intf = be_interface::narrow_from_scope (node);

  if (intf == 0)
    {
      return 0;
    }

  if (intf->is_abstract ())
    {
      return 0;
    }

  if (! intf->has_mixed_parentage ())
    {
      return 0;
    }

  be_interface::tao_code_emitter helper = 0;

  switch (this->ctx_->state ())
    {
      case TAO_CodeGen::TAO_INTERFACE_CH:
        helper =
          be_visitor_interface_ch::gen_abstract_ops_helper;
        break;
      case TAO_CodeGen::TAO_ROOT_CS:
        helper =
          be_visitor_interface_cs::gen_abstract_ops_helper;
        break;
      case TAO_CodeGen::TAO_ROOT_SH:
        helper =
          be_visitor_interface_sh::gen_abstract_ops_helper;
        break;
      case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
        helper =
          be_visitor_interface_direct_proxy_impl_sh::gen_abstract_ops_helper;
        break;
      case TAO_CodeGen::TAO_ROOT_SS:
        helper =
          be_visitor_interface_ss::gen_abstract_ops_helper;
        break;
      case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
        helper =
          be_visitor_interface_direct_proxy_impl_ss::gen_abstract_ops_helper;
        break;
      default:
        break;
    }

  if (helper == 0)
    {
      return 0;
    }

  // 'abstract_paths_only' arg is FALSE here because we want to traverse
  // through concrete parents to get to all the abstract ones. The helper
  // will do nothing unless the base interface passed to it is abstract.
  int status =
    intf->traverse_inheritance_graph (helper,
                                      this->ctx_->stream (),
                                      false);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_*::"
                         "visit_scope - "
                         "traversal of inheritance graph failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::is_amh_rh_node (be_interface *node)
{
   //If, is implied-IDL
  if (node->original_interface () != 0)
    {
      // and the name starts with AMH
      if (ACE_OS::strncmp (node->local_name (), "AMH", 3) == 0)
        {
          // then it is an AMH node.
          return 1;
        }
    }

  return 0;
}

void
be_visitor_interface::add_abstract_op_args (AST_Operation *old_op,
                                            be_operation &new_op)
{
  AST_Decl *d = 0;

  for (UTL_ScopeActiveIterator si (old_op, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      d = si.item ();
      new_op.add_to_scope (d);
    }

  UTL_ExceptList *excep_list = old_op->exceptions ();

  if (0 != excep_list)
    {
      new_op.be_add_exceptions (excep_list->copy ());
    }
}

// All common visit methods for interface visitor.

int
be_visitor_interface::visit_attribute (be_attribute *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);

  // Same visitor for all the above cases where an action is taken.
  be_visitor_attribute visitor (&ctx);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_attribute - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_constant (be_constant *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_constant - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_native (be_native *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_native - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_enum (be_enum *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_INTERFACE_CH:
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_enum - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

// visit an exception
int
be_visitor_interface::visit_exception (be_exception *node)
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_exception - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

// visit an operation
int
be_visitor_interface::visit_operation (be_operation *node)
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
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);

      if (node->is_sendc_ami ())
        {
          be_visitor_operation_ami_cs visitor (&ctx);
          status = node->accept (&visitor);
        }
      else
        {
          be_visitor_operation_cs visitor (&ctx);
          status = node->accept (&visitor);
        }

      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
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
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      {
        be_visitor_operation_tie_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SS:
      {
        be_visitor_operation_tie_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      {
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      {
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      {
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
      {
        be_visitor_amh_rh_operation_sh visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
      {
        be_visitor_amh_rh_operation_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      {
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interface::"
                           "visit_operation - "
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_operation - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_structure_fwd (be_structure_fwd *node)
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_structure_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_union (be_union *node)
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_union - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_union_fwd (be_union_fwd *node)
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
                         "(%N:%l) be_visitor_interface::"
                         "visit_union_fwd - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface::visit_typedef (be_typedef *node)
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
    default:
                  return 0; // nothing to be done
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::"
                         "visit_typedef - "
                         "failed to accept visitor\n"),
                        -1);
    }

  return 0;
}
