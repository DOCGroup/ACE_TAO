// $Id$

#ifndef FE_TEMPLATE_INTERFACE_HEADER_H
#define FE_TEMPLATE_INTERFACE_HEADER_H

// FE_template_interfade_header
// Internal class for FE to describe template interface headers.

#include "utl_scoped_name.h"
#include "idl_global.h"

class UTL_NameList;
class AST_Template_Interface;

class TAO_IDL_FE_Export FE_Template_InterfaceHeader
{
public:
  FE_Template_InterfaceHeader (UTL_ScopedName *n);

  virtual ~FE_Template_InterfaceHeader (void);

  // Data Accessors.
  UTL_ScopedName *name (void) const;
  AST_Template_Interface **inherits (void) const;
  long n_inherits (void) const;
  AST_Template_Interface **inherits_flat (void) const;
  long n_inherits_flat (void) const;

  void destroy (void);
  // Destroy anything allocated for this class.

  // Data.
protected:
  UTL_ScopedName *pd_interface_name;

  // Inherited interfaces.
  AST_Template_Interface **pd_inherits;
  long pd_n_inherits;

  // Used for name clash checking.
  AST_Template_Interface  **pd_inherits_flat;
  long pd_n_inherits_flat;

protected:
//  void compile_inheritance (UTL_NameList *ifaces);

//  void compile_one_inheritance (AST_Template_Interface *i);

  // Called from compile_inheritance().
//  int check_inherit (AST_Template_Interface *i);
};

#endif           // FE_TEMPLATE_INTERFACE_HEADER_H
