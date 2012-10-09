// $Id$

#include "ast_valuetype_fwd.h"
#include "ast_interface.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

AST_Decl::NodeType const
AST_ValueTypeFwd::NT = AST_Decl::NT_valuetype_fwd;

AST_ValueTypeFwd::AST_ValueTypeFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (false,
                 dummy->is_abstract ()),
    AST_Decl (AST_Decl::NT_valuetype_fwd,
              n),
    AST_Type (AST_Decl::NT_valuetype_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n)
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
      this->dump_i (o, "abstract ");
    }

  this->dump_i (o, "valuetype ");

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
  this->AST_InterfaceFwd::destroy ();
}



IMPL_NARROW_FROM_DECL (AST_ValueTypeFwd)
