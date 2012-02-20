
//=============================================================================
/**
 *  @file    obv_module.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for the Module class
 *  This provides code generation for the module in the header
 *  for the OBV_ namespace (see C++ mapping OMG 20.17)
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================

#include "valuetype.h"

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
          *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

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
          *os << be_uidt_nl << "}";
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
      {
        ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CH);
        be_visitor_valuetype_obv_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_OBV_CI:
      {
        be_visitor_valuetype_obv_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_MODULE_OBV_CS:
      {
        be_visitor_valuetype_obv_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
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

  return 0;
}

int
be_visitor_obv_module::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}
