//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_servant_operation_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the server skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_operation, ami_handler_servant_operation_cs, "$Id$")


// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

be_visitor_operation_ami_handler_servant_operation_cs::be_visitor_operation_ami_handler_servant_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_servant_operation_cs::~be_visitor_operation_ami_handler_servant_operation_cs (void)
{
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_ami_handler_servant_operation_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_ami_handler_servant_operation_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node for return type

  os = this->ctx_->stream (); // grab the o/p stream
  this->ctx_->node (node); // save the node for future use

  os->indent (); // start with the current indentation level

  // if there is an argument of type "native", return immediately
  if (node->has_native ())
    return 0;

  // retrieve the operation return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

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
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Default implementation
  os->indent ();
  *os << "void " << intf->full_skel_name () << "::";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->local_name () << " ";
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if ((!visitor) || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_thru_poa_collocated_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;
  os->indent ();
  *os << "{" << be_idt_nl;

  *os << "ACE_DEBUG ((LM_DEBUG," << be_idt << be_idt_nl
      <<"\"(%P | %t):" << intf->full_skel_name () << "\\n\"));"
      << be_uidt << be_uidt_nl;

  *os << be_uidt_nl << "}" << be_nl << be_nl;



  // generate the signature of the static skeleton
  os->indent ();
  *os << "void " << intf->full_skel_name () << "::";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->local_name ()
      << "_skel (" << be_idt << be_idt_nl;

  *os << "CORBA::ServerRequest &_tao_server_request, " << be_nl;

  *os << "void *_tao_object_reference, " << be_nl
      << "void * /* context */, " << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt << be_uidt_nl
      << ")" << be_nl;

  // generate the actual code for the skeleton. However, if any of the argument
  // types is "native", we do not generate any skeleton
  // last argument - is always CORBA::Environment
  *os << "{\n" << be_idt;

  // generate all the tables and other pre-skel info
  if (this->gen_pre_skel_info (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "gen_pre_skel_info failed\n"),
                        -1);
    }

  os->indent ();
  // get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl = ("
      << intf->full_skel_name () << " *)_tao_object_reference;\n\n";

  // declare a return type variable
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_RETVAL_DECL_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
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
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  // Demarshal parameters
  if (this->gen_demarshal_params (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }

  // make the upcall
  os->indent ();
  *os << "_tao_impl->" << node->local_name () << " (" << be_idt << "\n";

  if (!this->void_return_type (bt))
  {
    os->indent ();
    *os << "_tao_retval,\n";
  }

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_UPCALL_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

  // end the upcall
  os->indent ();
  *os << be_uidt_nl << ");\n";

  os->indent ();
  *os << "ACE_CHECK;\n"
      << "_tao_server_request.init_reply (ACE_TRY_ENV);\n";

  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_operation_ami_handler_servant_operation_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  os->indent ();
  *os << "{" << bt->tc_name () << ", ";
  switch (node->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "CORBA::ARG_IN, ";
      break;
    case AST_Argument::dir_INOUT:
      *os << "CORBA::ARG_INOUT, ";
      break;
    case AST_Argument::dir_OUT:
      *os << "CORBA::ARG_OUT, ";
      break;
    }
  *os << "0}";

  return 0;
}

int
be_visitor_operation_ami_handler_servant_operation_cs::gen_raise_exception (be_type *,
                                              const char *excep,
                                              const char *completion_status,
                                              const char * /* env */)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  *os << "ACE_THROW ("
      << excep << " (" << completion_status << ") "
      << ");\n";
  return 0;
}

int
be_visitor_operation_ami_handler_servant_operation_cs::gen_check_exception (be_type *, const char * /* env */)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // check if there is an exception
  *os << "ACE_CHECK;\n";
  // << env << ");\n";

  return 0;
}


// *********************************************************************
// Operation visitor for server skeletons using interpretive marshaling
// *********************************************************************

be_interpretive_visitor_operation_ami_handler_servant_operation_cs::
be_interpretive_visitor_operation_ami_handler_servant_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_servant_operation_cs (ctx)
{
}

be_interpretive_visitor_operation_ami_handler_servant_operation_cs::
~be_interpretive_visitor_operation_ami_handler_servant_operation_cs (void)
{
}

int
be_interpretive_visitor_operation_ami_handler_servant_operation_cs::gen_pre_skel_info (be_operation *node,
                                                         be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor_context ctx;

  os->indent ();
  // generate the param_data and call_data tables. We generate these if and
  // only if none of our arguments is of "native" type. Native types cannot be
  // marshaled.
  // native type does not exist. Generate the static tables

  // generate the TAO_Param_Data_Skel table
  *os << "static const TAO_Param_Data_Skel ";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flat_name () <<
    "_paramdata [] = " << be_nl;
  *os << "{\n";
  os->incr_indent ();

      // entry for the return type
  *os << "{" << bt->tc_name () << ", 0, 0}";
  if (node->nmembers () > 0)
    *os << ",\n";

      // generate entries for the param data table for arguments
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "visit scope failed\n"),
                        -1);
    }
  *os << "\n";
  os->decr_indent ();
  *os << "}; // " << node->flat_name () << "_paramdata\n\n";

  // now generate the calldata table
  os->indent ();
  *os << "static const TAO_Call_Data_Skel ";
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flat_name ()
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
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flat_name () << "_paramdata};\n\n";

  return 0;
}

int
be_interpretive_visitor_operation_ami_handler_servant_operation_cs::gen_demarshal_params (be_operation *node,
                                                            be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // setup parameters for demarshaling and demarshal them
  os->indent ();
  *os << "_tao_server_request.demarshal (" << be_idt_nl
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
  *os << node->flat_name () << "_calldata,\n";

  // pass the appropriate return value to the demarshal operation
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DEMARSHAL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "codegen for return var in demarshal failed\n"),
                        -1);
    }
  // insert a comma after the return val if there are arguments
  if (node->argument_count () > 0)
    *os << ",\n";

  // pass each argument to the demarshal operation
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "codegen for argument in demarshal failed\n"),
                        -1);
    }

  // end the demarshal call
  *os << be_uidt_nl;
  *os << ");" << be_nl;

  *os << "ACE_CHECK;\n";

  return 0;
}

int
be_interpretive_visitor_operation_ami_handler_servant_operation_cs::gen_marshal_params (be_operation *node,
                                                          be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // setup parameters for marshaling and marshal them into the
  // outgoing stream
  os->indent ();
  *os << "_tao_server_request.marshal (" << be_idt_nl
      << "ACE_TRY_ENV, " << be_nl
    //      << "_tao_skel_environment, " << be_nl
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
  *os << node->flat_name () << "_calldata,\n";

  // pass the appropriate return value to the marshal operation
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_MARSHAL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "codegen for return var in marshal failed\n"),
                        -1);
    }
  // insert a comma after the return val if there are arguments
  if (node->argument_count () > 0)
    {
      *os << ",\n";
    }

  // pass each argument to the marshal operation
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_servant_operation_cs::"
                         "visit_operation - "
                         "codegen for argument in marshal failed\n"),
                        -1);
    }
  // end the marshal call
  *os << be_uidt_nl;
  *os << ");\n";

  return 0;
}

// *********************************************************************
// Operation visitor for server skeletons using compiled marshaling
// *********************************************************************

be_compiled_visitor_operation_ami_handler_servant_operation_cs::
be_compiled_visitor_operation_ami_handler_servant_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_servant_operation_cs (ctx)
{
}

be_compiled_visitor_operation_ami_handler_servant_operation_cs::
~be_compiled_visitor_operation_ami_handler_servant_operation_cs (void)
{
}

int
be_compiled_visitor_operation_ami_handler_servant_operation_cs::gen_pre_skel_info (be_operation *node,
                                                         be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in
  if (this->has_param_type (node, AST_Argument::dir_INOUT) ||
      this->has_param_type (node, AST_Argument::dir_OUT) ||
      !this->void_return_type (bt))
    {
      // instantiate a TAO_InputCDR variable
      os->indent ();
      *os << "TAO_InputCDR &_tao_in = _tao_server_request.incoming ();\n";
    }

  return 0;
}

int
be_compiled_visitor_operation_ami_handler_servant_operation_cs::gen_demarshal_params (be_operation *node,
                                                        be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in
  if (this->has_param_type (node, AST_Argument::dir_INOUT) ||
      this->has_param_type (node, AST_Argument::dir_OUT) ||
      !this->void_return_type (bt))
    {
      os->indent ();

      // demarshal the inout and out arguments and return values
      *os << "if (!(\n" << be_idt;
  
      // demarshal
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_RETVAL_DEMARSHAL_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_handler_servant_operation_cs::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }

      // Print the && if there are OUT or INOUT arguements in the
      // signature.
      if (this->has_param_type (node, AST_Argument::dir_OUT) ||
          this->has_param_type (node, AST_Argument::dir_INOUT))
        *os << " &&\n";

      // marshal each in and inout argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARG_DEMARSHAL_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_handler_servant_operation_cs::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }
      *os << be_uidt_nl << "))\n" << be_idt;

      // if marshaling fails, raise exception
      if (this->gen_raise_exception (bt, "CORBA::MARSHAL",
                                     "",
                                     "ACE_TRY_ENV") == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ami_handler_servant_operation_cs::"
                             "gen_marshal_and invoke - "
                             "codegen for return var failed\n"),
                            -1);
        }
      *os << be_uidt << "\n";

    };

  return 0;
}

int
be_compiled_visitor_operation_ami_handler_servant_operation_cs::gen_marshal_params (be_operation *node,
                                                      be_type *bt)
{
  // because we do not want to return something.
  return 0;
}
