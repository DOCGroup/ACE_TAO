//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_upcall_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code to do post-processing of arguments following an
//    upcall.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID (be_visitor_argument, 
           post_upcall_ss, 
           "$Id$")

// ************************************************************************
//  visitor for doing any post-processing after the upcall is made
// ************************************************************************

be_visitor_args_post_upcall_ss::be_visitor_args_post_upcall_ss (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_args_post_upcall_ss::~be_visitor_args_post_upcall_ss (void)
{
}

int
be_visitor_args_post_upcall_ss::visit_operation (be_operation *node)
{
  return this->visit_scope (node);
}

int be_visitor_args_post_upcall_ss::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_upcall::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_upcall::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_post_upcall_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_argument *arg = this->ctx_->be_node_as_argument ();

  // if the current type is an alias, use that
  be_type *bt = node;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();

  switch (arg->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_forany _tao_forany_"
              << arg->local_name () << " (" << be_idt << be_idt_nl
              << arg->local_name () << ".inout ()" << be_uidt_nl
              << ");\n" << be_uidt;
        }
      else
        {
          *os << bt->name () << "_forany _tao_forany_"
              << arg->local_name () << " (" << be_idt << be_idt_nl
              << arg->local_name () << be_uidt_nl
              << ");\n" << be_uidt;
        }
      break;
    }
  return 0;
}

int be_visitor_args_post_upcall_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_upcall::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
