//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_result.cpp
//
// = DESCRIPTION
//    Visitor that generates the Dyanmic::ParameterList
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, request_info_result, "$Id$")


// ************************************************************
// be_visitor_args_request_info_result for generating the result as
// stored in the request info for interceptors
// ************************************************************

be_visitor_args_request_info_result::be_visitor_args_request_info_result (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_request_info_result::~be_visitor_args_request_info_result (void)
{
}

int be_visitor_args_request_info_result::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node
  be_type *bt;
 
  os->indent ();
  *os << "this->result_val_ <<=";
  // Insertion into an Any has some special cases which need to be 
  // dealt with.

  // retrieve the type
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_::"
                         "visit_argument - "
                         "Bad return type\n"),
                        -1);
    }

  switch (bt->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      {
        if (bt->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "be_visitor_args_request_info_result::"
                               "visit_argument - "
                               "cannot accept visitor\n"),
                              -1);
          }
      
        *os <<"result_;" ;
        break;
      }
      
    case AST_Decl::NT_string:
      *os << "from_string (this->result_);" ;
      break;
      
    case AST_Decl::NT_wstring:
      *os << "from_wstring (this->result_);" ;
      break;
      
   default:
     *os << " this->result_;" << be_nl;

   }

  *os << be_nl;
  // Set the appropriate mode for each parameter.
  return 0;
}


int 
be_visitor_args_request_info_result::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_boolean:
      *os << "from_boolean (this->";
      break;
    case AST_PredefinedType::PT_char:
      *os << "from_char (this->";
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "from_wchar (this->";
      break;
    case AST_PredefinedType::PT_octet:
      *os << "from_octet (this->";
      break;
            
    default:
      break;
    }

  return 0;

}

 
