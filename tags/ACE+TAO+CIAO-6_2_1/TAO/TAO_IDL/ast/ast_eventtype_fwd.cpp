// $Id$

#include "ast_eventtype_fwd.h"
#include "ast_interface.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

AST_Decl::NodeType const
AST_EventTypeFwd::NT = AST_Decl::NT_eventtype_fwd;

AST_EventTypeFwd::AST_EventTypeFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (false,
                 dummy->is_abstract ()),
    AST_Decl (AST_Decl::NT_eventtype_fwd,
              n),
    AST_Type (AST_Decl::NT_eventtype_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n),
    AST_ValueTypeFwd (dummy,
                      n)
{
}

AST_EventTypeFwd::~AST_EventTypeFwd (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_InterfaceFwd node to the ostream o.
void
AST_EventTypeFwd::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_abstract ())
    {
      this->dump_i (o, "abstract ");
    }

  this->dump_i (o, "eventtype ");

  this->local_name ()->dump (o);
}

int
AST_EventTypeFwd::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_eventtype_fwd (this);
}

void
AST_EventTypeFwd::destroy (void)
{
}



IMPL_NARROW_FROM_DECL (AST_EventTypeFwd)
