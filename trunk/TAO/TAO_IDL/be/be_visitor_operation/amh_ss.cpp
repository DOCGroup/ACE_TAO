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
  
  *os << "// special code generation for AMH starts here: " << be_nl;
  
  // Get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl =" << be_idt_nl
      << "ACE_static_cast (" << be_idt << be_idt_nl
      << intf->full_skel_name () << " *," << be_nl
      << "_tao_object_reference" << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt<< be_uidt_nl;
  
  // Declare a return type variable.
  // AMH operations do not have return | OUT
  *os << be_idt_nl << "_tao_server_request.argument_flag (0);" << be_uidt_nl << be_nl;
  
  // Declare variables for arguments.
  *os << be_idt;
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }
  
  // Demarshal parameters.
  if (this->demarshal_params (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }
  
  // NO interceptors code
  
  // Make the upcall.
  *os << be_uidt_nl
      << "_tao_impl->" << node->local_name () << " (" << be_idt << be_idt_nl;
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }
  
  // End the upcall.
  *os << be_uidt << be_uidt_nl << ");" << be_uidt_nl << be_nl;
  
  if (!be_global->exception_support ())
    {
      *os << "TAO_INTERCEPTOR_CHECK;";
    }
  
  *os << be_idt_nl
      << "// In case _tao_servant_upcall is not used in this function"
      << be_nl
      << "ACE_UNUSED_ARG (_tao_servant_upcall);" << be_uidt_nl << be_nl;
  
  *os << be_uidt << "}\n" << be_uidt_nl;
  
  return 0;
  
}


int
be_visitor_amh_operation_ss::demarshal_params (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;
  
  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      os->indent ();
      
      // demarshal the in and inout arguments
      *os << "if (!(\n" << be_idt;
      
      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ss::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }
      
      *os << be_uidt_nl << "))\n" << be_idt;
      
      // Look into this in detail later
      /*      // If marshaling fails, raise exception.
              if (this->gen_raise_exception (bt, "CORBA::MARSHAL",
              "",
              "ACE_TRY_ENV") == -1)
              {
              ACE_ERROR_RETURN ((LM_ERROR,
              "(%N:%l) be_compiled_visitor_operation_ss::"
              "gen_marshal_and invoke - "
              "codegen for return var failed\n"),
              -1);
              }
              
              *os << be_uidt << "\n";
              */
    };
  
  return 0;
}
