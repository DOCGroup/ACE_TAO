//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_ami_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_cs::be_visitor_operation_ami_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_cs::~be_visitor_operation_ami_cs (void)
{
}

// Processing to be done after every element in the scope is
// processed.
int
be_visitor_operation_ami_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_ami_cs::visit_operation (be_operation *node)
{
  // No sendc method for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    return 0;

  TAO_OutStream *os; // output stream
  be_visitor_context ctx;  // visitor context
  be_visitor *visitor; // visitor

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  os->indent (); // start with the current indentation level

  // Generate the return type mapping. Return type is simply void.
  *os << "void" << be_nl;

  // Generate the operation name.

  // Grab the scope name.
  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();
  if (parent == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_operation_ami_cs::"
                       "visit_operation - "
                       "scope name is nil\n"),
                      -1);

  // Generate the scope::operation name.
  *os << parent->full_name ()
      << "::sendc_";

    // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "set_";
      else
        *os << "get_";
    }

   *os << node->local_name ()->get_string ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);
  if ((!visitor) || (node->hidden_operation ()->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always CORBA::Environment
  *os << "{" << be_idt_nl;

  be_type *bt = be_type::narrow_from_decl (node->hidden_operation ()->return_type ());

  // generate any pre stub info if and only if none of our parameters is of the
  // native type
  if (!node->has_native ())
    {
      // native type does not exist.

      // Generate any "pre" stub information such as tables or declarations
      // This is a template method and the actual work will be done by the
      // derived class
      if (this->gen_pre_stub_info (node, bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "gen_pre_stub_info failed\n"),
                            -1);
        }
    }

  if (node->has_native ()) // native exists => no stub
    {
      if (this->gen_raise_exception (bt,
                                     "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "codegen for return var failed\n"),
                            -1);
        }
    }
  else
    {
      // Generate code that retrieves the underlying stub object and then
      // invokes do_static_call on it.
      *os << be_nl
          << "TAO_Stub *istub = this->_stubobj ();" << be_nl
          << "if (istub == 0)" << be_idt_nl;

      // if the stub object was bad, then we raise a system exception
      if (this->gen_raise_exception (bt, "CORBA::INV_OBJREF",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "codegen for checking exception failed\n"),
                            -1);

        }
      *os << be_uidt_nl << "\n";

#if 0
      // do any pre marshal and invoke processing with return type. This
      // includes allocating memory, initialization.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_PRE_INVOKE_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }
#endif /* 0 */
      // Generate the code for marshaling in the parameters and transmitting
      // them.
      if (this->gen_marshal_and_invoke (node, bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "visit_operation - "
                             "codegen for marshal and invoke failed\n"),
                            -1);

        }

      // No return values.
      *os << "return;";
    } // end of if (!native)

  *os << be_uidt_nl << "}\n\n";

  return 0;
}

int
be_visitor_operation_ami_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_cs::"
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
be_visitor_operation_ami_cs::gen_raise_exception (be_type *bt,
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
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "gen_raise_exception - "
                             "codegen for return var failed\n"),
                            -1);
        }
      *os << ");\n";
    }
  return 0;
}

int
be_visitor_operation_ami_cs::gen_check_exception (be_type *bt)
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
                             "(%N:%l) be_visitor_operation_ami_cs::"
                             "gen_check_exception - "
                             "codegen failed\n"),
                            -1);
        }
      *os << ");\n";
    }

  return 0;
}

// ************************************************************
// Operation visitor for interpretive client stubs
// ************************************************************

be_interpretive_visitor_operation_ami_cs::
be_interpretive_visitor_operation_ami_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_cs (ctx)
{
}

be_interpretive_visitor_operation_ami_cs::~be_interpretive_visitor_operation_ami_cs (void)
{
}

// concrete implementation of the template methods

int
be_interpretive_visitor_operation_ami_cs::gen_pre_stub_info (be_operation *node,
                                                         be_type *bt)
{
  return 0;
}

int
be_interpretive_visitor_operation_ami_cs::gen_marshal_and_invoke (be_operation
                                                                  *node,
                                                                  be_type *bt)
{

  return 0;
}

// ************************************************************
// Operation visitor for compiled client stubs
// ************************************************************

be_compiled_visitor_operation_ami_cs::
be_compiled_visitor_operation_ami_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_cs (ctx)
{
}

be_compiled_visitor_operation_ami_cs::~be_compiled_visitor_operation_ami_cs (void)
{
}

// concrete implementation of the template methods

int
be_compiled_visitor_operation_ami_cs::gen_pre_stub_info (be_operation *node,
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
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) "
                             "be_compiled_visitor_operation_ami_cs::"
                             "gen_pre_stub_info - "
                             "Exceptionlist generation error\n"),
                            -1);
        }
    }

  return 0;
}

int
be_compiled_visitor_operation_ami_cs::gen_marshal_and_invoke (be_operation *node,
                                                              be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  os->indent ();

  // Create the GIOP_Invocation and grab the outgoing CDR stream.
  switch (node->flags ())
    {
    case AST_Operation::OP_oneway:
      // If it is a oneway, we wouldnt have come here to generate AMI
      // sendc method.
      break;
    default:
      *os << "TAO_GIOP_Twoway_Asynch_Invocation _tao_call ";
    }
  *os << "(" << be_idt << be_idt_nl
      << "istub," << be_nl;

  *os << "\"";

  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }

  *os << node->local_name ()
      << "\"," << be_nl
      << "istub->orb_core ()," << be_nl;

  // Next argument is the reply handler skeleton for this method.

  // Get the interface.
  be_decl *interface = be_interface::narrow_from_scope (node->defined_in ())->decl ();

  {
    char *full_name = 0;

    interface->compute_full_name ("AMI_",
                                  "Handler",
                                  full_name);

    *os << "&" << full_name << "::";

  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }

    *os << node->local_name () << "_reply_stub," << be_nl;

    delete full_name;
  }



  // Next argument is the ami handler passed in for this method.
  *os << "ami_handler" << be_uidt_nl

      << ");" << be_uidt_nl;

  *os << "\n" << be_nl
      << "for (;;)" << be_nl
      << "{" << be_idt_nl;

  *os << "_tao_call.start (ACE_TRY_ENV);" << be_nl;
  // Check if there is an exception.
  // Return type is void, so we know what to generate here.
  *os << "ACE_CHECK;";

  // Prepare the request header
  *os << "_tao_call.prepare_header (";
  switch (node->flags ())
    {
    case AST_Operation::OP_oneway:
      *os << "0";
      break;
    default:
      *os << "1";
    }
  *os << ", ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK;\n" << be_nl;

  // Now make sure that we have some in and inout
  // parameters. Otherwise, there is nothing to be marshaled in.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      *os << be_nl
          << "TAO_OutputCDR &_tao_out = _tao_call.out_stream ();"
          << be_nl
          << "if (!(\n" << be_idt << be_idt << be_idt;

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->hidden_operation ()->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for return var in do_static_call failed\n"),
                            -1);
        }
      *os << be_uidt << be_uidt_nl
          << "))" << be_nl;

      // If marshaling fails, raise exception.
      if (this->gen_raise_exception (bt, "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_cs::"
                             "gen_marshal_and invoke - "
                             "codegen for return var failed\n"),
                            -1);
        }
      *os << be_uidt;
    }

  *os << be_nl
      << "int _invoke_status =" << be_idt_nl;
  if (node->exceptions ())
    {
      *os << "_tao_call.invoke (_tao_" << node->flat_name ()
          << "_exceptiondata, "
          << node->exceptions ()->length ()
          << ", ACE_TRY_ENV);";
    }
  else
    {
      *os << "_tao_call.invoke (0, 0, ACE_TRY_ENV);";
    }

  *os << be_uidt_nl;

  // Check if there is an exception.
  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_ami_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << be_nl
      << "if (_invoke_status == TAO_INVOKE_RESTART)" << be_idt_nl
      << "continue;" << be_uidt_nl
      << "if (_invoke_status != TAO_INVOKE_OK)" << be_nl
      << "{" << be_idt_nl;

  if (this->gen_raise_exception (bt,
                                 "CORBA::UNKNOWN",
                                 "TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES") == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_ami_cs::"
                         "gen_marshal_and invoke - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}" << be_nl
      << "break;" << be_nl
      << be_uidt_nl << "}" << be_nl;

  // Return type is void and we are going to worry about OUT or INOUT
  // parameters. Return from here.
  return 0;
}
