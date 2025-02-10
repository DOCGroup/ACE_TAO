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

  virtual ~be_template_module_inst ();

  // Cleanup function.
  virtual void destroy ();

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif           // BE_TEMPLATE_MODULE_INST_H
