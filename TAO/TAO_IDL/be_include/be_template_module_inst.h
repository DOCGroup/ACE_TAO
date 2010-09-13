// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef BE_TEMPLATE_MODULE_INST_H
#define BE_TEMPLATE_MODULE_INST_H

#include "ast_template_module_inst.h"
#include "be_field.h"

class be_template_module_inst
  : public virtual AST_Template_Module_Inst,
    public virtual be_field
{
public:
  be_template_module_inst (UTL_ScopedName *n,
                           AST_Template_Module *ref,
                           FE_Utils::T_ARGLIST *template_args);

  virtual ~be_template_module_inst (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_template_module_inst);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif           // BE_TEMPLATE_MODULE_INST_H
