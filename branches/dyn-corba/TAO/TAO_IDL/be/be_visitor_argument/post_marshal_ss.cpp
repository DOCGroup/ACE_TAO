//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    post_marshal_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code that does post-processing of arguments following
//    any marshaling. This involves any cleanup.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_argument, 
           post_marshal_ss, 
           "$Id$")

// ************************************************************************
//  visitor for doing any post-processing after the marshaling is done
// ************************************************************************

be_visitor_args_post_marshal_ss::be_visitor_args_post_marshal_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_post_marshal_ss::~be_visitor_args_post_marshal_ss (void)
{
}

int be_visitor_args_post_marshal_ss::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_marshal_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_marshal_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_post_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_post_marshal_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}
