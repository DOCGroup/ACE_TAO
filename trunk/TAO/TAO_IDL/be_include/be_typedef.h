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

#include "be_type.h"
#include "ast_typedef.h"

class UTL_StrList;
class be_visitor;

class be_typedef : public virtual AST_Typedef,
                   public virtual be_type
{
public:
  be_typedef (void);
  // Default constructor.

  be_typedef (AST_Type *bt,
              UTL_ScopedName *n,
              UTL_StrList *p,
              idl_bool l,
              idl_bool a);
  // Constructor.

  be_type *primitive_base_type (void);
  // Return the most primitive base type by traversing the chain of typedefed
  // base types.

  virtual AST_Decl::NodeType base_node_type (void) const;
  // Return the most "unaliased" type node for the base type (see
  // be_type.h).

  virtual void destroy (void);
  // Cleanup function.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type);
  DEF_NARROW_FROM_DECL (be_typedef);

protected:
  virtual int compute_size_type (void);
  // Compute the size type if it is unknown.

};

#endif
