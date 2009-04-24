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
  be_operation (void);
  // Default constructor.

  be_operation (AST_Type *rt,
                AST_Operation::Flags fl,
                UTL_ScopedName *n,
                bool local,
                bool abstract);
  // Constructor

  ~be_operation (void);
  // Destructor.

  virtual void destroy (void);
  // Cleanup method.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  be_operation_strategy *set_strategy (be_operation_strategy *new_strategy);

  TAO_CodeGen::CG_STATE next_state (TAO_CodeGen::CG_STATE current_state,
                                    int is_extra_state = 0);
  // Decide on the next state.

  int has_extra_code_generation (TAO_CodeGen::CG_STATE current_state);
  // Returns true if we have to genrate extra code.

  be_operation *marshaling (void);
  // returns the operation containing special marshaling information,
  // this makes sense if not all arguments get marshaled, e.g. AMI
  // sendc_ operations.

  be_operation *arguments (void);
  // Returns a customized arguments list, e.g. AMI sendc_ operations
  // only use the in and inout arguments but not the out arguments,
  // also the first argument is the reply handler.

  // Narrowing

  DEF_NARROW_FROM_DECL (be_operation);
  DEF_NARROW_FROM_SCOPE (be_operation);

protected:
  be_operation_strategy *strategy_;
  // Member for holding the strategy for covering
  // differences between various operations, e.g. sendc_, raise_
  // operations in the AMI spec.
};

#endif
