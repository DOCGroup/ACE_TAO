/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_operation_strategy.h
 *
 *  $Id$
 *
 *  Strategy to cover differences between operations, e.g.
 *  the sendc_ and raise_ operations in the AMI spec.
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#ifndef TAO_BE_OPERATION_STRATEGY_H
#define TAO_BE_OPERATION_STRATEGY_H

#include "be_codegen.h"

class be_operation;

// Base class for operation level strategies.
class be_operation_strategy
{
public:
  enum Strategy_Kind
  {
    DEFAULT = 0,
    AMI_SENDC,
    AMI_HANDLER_REPLY_STUB,
    AMI_EXCEPTION_HOLDER_RAISE
  };

  be_operation_strategy (be_operation *node,
                         Strategy_Kind strategy_type);

  virtual ~be_operation_strategy (void);

  /// Return the type of the strategy.
  int strategy_type (void);

  /// Change the state if necessary.
  virtual TAO_CodeGen::CG_STATE next_state (
      TAO_CodeGen::CG_STATE current_state,
      int is_extra_state = 0
    ) = 0;

  /// Returns true if we have to generate extra code.
  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  /**
   * Returns the operation containing special marshaling information,
   * this makes sense if not all arguments get marshaled, e.g. AMI
   * sendc_ operations.
   */
  virtual be_operation *marshaling (void);

  /**
   * Returns a customized arguments list, e.g. AMI sendc_ operations
   * only use the in and inout arguments but not the out arguments,
   * also the first argument is the reply handler.
   */
  virtual be_operation *arguments (void);

  /// Overrides return a deep copy.
  virtual be_operation_strategy *copy (void);

  /// Cleanup.
  virtual void destroy (void);

protected:
  /// The node we strategize.
  be_operation *node_;

  /// The type of strategy.
  Strategy_Kind strategy_type_;
};



// Default (do nothing) strategy for operations.
class be_operation_default_strategy
  : public be_operation_strategy
{
public:
  be_operation_default_strategy (be_operation *node);

  virtual ~be_operation_default_strategy (void);

  // Overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);

  /// Returns a deep copy.
  virtual be_operation_strategy *copy (void);
};


// Strategy to mark normal reply handler operations
// in order to have them generate the reply stub
// alias client skeleton for AMI.
class be_operation_ami_handler_reply_stub_strategy
  : public be_operation_strategy
{
public:
  be_operation_ami_handler_reply_stub_strategy (be_operation *node);

  virtual ~be_operation_ami_handler_reply_stub_strategy (void);

  // Overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);

  /// Returns true if we have to generate extra code.
  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  /// Returns a deep copy.
  virtual be_operation_strategy *copy (void);
};


// Strategy to mark normal sendc_ operations
// in AMI mode.
class be_operation_ami_sendc_strategy : public be_operation_strategy
{
public:
  be_operation_ami_sendc_strategy (be_operation *node,
                                   be_operation *marshaling,
                                   be_operation *arguments);

  virtual ~be_operation_ami_sendc_strategy (void);

  // Overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);

  /// Returns true if we have to generate extra code.
  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  /**
   * Returns the operation containing special marshaling information,
   * this makes sense if not all arguments get marshaled, e.g. AMI
   * sendc_ operations
   */
  virtual be_operation *marshaling (void);

  /**
   * Returns a customized arguments list, e.g. AMI sendc_ operations
   * only use the in and inout arguments but not the out arguments,
   * also the first argument is the reply handler.
   */
  virtual be_operation *arguments (void);

  /// Returns a deep copy.
  virtual be_operation_strategy *copy (void);

  /// Cleanup.
  virtual void destroy (void);

private:
  be_operation *marshaling_;
  be_operation *arguments_;
  bool owns_operations_;
};


// Strategy for raise operations in the AMI exception
// holder valuetype.
class be_operation_ami_exception_holder_raise_strategy
  : public be_operation_strategy
{
public:
  be_operation_ami_exception_holder_raise_strategy (be_operation *node);

  virtual ~be_operation_ami_exception_holder_raise_strategy ();

  // Overridden methods.
  virtual TAO_CodeGen::CG_STATE next_state (
      TAO_CodeGen::CG_STATE current_state,
      int is_extra_state = 0
    );

  /// Returns a deep copy.
  virtual be_operation_strategy *copy (void);
};


// AMH strategy.
class be_operation_amh_strategy
  : public be_operation_strategy
{
public:
  be_operation_amh_strategy (be_operation *node);

  virtual ~be_operation_amh_strategy (void);

  // Overridden methods.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);

  virtual int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  virtual be_operation *arguments (void);

  /// Returns a deep copy.
  virtual be_operation_strategy *copy (void);

private:
  be_operation *arguments_;
};

#endif  // TAO_BE_OPERATION_STRATEGY_H
