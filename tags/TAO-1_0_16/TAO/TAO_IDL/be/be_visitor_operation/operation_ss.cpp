//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ss.cpp
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

ACE_RCSID(be_visitor_operation, operation_ss, "$Id$")


// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

be_visitor_operation_ss::be_visitor_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx),
    operation_name_ (0)
{
}

be_visitor_operation_ss::~be_visitor_operation_ss (void)
{
  delete[] this->operation_name_;
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_ss::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_ss::visit_operation (be_operation *node)
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
                         "(%N:%l) be_visitor_operation_ss::"
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
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Skip the generation of static skeloton if the interface is
  // locality constraint.
  if (idl_global->gen_locality_constraint ())
    return 0;

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
      << "_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &_tao_server_request," << be_nl
      << "void *_tao_object_reference, " << be_nl
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
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_pre_skel_info failed\n"),
                        -1);
    }

  os->indent ();
  // get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl = ("
      << intf->full_skel_name () << " *)_tao_object_reference;\n\n";

  // declare a return type variable
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  // declare variables for arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }

  // Demarshal parameters
  if (this->gen_demarshal_params (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }

  // Fish out the interceptor and do preinvoke
  *os << "#if defined (TAO_HAS_INTERCEPTORS)" << be_nl
      << "TAO_ServerRequestInterceptor_Adapter" << be_idt_nl
      << "_tao_vfr (_tao_server_request.orb ()->_get_server_interceptor (ACE_TRY_ENV));" << be_uidt_nl
      << "ACE_CHECK;" << be_nl
      << "PortableInterceptor::Cookies _tao_cookies;" << be_nl
      << "CORBA::NVList_var _tao_interceptor_args;" << be_nl
      << "CORBA::Object_var _tao_objref;" << be_nl
      << "if (_tao_vfr.valid ())" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_server_request.orb ()->create_list "
      << "(0, _tao_interceptor_args.inout (), ACE_TRY_ENV);"
      << be_nl << "ACE_CHECK;\n" << be_nl
      << "_tao_objref = "
      << "_tao_server_request.objref (ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK;" << be_uidt_nl << "}\n" << be_uidt_nl
      << "ACE_TRY" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_vfr.preinvoke (" << be_idt << be_idt_nl
      << "_tao_server_request.request_id ()," << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    *os << "0";
  else
    *os << "1";

  *os << "," << be_nl << "_tao_objref.in ()," << be_nl
      << this->compute_operation_name (node) << ","
      << be_nl << "_tao_server_request.service_info ()," << be_nl
      << "_tao_interceptor_args.inout ()," << be_nl
      << "_tao_cookies," << be_nl << "ACE_TRY_ENV" << be_uidt_nl
      << ");" << be_uidt_nl
      << "TAO_INTERCEPTOR_CHECK;\n";
  if (node->flags () == AST_Operation::OP_oneway
      && !this->has_param_type (node, AST_Argument::dir_IN))
    {
      *os << "#else" << be_nl
          << "ACE_UNUSED_ARG (_tao_server_request);\n";
    }

  *os << "#endif /* TAO_HAS_INTERCEPTORS */\n\n";

  // do pre upcall processing if any
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_PRE_UPCALL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for pre upcall failed\n"),
                        -1);
    }
  // make the upcall and assign to the return val
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for retval assignment failed\n"),
                        -1);
    }

  // make the upcall
  *os << "_tao_impl->" << node->local_name () << " (" << be_idt << "\n";
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

  // end the upcall
  *os << be_uidt_nl << ");\n" << be_nl
      << "TAO_INTERCEPTOR_CHECK;\n\n";

  // do postinvoke, and check for exception.
  *os << "#if defined (TAO_HAS_INTERCEPTORS)" << be_nl
      << "_tao_vfr.postinvoke (" << be_idt << be_idt_nl
      << "_tao_server_request.request_id ()," << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    *os << "0";
  else
    *os << "1";

  *os << "," << be_nl << "_tao_objref.in ()," << be_nl
      << this->compute_operation_name (node) << ","
      << be_nl << "_tao_server_request.service_info ()," << be_nl
      << "_tao_interceptor_args.inout ()," << be_nl
      << "_tao_cookies," << be_nl << "ACE_TRY_ENV" << be_uidt_nl
      << ");" << be_uidt_nl
      << "TAO_INTERCEPTOR_CHECK;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "ACE_CATCHANY" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_vfr.exception_occurred (" << be_idt << be_idt_nl
      << "_tao_server_request.request_id ()," << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    *os << "0";
  else
    *os << "1";

  *os << "," << be_nl << "_tao_objref.in ()," << be_nl
      << this->compute_operation_name (node) << "," << be_nl
      << "_tao_cookies," << be_nl << "ACE_TRY_ENV" << be_uidt_nl
      << ");" << be_uidt_nl
      << "ACE_RETHROW;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "ACE_ENDTRY;" << be_nl
      << "ACE_CHECK;\n"
      << "#endif /* TAO_HAS_INTERCEPTORS */\n\n";

  // do any post processing for the arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_POST_UPCALL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for args in post upcall failed\n"),
                        -1);
    }

  // check if we are oneway in which case, we are done
  if (node->flags () == AST_Operation::OP_oneway)
    {
      // we are done. Nothing else to do, except closing the function body.
      os->indent ();
      *os << "if (_tao_server_request.response_expected ()" << be_idt << be_idt_nl
          << "&& !_tao_server_request.sync_with_server ())" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_server_request.init_reply (ACE_TRY_ENV);" << be_nl
          << "ACE_CHECK;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}\n\n";
      return 0;
    }

  // marshal outgoing parameters
  if (this->gen_marshal_params (node, bt) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "gen_marshal_params failed\n"),
                        -1);
    }

  // do any post processing for the retval
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_POST_UPCALL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for return type post upcall failed\n"),
                        -1);
    }

  // do any post processing for the arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_POST_MARSHAL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for args in post marshal failed\n"),
                        -1);
    }

  os->decr_indent ();
  *os << "}\n\n";
  return 0;
}

int
be_visitor_operation_ss::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
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
be_visitor_operation_ss::gen_raise_exception (be_type *,
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
be_visitor_operation_ss::gen_check_exception (be_type *, const char * /* env */)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // check if there is an exception
  *os << "ACE_CHECK;\n";
  // << env << ");\n";

  return 0;
}

int
be_visitor_operation_ss::gen_check_interceptor_exception (be_type *, const char * /* env */)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();
  // check if there is an exception
  *os << "TAO_INTERCEPTOR_CHECK;\n";
  // << env << ");\n";

  return 0;
}

const char *
be_visitor_operation_ss::compute_operation_name (be_operation *node)
{
  if (this->operation_name_ == 0)
    {
      size_t len = 3;           // the null termination char.
      if (this->ctx_->attribute ())
        len += 5;               // "Added length for "_set_" or "_get_".

      len += ACE_OS::strlen (node->local_name ()->get_string ());

      ACE_NEW_RETURN (this->operation_name_,
                      char [len],
                      0);

      ACE_OS::strcpy (this->operation_name_, "\"");
      if (this->ctx_->attribute ())
        {
          if (node->nmembers () == 1)
            ACE_OS::strcat (this->operation_name_, "_set_");
          else
            ACE_OS::strcat (this->operation_name_, "_get_");
        }
      ACE_OS::strcat (this->operation_name_,
                      node->local_name ()->get_string ());
      ACE_OS::strcat (this->operation_name_, "\"");
    }
  return this->operation_name_;
}

// *********************************************************************
// Operation visitor for server skeletons using interpretive marshaling
// *********************************************************************

be_interpretive_visitor_operation_ss::
be_interpretive_visitor_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation_ss (ctx)
{
}

be_interpretive_visitor_operation_ss::
~be_interpretive_visitor_operation_ss (void)
{
}

int
be_interpretive_visitor_operation_ss::gen_pre_skel_info (be_operation *node,
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
                         "(%N:%l) be_visitor_operation_ss::"
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
be_interpretive_visitor_operation_ss::gen_demarshal_params (be_operation *node,
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
                         "(%N:%l) be_visitor_operation_ss::"
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
                         "(%N:%l) be_visitor_operation_ss::"
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
be_interpretive_visitor_operation_ss::gen_marshal_params (be_operation *node,
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
                         "(%N:%l) be_visitor_operation_ss::"
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
                         "(%N:%l) be_visitor_operation_ss::"
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

be_compiled_visitor_operation_ss::
be_compiled_visitor_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation_ss (ctx)
{
}

be_compiled_visitor_operation_ss::
~be_compiled_visitor_operation_ss (void)
{
}

int
be_compiled_visitor_operation_ss::gen_pre_skel_info (be_operation *node,
                                                         be_type *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      // instantiate a TAO_InputCDR variable
      os->indent ();
      *os << "TAO_InputCDR &_tao_in = _tao_server_request.incoming ();\n";
    }

  return 0;
}

int
be_compiled_visitor_operation_ss::gen_demarshal_params (be_operation *node,
                                                        be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled in
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      os->indent ();

      // demarshal the in and inout arguments
      *os << "if (!(\n" << be_idt;

      // marshal each in and inout argument
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

      // if marshaling fails, raise exception
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

    };

  return 0;
}

int
be_compiled_visitor_operation_ss::gen_marshal_params (be_operation *node,
                                                      be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // setup parameters for marshaling and marshal them into the
  // outgoing stream
  // the code below this is for 2way operations only

  // We will be here only if we are 2way
  // first initialize a reply message
  os->indent ();
  *os << "_tao_server_request.init_reply (ACE_TRY_ENV);" << be_nl
      << "ACE_CHECK;" << be_nl;

  // We still need the following check because we maybe 2way and yet have no
  // parameters and a void return type
  if (this->void_return_type (bt) &&
      !this->has_param_type (node, AST_Argument::dir_INOUT) &&
      !this->has_param_type (node, AST_Argument::dir_OUT))
    {
      return 0;
    }

  // Create temporary variables for the out and return parameters..
  if (!this->void_return_type (bt))
    {
      ctx = *this->ctx_;
      be_visitor_context *new_ctx =
        new be_visitor_context (ctx);
      be_visitor_operation_compiled_rettype_post_upcall visitor (new_ctx);
      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ss::"
                             "gen_marshal_params - "
                             "codegen for return var [post upcall] failed\n"),
                            -1);
        }
    }

  // Generate any temporary variables to demarshal the arguments
  ctx = *this->ctx_;
  be_visitor_compiled_args_post_upcall vis1 (new be_visitor_context (ctx));
  if (node->accept (&vis1) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_cs::"
                         "gen_pre_stub_info - "
                         "codegen for pre args failed\n"),
                        -1);
    }

  *os << "TAO_OutputCDR &_tao_out = _tao_server_request.outgoing ();" << be_nl;
  *os << "if (!(\n" << be_idt;

  if (!this->void_return_type (bt))
    {
      // demarshal the return val and each inout and out argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_MARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ss::"
                             "gen_marshal_params - "
                             "codegen for return var failed\n"),
                            -1);
        }
    }

  if (this->has_param_type (node, AST_Argument::dir_INOUT) ||
      this->has_param_type (node, AST_Argument::dir_OUT))
    {

      if (!this->void_return_type (bt))
        // we have already printed the return val. SO put a &&
        *os << " &&\n";

      // marshal each in and inout argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_compiled_visitor_operation_ss::"
                             "gen_marshal_params - "
                             "codegen for args failed\n"),
                            -1);
        }
    }

  *os << be_uidt_nl << "))\n" << be_idt;
  // if marshaling fails, raise exception
  if (this->gen_raise_exception (bt, "CORBA::MARSHAL",
                                 "",
                                 "ACE_TRY_ENV") == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_compiled_visitor_operation_ss::"
                         "gen_marshal_params - "
                         "codegen for raising exception failed\n"),
                        -1);
    }
  *os << be_uidt << be_uidt << "\n";

  return 0;
}
