// $Id$

#include "ast_valuetype_fwd.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

ACE_RCSID( ast, 
           ast_valuetype_fwd, 
           "$Id$")

AST_ValueTypeFwd::AST_ValueTypeFwd (void)
{
}

AST_ValueTypeFwd::AST_ValueTypeFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : AST_InterfaceFwd (dummy,
                      n),
    AST_Type (AST_Decl::NT_valuetype_fwd,
              n),
    AST_Decl (AST_Decl::NT_valuetype_fwd,
              n),
    COMMON_Base (I_FALSE,
                 I_FALSE)
{
}

AST_ValueTypeFwd::~AST_ValueTypeFwd (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_InterfaceFwd node to the ostream o.
void
AST_ValueTypeFwd::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_abstract ())
    {
      o << "abstract ";
    }

  o << "valuetype ";

  this->local_name ()->dump (o);
}

int
AST_ValueTypeFwd::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_valuetype_fwd (this);
}

void
AST_ValueTypeFwd::destroy (void)
{
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_ValueTypeFwd, AST_InterfaceFwd)
IMPL_NARROW_FROM_DECL (AST_ValueTypeFwd)
