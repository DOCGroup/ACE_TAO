//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    paramlist.cpp
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

ACE_RCSID(be_visitor_argument, paramlist, "$Id$")


// ************************************************************
// be_visitor_args_paramlist for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_paramlist::be_visitor_args_paramlist (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_paramlist::~be_visitor_args_paramlist (void)
{
}

int be_visitor_args_paramlist::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node
 
  os->indent ();

  *os << be_idt << "CORBA::ULong length = this->parameter_list_.length ();" << be_nl
      << "  this->parameter_list_.length (length + 1);" << be_nl
      << "  this->parameter_list_[length].argument "; //<<= this->arg_;
  // Insertion into an Any has some special cases which need to be 
  // dealt with.

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_arglist::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

   os->indent (); // start with current indentation level

  *os << "<<= ";

  switch (bt->node_type ())
    {
    case AST_Decl::NT_pre_defined:
      {
        if (bt->accept (this) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "be_visitor_args_paramlist::"
                               "visit_argument - "
                               "cannot accept visitor\n"),
                              -1);
          }
      
        *os <<node->local_name () << "_;" ;
        break;
      }
      
    case AST_Decl::NT_string:
      *os << "from_string (this->"<< node->local_name () << "_);" ;
      break;
      
    case AST_Decl::NT_wstring:
      *os << "from_wstring (this->"<< node->local_name () << "_);" ;
      break;
      
   default:
     *os << " this->" << node->local_name () << "_;" << be_nl;

   }

  *os << be_nl;
  // Set the appropriate mode for each parameter.

  switch (node->direction ())
    {
      case AST_Argument::dir_IN:
         *os <<"this->parameter_list_[length].mode = Dynamic::PARAM_IN;"<<be_nl;
         break;
      case AST_Argument::dir_OUT:
         *os <<"this->parameter_list_[length].mode = Dynamic::PARAM_OUT;"<<be_nl;
         break;
      case AST_Argument::dir_INOUT:
         *os <<"this->parameter_list_[length].mode = Dynamic::PARAM_INOUT;"<<be_nl;
         break;
    default: 
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_args_paramlist::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
     

    }
  
  return 0;
}


int 
be_visitor_args_paramlist::visit_predefined_type (be_predefined_type *node)
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

 
