//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    obv_module.cpp
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This provides code generation for the module in the header
//    for the OBV_ namespace (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

ACE_RCSID (be_visitor_obv_module, 
           obv_module, 
           "$Id$")

// ************************************************************
// OBV module visitor for server header.
// ************************************************************

be_visitor_obv_module::be_visitor_obv_module (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_obv_module::~be_visitor_obv_module (void)
{
}

int
be_visitor_obv_module::visit_module (be_module *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (node->has_nested_valuetype ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      if (this->ctx_->state () == TAO_CodeGen::TAO_MODULE_OBV_CH)
        {
          *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

          *os << "namespace ";

          if (!node->is_nested ())
            {
              // We are outermost module, so prepend.
              *os << "OBV_" << node->local_name () << be_nl;
            }
          else
            {
              // We are inside another module.
              *os << node->local_name () << be_nl;
            }

          *os << "{" << be_idt;
        }

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_module - "
                             "codegen for scope failed\n"), 
                            -1);
        }

      if (this->ctx_->state () == TAO_CodeGen::TAO_MODULE_OBV_CH)
        {
          *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__;

          *os << be_uidt_nl << be_nl << "}";
        }
    }

  return 0;
}

int
be_visitor_obv_module::visit_valuetype (be_valuetype *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_OBV_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_OBV_CI:
      {
        // This context state is not involved in any strategies.
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CI);
        be_visitor_valuetype_obv_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_OBV_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CS);
      break;
    default:
      return 0;
    }

  if (status == 0)
    {
      return 0;
    }
  else if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  // Change the state depending on the kind of node strategy.
  ctx.state (node->next_state (ctx.state ()));

  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "NUL visitor\n"),  
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  delete visitor;
  visitor = 0;

   // Do addtional "extra" code generation if necessary.
  if (node->has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy.
      ctx.state (node->next_state (ctx.state (), 1));

      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_valuetype - "
                             "NUL visitor\n"),  
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_valuetype - "
                             "failed to accept visitor\n"),  
                            -1);
        }

      delete visitor;
      visitor = 0;
    }

  return 0;
}

int
be_visitor_obv_module::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 1;

  switch (this->ctx_->state ())
    {
      case TAO_CodeGen::TAO_MODULE_OBV_CH:
        ctx.state (TAO_CodeGen::TAO_EVENTTYPE_OBV_CH);
        break;
      case TAO_CodeGen::TAO_MODULE_OBV_CI:
        {
          // This context state is not involved in any strategies.
//          ctx.state (TAO_CodeGen::TAO_EVENTTYPE_OBV_CI);
          be_visitor_eventtype_obv_ci visitor (&ctx);
          status = node->accept (&visitor);
          break;
        }
      case TAO_CodeGen::TAO_MODULE_OBV_CS:
        ctx.state (TAO_CodeGen::TAO_EVENTTYPE_OBV_CS);
        break;
      default:
        return 0;
    }

  if (status == 0)
    {
      return 0;
    }
  else if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  // Change the state depending on the kind of node strategy.
  ctx.state (node->next_state (ctx.state ()));

  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "NUL visitor\n"),  
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  delete visitor;
  visitor = 0;

   // Do addtional "extra" code generation if necessary.
  if (node->has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy.
      ctx.state (node->next_state (ctx.state (), 1));

      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_valuetype - "
                             "NUL visitor\n"),  
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_valuetype - "
                             "failed to accept visitor\n"),  
                            -1);
        }

      delete visitor;
      visitor = 0;
    }

  return 0;
}
