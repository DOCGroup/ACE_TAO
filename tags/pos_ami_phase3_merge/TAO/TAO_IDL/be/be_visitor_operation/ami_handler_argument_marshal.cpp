//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_argument_marshal.cpp
//
// = DESCRIPTION
//    Visitor to pass arguments to the CDR operators. This one helps in
//    generating the && and the , at the right place. This one is for the
//    skeleton side.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_handler_argument_marshal, "$Id$")


// ************************************************************
// operation visitor to handle the passing of arguments to the CDR operators
// ************************************************************

be_compiled_visitor_operation_ami_handler_argument_marshal::
be_compiled_visitor_operation_ami_handler_argument_marshal (be_visitor_context
                                               *ctx)
  : be_visitor_operation_argument (ctx),
    last_arg_printed_ (be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_NONE)
{
}

be_compiled_visitor_operation_ami_handler_argument_marshal::
~be_compiled_visitor_operation_ami_handler_argument_marshal (void)
{
}

int
be_compiled_visitor_operation_ami_handler_argument_marshal::pre_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  be_argument *arg = be_argument::narrow_from_decl (bd);

  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_compiled_visitor_operation_ami_handler_argument_marshal"
                         "::post_process - "
                         "Bad argument node\n"),
                        -1);
    }
  switch (arg->direction ())
    {
    case AST_Argument::dir_IN:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
        {
          // nothing
        }
      else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
        {
          // nothing
        }
      break;
    case AST_Argument::dir_INOUT:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
        {
          if (this->last_arg_printed_ !=
              be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_NONE)
            *os << " &&\n";
        }
      else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
        {
          if (this->last_arg_printed_ !=
              be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_NONE)
            *os << " &&\n";
        }
      break;
    case AST_Argument::dir_OUT:
      if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
        {
          if (this->last_arg_printed_ !=
              be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_NONE)
            *os << " &&\n";
        }
      else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
        {
          if (this->last_arg_printed_ !=
              be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_NONE)
            *os << " &&\n";
        }
      break;
    }

  return 0;
}

int
be_compiled_visitor_operation_ami_handler_argument_marshal::post_process (be_decl *bd)
{
  be_argument *arg = be_argument::narrow_from_decl (bd);

  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_compiled_visitor_operation_ami_handler_argument_marshal"
                         "::post_process - "
                         "Bad argument node\n"),
                        -1);
    }
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          // these arguments don't get printed for the << operator on the stub
          break;
        case AST_Argument::dir_INOUT:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_INOUT;
          break;
        case AST_Argument::dir_OUT:
           // only these arguments get printed
          this->last_arg_printed_ =
            be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_OUT;
         break;
        }
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          // these arguments don't get printed for the >> on the stub
          break;
        case AST_Argument::dir_INOUT:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_INOUT;
          break;
        case AST_Argument::dir_OUT:
          // only these arguments get printed
          this->last_arg_printed_ =
            be_compiled_visitor_operation_ami_handler_argument_marshal::TAO_ARG_OUT;
          break;
        }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) "
                         "be_compiled_visitor_operation_ami_handler_argument_marshal"
                         "::post_process - "
                         "Bad sub state\n"),
                        -1);
    }
  return 0;
}

