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
  // If there is an argument of type "native", return immediately.
  if (node->has_native ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  if (this->generate_shared_prolog (node, os, "") == -1)
    return -1;

  {
    // Declare variables for arguments.
    be_visitor_context ctx = *this->ctx_;
    ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);
    be_visitor *visitor = tao_cg->make_visitor (&ctx);

    if (!visitor || (node->accept (visitor) == -1))
      {
        delete visitor;
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_amh_operation_ss::"
                           "visit_operation - "
                           "codegen for return var decl failed\n"),
                          -1);
      }
    delete visitor;
  }

  int argument_count =
    node->count_arguments_with_direction (AST_Argument::dir_IN
                                          | AST_Argument::dir_INOUT);

  if (argument_count != 0)
    {
      // Avoid warnings in generated code due to unused arguments.
      *os << be_nl
          << "TAO_InputCDR &_tao_in ="
          << " _tao_server_request.incoming ();\n\n";
    }

  // Demarshal parameters.
  if (this->demarshal_params (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_operation_ss::"
                         "visit_operation - "
                         "demarshal_params failed\n"),
                        -1);
    }

  if (this->generate_shared_section (node, os, argument_count) == -1)
    return -1;

  {
    be_visitor_context ctx (*this->ctx_);
    ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS);
    be_visitor_operation_argument visitor (&ctx);
    if (node->accept (&visitor) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_amh_operation_ss::"
                           "visit_operation - "
                           "codegen for upcall args failed\n"),
                          -1);
      }
  }

  if (this->generate_shared_epilogue (os) == -1)
    return -1;

  return 0;

}

int
be_visitor_amh_operation_ss::visit_attribute (be_attribute *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  if (this->generate_shared_prolog (node, os, "_get_") == -1)
    return -1;

  if (this->generate_shared_section (node, os, 0) == -1)
    return -1;

  *os << "TAO_ENV_ARG_PARAMETER";

  if (this->generate_shared_epilogue (os) == -1)
    return -1;

  if (node->readonly ())
    return 0;

  if (this->generate_shared_prolog (node, os, "_set_") == -1)
    return -1;

  be_argument the_argument (AST_Argument::dir_IN,
                            node->field_type (),
                            node->name ());

  {
    be_visitor_context ctx (*this->ctx_);
    be_visitor_args_vardecl_ss visitor (&ctx);

    if (visitor.visit_argument (&the_argument) == -1)
      return -1;
  }

  *os << be_nl
      << "TAO_InputCDR &_tao_in ="
      << " _tao_server_request.incoming ();\n"
      << be_nl
      << "if (!(\n" << be_idt;

  {
    be_visitor_context ctx (*this->ctx_);
    ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
    ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
    be_visitor_args_marshal_ss visitor (&ctx);

    if (visitor.visit_argument (&the_argument) == -1)
      return -1;

      *os << be_uidt_nl << "))" << be_idt_nl;

      // If marshaling fails, raise exception.
      if (this->gen_raise_exception (0,
                                     "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) gen_raise_exception failed\n"),
                            -1);
        }
      *os << be_uidt << "\n";
  }

  if (this->generate_shared_section (node, os, 1) == -1)
    return -1;

  *os << node->local_name ()
      << be_nl << "TAO_ENV_ARG_PARAMETER";

  if (this->generate_shared_epilogue (os) == -1)
    return -1;

  return 0;
}

int
be_visitor_amh_operation_ss::generate_shared_prolog (be_decl *node,
                                                     TAO_OutStream *os,
                                                     const char *skel_prefix)
{
  *os << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl;

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf =
    be_interface::narrow_from_scope (node->defined_in ());

  if (intf == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }


  char *buf;
  intf->compute_full_name ("AMH_", "", buf);
  ACE_CString amh_skel_name ("POA_");
  amh_skel_name += buf;
  delete[] buf;

  os->indent ();
  *os << "void" << be_nl
      << amh_skel_name.c_str () << "::"
      << skel_prefix
      << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request," << be_nl
      << "void *_tao_object_reference, " << be_nl
      << "void * /* context */ " << be_nl
      << "TAO_ENV_ARG_DECL" << be_uidt_nl
      << ")" << be_uidt_nl;

  // Generate the actual code for the skeleton.
  // last argument - is always TAO_ENV_ARG_PARAMETER.
  *os << "{" << be_idt_nl;

  // Get the right object implementation.
  *os << amh_skel_name.c_str () << " *_tao_impl =" << be_idt_nl
      << "ACE_static_cast (" << be_idt << be_idt_nl
      << amh_skel_name.c_str () << " *," << be_nl
      << "_tao_object_reference" << be_uidt << be_uidt_nl
      << ");\n" << be_uidt;

  return 0;
}

int
be_visitor_amh_operation_ss::generate_shared_section (be_decl *node,
                                                      TAO_OutStream *os,
                                                      int argument_count)
{
  be_interface *intf =
    be_interface::narrow_from_scope (node->defined_in ());

  // Create the response handler
  char *buf;
  intf->compute_full_name ("AMH_", "ResponseHandler", buf);
  ACE_CString response_handler_name (buf);
  delete[] buf;

  intf->compute_full_name ("TAO_AMH_", "ResponseHandler", buf);
  ACE_CString response_handler_implementation_name ("POA_");
  response_handler_implementation_name += buf;
  delete[] buf;

  *os << be_nl << response_handler_name.c_str ()
      << "_var _tao_rh =" << be_idt_nl
      << "new " << response_handler_implementation_name.c_str ()
      << " (_tao_server_request);\n" << be_uidt;

  // Make the upcall.
  *os << be_nl << "_tao_impl->"
      << node->local_name () << " (_tao_rh.in ()"
      << be_idt << be_idt_nl;

  if (argument_count != 0)
    *os << ",";

  return 0;
}

int
be_visitor_amh_operation_ss::generate_shared_epilogue (TAO_OutStream *os)
{
  *os << be_uidt_nl << ");"
      << be_uidt << be_uidt_nl
      << "}\n\n";

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

      *os << be_uidt_nl << "))" << be_idt_nl;

      // If marshaling fails, raise exception.
      if (this->gen_raise_exception (0,
                                     "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) gen_raise_exception failed\n"),
                            -1);
        }
      *os << be_uidt << "\n";
    }

  return 0;
}
