//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_cs::be_visitor_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx),
    operation_name_ (0)
{
}

be_visitor_operation_cs::~be_visitor_operation_cs (void)
{
  delete[] operation_name_;
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  be_visitor_context ctx;  // visitor context
  be_visitor *visitor; // visitor

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node for future use

  if (this->ctx_->state () == TAO_CodeGen::TAO_LOCAL_OPERATION_S)
    return 0;

  os->indent (); // start with the current indentation level

  // retrieve the operation return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);

  if ((!visitor) || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // Generate the operation name
  *os << " " << node->name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);
  if ((!visitor) || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always CORBA::Environment
  *os << "{" << be_idt_nl;

  // Deal with differences between IDL mapping for true C++ exceptions and
  // alternate mapping. Since our code uses the ACE_TRY_ENV variable in a
  // number of places, for the true exception case, we will have to explicitly
  // declare the ACE_TRY_ENV variable.
  *os << this->gen_environment_var () << "\n";

  // Generate any pre stub info if and only if none of our parameters is of the
  // native type.
  if (!node->has_native ())
    {
      // native type does not exist.

      // Generate any "pre" stub information such as tables or declarations
      // This is a template method and the actual work will be done by the
      // derived class
      if (this->gen_pre_stub_info (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "gen_pre_stub_info failed\n"),
                            -1);
        }
    }

  // Declare return type.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  if (node->has_native ()) // native exists => no stub
    {
      if (this->gen_raise_exception (bt, "CORBA::MARSHAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
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
      if (this->gen_raise_exception (bt, "CORBA::INTERNAL",
                                     "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for checking exception failed\n"),
                            -1);

        }
      *os << be_uidt_nl << "\n";

      // do any pre marshal and invoke processing with return type. This
      // includes allocating memory, initialization.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_PRE_INVOKE_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

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

      // generate the code for marshaling in the parameters and transmitting
      // them
      if (this->gen_marshal_and_invoke (node, bt) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for marshal and invoke failed\n"),
                            -1);

        }

      if (!this->void_return_type (bt))
        {
          // now generate the normal successful return statement
          os->indent ();
          if (bt->size_type () == be_decl::VARIABLE
              || bt->base_node_type () == AST_Decl::NT_array)
            {
              *os << "return _tao_safe_retval._retn ();";
            }
          else
            {
              *os << "return _tao_retval;";
            }
        }
    } // end of if (!native)

  *os << be_uidt_nl << "}\n\n";

  return 0;
}

int
be_visitor_operation_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
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
be_visitor_operation_cs::gen_pre_stub_info (be_operation *node)
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
                             "be_compiled_visitor_operation_cs::"
                             "gen_pre_stub_info - "
                             "Exceptionlist generation error\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_operation_cs::gen_marshal_and_invoke (be_operation *node,
                                                 be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  os->indent ();

  // create the GIOP_Invocation and grab the outgoing CDR stream
  switch (node->flags ())
    {
    case AST_Operation::OP_oneway:
      *os << "TAO_GIOP_Oneway_Invocation _tao_call ";
      break;
    default:
      *os << "TAO_GIOP_Twoway_Invocation _tao_call ";
    }

  // Do we have "_set_" or "_get_" prepended?
  size_t ext = this->ctx_->attribute () ? 5 : 0;

  *os << "(" << be_idt << be_idt_nl
      << "istub," << be_nl
      << this->compute_operation_name (node)
      << "," << be_nl
      << ACE_OS::strlen (node->original_local_name ()->get_string ()) + ext
      << "," << be_nl
      << "istub->orb_core ()" << be_uidt_nl
      << ");\n";

  // fish out the interceptor from the ORB
  *os << "\n#if (TAO_HAS_INTERCEPTORS == 1)" << be_nl
      << "TAO_ClientRequestInterceptor_Adapter" << be_idt_nl
      << "_tao_vfr (istub->orb_core ()->orb ()->_get_client_interceptor (ACE_TRY_ENV));\n"
      << be_uidt;
  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }
  os->indent ();
  *os << "PortableInterceptor::Cookies _tao_cookies;" << be_nl
      << "CORBA::NVList_var _tao_interceptor_args;" << be_nl
      << "if (_tao_vfr.valid ())" << be_idt_nl << "{" << be_idt_nl
      << "istub->orb_core ()->orb ()->create_list "
      << "(0, _tao_interceptor_args.inout (), ACE_TRY_ENV);\n";
  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }
  os->decr_indent ();
  *os << "}\n" << be_uidt_nl;

  *os << "ACE_TRY" << be_idt_nl
      << "{\n"
      << "#endif /* TAO_HAS_INTERCEPTORS */\n";

  *os << be_nl
      << "for (;;)" << be_nl
      << "{" << be_idt_nl;

  // *os << "ACE_TRY_ENV.clear ();" << be_nl;
  *os << "_tao_call.start (ACE_TRY_ENV);\n";
  // check if there is an exception
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }

  // Invoke preinvoke interceptor
  *os << be_nl << "TAO_INTERCEPTOR (" << be_idt << be_idt_nl
      << "_tao_vfr.preinvoke (" << be_idt << be_idt_nl
      << "_tao_call.request_id ()," << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    *os << "0";
  else
    *os << "1";

  *os << "," << be_nl << "this," << be_nl
      << this->compute_operation_name (node) << ","
      << be_nl << "_tao_call.service_info ()," << be_nl
      << "_tao_interceptor_args.inout ()," << be_nl
      << "_tao_cookies," << be_nl << "ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl << ");\n" << be_uidt;
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }

  // Prepare the request header
  os->indent ();
  *os << "CORBA::Short flag = ";

  switch (node->flags ())
    {
    case AST_Operation::OP_oneway:
      *os << "_tao_call.sync_scope ();";
      break;
    default:
      *os << "131;" << be_nl;
      *os << "// Tremporary hack until GIOP 1.2 is implemented." << be_nl;
    }

  *os << be_nl
      << "_tao_call.prepare_header (ACE_static_cast (CORBA::Octet, flag), ACE_TRY_ENV);\n";

  // check if there is an exception
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }


  // now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      *os << be_nl
          << "TAO_OutputCDR &_tao_out = _tao_call.out_stream ();"
          << be_nl
          << "if (!(\n" << be_idt << be_idt << be_idt;

      // marshal each in and inout argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for return var in do_static_call failed\n"),
                            -1);
        }
      *os << be_uidt << be_uidt_nl
          << "))" << be_nl;

      // if marshaling fails, raise exception
      if (this->gen_raise_interceptor_exception (bt, "CORBA::MARSHAL",
                                                 "") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_cs::"
                             "gen_marshal_and invoke - "
                             "codegen for return var failed\n"),
                            -1);
        }
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
          *os << "_tao_call.invoke (_tao_" << node->flat_name ()
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
  // check if there is an exception
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << be_nl
      << "if (_invoke_status == TAO_INVOKE_RESTART)" << be_idt_nl
      << "continue;" << be_uidt_nl
      << "if (_invoke_status != TAO_INVOKE_OK)" << be_nl
      << "{" << be_idt_nl;

  if (this->gen_raise_interceptor_exception (bt,
                                             "CORBA::UNKNOWN",
                                             "TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES") == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_cs::"
                         "gen_marshal_and invoke - "
                         "codegen for return var failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}" << be_nl;

  // if we reach here, we are ready to proceed.
  // the code below this is for 2way operations only

  if (!this->void_return_type (bt) ||
      this->has_param_type (node, AST_Argument::dir_INOUT) ||
      this->has_param_type (node, AST_Argument::dir_OUT))

    {
      // Do any post_invoke stuff that might be necessary.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_POST_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_cs::"
                             "gen_marshal_and_invoke - "
                             "codegen for args in post do_static_call\n"),
                            -1);
        }


      // Generate any temporary variables to demarshal the arguments
      ctx = *this->ctx_;
      be_visitor_compiled_args_decl vis1 (new be_visitor_context (ctx));
      if (node->accept (&vis1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_cs::"
                             "gen_pre_stub_info - "
                             "codegen for pre args failed\n"),
                            -1);
        }

      if (!this->void_return_type (bt))
        {
          // Generate any temporary variables to demarshal the return value
          ctx = *this->ctx_;
          be_visitor_context *new_ctx =
            new be_visitor_context (ctx);
          be_visitor_operation_rettype_post_invoke_cs vis2 (new_ctx);
          if (bt->accept (&vis2) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_compiled_visitor_operation_cs::"
                                 "gen_pre_stub_info - "
                                 "codegen rettype [post invoke] failed\n"),
                                -1);
            }
        }

      // check if there was a user exception, else demarshal the
      // return val (if any) and parameters (if any) that came with
      // the response message
      *os << "TAO_InputCDR &_tao_in = _tao_call.inp_stream ();" << be_nl
          << "if (!(\n" << be_idt << be_idt << be_idt;

      if (!this->void_return_type (bt))
        {
          // demarshal the return val
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_INVOKE_CS);
          ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_compiled_visitor_operation_cs::"
                                 "gen_marshal_and_invoke - "
                                 "codegen for return var failed\n"),
                                -1);
            }
        }

      if (this->has_param_type (node, AST_Argument::dir_INOUT) ||
          this->has_param_type (node, AST_Argument::dir_OUT))
        {
          if (!this->void_return_type (bt))
            *os << " &&\n";

          // demarshal each out and inout argument
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
          ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
          visitor = tao_cg->make_visitor (&ctx);
          if (!visitor || (node->accept (visitor) == -1))
            {
              delete visitor;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_compiled_visitor_operation_cs::"
                                 "gen_marshal_and_invoke - "
                                 "codegen for return var failed\n"),
                                -1);
            }
        }

      *os << be_uidt << be_uidt << be_nl
          << "))" << be_nl;
      // if marshaling fails, raise exception
      if (this->gen_raise_interceptor_exception
          (bt, "CORBA::MARSHAL",
           "TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_cs::"
                             "gen_marshal_and invoke - "
                             "codegen for return var failed\n"),
                            -1);
        }
      *os << be_uidt;
    }

  // Invoke postinvoke interceptor
  *os << be_nl << "TAO_INTERCEPTOR (" << be_idt << be_idt_nl
      << "_tao_vfr.postinvoke (" << be_idt << be_idt_nl
      << "_tao_call.request_id ()," << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    *os << "0";
  else
    *os << "1";

  *os << "," << be_nl << "this," << be_nl
      << this->compute_operation_name (node)
      << "," << be_nl << "_tao_call.service_info ()," << be_nl
      << "_tao_interceptor_args.inout ()," << be_nl
      << "_tao_cookies," << be_nl << "ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt << be_uidt_nl << ");\n" << be_uidt;
  if (this->gen_check_interceptor_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);

    }

  os->indent ();
  *os << "break;" << be_nl
      << be_uidt_nl << "}\n";

  // Generate exception occurred interceptor code
  *os << "#if (TAO_HAS_INTERCEPTORS == 1)" << be_nl
      << be_uidt_nl << "}" << be_uidt_nl
      << "ACE_CATCHANY" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_vfr.exception_occurred (" << be_idt << be_idt_nl
      << "_tao_call.request_id ()," << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    *os << "0";
  else
    *os << "1";

  *os << "," << be_nl << "this," << be_nl
      << this->compute_operation_name (node)
      << "," << be_nl // _tao_call.service_info (), "
      << "_tao_cookies," << be_nl << "ACE_TRY_ENV" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_RETHROW;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "ACE_ENDTRY;\n";

  if (this->gen_check_exception (bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_cs::"
                         "gen_marshal_and_invoke - "
                         "codegen for checking exception failed\n"),
                        -1);
    }

  *os << "#endif /* TAO_HAS_INTERCEPTORS */\n";

  return 0;
}

int
be_visitor_operation_cs::gen_raise_exception (be_type *bt,
                                              const char *excep,
                                              const char *completion_status)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->void_return_type (bt))
    {
      *os << "ACE_THROW ("
          << excep << " (" << completion_status << "));\n";
    }
  else
    {
      if (bt->size_type () == be_decl::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "ACE_THROW_RETURN (" << excep
              << " (" << completion_status << "), 0);\n";
        }
      else
        {
          *os << "ACE_THROW_RETURN (" << excep
              << " (" << completion_status << "), _tao_retval);\n";
        }
    }
  return 0;
}

int
be_visitor_operation_cs::gen_raise_interceptor_exception (be_type *bt,
                                                          const char *excep,
                                                          const char *completion_status)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->void_return_type (bt))
    {
      *os << "TAO_INTERCEPTOR_THROW ("
          << excep << " ("
          << completion_status << ")"
          << ");";
    }
  else
    {
      if (bt->size_type () == be_decl::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << excep
              << " (" << completion_status << "), 0);\n";
        }
      else
        {
          *os << "TAO_INTERCEPTOR_THROW_RETURN (" << excep
              << " (" << completion_status << "), _tao_retval);\n";
        }
    }
  return 0;
}

int
be_visitor_operation_cs::gen_check_exception (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // check if there is an exception
  if (this->void_return_type (bt))
    {
      *os << "ACE_CHECK;\n";
      //<< "_tao_environment);\n";
    }
  else
    {
      if (bt->size_type () == be_decl::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "ACE_CHECK_RETURN (0);\n";
        }
      else
        {
          *os << "ACE_CHECK_RETURN  (_tao_retval);\n";
        }
    }

  return 0;
}

int
be_visitor_operation_cs::gen_check_interceptor_exception (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // check if there is an exception
  if (this->void_return_type (bt))
    {
      *os << "TAO_INTERCEPTOR_CHECK;\n";
      //<< "_tao_environment);\n";
    }
  else
    {
      if (bt->size_type () == be_decl::VARIABLE
          || bt->base_node_type () == AST_Decl::NT_array)
        {
          *os << "TAO_INTERCEPTOR_CHECK_RETURN (0);\n";
        }
      else
        {
          *os << "TAO_INTERCEPTOR_CHECK_RETURN  (_tao_retval);\n";
        }
    }

  return 0;
}

const char*
be_visitor_operation_cs::compute_operation_name (be_operation *node)
{
  if (this->operation_name_ == 0)
    {
      size_t len = 3;           // length for two double quotes
                                // and the null termination char.
      if (this->ctx_->attribute ())
        len += 5;               // "Added length for "_set_" or "_get_".

      len += ACE_OS::strlen (node->original_local_name ()->get_string ());

      ACE_NEW_RETURN (this->operation_name_,
                      char [len],
                      0);

      ACE_OS::strcpy (this->operation_name_, "\"");
      if (this->ctx_->attribute ())
        {
          // now check if we are a "get" or "set" operation
          if (node->nmembers () == 1) // set
            ACE_OS::strcat (this->operation_name_, "_set_");
          else
            ACE_OS::strcat (this->operation_name_, "_get_");
        }
      ACE_OS::strcat (this->operation_name_,
                      node->original_local_name ()->get_string ());
      ACE_OS::strcat (this->operation_name_, "\"");
    }
  return this->operation_name_;
}
