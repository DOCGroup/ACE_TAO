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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_attribute.h"

ACE_RCSID(be_visitor_attribute, attribute, "$Id$")


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
  this->ctx_->node (node); // save the node
  this->ctx_->attribute (node); // save this attribute node


  // first the "get" operation
  be_operation get_op (node->field_type (),
                       AST_Operation::OP_noflags,
                       node->name (),
                       node->is_local (),
                       node->is_abstract ());

  get_op.set_name ((UTL_IdList *) node->name ()->copy ());
  get_op.set_defined_in (node->defined_in ());

  // Get the strategy from the attribute and hand it over
  // to the operation
  delete get_op.set_strategy (node->get_get_strategy ());

  be_visitor_context ctx (*this->ctx_);

  // this switch statement eliminates the need for different classes that have
  // exactly the same code except different states.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
      break;

    case TAO_CodeGen::TAO_ATTRIBUTE_BASE_PROXY_IMPL_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_PROXY_IMPL_XH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_REMOTE_PROXY_IMPL_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS);
      break;

    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
      break;

    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }

  // Change the state depending on the kind of node strategy
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
      // Change the state depending on the kind of node strategy
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

  // Do nothing for readonly attributes.
  if (node->readonly ())
    {
      return 0;
    }

  // Create the set method.
  Identifier *id = 0;
  UTL_ScopedName *sn = 0;

  ACE_NEW_RETURN (id,
                  Identifier ("void"),
                  -1);

  ACE_NEW_RETURN (sn,
                  UTL_ScopedName (id,
                                  0),
                  -1);

  // the return type  is "void"
  be_predefined_type rt (AST_PredefinedType::PT_void,
                         sn);
  // argument type is the same as the attribute type
  be_argument arg (AST_Argument::dir_IN,
                   node->field_type (),
                   node->name ());
  arg.set_name ((UTL_IdList *) node->name ()->copy ());
  // create the operation
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
  // this switch statement eliminates the need for different classes that have
  // exactly the same code except different states.
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_IS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS);
      break;

    case TAO_CodeGen::TAO_ATTRIBUTE_BASE_PROXY_IMPL_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_PROXY_IMPL_XH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_REMOTE_PROXY_IMPL_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_PROXY_IMPL_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
      ctx.state (TAO_CodeGen::TAO_OPERATION_TIE_SI);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SH:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH);
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SS:
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS);
      break;

    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_attribute::"
                         "visit_attribute - "
                         "bad codegen state\n"),
                        -1);
    }


  // Change the state depending on the kind of node strategy
  ctx.state (set_op.next_state (ctx.state ()));
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
