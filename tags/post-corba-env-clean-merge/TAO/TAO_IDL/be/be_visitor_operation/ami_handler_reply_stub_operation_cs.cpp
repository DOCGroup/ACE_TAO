//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_reply_stub_operation_cs.cpp
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

ACE_RCSID(be_visitor_operation, ami_handler_reply_stub_operation_cs, "$Id$")


// ************************************************************
// Operation visitor for client stubs
// ************************************************************

be_visitor_operation_ami_handler_reply_stub_operation_cs::be_visitor_operation_ami_handler_reply_stub_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_reply_stub_operation_cs::~be_visitor_operation_ami_handler_reply_stub_operation_cs (void)
{
}

// processing to be done after every element in the scope is processed
int
be_visitor_operation_ami_handler_reply_stub_operation_cs::post_process (be_decl *bd)
{
  // all we do here is to insert a comma and a newline
  TAO_OutStream *os = this->ctx_->stream ();
  if (!this->last_node (bd))
    *os << ",\n";
  return 0;
}

int
be_visitor_operation_ami_handler_reply_stub_operation_cs::visit_operation (be_operation *node)
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
                         "(%N:%l) be_visitor_ami_handler_reply_stub_operation_cs::"
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
  if (!parent)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "node information not sufficient :-<\n"),
                        -1);
    }

  // Genereate scope name.
  *os << parent->full_name ();

  // Generate the operation name.
  *os << "::";
  // Check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }
  *os << node->local_name () << "_reply_stub (" << be_idt_nl;

  // Generate the argument list.
  *os << "TAO_InputCDR &_tao_in, " << be_nl
      << "Messaging::ReplyHandler_ptr _tao_reply_handler," << be_nl
      << "CORBA::ULong reply_status";

  *os << be_nl
      << "TAO_ENV_ARG_DECL";

  *os << ")" << be_uidt << be_uidt_nl;

  // Generate the actual code for the stub. However, if any of the argument
  // types is "native", we flag a MARSHAL exception.
  // last argument - is always TAO_ENV_ARG_PARAMETER
  *os << "{\n" << be_idt;

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
  *os << parent->full_name () << "_var "
      << "_tao_reply_handler_object =" << be_idt_nl;

  *os << parent->full_name ();
  *os << "::_narrow(_tao_reply_handler TAO_ENV_ARG_PARAMETER);" << be_uidt_nl;

  *os << "ACE_CHECK;" << be_nl << be_nl
      << "// Exception handling" << be_nl
      << "switch (reply_status)" << be_nl
      << "{" << be_idt_nl
      << "case TAO_AMI_REPLY_OK:" << be_nl
      << "{" << be_idt << "\n";

  // declare variables for arguments
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_reply_stub_operation_cs::"
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
                         "(%N:%1) ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "gen_demarshal_params failed\n"),
                        -1);
    }

  *os << "break;" << be_uidt_nl
      << "}" << be_nl
      << "case TAO_AMI_REPLY_USER_EXCEPTION:" << be_nl
      << "case TAO_AMI_REPLY_SYSTEM_EXCEPTION:" << be_nl
      << "{" << be_idt_nl
      << "const ACE_Message_Block* cdr = _tao_in.start ();" << be_nl << be_nl;

  be_interface *original =
    (be_interface::narrow_from_decl (parent))->original_interface ();

  if (!original)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%1) ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "original interface is not set\n"),
                        -1);
    }


  *os << original->compute_local_name ("AMI_", "ExceptionHolder")
      << "_var exception_holder_var;" << be_nl
      << "ACE_NEW (" << be_idt << be_idt_nl
      << "exception_holder_var," << be_nl;

  if (original->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      be_decl *scope =
        be_scope::narrow_from_scope (original->defined_in ())->decl ();

      *os << "OBV_" << scope->name() << "::"
          << "_tao_"
          << original->compute_local_name ("AMI_", "ExceptionHolder");
    }
  else
    {
      *os << "_tao_"
          << original->compute_local_name ("AMI_", "ExceptionHolder");
    }

  *os << " ()" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << "Messaging::ExceptionHolder::_tao_seq_Octet "
      << "_tao_marshaled_exception (" << be_idt << be_idt_nl
      << "cdr->length ()," << be_nl
      << "cdr->length ()," << be_nl
      << "(unsigned char*) cdr->rd_ptr ()," << be_nl
      << "0" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "exception_holder_var->marshaled_exception (_tao_marshaled_exception);"
      << be_nl << be_nl;

  *os << "if (reply_status == TAO_AMI_REPLY_SYSTEM_EXCEPTION)" << be_idt_nl
      << "exception_holder_var->is_system_exception (1);" << be_uidt_nl
      << "else" << be_idt_nl
      << "exception_holder_var->is_system_exception (0);"
      << be_uidt_nl << be_nl
      << "exception_holder_var->byte_order (_tao_in.byte_order ());" << be_nl
      << be_nl
      << "_tao_reply_handler_object->"
      << node->local_name () << "_excep (" << be_idt << be_idt_nl
      << "exception_holder_var";

  if (!be_global->exception_support ())
    {
      *os << " TAO_ENV_ARG_PARAMETER";
    }

  *os << be_uidt_nl << ");" << be_uidt_nl;

  if (!be_global->exception_support ())
    {
      *os << "ACE_CHECK;" << be_nl;
    }

  *os << "break;" << be_uidt_nl
      << "}" << be_nl;

  *os << "case TAO_AMI_REPLY_NOT_OK:" << be_idt_nl
      << "// @@ Michael: Not even the spec mentions this case." << be_nl
      << "//             We have to think about this case." << be_nl
      << "break;" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl;
  *os << "}" << be_nl << be_nl;

  return 0;
}

int
be_visitor_operation_ami_handler_reply_stub_operation_cs::visit_argument (be_argument *node)
{
  // this method is used to generate the ParamData table entry

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt; // argument type

  // retrieve the type for this argument
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_reply_stub_operation_cs::"
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
be_visitor_operation_ami_handler_reply_stub_operation_cs::gen_raise_exception (
    be_type *bt,
    const char *excep,
    const char *completion_status
  )
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  if (this->void_return_type (bt))
    {
      if (be_global->use_raw_throw ())
        *os << "throw ";
      else
        *os << "ACE_THROW (";

      *os << excep << " (" << completion_status << ")";

      if (be_global->use_raw_throw ())
        *os << ";\n";
      else
        *os << ");\n";
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
          ACE_ERROR_RETURN ((
              LM_ERROR,
              "(%N:%l) be_visitor_operation_ami_handler_reply_stub_operation_cs::"
              "gen_raise_exception - "
              "codegen for return var failed\n"
            ),
            -1
          );
        }
      *os << ");\n";
    }
  return 0;
}

int
be_visitor_operation_ami_handler_reply_stub_operation_cs::gen_check_exception (be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  os->indent ();
  // check if there is an exception
  if (this->void_return_type (bt))
    {
      *os << "ACE_CHECK;\n";
    }
  else
    {
      *os << "ACE_CHECK_RETURN (";

      // return the appropriate return value
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_ami_handler_reply_stub_operation_cs::"
                             "gen_check_exception - "
                             "codegen failed\n"),
                            -1);
        }
      *os << ");\n";
    }

  return 0;
}

// Currently we do not support interpretative exception handling for AMI
#if 0


// ************************************************************
// Operation visitor for interpretive client skeletons of the AMI reply handler
// ************************************************************

be_interpretive_visitor_operation_ami_handler_reply_stub_operation_cs::
be_interpretive_visitor_operation_ami_handler_reply_stub_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_reply_stub_operation_cs (ctx)
{
}

be_interpretive_visitor_operation_ami_handler_reply_stub_operation_cs::~be_interpretive_visitor_operation_ami_handler_reply_stub_operation_cs (void)
{
}

// concrete implementation of the template methods

int
be_interpretive_visitor_operation_ami_handler_reply_stub_operation_cs::gen_pre_stub_info (be_operation *node,
                                                                              be_type *bt)
{
  ACE_UNUSED_ARG (node);
  ACE_UNUSED_ARG (bt);

  // @@ Michael: To be done.
  return 0;
}

int
be_interpretive_visitor_operation_ami_handler_reply_stub_operation_cs::
gen_marshal_and_invoke (be_operation*node,
                        be_type *bt)
{
  ACE_UNUSED_ARG (node);
  ACE_UNUSED_ARG (bt);

  // @@ Michael: To be done.
  return 0;
}

#endif /* 0 */

// ************************************************************
// Operation visitor for compiled client skeletons of the AMI reply handler
// ************************************************************

be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs::
be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs (be_visitor_context *ctx)
  : be_visitor_operation_ami_handler_reply_stub_operation_cs (ctx)
{
}

be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs::~be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs (void)
{
}

// concrete implementation of the template methods

int
be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs::gen_pre_stub_info (be_operation *node,
                                                                          be_type *bt)
{
  ACE_UNUSED_ARG (bt);
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
                             "be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs::"
                             "gen_pre_stub_info - "
                             "Exceptionlist generation error\n"),
                            -1);
        }
      delete visitor;
    }

  return 0;
}

int
be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs::
  gen_marshal_and_invoke (be_operation *node,
                          be_type *bt)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor = 0;
  be_visitor_context ctx;

  ACE_UNUSED_ARG (bt);

  os->indent ();

  *os << "// Demarshall all the arguments." << be_nl;
  if (this->has_param_type (node, AST_Argument::dir_IN))
    {
      *os << "if (!(\n" << be_idt << be_idt << be_idt;

      // demarshal each in and inout argument
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l)  be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs::"
                             "gen_marshal_params - "
                             "codegen for args failed\n"),
                            -1);
        }
      delete visitor;

      *os << be_uidt << be_uidt_nl
          << " ))" << be_nl;
      if (be_global->use_raw_throw ())
        *os << "throw CORBA::MARSHAL ();" << be_uidt_nl << be_nl;
      else
        *os << "ACE_THROW (CORBA::MARSHAL ());" << be_uidt_nl << be_nl;
    }


  // Invoke the callback method
  *os << "// Invoke the call back method." << be_nl
      << "_tao_reply_handler_object->";

  // check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "set_";
      else
        *os << "get_";
    }

  *os << node->local_name () << " (" << be_idt << be_idt_nl;


  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS);
  // generate the argument list containing the inout and inout arguments
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) ami_handler_reply_stub_operation_cs::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;

//  os->indent ();

  *os << be_uidt_nl << ");" << be_uidt_nl;

  *os << "ACE_CHECK;" << be_nl;

  return 0;
}
