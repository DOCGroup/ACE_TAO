// $Id$

#ifndef FE_TEMPLATE_INTERFACE_HEADER_H
#define FE_TEMPLATE_INTERFACE_HEADER_H

// FE_template_InterfaceHeader
// Internal class for FE to describe template interfaces.

#include "fe_interface_header.h"
#include "fe_template_header_common.h"

class AST_Template_Interface;

class TAO_IDL_FE_Export FE_Template_InterfaceHeader
  : public FE_InterfaceHeader,
    public FE_TemplateHeader_Common
{
public:
  FE_Template_InterfaceHeader (UTL_ScopedName *n,
                               FE_Utils::T_PARAMLIST_INFO *params,
                               FE_Utils::T_REFLIST_INFO *inherited);

  virtual ~FE_Template_InterfaceHeader (void);

  // Destroy anything allocated for this class.
  virtual void destroy (void);

private:
  void compile_template_inheritance (void);
  bool match_params (AST_Template_Interface *node);

  // Data.
private:
  FE_Utils::T_REFLIST_INFO *parent_info_;
};

#endif           // FE_TEMPLATE_INTERFACE_HEADER_H
