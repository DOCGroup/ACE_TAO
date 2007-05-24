//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    attribute.cpp
//
// = DESCRIPTION
//    Visitor for generation of code for Attribute
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_attribute,
           attribute,
           "$Id$")

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
  : be_visitor_decl (ctx)
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


  // first the "get" operation
  be_operation get_op (node->field_type (),
                       AST_Operation::OP_noflags,
                       node->name (),
                       node->is_local (),
                       node->is_abstract ());

  get_op.set_name ((UTL_IdList *) node->name ()->copy ());
  get_op.set_defined_in (node->defined_in ());

  UTL_ExceptList *get_exceptions = node->get_get_exceptions ();

  if (0 != get_exceptions)
    {
      get_op.be_add_exceptions (get_exceptions->copy ());
    }

  // Get the strategy from the attribute and hand it over
  // to the operation.
  be_operation_strategy *old_strategy =
    get_op.set_strategy (node->get_get_strategy ()->copy ());

  if (0 != old_strategy)
    {
      old_strategy->destroy ();
      delete old_strategy;
      old_strategy = 0;
    }

  be_visitor_context ctx (*this->ctx_);
  int status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involved a strategy.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
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
  else if (status == 1)
    {
      // Change the state depending on the kind of node strategy.
      ctx.state (get_op.next_state (ctx.state ()));

      be_visitor *visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (get_op.accept (visitor) == -1))
        {
          delete visitor;
          visitor = 0;
          get_op.destroy ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_attribute::"
                             "visit_attribute - "
                             "codegen for get_attribute failed\n"),
                            -1);
        }

      delete visitor;
      visitor = 0;

      if (get_op.has_extra_code_generation (ctx.state ()))
        {
          // Change the state depending on the kind of node strategy.
          ctx.state (get_op.next_state (ctx.state (), 1));
          be_visitor *visitor = tao_cg->make_visitor (&ctx);

          if (!visitor || (get_op.accept (visitor) == -1))
            {
              delete visitor;
              visitor = 0;
              get_op.destroy ();
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_attribute::"
                                 "visit_attribute - "
                                 "codegen for get_attribute failed\n"),
                                -1);
            }

          delete visitor;
          visitor = 0;
        }
    }

  get_op.destroy ();

  // Do nothing for readonly attributes.
  if (node->readonly ())
    {
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
                       node->name (),
                       node->is_local (),
                       node->is_abstract ());
  set_op.set_name ((UTL_IdList *) node->name ()->copy ());
  set_op.set_defined_in (node->defined_in ());
  set_op.be_add_argument (arg);

  UTL_ExceptList *set_exceptions = node->get_set_exceptions ();

  if (0 != set_exceptions)
    {
      set_op.be_add_exceptions (set_exceptions->copy ());
    }

  // Get the strategy from the attribute and hand it over
  // to the operation, thereby deleting the old one.
  old_strategy =
    set_op.set_strategy (node->get_set_strategy ()->copy ());

  if (0 != old_strategy)
    {
      old_strategy->destroy ();
      delete old_strategy;
      old_strategy = 0;
    }

  ctx = *this->ctx_;
  status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involved a strategy.
    case TAO_CodeGen::TAO_ROOT_CH:
    case TAO_CodeGen::TAO_INTERFACE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
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
      set_op.destroy ();
      rt.destroy ();
      return 0;
    }
  else if (status == -1)
    {
      set_op.destroy ();
      rt.destroy ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for get_attribute failed\n"),
                        -1);
    }

  // Change the state depending on the kind of node strategy
  ctx.state (set_op.next_state (ctx.state ()));
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor || (set_op.accept (visitor) == -1))
    {
      delete visitor;
      visitor = 0;
      set_op.destroy ();
      rt.destroy ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "codegen for set_attribute failed\n"),
                        -1);
    }

  delete visitor;
  visitor = 0;

  if (set_op.has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy
      ctx.state (set_op.next_state (ctx.state (), 1));

      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (set_op.accept (visitor) == -1))
        {
          delete visitor;
          visitor = 0;
          set_op.destroy ();
          rt.destroy ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_attribute::"
                             "visit_attribute - "
                             "codegen for set_attribute failed\n"),
                            -1);
        }

      delete visitor;
      visitor = 0;
    }

  set_op.destroy ();
  rt.destroy ();
  return 0;
}
