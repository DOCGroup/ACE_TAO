// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_typedef.h
//
// = DESCRIPTION
//    Extension of class AST_typedef that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_TYPEDEF_H
#define BE_TYPEDEF_H

/*
 * BE_Typedef
 */
class be_typedef : public virtual AST_Typedef,
                   public virtual be_type
{
public:
  // =Operations

  be_typedef (void);
  // default constructor

  be_typedef (AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p);
  // constructor

  be_type *primitive_base_type (void);
  // return the most primitive base type by traversing the chain of typedefed
  // base types

  virtual AST_Decl::NodeType base_node_type (void) const;
  // Return the most "unaliased" type node for the base type (see
  // be_type.h).

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type);
  DEF_NARROW_FROM_DECL (be_typedef);

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown

};

#endif
