//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_operation_cs.cpp
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

ACE_RCSID(be_visitor_operation, ami_handler_operation_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_handler_operation_cs::be_visitor_operation_ami_handler_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_operation_cs::~be_visitor_operation_ami_handler_operation_cs (void)
{
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_ami_handler_operation_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_ami_handler_operation_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  be_visitor_context ctx;  // visitor context
  be_visitor *visitor; // visitor

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  // Start with the current indentation level.
  os->indent (); 

  // Init the return type variable.
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_operation_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }
  
  // Generate the return type. Return type is simply void.
  *os << "void " << be_nl;

  // Get the scope name.
  be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
  if (parent == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                         "visit_operation - "
                         "scopeless operation :-<\n"),
                        -1);
    }

  // Genereate scope name.
  *os << parent->compute_name ("AMI_", "_Handler");
  
  // Generate the operation name.
  *os << "::" << node->local_name ();
  
  // Generate the argument list with the appropriate mapping (same as
  // in the header file).
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if ((!visitor) || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  os->indent ();
  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always CORBA::Environment
  *os << "{" << be_idt_nl;

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

  // Declaring return type is not necessary. Since return  type is
  // void. 
  
  // Native exists => no stub.
  if (node->has_native ())
    {
      if (this->gen_raise_exception (bt, "CORBA::MARSHAL", "") == -1)  
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                             "visit_operation - "
                             "codegen for return var failed\n"),
                            -1);
        }
    }
  else
    {
      os->indent ();
      // Generate code that retrieves the underlying stub object and then
      // invokes do_static_call on it.
      *os << be_nl
          << "TAO_Stub *istub = this->_stubobj ();" << be_nl
          << "if (istub == 0)" << be_idt_nl;
      
      // If the stub object was bad, then we raise a system
      // exception.
      *os << "ACE_THROW (CORBA::INTERNAL ());\n\n";
      
      *os << be_uidt;

      // do any pre marshal and invoke stuff with arguments
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_PRE_INVOKE_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for argument pre invoke failed\n"),
                            -1);
        }

      // Generate the code for marshaling in the parameters and transmitting
      // them.
      if (this->gen_marshal_and_invoke (node, bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for marshal and invoke failed\n"),
                            -1);

        }
      
      // Return type is void,

    } // end of if (!native)

  *os << be_uidt_nl << "}" << be_nl << be_nl;
  
  // Generate the skeleton method.
  
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_SKELETON_CS);
  visitor = tao_cg->make_visitor (&ctx);
  
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for AMI Handler skeleton failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}

int
be_visitor_operation_ami_handler_operation_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
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
be_visitor_operation_ami_handler_operation_cs::gen_raise_exception (be_type *bt,
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
                             "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                             "gen_raise_exception - "
                             "codegen for return var failed\n"),
                            -1);
        }
      *os << ");\n";
    }
  return 0;
}

int
be_visitor_operation_ami_handler_operation_cs::gen_check_exception (be_type *bt)
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
                             "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                             "gen_check_exception - "
                             "codegen failed\n"),
                            -1);
        }
      *os << ");" << be_nl;
    }

  return 0;
}

// ************************************************************
// Operation visitor for interpretive client stubs
// ************************************************************

be_interpretive_visitor_operation_ami_handler_operation_cs::
be_interpretive_visitor_operation_ami_handler_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_operation_cs (ctx)
{
}

be_interpretive_visitor_operation_ami_handler_operation_cs::~be_interpretive_visitor_operation_ami_handler_operation_cs (void)
{
}

// concrete implementation of the template methods

int
be_interpretive_visitor_operation_ami_handler_operation_cs::gen_pre_stub_info (be_operation *node,
                                                                               be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // Generate the TAO_Param_Data table
  os->indent ();
  *os << "static const TAO_Param_Data ";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flatname () <<
    "_paramdata [] = " << be_nl;
  *os << "{\n";
  os->incr_indent ();

  // entry for the return type
  *os << "{" << bt->tc_name () << ", PARAM_RETURN, 0}";
  if (node->nmembers () > 0)
    *os << ",\n";

      // generate entries for the param data table for arguments
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interpretive_visitor_operation_ami_handler_operation_cs::"
                         "gen_pre_stub_info - "
                         "visit scope failed\n"),
                        -1);
    }

  os->indent ();
  *os << be_nl;
  os->decr_indent ();
  *os << "}; // " << node->flatname () << "_paramdata\n\n";

  // Check if this operation raises any exceptions. In that case, we must
  // generate a list of exception typecodes. This is not valid for
  // attributes
  if (!this->ctx_->attribute ())
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_EXCEPTLIST_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) "
                             "be_interpretive_visitor_operation_ami_handler_operation_cs::"
                             "gen_pre_stub_info - "
                             "Exceptionlist generation error\n"),
                            -1);
        }
    }

  // now generate the calldata table
  os->indent ();
  *os << "static const TAO_Call_Data ";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flatname ()
      << "_calldata = " << be_nl
      << "{"
      << "\"";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->local_name () << "\", ";

      // are we oneway or two operation?
  if (node->flags () == AST_Operation::OP_oneway)
    {
      *os << "0, "; // for false
    }
  else
    {
      *os << "1, "; // for true
    }
  // insert the size of the paramdata table i.e., number of arguments + 1
  // for return type
  *os << (node->argument_count () + 1) << ", ";

      // insert the address of the paramdata table
      // first check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flatname () << "_paramdata, ";

      // insert exception list (if any) - node for attributes
  if (this->ctx_->attribute ())
    *os << "0, 0};\n\n";
  else
    {
      if (node->exceptions ())
        {
          *os << node->exceptions ()->length ()
              << ", _tao_" << node->flatname () << "_exceptiondata};\n\n";
        }
      else
        *os << "0, 0};\n\n";
    }
  return 0;
}

int
be_interpretive_visitor_operation_ami_handler_operation_cs::
gen_marshal_and_invoke (be_operation*node,
                        be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  os->indent ();
  *os << "void* _tao_arguments["
      << node->argument_count () + 1 << "];" << be_nl
      << "const void** _tao_arg = ACE_const_cast (const void**,_tao_arguments);" << be_nl
      << "*_tao_arg = ";

  // pass the appropriate return value to docall
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_INVOKE_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interpretive_visitor_operation_ami_handler_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for return var in do_static_call failed\n"),
                        -1);
    }
  *os << "; _tao_arg++;\n";

  // pass each argument to do_static_call
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interpretive_visitor_operation_ami_handler_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for return var in do_static_call failed\n"),
                        -1);
    }
  delete visitor;
  visitor=0;

  // call do_static_call with appropriate number of arguments
  os->indent ();
  *os << "istub->do_static_call (" << be_idt_nl
      << "ACE_TRY_ENV, " << be_nl
      << "&";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flatname () << "_calldata," << be_nl
      << "_tao_arguments" << be_uidt_nl
      << ");\n";

  os->indent ();
  // check if there is an exception
  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interpretive_visitor_operation_ami_handler_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }

  // do any post processing for the arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_POST_INVOKE_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                         "visit_operation - "
                         "codegen for args post do_static_call failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  // do any post processing for the retval
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_POST_INVOKE_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_operation_cs::"
                         "visit_operation - "
                         "codegen for return type post do_static_call failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}

// ************************************************************
// Operation visitor for compiled client stubs
// ************************************************************

be_compiled_visitor_operation_ami_handler_operation_cs::
be_compiled_visitor_operation_ami_handler_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_operation_cs (ctx)
{
}

be_compiled_visitor_operation_ami_handler_operation_cs::~be_compiled_visitor_operation_ami_handler_operation_cs (void)
{
}

// concrete implementation of the template methods

int
be_compiled_visitor_operation_ami_handler_operation_cs::gen_pre_stub_info (be_operation *node,
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
                             "be_compiled_visitor_operation_ami_handler_operation_cs::"
                             "gen_pre_stub_info - "
                             "Exceptionlist generation error\n"),
                            -1);
        }
      delete visitor;
    }

  return 0;
}

int
be_compiled_visitor_operation_ami_handler_operation_cs::
gen_marshal_and_invoke (be_operation *node,
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
      *os << "TAO_GIOP_Oneway_Invocation _tao_call ";
      break;
    default:
      *os << "TAO_GIOP_Twoway_Invocation _tao_call ";
    }
  *os << "(" << be_idt << be_idt_nl
      << "istub," << be_nl;

  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "\"_set_\"";
      else
        *os << "\"_get_\"";
    }

  *os << "\"" << node->original_local_name ()
      << "\"," << be_nl
      << "istub->orb_core ()" << be_uidt_nl
      << ");" << be_uidt_nl;

  *os << "\n" << be_nl
      << "for (;;)" << be_nl
      << "{" << be_idt_nl;

  *os << "ACE_TRY_ENV.clear ();" << be_nl;
  *os << "_tao_call.start (ACE_TRY_ENV);" << be_nl;
  // Check if there is an exception.
  *os << "ACE_CHECK;";

  // Now make sure that we have some AMI result  parameter or OUT or
  // INOUT parameters. Otherwise, there is nothing to be marshaled
  // in.
  if (!this->void_return_type (bt) || 
      this->has_param_type (node, AST_Argument::dir_OUT) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      *os << be_nl
          << "TAO_OutputCDR &_tao_out = _tao_call.out_stream ();"
          << be_nl
          << "if (!(\n" << be_idt << be_idt << be_idt;

      // @@ Michael: This has to be replaced witht he code in the 
      //             "#if 0" clause
      // Marshal the ami result argument, if the return type is not
      // void. 
      if (!this->void_return_type (bt))
        {
          os->indent ();
          *os << "(_tao_out << _tao_retval)";

          // Print the && if there are OUT or INOUT arguements in the
          // signature. 
          if (this->has_param_type (node, AST_Argument::dir_OUT) ||
              this->has_param_type (node, AST_Argument::dir_INOUT))
            *os << " &&\n";
        }
     
#if 0
      // @@ This for giving the _tao_retval argument only. But
      //    this may be needed for some data types.
      //    But the one that is above is ok for basic types. 
      // @@ We may need to do this.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_RETVAL_MARSHAL_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_handler_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for return var in do_static_call failed\n"),
                            -1);
        }
      delete visitor;
      visitor = 0;
#endif /* 0 */

      // Marshal each out and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_MARSHAL_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_handler_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for return var in do_static_call failed\n"),
                            -1);
        }
      delete visitor;
      visitor = 0;

      *os << be_uidt << be_uidt_nl
          << "))" << be_nl;

      // if marshaling fails, raise exception.
      *os << "ACE_THROW (CORBA::MARSHAL ());";

      *os << be_uidt;
    }

  *os << be_nl
      << "int _invoke_status =" << be_idt_nl;
  if (node->flags () == AST_Operation::OP_oneway)
    {
      // oneway operation
      *os << "_tao_call.invoke (ACE_TRY_ENV);";
    }
  else
    {
      if (node->exceptions ())
        {
          *os << "_tao_call.invoke (_tao_" << node->flatname ()
              << "_exceptiondata, "
              << node->exceptions ()->length ()
              << ", ACE_TRY_ENV);";
        }
      else
        {
          *os << "_tao_call.invoke (0, 0, ACE_TRY_ENV);";
        }
    }

  *os << be_uidt_nl;

  // Check if there is an exception.
  *os << "ACE_CHECK;";
  
  *os << be_nl
      << "if (_invoke_status == TAO_INVOKE_RESTART)" << be_idt_nl
      << "continue;" << be_uidt_nl
      << "// if (_invoke_status == TAO_INVOKE_EXCEPTION)" << be_idt_nl
      << "// cannot happen" << be_uidt_nl
      << "if (_invoke_status != TAO_INVOKE_OK)" << be_nl
      << "{" << be_idt_nl;

  // Check if there is an exception.
  *os << "ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));";

  *os << be_uidt_nl
      << "}" << be_nl
      << "break;" << be_nl
      << be_uidt_nl << "}" << be_nl;
  
  return 0;
}
