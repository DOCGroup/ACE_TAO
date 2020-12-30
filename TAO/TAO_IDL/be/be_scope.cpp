#include "be_scope.h"
#include "be_valuetype.h"
#include "be_eventtype.h"
#include "be_connector.h"
#include "be_home.h"
#include "be_module.h"
#include "be_exception.h"
#include "be_union.h"
#include "be_structure.h"
#include "be_enum.h"
#include "be_operation.h"
#include "be_factory.h"
#include "be_finder.h"
#include "be_root.h"
#include "be_visitor.h"

// Default Constructor.
be_scope::be_scope ()
  : UTL_Scope (),
    comma_ (0)
{
}

// Constructor.
be_scope::be_scope (AST_Decl::NodeType type)
  : UTL_Scope (type),
    comma_ (0)
{
}

be_scope::~be_scope ()
{
}

// Code generation methods.

AST_Field *
be_scope::be_add_field (AST_Field *f)
{
  return this->fe_add_field (f);
}

AST_Argument *
be_scope::be_add_argument (AST_Argument *)
{
  return nullptr;
}

void
be_scope::comma (unsigned short comma)
{
  this->comma_ = comma;
}

int
be_scope::comma () const
{
  return this->comma_;
}

// Return the scope created by this node (if one exists, else NULL).
be_decl *
be_scope::decl ()
{
  switch (this->scope_node_type ())
    {
    case AST_Decl::NT_interface:
      return dynamic_cast<be_interface*> (this);
    case AST_Decl::NT_valuetype:
      return dynamic_cast<be_valuetype*> (this);
    case AST_Decl::NT_eventtype:
      return dynamic_cast<be_eventtype*> (this);
    case AST_Decl::NT_component:
      return dynamic_cast<be_component*> (this);
    case AST_Decl::NT_connector:
      return dynamic_cast<be_connector*> (this);
    case AST_Decl::NT_home:
      return dynamic_cast<be_home*> (this);
    case AST_Decl::NT_module:
      return dynamic_cast<be_module*> (this);
    case AST_Decl::NT_root:
      return dynamic_cast<be_root*> (this);
    case AST_Decl::NT_except:
      return dynamic_cast<be_exception*> (this);
    case AST_Decl::NT_union:
      return dynamic_cast<be_union*> (this);
    case AST_Decl::NT_struct:
      return dynamic_cast<be_structure*> (this);
    case AST_Decl::NT_enum:
      return dynamic_cast<be_enum*> (this);
    case AST_Decl::NT_op:
      return dynamic_cast<be_operation*> (this);
    case AST_Decl::NT_factory:
      return dynamic_cast<be_factory*> (this);
    case AST_Decl::NT_finder:
      return dynamic_cast<be_finder*> (this);
    default:
      return (be_decl *)nullptr;
    }
}

void
be_scope::destroy ()
{
}

int
be_scope::accept (be_visitor *visitor)
{
  return visitor->visit_scope (this);
}
