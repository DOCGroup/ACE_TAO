// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_operation.h
//
// = DESCRIPTION
//    Extension of class AST_Operation that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_OPERATION_H
#define BE_OPERATION_H

#include "be_scope.h"
#include "be_decl.h"
#include "ast_operation.h"
#include "be_operation_strategy.h"

class AST_Type;
class UTL_StrList;
class be_visitor;
class be_argument;

/*
 * BE_Operation
 */
class be_operation : public virtual AST_Operation,
                     public virtual be_scope,
                     public virtual be_decl
{
public:
  // =Operations

  be_operation (void);
  // default constructor

  be_operation (AST_Type *rt, AST_Operation::Flags fl, UTL_ScopedName *n,
                UTL_StrList *p);
  // constructor

  ~be_operation ();
  // destructor

  int void_return_type ();
  // Returns 1 if the operation has a void return type.

  virtual int argument_count (void);
  // return the count of members

  virtual int has_native (void);
  // Any of the arguments or the return value is a <native> type.
  // This is important because in that case no code should be
  // generated for the stubs.

  be_argument *add_argument_to_scope (be_argument *arg);
  // add an argument to the scope

  // Visiting
  virtual int accept (be_visitor *visitor);

  be_operation_strategy *set_strategy (be_operation_strategy *new_strategy);
  
  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);
  // decide on the next state

  int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
  // returns true if we have to genrate extra code.

  be_operation *marshaling ();
  // returns the operation containing special marshaling information,
  // this makes sense if not all arguments get marshaled, e.g. AMI
  // sendc_ operations

  be_operation *arguments ();
  // returns a customized arguments list, e.g. AMI sendc_ operations
  // only use the in and inout arguments but not the out arguments,
  // also the first argument is the reply handler.

  // Narrowing
  DEF_NARROW_METHODS3 (be_operation, AST_Operation, be_scope, be_decl);
  DEF_NARROW_FROM_DECL (be_operation);
  DEF_NARROW_FROM_SCOPE (be_operation);

protected:
  //=helper
  int compute_size_type (void);
  // compute the size type if it is unknown

  int compute_argument_attr (void);
  // count the number of arguments

  int argument_count_;
  // number of arguments

  int has_native_;
  // Is any argument of type native.

  be_operation_strategy *strategy_;
  // Member for holding the strategy for covering
  // differences between various operations, e.g. sendc_, raise_
  // operations in the AMI spec.
};

#endif
