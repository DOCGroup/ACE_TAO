//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_ss.cpp
//
// = DESCRIPTION
//    Visitor that generates the constructor memberlist in Request_Info class for interceptors
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_argument, 
           request_info_ss, 
           "$Id$")


// ************************************************************
// be_visitor_args_request_info_ss for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_request_info_ss::be_visitor_args_request_info_ss (
    be_visitor_context *ctx
  )
  : be_visitor_args (ctx)
{
}

be_visitor_args_request_info_ss::~be_visitor_args_request_info_ss (void)
{
}

int be_visitor_args_request_info_ss::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node
  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_request_info_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_SS:
      {
        // Generate the memberlist definition for the constructor.
        *os << node->local_name () << "_ " << "("<< node->local_name ()
            <<")";
        break;
      }
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_SS:
      {
        // Generate the arglist for object instantiation.
        //*os << node->local_name ();

        // retrieve the type of the argument
        /* be_type *bt = be_type::narrow_from_decl (node->field_type ());
        if (!bt)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "be_visitor_args_compiled_marshal_ss::"
                               "visit_argument - "
                               "Bad argument type\n"),
                              -1);
          }

        // Different types have different mappings when used as in/out or
        // inout parameters. Let this visitor deal with the type
        if (bt->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_compiled_marshal_ss::"
                               "visit_argument - "
                               "cannot accept visitor\n"),
                              -1);
          }
         */
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interceptors_request_info_ss::"
                           "post_process - "
                           "Bad context\n"),
                          -1);
      }
    }
  return 0;
}
