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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be_visitor_obv_module, obv_module, "$Id$")


// ************************************************************
// Module visitor for server header
// ************************************************************

be_visitor_obv_module::be_visitor_obv_module (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_obv_module::~be_visitor_obv_module (void)
{
}



// This states are processed:
//    TAO_MODULE_OBV_CH
//    TAO_MODULE_OBV_CI
//    TAO_MODULE_OBV_CH


int
be_visitor_obv_module::visit_module (be_module *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  if (node->has_nested_valuetype ())
    {
      if (this->ctx_->state () == TAO_CodeGen::TAO_MODULE_OBV_CH)
        {
          os->indent ();

          *os << "TAO_NAMESPACE "; // << idl_global->skel_export_macro ()

          if (!node->is_nested ())
              // we are outermost module, so prepend
              *os << " OBV_" << node->local_name () << be_nl;
          else
              // we are inside another module
              *os << " " << node->local_name () << be_nl;

          *os << "{" << be_nl
              << be_idt;
        } // client header

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_module - "
                             "codegen for scope failed\n"), -1);
        }

      if (this->ctx_->state () == TAO_CodeGen::TAO_MODULE_OBV_CH)
        {
          os->decr_indent ();
          *os << "}\nTAO_NAMESPACE_CLOSE\n\n";
        }
    }
  return 0;
}


int
be_visitor_obv_module::visit_valuetype (be_valuetype *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_OBV_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_OBV_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_OBV_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_OBV_CS);
      break;
  default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_obv_module::"
                           "visit_valuetype - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  // Change the state depending on the kind of node strategy
  ctx.state (node->next_state (ctx.state ()));

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_obv_module::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  visitor = 0;

   // Do addtional "extra" code generation if necessary
  if (node->has_extra_code_generation (ctx.state ()))
    {
      // Change the state depending on the kind of node strategy
      ctx.state (node->next_state (ctx.state (), 1));

      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_valuetype - "
                             "NUL visitor\n"
                             ),  -1);
        }

      // let the node accept this visitor
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_obv_module::"
                             "visit_valuetype - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      delete visitor;
      visitor = 0;
    }

  return 0;
}
