// $Id$

#ifndef FE_TEMPLATE_INTERFACE_HEADER_H
#define FE_TEMPLATE_INTERFACE_HEADER_H

// FE_template_interfade_header
// Internal class for FE to describe template interface headers.

#include "fe_interface_header.h"
#include "fe_utils.h"

class AST_Template_Interface;

class TAO_IDL_FE_Export FE_Template_InterfaceHeader
  : public FE_InterfaceHeader
{
public:
  FE_Template_InterfaceHeader (UTL_ScopedName *n,
                               FE_Utils::T_PARAMLIST_INFO *params,
                               FE_Utils::T_REFLIST_INFO *inherited);

  virtual ~FE_Template_InterfaceHeader (void);

  // Data Accessor.
  FE_Utils::T_PARAMLIST_INFO *param_info (void) const;

  // Destroy anything allocated for this class.
  virtual void destroy (void);

  // Data.
protected:
  FE_Utils::T_PARAMLIST_INFO *param_info_;
  FE_Utils::T_REFLIST_INFO *parent_info_;

protected:
  void compile_template_inheritance (void);
  bool match_params (AST_Template_Interface *ti);
  bool duplicate_param_id (void);
};

#endif           // FE_TEMPLATE_INTERFACE_HEADER_H
