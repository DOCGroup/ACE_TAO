// $Id$

// AST_StructureFwd nodes denote forward declarations of IDL structs.
// AST_StructureFwd nodes have a field containing the full declaration
// of the struct, which is initialized when that declaration is
// encountered.

#include "ast_structure_fwd.h"
#include "ast_structure.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "ace/streams.h"

ACE_RCSID( ast, 
           ast_structure_fwd, 
           "$Id$")

AST_StructureFwd::AST_StructureFwd (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    pd_full_definition (0)
{
}

AST_StructureFwd::AST_StructureFwd (AST_Structure *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_struct_fwd,
              n),
    AST_Type (AST_Decl::NT_struct_fwd,
              n),
    pd_full_definition (dummy)
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
  o << "struct ";
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
  delete this->pd_full_definition;
  this->pd_full_definition = 0;
  this->pd_full_definition = nfd;
}

idl_bool
AST_StructureFwd::is_defined (void)
{
  return (this->pd_full_definition != 0);
}

void
AST_StructureFwd::destroy (void)
{
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_StructureFwd, AST_Type)
IMPL_NARROW_FROM_DECL (AST_StructureFwd)
