// $Id$

#ifndef FE_HOME_HEADER_H
#define FE_HOME_HEADER_H

#include "fe_component_header.h"

class AST_Home;
class AST_ValueType;

// FE_HomeHeader
// Internal class for FE to describe component home headers

// We use the 'base_component' member of the base class to
// store the 'managed_component' member of the derived class.
// By inheriting from FE_ComponentHeader, we also get the
// reuse of the mechanism described in the comment above
// for handling the supported interface list.
class FE_HomeHeader : public FE_ComponentHeader
{
public:
  FE_HomeHeader (UTL_ScopedName *n,
                 UTL_ScopedName *base_home,
                 UTL_NameList *supports,
                 UTL_ScopedName *managed_component,
                 UTL_ScopedName *primary_key);

  virtual ~FE_HomeHeader (void);

  // Data Accessors.
  AST_Home *base_home (void) const;
  AST_Component *managed_component (void) const;
  AST_Type *primary_key (void) const;

protected:
  AST_Home *base_home_;
  AST_Component *managed_component_;
  AST_Type *primary_key_;

protected:
  void compile_inheritance (UTL_ScopedName *base_home);
  void compile_managed_component (UTL_ScopedName *managed_compoent);
  void compile_primary_key (UTL_ScopedName *primary_key);
};

#endif /* FE_HOME_HEADER_H */

