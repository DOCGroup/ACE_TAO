//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_cs.cpp
//
// = DESCRIPTION
//    Visitor that generates the constructor memberlist in Request_Info class for interceptors
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, request_info_cs, "$Id$")


// ************************************************************
// be_visitor_args_request_info_cs for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_request_info_cs::be_visitor_args_request_info_cs (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_request_info_cs::~be_visitor_args_request_info_cs (void)
{
}

int be_visitor_args_request_info_cs::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Retrieve the type.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_request_info_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_CS:
      {
        // Generate the memberlist definition for the constructor.
        *os << "    " << node->local_name ()
            << "_ " << "(" << node->local_name () << ")";
        break;
      }
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CS:
      {
        // Generate the arglist for object instantiation.
        *os << node->local_name ();
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_interceptors_request_info_cs::"
                           "post_process - "
                           "Bad context\n"),
                          -1);
      }
    }

  return 0;
}
