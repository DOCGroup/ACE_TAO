// $Id$

#include "ast_eventtype_fwd.h"
#include "ast_interface.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "ace/streams.h"

ACE_RCSID( ast, 
           ast_eventtype_fwd, 
           "$Id$")

AST_EventTypeFwd::AST_EventTypeFwd (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_InterfaceFwd (),
    AST_ValueTypeFwd ()
{
}

AST_EventTypeFwd::AST_EventTypeFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (I_FALSE,
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
      o << "abstract ";
    }

  o << "eventtype ";

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

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_EventTypeFwd, AST_ValueTypeFwd)
IMPL_NARROW_FROM_DECL (AST_EventTypeFwd)
