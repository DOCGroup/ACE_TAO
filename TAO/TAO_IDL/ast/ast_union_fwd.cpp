// $Id$

// AST_UnionFwd nodes denote forward declarations of IDL union.
// AST_UnionFwd nodes have a field containing the full declaration
// of the union, which is initialized when that declaration is
// encountered.

#include "ast_union_fwd.h"
#include "ast_union.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "ace/streams.h"

ACE_RCSID (ast, 
           ast_union_fwd, 
           "$Id$")

AST_UnionFwd::AST_UnionFwd (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_StructureFwd ()
{
}

AST_UnionFwd::AST_UnionFwd (AST_Union *dummy,
                            UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_union_fwd,
              n),
    AST_Type (AST_Decl::NT_union_fwd,
              n),
    AST_StructureFwd (dummy,
                      n)
{
}

AST_UnionFwd::~AST_UnionFwd (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_StructureFwd node to the ostream o.
void
AST_UnionFwd::dump (ACE_OSTREAM_TYPE &o)
{
  o << "union ";
  this->local_name ()->dump (o);
}

int
AST_UnionFwd::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_union_fwd (this);
}

void
AST_UnionFwd::destroy (void)
{
  this->AST_StructureFwd::destroy ();
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_UnionFwd, AST_StructureFwd)
IMPL_NARROW_FROM_DECL (AST_UnionFwd)
