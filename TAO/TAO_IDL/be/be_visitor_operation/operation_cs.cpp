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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"


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
    }
  else
    {
      // Generate code that retrieves the underlying stub object and then
      // invokes do_static_call on it.
      *os << "STUB_Object *istub = this->_stubobj ();" << be_nl
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

      // if there is an exception, return
      os->indent ();
      *os << "if (_tao_environment.exception ())" << be_idt << "\n";
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
      *os << be_uidt;

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
