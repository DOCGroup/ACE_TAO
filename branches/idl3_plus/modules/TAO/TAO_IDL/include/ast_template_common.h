// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_COMMON_H
#define AST_TEMPLATE_COMMON_H

#include "ast_decl.h"
#include "utl_scope.h"

#include "fe_utils.h"

class TAO_IDL_FE_Export AST_Template_Common
  : public virtual AST_Decl,
    public virtual UTL_Scope
{
public:
  FE_Utils::T_PARAMLIST_INFO *template_params (void);
  
  virtual void destroy (void);

  typedef ACE_Unbounded_Queue<AST_Decl *> T_ARGLIST;
  
  // Called by extended port instantiations.
  T_ARGLIST *match_arg_names (UTL_NameList *arg_names);
  
  // Called by porttype references in connectors.
  bool match_params (UTL_StrList *param_names);
  
protected:
  AST_Template_Common (AST_Decl::NodeType nt,
                       UTL_ScopedName *n,
                       FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~AST_Template_Common (void);
  
protected:
  FE_Utils::T_PARAMLIST_INFO * template_params_;
  
private:
  bool match_param_type (AST_Decl::NodeType my_type,
                         AST_Decl *d);
};

#endif // AST_TEMPLATE_COMMON_H

