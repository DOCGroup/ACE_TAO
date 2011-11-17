
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
//class be_operation_strategy;

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

  /// Add an argument to the scope.
  virtual AST_Argument *be_add_argument (AST_Argument *arg);

  /// Insert an exception at the head of the list.
  int be_insert_exception (AST_Exception *ex);

  bool is_sendc_ami (void) const;
  void is_sendc_ami (bool val);

  bool is_excep_ami (void) const;
  void is_excep_ami (bool val);

  bool is_attr_op (void) const;
  void is_attr_op (bool val);

  // Narrowing
  DEF_NARROW_FROM_DECL (be_operation);
  DEF_NARROW_FROM_SCOPE (be_operation);

protected:
  bool is_sendc_ami_;
  bool is_excep_ami_;
  bool is_attr_op_;
};

#endif
