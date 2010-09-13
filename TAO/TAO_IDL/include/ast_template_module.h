// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_MODULE_H
#define AST_TEMPLATE_MODULE_H

#include "ast_module.h"
#include "ast_type.h"

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_Template_Module
  : public virtual AST_Module,
    public virtual AST_Type
{
public:
  AST_Template_Module (UTL_ScopedName *n,
                       FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_Template_Module (void);

  FE_Utils::T_PARAMLIST_INFO *
  template_params (void) const;

  // Checks for errors in the template args of an instantiation.
  bool match_arg_names (FE_Utils::T_ARGLIST *args);

  // Checks for errors in the template param refs of an alias.
  bool match_param_refs (UTL_StrList *refs, UTL_Scope *decl_scope);

  // Narrowing.
  DEF_NARROW_FROM_DECL (AST_Template_Module);
  DEF_NARROW_FROM_SCOPE (AST_Template_Module);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Scope Management Protocol
  virtual
  AST_Template_Module_Ref *fe_add_template_module_ref (
    AST_Template_Module_Ref *m);

  static AST_Decl::NodeType const NT;

protected:
  FE_Utils::T_PARAMLIST_INFO * template_params_;

private:
  // Match with a template arg.
  bool match_one_param (FE_Utils::T_Param_Info *param,
                        AST_Decl *d);

  // Find one of our params by name.
  FE_Utils::T_Param_Info *find_param (UTL_String *name);

  // Match a param with an aliased id by type.
  bool match_param_by_type (FE_Utils::T_Param_Info *param);
};

#endif           // AST_TEMPLATE_MODULE_H
