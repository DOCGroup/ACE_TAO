/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_operation_strategy.h
//
// = DESCRIPTION
//    Strategy to cover differences between operations, e.g.
//    the sendc_ and raise_ operations in the AMI spec.
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#ifndef TAO_BE_OPERATION_STRATEGY_H
#define TAO_BE_OPERATION_STRATEGY_H

#include "be_codegen.h"

class be_operation;

// Base class for operation level strategeis

class be_operation_strategy
{
public:
  enum Strategy_Kind {
      DEFAULT = 0,
      AMI_SENDC,
      AMI_HANDLER_REPLY_STUB,
      AMI_EXCEPTION_HOLDER_RAISE
  };

  be_operation_strategy (be_operation *node,
                         Strategy_Kind strategy_type);

  virtual ~be_operation_strategy ();

  int strategy_type ();
  // Return the type of the strategy.

  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0) = 0;
  // Change the sate if necessary

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
  // returns true if we have to genrate extra code.

  virtual be_operation *hidden_operation ();
  // returns the operation node which contains extra data,

protected:

  be_operation *node_;
  // The node we strategize

  Strategy_Kind strategy_type_;
  // the type of strategy
};



// Default (do nothing) strategy for operations

class be_operation_default_strategy
  : public be_operation_strategy
{
public:
  be_operation_default_strategy (be_operation *node);

  virtual ~be_operation_default_strategy ();

  // overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);

};


// Strategy to mark normal reply handler operations
// in order to have them generate the reply stub 
// alias client skeleton for AMI

class be_operation_ami_handler_reply_stub_strategy
  : public be_operation_strategy
{
public:
  be_operation_ami_handler_reply_stub_strategy (be_operation *node);

  virtual ~be_operation_ami_handler_reply_stub_strategy ();

  // overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
  // returns true if we have to genrate extra code.
};


// Strategy to mark normal sendc_ operations 
// in AMI mode.

class be_operation_ami_sendc_strategy
  : public be_operation_strategy
{
public:
  be_operation_ami_sendc_strategy (be_operation *node,
                                   be_operation *hidden_operation);

  virtual ~be_operation_ami_sendc_strategy ();

  // overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
  // returns true if we have to genrate extra code.

  virtual be_operation *hidden_operation ();
  // returns the operation node which contains extra data,

private:
  be_operation *hidden_operation_;
};


// Strategy for raise operations in the AMI exception
// holder valuetype.

class be_operation_ami_exception_holder_raise_strategy
  : public be_operation_strategy
{
public:
  be_operation_ami_exception_holder_raise_strategy (be_operation *node);

  virtual ~be_operation_ami_exception_holder_raise_strategy ();

  // overridden methods.
  virtual TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                            int is_extra_state = 0);
};

#endif  // if !defined
