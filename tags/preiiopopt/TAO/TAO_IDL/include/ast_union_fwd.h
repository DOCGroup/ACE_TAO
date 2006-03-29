// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_UNION_FWD_AST_UNION_FWD_HH
#define _AST_UNION_FWD_AST_UNION_FWD_HH

#include "ast_structure_fwd.h"

class AST_Union;

// Representation of a forward union declaration.

class TAO_IDL_FE_Export AST_UnionFwd : public virtual AST_StructureFwd
{
public:
  AST_UnionFwd (void);

  AST_UnionFwd (AST_Union *dummy,
                UTL_ScopedName *n);

  virtual ~AST_UnionFwd (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_UnionFwd, AST_StructureFwd);
  DEF_NARROW_FROM_DECL(AST_UnionFwd);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif           // _AST_UNION_FWD_AST_UNION_FWD_HH
