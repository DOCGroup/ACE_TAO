//=============================================================================
/**
*  @file   amh_ss.cpp
*
*  $Id$
*
*  Creates code for AMH operations.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_operation.h"

be_visitor_amh_operation_ss::be_visitor_amh_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_operation_ss::~be_visitor_amh_operation_ss (void)
{
}

int
be_visitor_amh_operation_ss::visit_operation (be_operation *node)
{
  this->ctx_->node (node);
  TAO_OutStream *os = this->ctx_->stream ();
  
  // If there is an argument of type "native", return immediately.
  if (node->has_native ())
    return 0;
  
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());
  
if (!intf)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_operation_ss::"
                       "visit_operation - "
                       "bad interface scope\n"),
                      -1);
  }
 
 os->indent ();
 *os << "void " << intf->full_skel_name () << "::";
 
 // Check if we are an attribute node in disguise
 if (this->ctx_->attribute ())
   {
     // now check if we are a "get" or "set" operation
     if (node->nmembers () == 1) // set
       *os << "_set_";
     else
       *os << "_get_";
   }
 
 *os << node->local_name ()
     << "_skel (" << be_idt << be_idt_nl
     << "TAO_ServerRequest &_tao_server_request," << be_nl
     << "void *_tao_object_reference, " << be_nl
     << "void * /* context */ " << be_nl
     << "TAO_ENV_ARG_DECL" << be_uidt_nl
     << ")" << be_uidt_nl;
 
 // Generate the actual code for the skeleton. However, if any of the argument
 // types is "native", we do not generate any skeleton
 // last argument - is always TAO_ENV_ARG_PARAMETER.
 *os << "{" << be_idt_nl;
 
 *os << "// foo" << be_uidt_nl;
 
 *os << "}" << be_nl << be_nl;
 
 
 // see <be_visitor_operation_ss::visit_operation> for more info
 
 return 0;
}



// ------------------------- RH -------------------------------------------

be_visitor_amh_rh_operation_ss::be_visitor_amh_rh_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_rh_operation_ss::~be_visitor_amh_rh_operation_ss (void)
{
}

int
be_visitor_amh_rh_operation_ss::visit_operation (be_operation *node)
{
  this->ctx_->node (node);
  TAO_OutStream *os = this->ctx_->stream ();
  
  // If there is an argument of type "native", return immediately.
  if (node->has_native ())
    return 0;
  
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());
  
  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }
  
  os->indent ();
  *os << "void " << intf->full_skel_name () << "::";
  
  // Check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  
  *os << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request," << be_nl
      << "void *_tao_object_reference, " << be_nl
      << "void * /* context */, " << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl;
  
  // Generate the actual code for the skeleton. However, if any of the argument
  // types is "native", we do not generate any skeleton
  // last argument - is always CORBA::Environment.
  *os << "{" << be_idt_nl;
  
  *os << "// foo" << be_uidt_nl;
  
  *os << "}" << be_nl << be_nl;
  
  
  // see <be_visitor_operation_ss::visit_operation> for more info
  
  return 0;
}
