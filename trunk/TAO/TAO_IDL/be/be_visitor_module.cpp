//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_module.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Module
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_module.h"

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
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_enum - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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
    case TAO_CodeGen::TAO_MODULE_SI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SI);
      break;
    case TAO_CodeGen::TAO_MODULE_SS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SS);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_interface - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
    }

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
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_interface_fwd - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_structure - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_union - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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
    case TAO_CodeGen::TAO_MODULE_SH:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_module::"
                           "visit_typedef - "
                           "Bad context state\n"
                           ), -1);
      }
      break;
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

// ******************************************************
// Module visitor for client header
// ******************************************************

be_visitor_module_ch::be_visitor_module_ch (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_ch::~be_visitor_module_ch (void)
{
}

int
be_visitor_module_ch::visit_module (be_module *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();

      // XXXASG - Modules really map to namespace. We need to see if our target
      // compiler supports namespaces or not. This visitor generates a class for a
      // module. We can have the factory generate another module visitor that can
      // generate namespaces

      os->indent (); // start from whatever indentation level we were at
      // now generate the class definition
      *os << "TAO_NAMESPACE " // << idl_global->export_macro ()
	  << " " << node->local_name () << be_nl
          << "{\n";
      os->incr_indent (0);

      // generate code for the module definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module_ch::"
                             "visit_module - "
                             "codegen for scope failed\n"), -1);
        }

      *os << be_uidt_nl
          << "}; // module " << node->name () << "\n\n";

    }
  return 0;
}

// ************************************************************
// Module visitor for server header
// ************************************************************

be_visitor_module_sh::be_visitor_module_sh (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_sh::~be_visitor_module_sh (void)
{
}

int
be_visitor_module_sh::visit_module (be_module *node)
{
  TAO_OutStream *os; // output stream

  if (!node->srv_hdr_gen () && !node->imported ()) // not generated and not imported
    {
      os = this->ctx_->stream ();

      // generate the skeleton class name

      os->indent (); // start with whatever indentation level we are at

      // now generate the class definition. The prefix POA_ is prepended to our
      // name only if we are the outermost module
      if (!node->is_nested ())
        // we are outermost module
        *os << "class "  << idl_global->export_macro ()
	    << " POA_" << node->local_name () << be_nl;
      else
        // we are inside another module
        *os << "class "  << idl_global->export_macro ()
	    << " " << node->local_name () << be_nl;

      *os << "{" << be_nl
          << "public:"
          << be_idt;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_module_sh::"
                             "visit_module - "
                             "codegen for scope failed\n"), -1);
        }

      os->decr_indent ();
      *os << "};\n\n";
    }
  return 0;

}

// ***************************************************************************
// Module visitor for generating Any operator declarations in the client header
// and stub
// ***************************************************************************

be_visitor_module_any_op::be_visitor_module_any_op (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_any_op::~be_visitor_module_any_op (void)
{
}

int
be_visitor_module_any_op::visit_module (be_module *node)
{
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module::visit_module - "
                         "codegen for scope failed\n"), -1);
    }
  return 0;
}
