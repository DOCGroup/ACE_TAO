/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_structure.h
//
// = DESCRIPTION
//    Extension of class AST_Structure that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_STRUCTURE_H
#define BE_STRUCTURE_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_structure.h"

class UTL_StrList;
class be_visitor;

/*
 * BE_Structure
 */
class be_structure : public virtual AST_Structure,
                     public virtual be_scope,
                     public virtual be_type
{
public:
  // =Operations

  be_structure (void);
  // default constructor

  be_structure (UTL_ScopedName *n,
                UTL_StrList *p,
                idl_bool local,
                idl_bool abstract);
  // constructor

  virtual int gen_var_defn (char *local_name = 0);
  // generate the _var class definition

  virtual int gen_var_impl (char *local_name = 0,
                            char *full_name = 0);
  // generate the implementation for the _var class

  virtual int gen_out_defn (char *local_name = 0);
  // generate the _out class definition

  virtual int gen_out_impl (char *local_name = 0,
                            char *full_name = 0);
  // generate the _out implementation

  virtual int member_count (void);
  // return the count of members

  virtual idl_bool in_recursion (be_type *node = 0);
  // are we or the node represented by node involved in recursion

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_structure, AST_Structure, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_structure);
  DEF_NARROW_FROM_SCOPE (be_structure);

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int member_count_;
  // number of members

};

#endif
