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

  // Get the strategy from the attribute and hand it over
  // to the operation.
  delete get_op.set_strategy (node->get_get_strategy ());

  be_visitor_context ctx (*this->ctx_);
  int status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involved a strategy.
    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
        be_visitor_operation_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
        be_visitor_operation_ih visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
        be_visitor_operation_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
        be_visitor_operation_is visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
        be_visitor_operation_thru_poa_collocated_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
        be_visitor_operation_thru_poa_collocated_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
        be_visitor_operation_direct_collocated_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
        be_visitor_operation_direct_collocated_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_BASE_PROXY_IMPL_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
        be_visitor_operation_base_proxy_impl_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_PROXY_IMPL_XH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH);
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_REMOTE_PROXY_IMPL_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS);
        be_visitor_operation_remote_proxy_impl_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_PROXY_IMPL_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS);
        be_visitor_operation_thru_poa_proxy_impl_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_PROXY_IMPL_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS);
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH);
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS);
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH);
        be_visitor_operation_interceptors_ch visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
        be_visitor_operation_interceptors_cs visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH);
        be_visitor_operation_interceptors_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS);
        be_visitor_operation_interceptors_ss visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
        be_visitor_operation_tie_sh visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
        be_visitor_operation_tie_si visitor (&ctx);
        status = get_op.accept (&visitor);
        break;
      }
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  if (status == -1)
    {
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
  be_argument arg (AST_Argument::dir_IN,
                   node->field_type (),
                   node->name ());
  arg.set_name ((UTL_IdList *) node->name ()->copy ());
  // Create the operation.
  be_operation set_op (&rt,
                       AST_Operation::OP_noflags,
                       node->name (),
                       node->is_local (),
                       node->is_abstract ());
  set_op.set_name ((UTL_IdList *) node->name ()->copy ());
  set_op.set_defined_in (node->defined_in ());
  set_op.add_argument_to_scope (&arg);

  // Get the strategy from the attribute and hand it over
  // to the operation, thereby deleting the old one.
  delete set_op.set_strategy (node->get_set_strategy ());

  ctx = *this->ctx_;
  status = 1;

  switch (this->ctx_->state ())
    {
    // These two cases are the only ones that could involved a strategy.
    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
        be_visitor_operation_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
        be_visitor_operation_ih visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
        be_visitor_operation_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
        be_visitor_operation_is visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
        be_visitor_operation_thru_poa_collocated_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
        be_visitor_operation_thru_poa_collocated_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
        be_visitor_operation_direct_collocated_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
        be_visitor_operation_direct_collocated_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_BASE_PROXY_IMPL_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
        be_visitor_operation_base_proxy_impl_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_PROXY_IMPL_XH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH);
        be_visitor_operation_proxy_impl_xh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_REMOTE_PROXY_IMPL_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS);
        be_visitor_operation_remote_proxy_impl_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_PROXY_IMPL_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS);
        be_visitor_operation_thru_poa_proxy_impl_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_PROXY_IMPL_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS);
        be_visitor_operation_direct_proxy_impl_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH);
        be_visitor_operation_smart_proxy_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS);
        be_visitor_operation_smart_proxy_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH);
        be_visitor_operation_interceptors_ch visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
        be_visitor_operation_interceptors_cs visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH);
        be_visitor_operation_interceptors_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SS:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS);
        be_visitor_operation_interceptors_ss visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
        be_visitor_operation_tie_sh visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
      {
        ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
        be_visitor_operation_tie_si visitor (&ctx);
        status = set_op.accept (&visitor);
        break;
      }
    default:
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  if (status == 0)
    {
      return 0;
    }
  else if (status == -1)
    {
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
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_attribute::"
                             "visit_attribute - "
                             "codegen for set_attribute failed\n"),
                            -1);
        }

      delete visitor;
      visitor = 0;
    }


  return 0;
}
