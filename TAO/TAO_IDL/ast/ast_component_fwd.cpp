// $Id$

#include "ast_component_fwd.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "ace/streams.h"

ACE_RCSID( ast, 
           ast_component_fwd, 
           "$Id$")

AST_ComponentFwd::AST_ComponentFwd (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_InterfaceFwd ()
{
}

AST_ComponentFwd::AST_ComponentFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (I_TRUE,
                 I_FALSE),
    AST_Decl (AST_Decl::NT_component_fwd,
              n),
    AST_Type (AST_Decl::NT_component_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n)
{
}

AST_ComponentFwd::~AST_ComponentFwd (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_InterfaceFwd node to the ostream o.
void
AST_ComponentFwd::dump (ACE_OSTREAM_TYPE &o)
{
  o << "component ";

  this->local_name ()->dump (o);
}

int
AST_ComponentFwd::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_component_fwd (this);
}

void
AST_ComponentFwd::destroy (void)
{
  this->AST_InterfaceFwd::destroy ();
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_ComponentFwd, AST_InterfaceFwd)
IMPL_NARROW_FROM_DECL (AST_ComponentFwd)
