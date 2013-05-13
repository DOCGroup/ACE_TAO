// $Id$

#include "ast_finder.h"
#include "ast_visitor.h"

#include "utl_identifier.h"

AST_Decl::NodeType const
AST_Finder::NT = AST_Decl::NT_finder;

AST_Finder::AST_Finder (UTL_ScopedName *n)
  : COMMON_Base (true,
                 false), //@@ Always local, never abstract
    AST_Decl (AST_Decl::NT_finder,
              n),
    UTL_Scope (AST_Decl::NT_finder),
    AST_Factory (n)
{
}

AST_Finder::~AST_Finder (void)
{
}

void
AST_Finder::destroy (void)
{
  this->AST_Factory::destroy ();
}

// Dump this AST_Factory node to the ostream o.
void
AST_Finder::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Decl *d = 0;

  this->dump_i (o, "finder ");
  this->local_name ()->dump (o);
  this->dump_i (o, "(");

  // Iterator must be explicitly advanced inside the loop.
  for (UTL_ScopeActiveIterator i (this, IK_decls);
       !i.is_done ();)
    {
      d = i.item ();
      d->dump (o);
      i.next ();

      if (!i.is_done())
        {
          this->dump_i (o, ", ");
        }
    }

  this->dump_i (o, ")");
}

int
AST_Finder::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_finder (this);
}

IMPL_NARROW_FROM_DECL(AST_Finder)
IMPL_NARROW_FROM_SCOPE(AST_Finder)

