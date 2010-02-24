// $Id$

#ifndef FE_COMPONENT_HEADER_H
#define FE_COMPONENT_HEADER_H

#include "fe_interface_header.h"

class AST_Component;

// FE_component_header
// Internal class for FE to describe component headers.

// Unlike value types, a component's supported interfaces are simply
// added to the inheritance list in generated code, so we use the
// existing base class mechanism for managing the inheritance list
// to manage the derived class's supported interface list.
class FE_ComponentHeader : public FE_InterfaceHeader
{
public:

  FE_ComponentHeader (UTL_ScopedName *n,
                      UTL_ScopedName *base_component,
                      UTL_NameList *supports,
                      bool compile_now);
  virtual ~FE_ComponentHeader (void);

  // Data Accessors.
  AST_Component *base_component (void) const;
  AST_Type **supports (void) const;
  long n_supports (void) const;
  AST_Interface **supports_flat (void) const;
  long n_supports_flat (void) const;

protected:
  void compile_inheritance (UTL_ScopedName *base_component);
  void compile_supports (UTL_NameList *supports);

protected:
  AST_Component *base_component_;
};

#endif /* FE_COMPONENT_HEADER_H */

