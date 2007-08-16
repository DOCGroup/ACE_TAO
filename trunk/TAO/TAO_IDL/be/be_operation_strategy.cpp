// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_operation_strategy.cpp
//
// = DESCRIPTION
//    A strategy to cover the differences between various variants
//    of an operations, e.g. AMI sendc_, AMI raise_
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#include "be_operation_strategy.h"
#include "be_operation.h"

ACE_RCSID (be,
           be_operation_strategy,
           "$Id$")


be_operation_strategy::be_operation_strategy (be_operation *node,
                                              Strategy_Kind strategy_type)
  : node_ (node),
    strategy_type_ (strategy_type)
{
}

be_operation_strategy::~be_operation_strategy (void)
{
}

int
be_operation_strategy::strategy_type (void)
{
  return strategy_type_;
}

int
be_operation_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE /* current_state */
  )
{
  return 0;
}

be_operation *
be_operation_strategy::marshaling (void)
{
  return 0;
}

be_operation *
be_operation_strategy::arguments (void)
{
  return 0;
}

be_operation_strategy *
be_operation_strategy::copy (void)
{
  return 0;
}

void
be_operation_strategy::destroy (void)
{
}

// ****************************************************************
// AMI sendc_ operation strategy

be_operation_default_strategy::be_operation_default_strategy (
    be_operation *node
  )
  : be_operation_strategy (node,
                           DEFAULT)
{
}

be_operation_default_strategy::~be_operation_default_strategy (void)
{
}

TAO_CodeGen::CG_STATE
be_operation_default_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int /* is_extra_state */
  )
{
  return current_state;
}

be_operation_strategy *
be_operation_default_strategy::copy (void)
{
  be_operation_default_strategy *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_operation_default_strategy (this->node_),
                  0);
  retval->strategy_type_ = this->strategy_type_;
  return retval;
}

// ****************************************************************
// AMI sendc_ operation strategy

be_operation_ami_sendc_strategy::be_operation_ami_sendc_strategy (
    be_operation *node,
    be_operation *marshaling,
    be_operation *arguments
  )
  : be_operation_strategy (node,
                           AMI_SENDC),
    marshaling_ (marshaling),
    arguments_ (arguments),
    owns_operations_ (true)
{
}

be_operation_ami_sendc_strategy::~be_operation_ami_sendc_strategy (void)
{
}

TAO_CodeGen::CG_STATE
be_operation_ami_sendc_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int is_extra_state
  )
{
  if (is_extra_state)
    {
      switch (current_state)
        {
        case TAO_CodeGen::TAO_OPERATION_CH:
          return TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CH;
        case TAO_CodeGen::TAO_OPERATION_CS:
          return TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CS;
        default:
          return current_state;
        }
    }
  else
    {
      return current_state;
    }
}

int
be_operation_ami_sendc_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE current_state
  )
{
  if (current_state == TAO_CodeGen::TAO_OPERATION_CH
      || current_state == TAO_CodeGen::TAO_OPERATION_CS)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

be_operation*
be_operation_ami_sendc_strategy::marshaling (void)
{
  return marshaling_;
}


be_operation*
be_operation_ami_sendc_strategy::arguments (void)
{
  return arguments_;
}

be_operation_strategy *
be_operation_ami_sendc_strategy::copy (void)
{
  be_operation_ami_sendc_strategy *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_operation_ami_sendc_strategy (this->node_,
                                                   this->marshaling_,
                                                   this->arguments_),
                  0);
  retval->strategy_type_ = this->strategy_type_;
  retval->owns_operations_ = false;
  return retval;
}

void
be_operation_ami_sendc_strategy::destroy (void)
{
  if (this->owns_operations_)
    {
      this->marshaling_->destroy ();
      delete this->marshaling_;
      this->marshaling_ = 0;

      this->arguments_->destroy ();
      delete this->arguments_;
      this->arguments_ = 0;
    }

  this->be_operation_strategy::destroy ();
}

// ****************************************************************
// AMI ExceptionHolder raise_ operation strategy

be_operation_ami_exception_holder_raise_strategy
::be_operation_ami_exception_holder_raise_strategy (be_operation *node)
  : be_operation_strategy (node,
                           AMI_EXCEPTION_HOLDER_RAISE)
{
}

be_operation_ami_exception_holder_raise_strategy
::~be_operation_ami_exception_holder_raise_strategy (void)
{
}


TAO_CodeGen::CG_STATE
be_operation_ami_exception_holder_raise_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int is_extra_state
  )
{
  if (!is_extra_state)
    {
      switch (current_state)
        {
        case TAO_CodeGen::TAO_OPERATION_CS:
          return TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS;
        default:
          return current_state;
        }
    }
  else
    {
      return current_state;
    }
}

be_operation_strategy *
be_operation_ami_exception_holder_raise_strategy::copy (void)
{
  be_operation_ami_exception_holder_raise_strategy *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_operation_ami_exception_holder_raise_strategy (
                      this->node_
                    ),
                  0);
  retval->strategy_type_ = this->strategy_type_;
  return retval;
}

// ****************************************************************
// AMI Reply Handler reply stub operation strategy

be_operation_ami_handler_reply_stub_strategy
::be_operation_ami_handler_reply_stub_strategy (be_operation *node)
  : be_operation_strategy (node,
                           AMI_EXCEPTION_HOLDER_RAISE)
{
}

be_operation_ami_handler_reply_stub_strategy
::~be_operation_ami_handler_reply_stub_strategy (void)
{
}

TAO_CodeGen::CG_STATE
be_operation_ami_handler_reply_stub_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int is_extra_state
  )
{
  if (is_extra_state)
    {
      switch (current_state)
        {
        case TAO_CodeGen::TAO_OPERATION_CH:
          return TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CH;

        case TAO_CodeGen::TAO_OPERATION_CS:
          return TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CS;

        default:
          return current_state;
        }
    }
  else
    {
      return current_state;
    }
}

int
be_operation_ami_handler_reply_stub_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE current_state
  )
{
  if (current_state == TAO_CodeGen::TAO_OPERATION_CH
      || current_state == TAO_CodeGen::TAO_OPERATION_CS)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

be_operation_strategy *
be_operation_ami_handler_reply_stub_strategy::copy (void)
{
  be_operation_ami_handler_reply_stub_strategy *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_operation_ami_handler_reply_stub_strategy (this->node_),
                  0);
  retval->strategy_type_ = this->strategy_type_;
  return retval;
}

// ****************************************************************
// AMH Strategy

be_operation_amh_strategy::be_operation_amh_strategy (be_operation *node)
  : be_operation_strategy (node, AMI_EXCEPTION_HOLDER_RAISE)
{
}

be_operation_amh_strategy::~be_operation_amh_strategy (void)
{
}

TAO_CodeGen::CG_STATE
be_operation_amh_strategy::next_state (
    TAO_CodeGen::CG_STATE current_state,
    int /* is_extra_state */
  )
{
  // We don't know yet if we need extra state for AMH but
  // we''ll leave the placeholder code here anyway ...
  return current_state;
}

be_operation*
be_operation_amh_strategy::arguments (void)
{
  return arguments_;
}

int
be_operation_amh_strategy::has_extra_code_generation (
    TAO_CodeGen::CG_STATE
  )
{
  return 0;
}

be_operation_strategy *
be_operation_amh_strategy::copy (void)
{
  be_operation_amh_strategy *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_operation_amh_strategy (this->node_),
                  0);
  retval->arguments_ = this->arguments_;
  retval->strategy_type_ = this->strategy_type_;
  return retval;
}
