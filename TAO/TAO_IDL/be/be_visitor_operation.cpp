//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_operation.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Operation
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

// ******************************************************
// primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_ch::be_visitor_operation_ch (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_ch::~be_visitor_operation_ch (void)
{
}

int
be_visitor_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // grab the right visitor to generate the return type
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_cs::be_visitor_operation_cs (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_cs::~be_visitor_operation_cs (void)
{
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_cs::post_process ()
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
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

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad visitor for return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
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
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
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
  *os << "{\n";
  os->incr_indent (0);

  // Generate the param_data and call_data tables. We generate these if and
  // only if none of our arguments is of "native" type. Native types cannot be
  // marshaled. Hence, stubs for such operations will generate MARSHAL
  // exceptions. As a result it is pointless generating these tables
  if (!node->has_native ())
    {
      // native type does not exist. Generate the static tables

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
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "visit scope failed\n"),
                            -1);
        }
      *os << "\n";
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
                                 "be_visitor_operation_cs::visit_operation - "
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
    } // end of if !(native)

  // Declare a return type
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

  os->indent ();
  if (node->has_native ()) // native exists => no stub
    {
      *os << "_tao_environment.exception (new CORBA::MARSHAL "
	  << "(CORBA::COMPLETED_NO));" << be_nl;
      *os << "return _tao_retval;\n";
    }
  else
    {
      // Generate code that retrieves the underlying stub object and then
      // invokes do_static_call on it.
      *os << "STUB_Object *istub = this->stubobj (_tao_environment);" << be_nl
          << "if (istub)" << be_nl
          << "{\n";
      os->incr_indent (0);

      // do any pre do_static_call processing with return type. This includes
      // allocating memory, initialization.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_PRE_DOCALL_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre do_static_call failed\n"),
                            -1);
        }


      // do any pre do_static_call stuff with arguments
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_PRE_DOCALL_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for argument pre do_static_call failed\n"),
                            -1);
        }

      // call do_static_call with appropriate number of arguments
      os->indent ();
      *os << "istub->do_static_call (" << be_idt_nl
          << "_tao_environment, " << be_nl
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
      *os << node->flatname () << "_calldata,\n";

      // pass the appropriate return value to docall
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DOCALL_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for return var in do_static_call failed\n"),
                            -1);
        }
      // insert a comma after the return val if there are arguments
      if (node->argument_count () > 0)
        {
          *os << ",\n";
        }

      // pass each argument to do_static_call
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DOCALL_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for return var in do_static_call failed\n"),
                            -1);
        }

      // end the do_static_call
      *os << be_uidt_nl;
      *os << ");\n";

      // do any post processing for the retval
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_POST_DOCALL_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for return type post do_static_call failed\n"),
                            -1);
        }

      // do any post processing for the arguments
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_POST_DOCALL_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for args in post do_static_call failed\n"),
                            -1);
        }

    } // end of if (!native)

  os->decr_indent ();
  *os << "} // end of if (istub)\n";
  // return the appropriate return value
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for return var failed\n"),
                        -1);
    }
  os->decr_indent ();
  *os << "}\n\n";
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

// ************************************************************
// Operation visitor for server header
// ************************************************************

be_visitor_operation_sh::be_visitor_operation_sh (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_sh::~be_visitor_operation_sh (void)
{
}

int
be_visitor_operation_sh::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // grab the right visitor to generate the return type
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_SH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  // generate the corresponding static skeleton method for this operation only
  // if there was no "native" type
  if (!node->has_native ())
    {
      os->indent ();
      *os << "static void ";
      // check if we are an attribute node in disguise
      if (this->ctx_->attribute ())
        {
          // now check if we are a "get" or "set" operation
          if (node->nmembers () == 1) // set
            *os << "_set_";
          else
            *os << "_get_";
        }
      *os << node->local_name () <<
        "_skel (" << be_idt << be_idt_nl
          << "CORBA::ServerRequest &_tao_req, " << be_nl
          << "void *_tao_obj, " << be_nl
          << "void *_tao_context, " << be_nl
          << "CORBA::Environment &_tao_env"
          << be_uidt_nl << be_uidt << ");\n\n";
    }

  return 0;
}

// ************************************************************
// Operation visitor for server skeletons
// ************************************************************

be_visitor_operation_ss::be_visitor_operation_ss (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_ss::~be_visitor_operation_ss (void)
{
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_ss::post_process ()
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
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
      << "CORBA::ServerRequest &_tao_server_request, " << be_nl
      << "void *_tao_object_reference, " << be_nl
      << "void * /* context */, " << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt << be_uidt_nl
      << ")" << be_nl;

  // generate the actual code for the skeleton. However, if any of the argument
  // types is "native", we do not generate any skeleton
  // last argument - is always CORBA::Environment
  *os << "{" << be_idt_nl;

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
  *os << node->flatname () <<
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
  *os << "}; // " << node->flatname () << "_paramdata\n\n";

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
  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->flatname () << "_paramdata};\n\n";

  os->indent ();
  // declare an environment variable for user raised exceptions
  *os << "CORBA::Environment _tao_skel_environment;" << be_nl;
  // get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl = ("
      << intf->full_skel_name () << " *)_tao_object_reference;\n";

  // declare a return type variable
  be_visitor_context ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (bt->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
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
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for return var decl failed\n"),
                        -1);
    }


  // setup parameters for demarshaling and demarshal them
  os->indent ();
  *os << "_tao_server_request.demarshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
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
  *os << node->flatname () << "_calldata,\n";

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
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument in demarshal failed\n"),
                        -1);
    }

  // end the demarshal call
  *os << be_uidt_nl;
  *os << ");\n";

  // check for exception
  os->indent ();
  *os << "if (_tao_environment.exception ()) return;\n";

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
                         "codegen for making upcall failed\n"),
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
  // last argument is the environment
  if (node->argument_count () > 0)
    *os << ",\n";
  os->indent ();
  *os << "_tao_skel_environment";
  // end the upcall
  *os << be_uidt_nl;
  *os << ");\n";

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
      os->decr_indent ();
      *os << "}\n\n";
      return 0;
    }

  // setup parameters for marshaling and marshal them into the
  // outgoing stream
  os->indent ();
  *os << "_tao_server_request.marshal (" << be_idt_nl
      << "_tao_environment, " << be_nl
      << "_tao_skel_environment, " << be_nl
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
  *os << node->flatname () << "_calldata,\n";

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
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument in marshal failed\n"),
                        -1);
    }
  // end the marshal call
  *os << be_uidt_nl;
  *os << ");\n";

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

// *************************************************************************
//  be_visitor_operation_collocated_sh --
//  This visitor generates code for the collocated operation signature in a
//  server header file
// *************************************************************************

be_visitor_operation_collocated_sh::be_visitor_operation_collocated_sh
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_collocated_sh::~be_visitor_operation_collocated_sh (void)
{
}

int be_visitor_operation_collocated_sh::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // grab the right visitor to generate the return type
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}

// *************************************************************************
//  be_visitor_operation_collocated_ss --
//  This visitor generates code for the collocated operation signature in a
//  server skeletons file
// *************************************************************************

be_visitor_operation_collocated_ss::be_visitor_operation_collocated_ss
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_collocated_ss::~be_visitor_operation_collocated_ss (void)
{
}

int be_visitor_operation_collocated_ss::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

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
                         "(%N:%l) be_visitor_operation_collocated_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_collocated_ss::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // STEP 2: generate the return type mapping (same as in the header file)
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_collocated_ss::"
                         "visit_operation - "
                         "Bad visitor for return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_collocated_ss::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  *os << " " << intf->full_coll_name () << "::"
      << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  *os << "{" << be_idt << "\n";

  os->indent ();
  if (bt->node_type () != AST_Decl::NT_pre_defined
      || be_predefined_type::narrow_from_decl (bt)->pt () != AST_PredefinedType::PT_void)
    {
      *os << "return ";
    }

  *os << "this->servant_->" << node->local_name () << " (" << be_idt << "\n";

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
  // last argument is the environment
  if (node->argument_count () > 0)
    *os << ",\n";
  os->indent ();
  *os << "_tao_environment";
  // end the upcall
  *os << be_uidt_nl;
  *os << ");" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}

// ****************************************************************************
// Operation visitor for return types. This generates the mapping for a return
// type in an operation signature
// ****************************************************************************

be_visitor_operation_rettype::be_visitor_operation_rettype (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype::~be_visitor_operation_rettype (void)
{
}

int
be_visitor_operation_rettype::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_slice") << " *";
  else
    *os << bt->name () << "_slice *";
  return 0;
}

int
be_visitor_operation_rettype::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();
  return 0;
}

int
be_visitor_operation_rettype::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << bt->name () << "_ptr";
  return 0;
}

int
be_visitor_operation_rettype::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope (), "_ptr");
  else
    *os << bt->name () << "_ptr";
  return 0;
}

int
be_visitor_operation_rettype::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ()) << " *";
  else
    *os << bt->name () << " *";
  return 0;
}

int
be_visitor_operation_rettype::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
        *os << bt->nested_type_name (this->ctx_->scope (), "_ptr");
      else
        *os << bt->name () << "_ptr";
      break;
    case AST_PredefinedType::PT_any:
      if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
        *os << bt->nested_type_name (this->ctx_->scope ()) << " *";
      else
        *os << bt->name () << " *";
      break;
    default:
      if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
        *os << bt->nested_type_name (this->ctx_->scope ());
      else
        *os << bt->name ();
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype::visit_sequence (be_sequence *node)
{
  // we should never directly be here because anonymous sequence return types
  // are not allowed
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ()) << " *";
  else
    *os << bt->name () << " *";
  return 0;
}

int
be_visitor_operation_rettype::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "char *";
  return 0;
}

int
be_visitor_operation_rettype::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();

  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << " *";
  return 0;
}

int
be_visitor_operation_rettype::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (this->ctx_->state () == TAO_CodeGen::TAO_OPERATION_RETTYPE_CH)
    *os << bt->nested_type_name (this->ctx_->scope ());
  else
    *os << bt->name ();

  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << " *";
  return 0;
}

// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_arglist::be_visitor_operation_arglist (be_visitor_context
                                                            *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_arglist::~be_visitor_operation_arglist (void)
{
}

int
be_visitor_operation_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->incr_indent (0); // these are for pretty printing
  os->incr_indent (0);
  *os << " (\n";

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // last argument - is always CORBA::Environment
  os->indent ();
  *os << "CORBA::Environment &_tao_environment\n";
  os->decr_indent ();
  *os << " )";
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
      *os << ";\n";
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
      // each method is pure virtual in the server header
      *os << " = 0;\n";
      break;
    default:
      *os << "\n";
    }

  os->decr_indent (0);
  return 0;
}

int
be_visitor_operation_arglist::visit_argument (be_argument *node)
{
  // get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // first grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();
  if (!op)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad operation\n"),
                        -1);
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_OTHERS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_arglist::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
    }

  // grab a visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"

                         "visit_argument - "
                         "codegen for arglist failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}

// ************************************************************
//    be_visitor_operation_rettype_vardecl_cs
//    This visitor generates code for variable declaration and initialization
//    of the return type.
// ************************************************************

be_visitor_operation_rettype_vardecl_cs::
be_visitor_operation_rettype_vardecl_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_vardecl_cs::
~be_visitor_operation_rettype_vardecl_cs (void)
{
}

int
be_visitor_operation_rettype_vardecl_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << "_slice *_tao_retval = 0;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << " _tao_retval = (" << bt->name () << ")0;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << "_ptr _tao_retval = " << bt->name () << "::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << "_ptr _tao_retval = " << bt->name () << "::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      os->indent ();
      *os << bt->name () << "_ptr _tao_retval = 0;\n";
      break;
    case AST_PredefinedType::PT_any:
      os->indent ();
      *os << bt->name () << " *_tao_retval = 0;\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      os->indent ();
      *os << bt->name () << " _tao_retval = 0;\n";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_sequence (be_sequence *node)
{
  // we should never directly be here because anonymous sequence return types
  // are not allowed
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << " *_tao_retval = 0;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "char *_tao_retval = 0;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << bt->name () << " *_tao_retval = 0;\n";
  else
    {
      *os << bt->name () << " _tao_retval;" << be_nl;
      *os << "ACE_OS::memset (&_tao_retval, 0, sizeof (" << bt->name () << "));\n";
    }
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_vardecl_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_vardecl_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << bt->name () << " *_tao_retval = 0;\n";
  else
    {
      *os << bt->name () << " _tao_retval;" << be_nl;
      *os << "ACE_OS::memset (&_tao_retval, 0, sizeof (" << bt->name () << "));\n";
    }
  return 0;
}

// *****************************************************************************
//    be_visitor_operation_rettype_pre_docall_cs
//
//    This visitor generates code that passes the return type variable to the
//    do_static_call method
// *****************************************************************************

be_visitor_operation_rettype_pre_docall_cs::
be_visitor_operation_rettype_pre_docall_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_pre_docall_cs::
~be_visitor_operation_rettype_pre_docall_cs (void)
{
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "CORBA::Object_ptr _tao_base_retval = CORBA::Object::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "CORBA::Object_ptr _tao_base_retval = CORBA::Object::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_any:
      os->indent ();
      *os << "ACE_NEW_RETURN (_tao_retval, CORBA::Any, _tao_retval);\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "ACE_NEW_RETURN (_tao_retval, " << node->name () << ", _tao_retval);\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    {
      os->indent ();
      *os << "ACE_NEW_RETURN (_tao_retval, " << node->name () << ", _tao_retval);\n";
    }
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_pre_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    {
      os->indent ();
      *os << "ACE_NEW_RETURN (_tao_retval, " << node->name () << ", _tao_retval);\n";
    }
  return 0;
}

// *****************************************************************************
//    be_visitor_operation_rettype_docall_cs
//
//    This visitor generates code that passes the return type variable to the
//    do_static_call method
// *****************************************************************************

be_visitor_operation_rettype_docall_cs::be_visitor_operation_rettype_docall_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_docall_cs::
~be_visitor_operation_rettype_docall_cs (void)
{
}

int
be_visitor_operation_rettype_docall_cs::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "&_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "&_tao_base_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "&_tao_base_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      *os << "0";
      break;
    case AST_PredefinedType::PT_any:
      *os << "_tao_retval";
      break;
    default:
      *os << "&_tao_retval";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "&_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // check if the struct is variable
  if (node->size_type () == be_type::VARIABLE)
    *os << "_tao_retval";
  else
    *os << "&_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_docall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    *os << "_tao_retval";
  else
    *os << "&_tao_retval";
  return 0;
}

// *******************************************************************************
//    be_visitor_operation_rettype_post_docall_cs
//
//    This visitor generates code that passes the return type variable to the
//    do_static_call method
// ********************************************************************************

be_visitor_operation_rettype_post_docall_cs::
be_visitor_operation_rettype_post_docall_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_post_docall_cs::
~be_visitor_operation_rettype_post_docall_cs (void)
{
}

int
be_visitor_operation_rettype_post_docall_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = " << node->name ()
      << "::_narrow (_tao_base_retval, _tao_environment);" << be_nl;
  *os << "CORBA::release (_tao_base_retval);\n";
  return 0;
}

int
be_visitor_operation_rettype_post_docall_cs::
visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = " << node->name ()
      << "::_narrow (_tao_base_retval, _tao_environment);" << be_nl;
  *os << "CORBA::release (_tao_base_retval);\n";
  return 0;
}

// ************************************************************
//    be_visitor_operation_rettype_return_cs
//
//    code to generate the return statement of the stub.
// ************************************************************

be_visitor_operation_rettype_return_cs::be_visitor_operation_rettype_return_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_return_cs::~be_visitor_operation_rettype_return_cs (void)
{
}

int
be_visitor_operation_rettype_return_cs::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // we must narrow it
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // we must narrow it
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      *os << "return;\n";
      break;
    default:
      *os << "return _tao_retval;\n";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_structure (be_structure *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "return _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_return_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_union (be_union *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "return _tao_retval;\n";
  return 0;
}

// ********************************************************************************
//    be_visitor_operation_rettype_vardecl_ss
//    This visitor generates code for variable declaration and initialization
//    of the return type.
// ********************************************************************************

be_visitor_operation_rettype_vardecl_ss::be_visitor_operation_rettype_vardecl_ss
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_vardecl_ss::
~be_visitor_operation_rettype_vardecl_ss (void)
{
}

int
be_visitor_operation_rettype_vardecl_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << "_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << " _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // due to the virtual inheritance and the fact that we will be passing the
  // address of the objref to the marshaling routine, we use the base
  // CORBA::Object_ptr as the type for the return value even though the actual
  // return type may be some derived class
  *os << "CORBA::Object_var _tao_retval = CORBA::Object::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::
visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  // due to the virtual inheritance and the fact that we will be passing the
  // address of the objref to the marshaling routine, we use the base
  // CORBA::Object_ptr as the type for the return value even though the actual
  // return type may be some derived class
  *os << "CORBA::Object_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      os->indent ();
      *os << bt->name () << "_var _tao_retval;\n";
      break;
    case AST_PredefinedType::PT_any:
      os->indent ();
      *os << bt->name () << "_var _tao_retval;\n";
      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      os->indent ();
      *os << bt->name () << " _tao_retval = 0;\n";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_sequence (be_sequence *node)
{
  // we should never directly be here because anonymous sequence return types
  // are not allowed
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () << "_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "CORBA::String_var _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << bt->name () << "_var _tao_retval;\n";
  else
    *os << bt->name () << " _tao_retval;\n";
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_vardecl_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_vardecl_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  be_type *bt; // return type

  if (this->ctx_->alias ()) // a typedefed return type
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  // based on whether we are variable or not, we return a pointer or the
  // aggregate type
  if (node->size_type () == be_decl::VARIABLE)
    *os << bt->name () << "_var _tao_retval;\n";
  else
    *os << bt->name () << " _tao_retval;\n";
  return 0;
}

// ****************************************************************************
// Visitor that passes the return value to the marshal/demarshal routine
// ****************************************************************************

be_visitor_operation_rettype_marshal_ss::
be_visitor_operation_rettype_marshal_ss (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_marshal_ss::
~be_visitor_operation_rettype_marshal_ss (void)
{
}

int
be_visitor_operation_rettype_marshal_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval.ptr ()";
  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "&_tao_retval";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval.ptr ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::
visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval.ptr ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      os->indent ();
      *os << "0";
      break;
    case AST_PredefinedType::PT_pseudo:
      os->indent ();
      *os << "_tao_retval.in ()";
      break;
    case AST_PredefinedType::PT_any:
      os->indent ();
      *os << "&_tao_retval.in ()";
      break;
    default:
      os->indent ();
      *os << "&_tao_retval";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval.ptr ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval.in ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  if (node->size_type () == be_type::VARIABLE)
    *os << "_tao_retval.ptr ()";
  else
    *os << "&_tao_retval";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_marshal_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  if (node->size_type () == be_type::VARIABLE)
    *os << "_tao_retval.ptr ()";
  else
    *os << "&_tao_retval";

  return 0;
}

// ****************************************************************************
// visitor for assignment to a return value variable from the upcall
// ****************************************************************************

be_visitor_operation_rettype_assign_ss::
be_visitor_operation_rettype_assign_ss (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_assign_ss::
~be_visitor_operation_rettype_assign_ss (void)
{
}

int
be_visitor_operation_rettype_assign_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";
  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::
visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      break;
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_any:
    default:
      *os << "_tao_retval = ";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_structure (be_structure *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_assign_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_assign_ss::visit_union (be_union *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "_tao_retval = ";

  return 0;
}

// ****************************************************************************
// visitor to do any post processing for return type after an upcall
// ****************************************************************************

be_visitor_operation_rettype_post_upcall_ss::be_visitor_operation_rettype_post_upcall_ss (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_post_upcall_ss::~be_visitor_operation_rettype_post_upcall_ss (void)
{
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_array (be_array *)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_enum (be_enum *node)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_interface (be_interface *node)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_interface_fwd (be_interface_fwd *node)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_predefined_type (be_predefined_type *node)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_sequence (be_sequence *node)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_string (be_string * /* node*/)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_structure (be_structure *node)
{
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_post_upcall_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_post_upcall_ss::visit_union (be_union *node)
{
  return 0;
}

// ************************************************************
// generic operation visitor to handle the pre/post do_static_call/upcall stuff with
// arguments
// ************************************************************

be_visitor_operation_argument::be_visitor_operation_argument (be_visitor_context
                                                            *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_argument::~be_visitor_operation_argument (void)
{
}

int
be_visitor_operation_argument::post_process (void)
{
  TAO_OutStream *os = this->ctx_->stream ();
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARG_DOCALL_CS:
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
      *os << ",\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_operation_argument::visit_operation (be_operation *node)
{
  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_argument::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation_argument::visit_argument (be_argument *node)
{
  // get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // first grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();
  if (!op)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad operation\n"),
                        -1);
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_DOCALL_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DOCALL_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_DOCALL_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_DEMARSHAL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_MARSHAL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_UPCALL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS);
      break;
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_MARSHAL_SS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_POST_MARSHAL_SS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_argument::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
    }

  // grab a visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_argument::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_argument::"

                         "visit_argument - "
                         "codegen for argument failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}


// ****************************************************************************
// visitor to generate the exception list for operations
// ****************************************************************************

be_visitor_operation_exceptlist_cs::be_visitor_operation_exceptlist_cs (be_visitor_context
                                                            *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_exceptlist_cs::~be_visitor_operation_exceptlist_cs (void)
{
}

int
be_visitor_operation_exceptlist_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream
  // don't do anything if the exception list is empty
  if (node->exceptions ())
    {
      os->indent ();
#if 0
      *os << "static CORBA::TypeCode_ptr " << "_tao_" << node->flatname ()
          << "_exceptlist [] = {" << be_idt_nl;
#endif
      *os << "static TAO_Exception_Data " << "_tao_" << node->flatname ()
          << "_exceptiondata [] = " << be_nl;
      *os << "{" << be_idt_nl;
      // initialize an iterator to iterate thru the exception list
      UTL_ExceptlistActiveIterator *ei;
      ACE_NEW_RETURN (ei,
                      UTL_ExceptlistActiveIterator (node->exceptions ()),
                      -1);
      // continue until each element is visited
      while (!ei->is_done ())
        {
          be_exception *excp = be_exception::narrow_from_decl (ei->item ());

          if (excp == 0)
            {
              delete ei;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_exceptlist_cs"
                                 "visit_operation - "
                                 "codegen for scope failed\n"), -1);

            }
          *os << "{";
          *os << excp->tc_name ();
          *os << ", ";
          *os << excp->name () << "::_alloc}";
          ei->next ();
          if (!ei->is_done ())
            {
              *os << ",\n";
              os->indent ();
            }
          // except the last one is processed?

        } // end of while loop
      delete ei;
      *os << be_uidt_nl << "};\n\n";
    } // end of if
  return 0;
}

// ************************************************************
// Operation visitor for server header for TIE class operations
// ************************************************************

be_visitor_operation_tie_sh::be_visitor_operation_tie_sh (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_sh::~be_visitor_operation_tie_sh (void)
{
}

int
be_visitor_operation_tie_sh::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // grab the right visitor to generate the return type
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  // we use the _CH state here because the _SH state produces pure virtual
  // methods.
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}

// ************************************************************
// Operation visitor for server inline for TIE class operations
// ************************************************************

be_visitor_operation_tie_si::be_visitor_operation_tie_si
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_si::~be_visitor_operation_tie_si (void)
{
}

int be_visitor_operation_tie_si::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

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
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  os->indent ();
  *os << "template <class T> ACE_INLINE\n";

  // generate the return type mapping (same as in the header file)
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "Bad visitor for return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  *os << " " << intf->full_skel_name () << "_tie<T>::"
      << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad visitor for argument list\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  *os << "{" << be_idt << "\n";

  os->indent ();
  if (bt->node_type () != AST_Decl::NT_pre_defined
      || be_predefined_type::narrow_from_decl (bt)->pt () != AST_PredefinedType::PT_void)
    {
      *os << "return ";
    }

  *os << "this->ptr_->" << node->local_name () << " (" << be_idt << "\n";

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
  // last argument is the environment
  if (node->argument_count () > 0)
    *os << ",\n";
  os->indent ();
  *os << "_tao_environment";
  // end the upcall
  *os << be_uidt_nl;
  *os << ");" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}
