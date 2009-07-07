// $Id$

#ifndef FE_TEMPLATE_INTERFACE_HEADER_H
#define FE_TEMPLATE_INTERFACE_HEADER_H

// FE_template_interfade_header
// Internal class for FE to describe template interface headers.

#include "fe_utils.h"

class AST_Template_Interface;

class TAO_IDL_FE_Export FE_Template_InterfaceHeader
{
public:
  FE_Template_InterfaceHeader (UTL_ScopedName *n,
                               FE_Utils::T_PARAMLIST_INFO *params,
                               FE_Utils::T_REFLIST_INFO *inherited);

  virtual ~FE_Template_InterfaceHeader (void);

  // Data Accessors.
  UTL_ScopedName *name (void) const;

  AST_Template_Interface **inherits (void) const;
  long n_inherits (void) const;

  AST_Template_Interface **inherits_flat (void) const;
  long n_inherits_flat (void) const;

  FE_Utils::T_PARAMLIST_INFO *param_info (void) const;

  // Destroy anything allocated for this class.
  virtual void destroy (void);

  // Data.
protected:
  UTL_ScopedName *interface_name_;
  FE_Utils::T_PARAMLIST_INFO *param_info_;
  FE_Utils::T_REFLIST_INFO *parent_info_;

  // Inherited interfaces.
  AST_Template_Interface **inherits_;
  long n_inherits_;

  // Used for name clash checking.
  AST_Template_Interface **inherits_flat_;
  long n_inherits_flat_;

protected:
//  void compile_inheritance (UTL_NameList *ifaces);

//  void compile_one_inheritance (AST_Template_Interface *i);

  // Called from compile_inheritance().
//  int check_inherit (AST_Template_Interface *i);
};

#endif           // FE_TEMPLATE_INTERFACE_HEADER_H
