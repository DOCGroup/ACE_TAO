#ifndef _AST_UNION_FWD_AST_UNION_FWD_HH
#define _AST_UNION_FWD_AST_UNION_FWD_HH

#include "ast_structure_fwd.h"

class AST_Union;

// Representation of a forward union declaration.
class TAO_IDL_FE_Export AST_UnionFwd : public virtual AST_StructureFwd
{
public:
  AST_UnionFwd (AST_Union *dummy,
                UTL_ScopedName *n);

  ~AST_UnionFwd () override;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
};

#endif           // _AST_UNION_FWD_AST_UNION_FWD_HH
