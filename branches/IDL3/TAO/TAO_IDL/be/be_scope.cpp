//
// $Id$
//
#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_scope, "$Id$")


// Default Constructor.
be_scope::be_scope (void)
  : comma_ (0)
{
}

// Constructor.
be_scope::be_scope (AST_Decl::NodeType type)
  : UTL_Scope (type),
    comma_ (0)
{
}

be_scope::~be_scope (void)
{
}

// Code generation methods.

void
be_scope::comma (unsigned short comma)
{
  this->comma_ = comma;
}

int
be_scope::comma (void) const
{
  return this->comma_;
}

// Return the scope created by this node (if one exists, else NULL).
be_decl *
be_scope::decl (void)
{
  switch (this->scope_node_type())
    {
    case AST_Decl::NT_interface:
      return be_interface::narrow_from_scope (this);
    case AST_Decl::NT_module:
      return be_module::narrow_from_scope (this);
    case AST_Decl::NT_root:
      return be_root::narrow_from_scope (this);
    case AST_Decl::NT_except:
      return be_exception::narrow_from_scope (this);
    case AST_Decl::NT_union:
      return be_union::narrow_from_scope (this);
    case AST_Decl::NT_struct:
      return be_structure::narrow_from_scope (this);
    case AST_Decl::NT_enum:
      return be_enum::narrow_from_scope (this);
    case AST_Decl::NT_op:
      return be_operation::narrow_from_scope (this);
    case AST_Decl::NT_factory:
      return be_factory::narrow_from_scope (this);
    default:
      return (be_decl *)0;
    }
}

void
be_scope::destroy (void)
{
  UTL_Scope::destroy ();
}

int
be_scope::accept (be_visitor *visitor)
{
  return visitor->visit_scope (this);
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (be_scope, UTL_Scope)
IMPL_NARROW_FROM_SCOPE (be_scope)
