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

int be_visitor_args_post_marshal_ss::visit_array (be_array *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_enum (be_enum *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_interface (be_interface *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT: // inout
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_interface_fwd (be_interface_fwd *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT: // inout
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_predefined_type (be_predefined_type *node)
{
  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	  break;
	case AST_Argument::dir_INOUT:
	  break;
	case AST_Argument::dir_OUT:
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_post_marshal_ss::visit_sequence (be_sequence *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_string (be_string *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_structure (be_structure *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
    }
  return 0;
}

int be_visitor_args_post_marshal_ss::visit_union (be_union *)
{
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
      break;
    case AST_Argument::dir_OUT:
      break;
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
