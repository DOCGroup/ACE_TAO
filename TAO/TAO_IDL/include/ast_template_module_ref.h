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

  // If IDL module has been created in a scope corresponding
  // to this node. That's the one we want to match, NOT this
  // AST_Template_Module_Ref, and it occurs in the scope right
  // after this node, so we'll match what we're looking for on
  // the next iteration of the search. So for this immediate
  // adjustment we return no match.
  virtual AST_Decl *adjust_found (bool ignore_fwd, bool full_def_only);

  /// Accessors for the member.
  bool processed (void) const;
  void processed (bool val);

  static AST_Decl::NodeType const NT;

private:
  UTL_StrList * param_refs_;

  /// Flag to prevent multiple module creations when we encounter
  /// 'chained' alias declarations.
  bool processed_;
};

#endif           // AST_TEMPLATE_MODULE_REF_H
