//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module.cpp
//
// = DESCRIPTION
//    Generic visitor generating code for Module
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_module.h"

ACE_RCSID(be_visitor_module, module, "$Id$")


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

// visit the Module node and its scope
int be_visitor_module::visit_module (be_module *node)
{
  if (node->nmembers () == 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_EMPTY_MODULE,
                                  node);
    }

  // all we have to do is to visit the scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::visit_module - "
                         "codegen for scope failed\n"), -1);
    }

  return 0;
}

// =all common visit methods for module visitor

// visit a constant
int
be_visitor_module::visit_constant (be_constant *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same


  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_constant - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_constant - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

//visit an enum
int
be_visitor_module::visit_enum (be_enum *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_enum - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_enum - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_enum - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an exception
int
be_visitor_module::visit_exception (be_exception *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_exception - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_exception - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an interface
int
be_visitor_module::visit_interface (be_interface *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_SH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SH);
      break;
    case TAO_CodeGen::TAO_MODULE_IH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_IH);
      break;
    case TAO_CodeGen::TAO_MODULE_SI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SI);
      break;
    case TAO_CodeGen::TAO_MODULE_SS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SS);
      break;
    case TAO_CodeGen::TAO_MODULE_IS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_IS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_interface - "
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
                         "(%N:%l) be_visitor_module::"
                         "visit_interface - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_interface - "
                         "failed to accept visitor\n"
                         ),  -1);
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

      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module::"
                             "visit_interface - "
                             "NUL visitor\n"
                             ),  -1);
        }

      // let the node accept this visitor
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module::"
                             "visit_interface - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      delete visitor;
      visitor = 0;
    }

  return 0;
}

// visit an interface_fwd
int
be_visitor_module::visit_interface_fwd (be_interface_fwd *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_interface_fwd - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_interface_fwd - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_interface_fwd - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an valuetype
int
be_visitor_module::visit_valuetype (be_valuetype *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0;    // nothing to do, resp. not yet impl.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
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
                         "(%N:%l) be_visitor_module::"
                         "visit_valuetype - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
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
                             "(%N:%l) be_visitor_module::"
                             "visit_valuetype - "
                             "NUL visitor\n"
                             ),  -1);
        }

      // let the node accept this visitor
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module::"
                             "visit_valuetype - "
                             "failed to accept visitor\n"
                             ),  -1);
        }
      delete visitor;
      visitor = 0;
    }

  return 0;
}

// visit a valuetype_fwd
int
be_visitor_module::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_valuetype_fwd - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_valuetype_fwd - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_valuetype_fwd - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an structure
int
be_visitor_module::visit_structure (be_structure *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_structure - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_structure - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_structure - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an union
int
be_visitor_module::visit_union (be_union *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_union - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_union - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_union - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit a typedef
int
be_visitor_module::visit_typedef (be_typedef *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // this switch is acceptable rather than having derived visitors overriding
  // this method and differing only in what state they set

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_MODULE_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IH:
    case TAO_CodeGen::TAO_MODULE_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_typedef - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_typedef - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::"
                         "visit_typedef - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}
