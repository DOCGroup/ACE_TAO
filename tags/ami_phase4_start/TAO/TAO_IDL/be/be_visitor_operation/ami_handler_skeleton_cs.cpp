//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_skeleton_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_handler_skeleton_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_handler_skeleton_cs::be_visitor_operation_ami_handler_skeleton_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_skeleton_cs::~be_visitor_operation_ami_handler_skeleton_cs (void)
{
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_ami_handler_skeleton_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_ami_handler_skeleton_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  be_visitor_context ctx;  // visitor context
  be_visitor *visitor = 0; // visitor

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Init the return type variable.
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_skeleton_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Start with the current indentation level.
  os->indent ();


  // Generate the return type. Return type is simply void.
  *os << be_nl << "void" << be_nl;

  // Get the scope name.
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  if (parent == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_skeleton_cs::"
                         "visit_operation - "
                         "scopeless operation :-<\n"),
                        -1);
    }

  // Genereate scope name.
  *os << parent->compute_name ("AMI_", "_Handler");

  // Generate the operation name.
  *os << "::" << node->local_name () << "_skel (" << be_idt_nl;

  // Generate the argument list.
  *os << "TAO_InputCDR &_tao_in, " << be_nl
      << "Messaging::ReplyHandler_ptr _tao_reply_handler," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV"<< be_uidt << be_uidt_nl
      << ")" << be_nl;

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always CORBA::Environment
  *os << "{\n" << be_idt;

  // Deal with differences between IDL mapping for true C++ exceptions and
  // alternate mapping. Since our code uses the ACE_TRY_ENV variable in a
  // number of places, for the true exception case, we will have to explicitly
  // declare the ACE_TRY_ENV variable.
  *os << this->gen_environment_var () << be_nl;

  // Generate any pre stub info if and only if none of our parameters is of the
  // native type.
  if (!node->has_native ())
    {
      // native type does not exist.

      // Generate any "pre" stub information such as tables or declarations
      // This is a template method and the actual work will be done by the
      // derived class
      if (this->gen_pre_stub_info (node, bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "gen_pre_stub_info failed\n"),
                            -1);
        }
    }

  os->indent();

  *os << "// Retrieve Reply Handler object." << be_nl;
  *os << parent->compute_name ("AMI_", "_Handler") << "_var "
      << "_tao_reply_handler_object =" << be_idt_nl;

  *os << parent->compute_name ("AMI_", "_Handler");
  *os << "::_narrow(_tao_reply_handler, ACE_TRY_ENV);" << be_uidt_nl;

  *os << "ACE_CHECK;" << be_nl << be_nl
      << "// @@ Error handling " << be_nl << be_nl;

  *os << "//Demarshall all the arguments.\n";

  // declare a return type variable
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_RETVAL_DECL_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_skeleton_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  // declare variables for arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_DECL_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_skeleton_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  // Demarshal parameters
  if (this->gen_marshal_and_invoke (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%1) ami_handler_skeleton_cs::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};" << be_nl << be_nl;

  return 0;
}

int
be_visitor_operation_ami_handler_skeleton_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_skeleton_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";
  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "PARAM_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "PARAM_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "PARAM_OUT, ";
      break;
    }
  *os << "0}";

  return 0;
}

int
be_visitor_operation_ami_handler_skeleton_cs::gen_raise_exception (be_type *bt,
                                                                    const char *excep,
                                                                    const char *completion_status)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  if (this->void_return_type (bt))
    {
      *os << "ACE_THROW ("
          << excep << " (" << completion_status << "));\n";
    }
  else
    {
      *os << "ACE_THROW_RETURN ("
          << excep << " (" << completion_status << "), ";

      // return the appropriate return value
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_handler_skeleton_cs::"
                             "gen_raise_exception - "
                             "codegen for return var failed\n"),
                            -1);
        }
      *os << ");\n";
    }
  return 0;
}

int
be_visitor_operation_ami_handler_skeleton_cs::gen_check_exception (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  os->indent ();
  // check if there is an exception
  if (this->void_return_type (bt))
    {
      *os << "ACE_CHECK;\n";
      //<< "_tao_environment);\n";
    }
  else
    {
      *os << "ACE_CHECK_RETURN (";
      // << "_tao_environment, ";

      // return the appropriate return value
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_handler_skeleton_cs::"
                             "gen_check_exception - "
                             "codegen failed\n"),
                            -1);
        }
      *os << ");\n";
    }

  return 0;
}

// ************************************************************
// Operation visitor for interpretive client skeletons of the AMI reply handler
// ************************************************************

be_interpretive_visitor_operation_ami_handler_skeleton_cs::
be_interpretive_visitor_operation_ami_handler_skeleton_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_skeleton_cs (ctx)
{
}

be_interpretive_visitor_operation_ami_handler_skeleton_cs::~be_interpretive_visitor_operation_ami_handler_skeleton_cs (void)
{
}

// concrete implementation of the template methods

int
be_interpretive_visitor_operation_ami_handler_skeleton_cs::gen_pre_stub_info (be_operation *node,
                                                                              be_type *bt)
{
  ACE_UNUSED_ARG (node);
  ACE_UNUSED_ARG (bt);

  // @@ Michael: To be done.
  return 0;
}

int
be_interpretive_visitor_operation_ami_handler_skeleton_cs::
gen_marshal_and_invoke (be_operation*node,
                        be_type *bt)
{
  ACE_UNUSED_ARG (node);
  ACE_UNUSED_ARG (bt);

  // @@ Michael: To be done.
  return 0;
}

// ************************************************************
// Operation visitor for compiled client skeletons of the AMI reply handler
// ************************************************************

be_compiled_visitor_operation_ami_handler_skeleton_cs::
be_compiled_visitor_operation_ami_handler_skeleton_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_skeleton_cs (ctx)
{
}

be_compiled_visitor_operation_ami_handler_skeleton_cs::~be_compiled_visitor_operation_ami_handler_skeleton_cs (void)
{
}

// concrete implementation of the template methods

int
be_compiled_visitor_operation_ami_handler_skeleton_cs::gen_pre_stub_info (be_operation *node,
                                                                           be_type *)
{

  // Check if this operation raises any exceptions. In that case, we must
  // generate a list of exception typecodes. This is not valid for
  // attributes
  if (!this->ctx_->attribute ())
    {
      be_visitor_context ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_EXCEPTLIST_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) "
                             "be_compiled_visitor_operation_ami_handler_skeleton_cs::"
                             "gen_pre_stub_info - "
                             "Exceptionlist generation error\n"),
                            -1);
        }
      delete visitor;
    }

  return 0;
}

int
be_compiled_visitor_operation_ami_handler_skeleton_cs::
gen_marshal_and_invoke (be_operation *node,
                        be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor = 0;
  be_visitor_context ctx;

  os->indent ();

  *os << "if (!(\n" << be_idt << be_idt << be_idt;

  if (!this->void_return_type (bt))
    {
      // demarshal the return val
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_RETVAL_DEMARSHAL_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_handler_skeleton_cs::"
                             "gen_demarshal_params - "
                             "codegen for return var failed\n"),
                            -1);
        }
      delete visitor;

      // Print the && if there are OUT or INOUT arguements in the
      // signature.
      if (this->has_param_type (node, AST_Argument::dir_OUT) ||
          this->has_param_type (node, AST_Argument::dir_INOUT))
        *os << " &&\n";
    }

  if (this->has_param_type (node, AST_Argument::dir_INOUT) ||
      this->has_param_type (node, AST_Argument::dir_OUT))
    {
      // demarshal each in and inout argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_DEMARSHAL_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l)  be_compiled_visitor_operation_ami_handler_skeleton_cs::"
                             "gen_marshal_params - "
                             "codegen for args failed\n"),
                            -1);
        }
      delete visitor;
    }

  *os << be_uidt << be_uidt_nl
      << " ))" << be_nl
      << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl << be_nl;

  // Invoke the callback method
  *os << "// Invoke the call back method." << be_nl
      << "_tao_reply_handler_object->"
      << node->local_name () << " (" << be_idt_nl;

  // if we have a non-void return type then pass it as the first argument
  if (!this->void_return_type (bt))
    *os << "_tao_retval,\n";

  // generate the argument list containing the inout and inout arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_UPCALL_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_skeleton_cs::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  *os << be_nl << ");" << be_uidt_nl;

  *os << "ACE_CHECK;" << be_nl;

  return 0;
}
