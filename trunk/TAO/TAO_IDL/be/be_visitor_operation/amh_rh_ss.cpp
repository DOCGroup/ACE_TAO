//=============================================================================
/**
 *  @file   amh_rh_ss.cpp
 *
 *  $Id$
 *
 *  Creates code for AMH-RH operations.
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
//=============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_visitor_operation.h"

be_visitor_amh_rh_operation_ss::be_visitor_amh_rh_operation_ss (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_amh_rh_operation_ss::~be_visitor_amh_rh_operation_ss (void)
{
}

int
be_visitor_amh_rh_operation_ss::visit_operation (be_operation *node)
{
  // Nothing to be done for oneway operations.
  if (node->flags () == AST_Operation::OP_oneway)
    {
      return 0;
    }

  // Output stream.
  TAO_OutStream *os = this->ctx_->stream ();

  be_interface *intf =
    be_interface::narrow_from_scope (node->defined_in ());
  if (this->ctx_->attribute () != 0)
    intf = be_interface::narrow_from_scope (this->ctx_->attribute()->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_rh_operation_ss::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  char *buf;
  intf->compute_full_name ("TAO_", "", buf);
  ACE_CString response_handler_implementation_name ("POA_");
  response_handler_implementation_name += buf;
  delete[] buf;

  // Step 1 : Generate return type: always void
  *os << be_nl << "void" << be_nl
      << response_handler_implementation_name.c_str () << "::";

  // Check if we are an attribute node in disguise
  if (this->ctx_->attribute ())
    {
      // now check if we are a "get" or "set" operation
      if (node->nmembers () == 1) // set
        *os << "_set_";
      else
        *os << "_get_";
    }

  *os << node->local_name ();

  // Step 2 : Generate the params of the method
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_rh_operation_ss::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_amh_rh_operation_ss::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  int is_an_exception_reply = 0;

  // Find out if the operation is one of the *_excep() operations, the
  // conditions are:
  // 1) The local_name ends in _excep()
  // 2) There is exactly one argument
  // 3) The argument takes an implied valuetype generated from the
  //    original interface
  // 4) The implied valuetype ends in ExceptionHolder
  if (ACE_OS::strstr (node->full_name (), "_excep") != 0)
    {
      if (node->nmembers () == 1)
        {
          UTL_ScopeActiveIterator i (node, UTL_Scope::IK_decls);
          if (!i.is_done ())
            {
              be_argument *argument =
                be_argument::narrow_from_decl (i.item ());
              be_valuetype *vt =
                be_valuetype::narrow_from_decl (argument->field_type ());
              if (vt != 0
                  && vt->original_interface () == intf->original_interface ()
                  && ACE_OS::strstr (vt->full_name (), "ExceptionHolder") != 0)
                {
                  is_an_exception_reply = 1;
                }
            }
        }
    }

  if (is_an_exception_reply)
    {
      *os << "{" << be_idt_nl
          << "ACE_TRY" << be_nl
          << "{" << be_nl
          << "ACE_UNUSED_ARG (holder);" << be_nl
        //          << "  holder->raise_" << node->local_name ()
        //          << "(ACE_ENV_SINGLE_ARG_PARAMETER);" << be_nl
        //          << "  ACE_TRY_CHECK;" << be_nl
          << "}" << be_nl
          << "ACE_CATCH (CORBA::Exception, ex)" << be_nl
          << "{" << be_nl
          << "  this->_tao_rh_send_exception (ex);" << be_nl
          << "}" << be_nl
          << "ACE_ENDTRY;" << be_uidt_nl
          << "}\n" << be_nl;
    }
  else
    {
      // Step 3: Generate actual code for the method
      *os << "{" << be_idt_nl
          << "this->_tao_rh_init_reply ();" << be_nl << be_nl;

      this->marshal_params (node);

      *os << "this->_tao_rh_send_reply ();" << be_uidt_nl
          << "}" << be_nl;
    }

  return 0;

}


int
be_visitor_amh_rh_operation_ss::marshal_params (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_visitor *visitor;
  be_visitor_context ctx;

  // Now make sure that we have some in and inout parameters. Otherwise, there
  // is nothing to be marshaled out.
  if (this->has_param_type (node, AST_Argument::dir_IN) ||
      this->has_param_type (node, AST_Argument::dir_INOUT))
    {
      os->indent ();

      // marshal the in and inout arguments
      *os << "if (!(\n" << be_idt;

      // Marshal each in and inout argument.
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS);
      ctx.sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);
      visitor = tao_cg->make_visitor (&ctx);

      if (!visitor || (node->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_amh_rh_operation_ss::"
                             "gen_demarshal_params - "
                             "codegen for demarshal failed\n"),
                            -1);
        }

      *os << be_uidt_nl << "))\n" << be_idt;

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
