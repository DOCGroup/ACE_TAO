// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_attribute.h
//
// = DESCRIPTION
//    Extension of class AST_Attribute that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_ATTRIBUTE_H
#define BE_ATTRIBUTE_H

#include "ast_attribute.h"
#include "be_decl.h"
#include "be_operation_strategy.h"

class AST_Type;
class be_visitor;

class be_attribute : public virtual AST_Attribute,
                     public virtual be_decl
{
public:
  be_attribute (void);
  // Default constructor.

  be_attribute (idl_bool ro,
                AST_Type *ft,
                UTL_ScopedName *n,
                idl_bool local,
                idl_bool abstract);
  // Constructor.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  be_operation_strategy *set_get_strategy (
      be_operation_strategy *new_strategy
    );
  // Set the get operation strategy.
  // Note, that it is not actually used, the operation
  // made out of the attribute is going to copy it.

  be_operation_strategy *set_set_strategy (
      be_operation_strategy *new_strategy
    );
  // Set the set operation strategy.
  // Note, that it is not actually used, the operation
  // made out of the attribute is going to copy it.

  be_operation_strategy *get_get_strategy (void);
  // Retrieve the underlying get_operation strategy.

  be_operation_strategy *get_set_strategy (void);
  // Retrieve the underlying set_operation strategy.

  // Narrowing
  DEF_NARROW_METHODS2 (be_attribute, AST_Attribute, be_decl);
  DEF_NARROW_FROM_DECL (be_attribute);

private:
  be_operation_strategy *get_strategy_;

  be_operation_strategy *set_strategy_;
  // Member for holding the strategy for covering
  // differences between various operations, e.g. sendc_, raise_
  // operations in the AMI spec.
};

#endif
