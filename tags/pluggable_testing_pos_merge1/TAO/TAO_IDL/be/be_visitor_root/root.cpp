//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Root node. This is a generic visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_root.h"

ACE_RCSID(be_visitor_root, root, "$Id$")


// Generic Root visitor
be_visitor_root::be_visitor_root (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_root::~be_visitor_root (void)
{
}

// this method must be overridden by the derived root visitors
int
be_visitor_root::init (void)
{
  return -1;
}

// visit the Root node and its scope
int be_visitor_root::visit_root (be_root *node)
{
  // open the appropriate output file based on what state we are in. The
  // overridden "init" method of derived classes will do the job.
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::init - "
                         "failed to initialize context\n"), -1);
    }

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::visit_root - "
                         "codegen for scope failed\n"), -1);
    }


  be_visitor *visitor;
  be_visitor_context ctx (*this->ctx_);

#ifdef IDL_HAS_VALUETYPE
  // make one more pass over the entire tree and generate the OBV_ namespaces
  // and OBV_ classes

  idl_bool obv = 0;
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      obv = 1;
      ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      obv = 1;
      ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      obv = 1;
      ctx.state (TAO_CodeGen::TAO_MODULE_OBV_CS);
      break;
    }
  if (obv)
    {
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "NUL visitor\n"
                             ),  -1);
        }

      if (visitor->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "failed to generate OBV_ things\n"
                             ),  -1);
        }
      delete visitor;
    }
#endif /* IDL_HAS_VALUETYPE */


  // The next thing we need to do is make one more pass thru the entire tree
  // and generate code for all the <<= and >>= operators for all the
  // user-defined types.
  //
  // XXXASG - this part of the code may be conditionally generated because at
  // times it is not necessary to have these operators at all. TO-DO.

  ctx = *this->ctx_;
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_ROOT_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_ROOT_ANY_OP_CS);
      break;

    case TAO_CodeGen::TAO_ROOT_IH:
      (void) tao_cg->end_implementation_header (idl_global->be_get_implementation_hdr_fname (0));
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      (void) tao_cg->end_server_header ();
      (void) tao_cg->end_server_template_header ();
      return 0;

    case TAO_CodeGen::TAO_ROOT_CI:
      break;
    case TAO_CodeGen::TAO_ROOT_IS:
      break;
    case TAO_CodeGen::TAO_ROOT_SI:
      return 0; // nothing to be done
    case TAO_CodeGen::TAO_ROOT_SS:
      (void) tao_cg->end_server_template_skeletons ();
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  // *ASG* - this is a tempoaray hack soln so that our CDR operators get
  // generated in the *.i file rather than the *.cpp file
  if (this->ctx_->state () != TAO_CodeGen::TAO_ROOT_CI && this->ctx_->state () != TAO_CodeGen::TAO_ROOT_IH && this->ctx_->state () != TAO_CodeGen::TAO_ROOT_IS)
    {
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "NUL visitor\n"
                             ),  -1);
        }

      // generate the << and >> operators for all the user-defined
      // data types in the outermost scope
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                             "visit_root - "
                             "failed to generate Any operators\n"
                             ),  -1);
        }
      delete visitor;
    }

  // make one more pass over the entire tree and generate the CDR << and >>
  // operators for compiled marshaling. Again, this code can be conditionally
  // generated if compiled marshaling is desired.
  ctx = *this->ctx_;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_ROOT_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_ROOT_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
    }  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_root - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // generate the << and >> operators for all the user-defined data types in
  // the outermost scope
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_root - "
                         "failed to generate CDR operators\n"
                         ),  -1);
    }
  delete visitor;

  // generate any final code such as #endifs
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      (void) tao_cg->end_client_header ();
      break;
    default:
      break;
    }
  return 0;
}

// =all common visit methods for root visitor

// visit a constant
int
be_visitor_root::visit_constant (be_constant *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_CONSTANT_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_constant - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_constant - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_constant - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

//visit an enum
int
be_visitor_root::visit_enum (be_enum *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_enum - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_enum - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_enum - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an exception
int
be_visitor_root::visit_exception (be_exception *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_exception - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_exception - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_exception - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an interface
int
be_visitor_root::visit_interface (be_interface *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SH);
      break;
    case TAO_CodeGen::TAO_ROOT_IH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_IH);
      break;
    case TAO_CodeGen::TAO_ROOT_SI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SI);
      break;
    case TAO_CodeGen::TAO_ROOT_SS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_SS);
      break;
    case TAO_CodeGen::TAO_ROOT_IS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_IS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_interface - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an interface_fwd
int
be_visitor_root::visit_interface_fwd (be_interface_fwd *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
                return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_interface_fwd - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface_fwd - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_interface_fwd - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

#ifdef IDL_HAS_VALUETYPE

// visit an valuetype
int
be_visitor_root::visit_valuetype (be_valuetype *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_IH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
    case TAO_CodeGen::TAO_ROOT_IS:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      return 0;    // nothing to do, resp. not yet impl.
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_valuetype - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_valuetype - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_valuetype - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an valuetype_fwd
int
be_visitor_root::visit_valuetype_fwd (be_valuetype_fwd *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
    case TAO_CodeGen::TAO_ROOT_CS:
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
                return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_valuetype_fwd - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_valuetype_fwd - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_valuetype_fwd - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */

// visit an module
int
be_visitor_root::visit_module (be_module *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_MODULE_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_MODULE_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_MODULE_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
      ctx.state (TAO_CodeGen::TAO_MODULE_SH);
      break;
    case TAO_CodeGen::TAO_ROOT_SI:
      ctx.state (TAO_CodeGen::TAO_MODULE_SI);
      break;
    case TAO_CodeGen::TAO_ROOT_SS:
      ctx.state (TAO_CodeGen::TAO_MODULE_SS);
      break;
    case TAO_CodeGen::TAO_ROOT_IS:
      ctx.state (TAO_CodeGen::TAO_MODULE_IS);
      break;
    case TAO_CodeGen::TAO_ROOT_IH:
      ctx.state (TAO_CodeGen::TAO_MODULE_IH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_MODULE_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_MODULE_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_MODULE_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_MODULE_CDR_OP_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_module - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_module - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_module - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an structure
int
be_visitor_root::visit_structure (be_structure *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_structure - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_structure - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_structure - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an union
int
be_visitor_root::visit_union (be_union *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_union - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_union - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_union - "
                         "failed to accept visitor\n"
                         ),  -1);
    }
  delete visitor;
  return 0;
}

// visit an typedef
int
be_visitor_root::visit_typedef (be_typedef *node)
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
    case TAO_CodeGen::TAO_ROOT_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CI:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CI);
      break;
    case TAO_CodeGen::TAO_ROOT_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH);
      break;
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      ctx.state (TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS);
      break;
    case TAO_CodeGen::TAO_ROOT_SH:
    case TAO_CodeGen::TAO_ROOT_SI:
    case TAO_CodeGen::TAO_ROOT_SS:
        case TAO_CodeGen::TAO_ROOT_IS:
        case TAO_CodeGen::TAO_ROOT_IH:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_root::"
                           "visit_typedef - "
                           "Bad context state\n"
                           ), -1);
      }
    }

  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_typedef - "
                         "NUL visitor\n"
                         ),  -1);
    }

  // let the node accept this visitor
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root::"
                         "visit_typedef - "
                         "failed to accept visitor"
                         ),  -1);
    }
  delete visitor;
  return 0;
}
