// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef AST_TEMPLATE_MODULE_REF_H
#define AST_TEMPLATE_MODULE_REF_H

#include "ast_field.h"
#include "fe_utils.h"

class AST_Template_Module;

class TAO_IDL_FE_Export AST_Template_Module_Ref
  : public virtual AST_Field
{
public:
  AST_Template_Module_Ref (UTL_ScopedName *n,
                           AST_Template_Module *ref,
                           UTL_StrList *param_refs);

  virtual ~AST_Template_Module_Ref (void);

  AST_Template_Module *ref (void) const;

  UTL_StrList const * param_refs (void) const;

  // Narrowing.
  DEF_NARROW_FROM_DECL (AST_Template_Module_Ref);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;

private:
  UTL_StrList * param_refs_;
};

#endif           // AST_TEMPLATE_MODULE_REF_H
