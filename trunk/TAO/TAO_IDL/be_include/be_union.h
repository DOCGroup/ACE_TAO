// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union.h
//
// = DESCRIPTION
//    Extension of class AST_Union that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_UNION_H
#define BE_UNION_H

/*
 * BE_Union
 */
class be_union : public virtual AST_Union,
                 public virtual be_scope,
                 public virtual be_type
{
public:
  // =Operations

  be_union (void);
  // default constructor

  be_union(AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  virtual int gen_var_defn (void);
  // generate the _var class definition

  virtual int gen_var_impl (void);
  // generate the implementation for the _var class

  virtual int gen_out_defn (void);
  // generate the _out class definition

  virtual int gen_out_impl (void);
  // generate the _out implementation

  virtual int member_count (void);
  // return the count of members

  virtual int default_index (void);
  // return the default index used

  virtual idl_bool in_recursion (be_type *node = 0);
  // ar we or the parameter node involved in some kind of recursion 

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type);
  DEF_NARROW_FROM_DECL(be_union);
  DEF_NARROW_FROM_SCOPE(be_union);

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown

private:
  //=helper

  int compute_member_count (void);
  // count the number of members

  int compute_default_index (void);
  // count the default index

  int member_count_;
  // number of members

  int default_index_;
  // default label index (zero based indexing)

};

#endif
