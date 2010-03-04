
//=============================================================================
/**
 *  @file    be_operation.h
 *
 *  $Id$
 *
 *  Extension of class AST_Operation that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_OPERATION_H
#define BE_OPERATION_H

#include "be_scope.h"
#include "be_decl.h"
#include "be_codegen.h"
#include "ast_operation.h"

class AST_Type;
class be_visitor;
class be_argument;
class be_operation_strategy;

class be_operation : public virtual AST_Operation,
                     public virtual be_scope,
                     public virtual be_decl
{
public:
  be_operation (AST_Type *rt,
                AST_Operation::Flags fl,
                UTL_ScopedName *n,
                bool local,
                bool abstract);

  ~be_operation (void);

  /// Cleanup method.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Add an argument to the scope.
  virtual AST_Argument *be_add_argument (AST_Argument *arg);

  // Insert an exception at the head of the list.
  int be_insert_exception (AST_Exception *ex);

  be_operation_strategy *set_strategy (be_operation_strategy *new_strategy);

  /// Decide on the next state.
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);

  /// Returns true if we have to generate extra code.
  int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);

  /**
   * returns the operation containing special marshaling information,
   * this makes sense if not all arguments get marshaled, e.g. AMI
   * sendc_ operations.
   */
  be_operation *marshaling (void);

  /**
   * Returns a customized arguments list, e.g. AMI sendc_ operations
   * only use the in and inout arguments but not the out arguments,
   * also the first argument is the reply handler.
   */
  be_operation *arguments (void);

  /// Sets the original operation from which this one was created,
  /// applies only to implied IDL.
  void original_operation (be_operation *original_operation);

  /// Returns the original operation from which this one was created,
  /// applies only to implied IDL
  be_operation *original_operation ();

  // Narrowing

  DEF_NARROW_FROM_DECL (be_operation);
  DEF_NARROW_FROM_SCOPE (be_operation);

protected:
  /**
   * Member for holding the strategy for covering
   * differences between various operations, e.g. sendc_, raise_
   * operations in the AMI spec.
   */
  be_operation_strategy *strategy_;
  
  be_operation *original_operation_;
};

#endif
