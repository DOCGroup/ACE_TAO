// $Id$

// AST_UnionFwd nodes denote forward declarations of IDL union.
// AST_UnionFwd nodes have a field containing the full declaration
// of the union, which is initialized when that declaration is
// encountered.

#include "ast_union_fwd.h"
#include "ast_union.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

ACE_RCSID( ast, 
           ast_union_fwd, 
           "$Id$")

AST_UnionFwd::AST_UnionFwd (void)
  : pd_full_definition (0)
{
}

AST_UnionFwd::AST_UnionFwd (UTL_ScopedName *n)
  : AST_Decl (AST_Decl::NT_union_fwd,
              n),
    pd_full_definition (0)
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

// Data accessors.

AST_Union *
AST_UnionFwd::full_definition (void)
{
  return this->pd_full_definition;
}

void
AST_UnionFwd::set_full_definition (AST_Union *nfd)
{
  this->pd_full_definition = nfd;
}

idl_bool
AST_UnionFwd::is_defined (void)
{
  return (this->pd_full_definition != 0);
}

void
AST_UnionFwd::destroy (void)
{
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_UnionFwd, AST_Type)
IMPL_NARROW_FROM_DECL (AST_UnionFwd)
