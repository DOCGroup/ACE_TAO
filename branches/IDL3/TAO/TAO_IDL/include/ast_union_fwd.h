// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_UNION_FWD_AST_UNION_FWD_HH
#define _AST_UNION_FWD_AST_UNION_FWD_HH

#include "ast_type.h"

class AST_Union;

// Representation of a forward union declaration.

class TAO_IDL_FE_Export AST_UnionFwd : public virtual AST_Type
{
public:
  AST_UnionFwd (void);

  AST_UnionFwd (UTL_ScopedName *n);

  virtual ~AST_UnionFwd (void);

  AST_Union *full_definition (void);
  void set_full_definition (AST_Union *nfd);

  virtual idl_bool is_defined (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_UnionFwd, AST_Type);
  DEF_NARROW_FROM_DECL(AST_UnionFwd);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  AST_Union *pd_full_definition;
  // The union this is a forward declaration of.
};

#endif           // _AST_UNION_FWD_AST_UNION_FWD_HH
