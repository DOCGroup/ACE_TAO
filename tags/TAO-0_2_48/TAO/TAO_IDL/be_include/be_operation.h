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
};

#endif
