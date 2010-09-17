
//=============================================================================
/**
 *  @file    attribute.cpp
 *
 *  $Id$
 *
 *  Visitor for generation of code for Attribute
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// Attribute gets mapped to one or possibly two operations based on whether
// it is readonly or not. The two operations "get" and "set" the value of the
// attribute.
//
// code generation for attributes is achieved by mapping an attribute node to
// a corresponding operation node. The reason is that we do not want to
// duplicate the code. However, an attribute node and an operation node have
// different interfaces. Hence we need some form of an adapter that can take us
// from an attribute node to an operation node. The following paragraph
// describes how this is accomplished.
//
// For a "get" operation on the attribute, we create an operation node with the
// same name as the attribute and a return type which is the same as the type
// of the attribute. This operation has no arguments.
//
// For a "set" operation on the attribute (if it is not readonly), we create an
// operation node with a "void" return type and an "in" parameter of the same
// type as the attribute.

// In both cases the context will hold sufficient information to indicate that
// the operation node is really an attribute node.

// *************************************************************************
// primary visitor for "attribute". The context state decides the next state
// *************************************************************************

be_visitor_attribute::be_visitor_attribute (be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    for_facets_ (false),
    op_scope_ (0),
    exec_class_extension_ ("_exec_i")
{
}

be_visitor_attribute::~be_visitor_attribute (void)
{
}

int
be_visitor_attribute::visit_attribute (be_attribute *node)
{
  this->ctx_->node (node);
  this->ctx_->attribute (node);
  be_interface *intf = this->ctx_->interface ();

  if (intf != 0)
    {
      AST_Decl::NodeType snt = intf->node_type ();
      AST_Decl::NodeType ant =
        ScopeAsDecl (node->defined_in ())->node_type ();

      if (snt == AST_Decl::NT_component
          && ant == AST_Decl::NT_porttype)
        {
          return 0;
        }
    }

  UTL_Scope *s = node->defined_in ();
  AST_Decl *d = ScopeAsDecl (s);
  ACE_CString op_name (this->ctx_->port_prefix ());
  op_name += node->local_name ()->get_string ();
  Identifier *op_id = 0;
  ACE_NEW_RETURN (op_id,
                  Identifier (op_name.c_str ()),
                  -1);

  UTL_ScopedName *op_ln = 0;
  ACE_NEW_RETURN (op_ln,
                  UTL_ScopedName (op_id, 0),
                  -1);

  UTL_ScopedName *op_sn =
    static_cast<UTL_ScopedName *> (d->name ()->copy ());
  op_sn->nconc (op_ln);

  // first the "get" operation
  be_operation get_op (node->field_type (),
                       AST_Operation::OP_noflags,
                       0,
                       node->is_local (),
                       node->is_abstract ());

  get_op.set_defined_in (s);
  get_op.set_name (op_sn);
  UTL_ExceptList *get_exceptions = node->get_get_exceptions ();

  if (0 != get_exceptions)
    {
      get_op.be_add_exceptions (get_exceptions->copy ());
    }

  be_visitor_context ctx (*this->ctx_);
  int status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involve a strategy.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
        be_visitor_operation_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
        be_visitor_operation_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_operation_ih visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_operation_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_operation_is visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      {
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      {
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      {
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      {
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      {
        be_visitor_operation_tie_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SS:
      {
        be_visitor_operation_tie_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_operation_svs visitor (&ctx);
        visitor.scope (this->op_scope_);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXS:
      {
        be_visitor_operation_exs visitor (&ctx);
        visitor.scope (this->op_scope_);
        visitor.class_extension (this->exec_class_extension_.c_str ());
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
      break;
    default:
      get_op.destroy ();
      return 0;
    }

  if (status == -1)
    {
      get_op.destroy ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  // Do nothing for readonly attributes.
  if (node->readonly ())
    {
      get_op.destroy ();
      return 0;
    }

  status = 1;

  // Create the set method.
  Identifier id ("void");
  UTL_ScopedName sn (&id,
                     0);

  // The return type  is "void".
  be_predefined_type rt (AST_PredefinedType::PT_void,
                         &sn);

  // Argument type is the same as the attribute type.
  AST_Argument *arg =
    idl_global->gen ()->create_argument (AST_Argument::dir_IN,
                                         node->field_type (),
                                         node->name ());

  arg->set_name ((UTL_IdList *) node->name ()->copy ());

  // Create the operation.
  be_operation set_op (&rt,
                       AST_Operation::OP_noflags,
                       0,
                       node->is_local (),
                       node->is_abstract ());
  set_op.set_defined_in (node->defined_in ());
  set_op.set_name (static_cast<UTL_ScopedName *> (op_sn->copy ()));
  set_op.be_add_argument (arg);

  UTL_ExceptList *set_exceptions = node->get_set_exceptions ();

  if (0 != set_exceptions)
    {
      set_op.be_add_exceptions (set_exceptions->copy ());
    }

  ctx = *this->ctx_;
  status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involved a strategy.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
        be_visitor_operation_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
        be_visitor_operation_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SH:
      {
        be_visitor_operation_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IH:
      {
        be_visitor_operation_ih visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SS:
      {
        be_visitor_operation_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_IS:
      {
        be_visitor_operation_is visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      {
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      {
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      {
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      {
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SH:
      {
        be_visitor_operation_tie_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_TIE_SS:
      {
        be_visitor_operation_tie_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_SVS:
      {
        be_visitor_operation_svs visitor (&ctx);
        visitor.scope (this->op_scope_);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXH:
      {
        be_visitor_operation_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_EXS:
      {
        be_visitor_operation_exs visitor (&ctx);
        visitor.scope (this->op_scope_);
        visitor.class_extension (this->exec_class_extension_.c_str ());
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ROOT_CNH:
    case TAO_CodeGen::TAO_ROOT_CNS:
      break;
    default:
      // Error.
      set_op.destroy ();
      rt.destroy ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  if (status == 0)
    {
      get_op.destroy ();
      set_op.destroy ();
      rt.destroy ();
      return 0;
    }
  else if (status == -1)
    {
      get_op.destroy ();
      set_op.destroy ();
      rt.destroy ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  get_op.destroy ();
  set_op.destroy ();
  rt.destroy ();
  return 0;
}

void
be_visitor_attribute::for_facets (bool val)
{
  this->for_facets_ = val;
}

void
be_visitor_attribute::op_scope (be_decl *node)
{
  this->op_scope_ = node;
}

void
be_visitor_attribute::exec_class_extension (const char *extension)
{
  this->exec_class_extension_ = extension;
}
