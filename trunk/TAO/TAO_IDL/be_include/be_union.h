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

#include "be_type.h"
#include "be_scope.h"
#include "ast_union.h"

class AST_ConcreteType;
class UTL_StrList;
class be_visitor;

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

  be_union(AST_ConcreteType *dt,
           UTL_ScopedName *n,
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

  virtual int default_index (void);
  // return the default index used

  virtual idl_bool in_recursion (be_type *node = 0);
  // Are we or the parameter node involved in some kind of recursion?

  virtual idl_bool has_duplicate_case_labels (void);
  // Do we have at least one member with multiple case labels?

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type);
  DEF_NARROW_FROM_DECL(be_union);
  DEF_NARROW_FROM_SCOPE(be_union);

  struct DefaultValue
  {
    union PermittedTypes
    {
      char char_val;
      ACE_CDR::WChar wchar_val;
      unsigned long bool_val;
      ACE_INT16 short_val;
      ACE_UINT16 ushort_val;
      ACE_INT32 long_val;
      ACE_UINT32 ulong_val;
      ACE_UINT32 enum_val;
      // TO-DO - handle (u)longlong types
    } u;
    long computed_;
    // computed == -1 => error condition
    //          == 0 => does not exist because all cases have been covered
    //          == 1 => already computed
    //          == -2 => initial value
  };

  int default_value (DefaultValue &);
  // get the default value

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown

private:
  //=helper

  int compute_default_index (void);
  // count the default index

  virtual int compute_default_value (void);
  // compute the implicit default value (if any)

  int default_index_;
  // default label index (zero based indexing)

  DefaultValue default_value_;
  // implicit default value (if any)
};

#endif
