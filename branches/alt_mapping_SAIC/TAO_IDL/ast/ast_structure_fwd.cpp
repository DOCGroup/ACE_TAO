// $Id$

// AST_StructureFwd nodes denote forward declarations of IDL structs.
// AST_StructureFwd nodes have a field containing the full declaration
// of the struct, which is initialized when that declaration is
// encountered.

#include "ast_structure_fwd.h"
#include "ast_structure.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

AST_Decl::NodeType const
AST_StructureFwd::NT = AST_Decl::NT_struct_fwd;

AST_StructureFwd::AST_StructureFwd (AST_Structure *full_defn,
                                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_struct_fwd,
              n),
    AST_Type (AST_Decl::NT_struct_fwd,
              n),
    pd_full_definition (full_defn),
    is_defined_ (false)
{
}

AST_StructureFwd::~AST_StructureFwd (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_StructureFwd node to the ostream o.
void
AST_StructureFwd::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "struct ");
  this->local_name ()->dump (o);
}

int
AST_StructureFwd::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_structure_fwd (this);
}

// Data accessors.

AST_Structure *
AST_StructureFwd::full_definition (void)
{
  return this->pd_full_definition;
}

void
AST_StructureFwd::set_full_definition (AST_Structure *nfd)
{
  this->pd_full_definition->destroy ();
  delete this->pd_full_definition;
  this->pd_full_definition = nfd;

  // In case it's not already set.
  this->is_defined_ = true;
}

bool
AST_StructureFwd::is_defined (void)
{
  return this->is_defined_;
}

void
AST_StructureFwd::set_as_defined (void)
{
  this->is_defined_ = true;
}

void
AST_StructureFwd::destroy (void)
{
  if (!this->is_defined_ && 0 != this->pd_full_definition)
    {
      this->pd_full_definition->destroy ();
      delete this->pd_full_definition;
      this->pd_full_definition = 0;
    }

  this->AST_Type::destroy ();
}

bool
AST_StructureFwd::is_fwd (void)
{
  return true; // This is a fwd declared type
}

// We don't actually want the forward declaration,
// but want to return the full definition member,
// whether defined yet or not.
AST_Decl *
AST_StructureFwd::adjust_found (
  bool ignore_fwd,
  bool full_def_only)
{
  if (ignore_fwd)
    {
      AST_Structure *s = this->full_definition ();
      return (full_def_only && !s->is_defined () ? 0 : s);
    }

  return this;
}

IMPL_NARROW_FROM_DECL (AST_StructureFwd)
