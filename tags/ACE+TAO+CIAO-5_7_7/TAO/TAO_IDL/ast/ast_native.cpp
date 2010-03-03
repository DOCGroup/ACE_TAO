// $Id$

#include "ast_native.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Native::NT = AST_Decl::NT_native;

AST_Native::AST_Native (UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_native,
              n),
    AST_Type (AST_Decl::NT_native,
              n),
    AST_ConcreteType (AST_Decl::NT_native,
                      n),
    UTL_Scope (AST_Decl::NT_native),
    AST_Structure (AST_Decl::NT_native,
                   n,
                   true,
                   false),
    AST_Exception (n,
                   true,
                   false)
{
}

AST_Native::~AST_Native (void)
{
}

void
AST_Native::destroy (void)
{
  this->AST_Exception::destroy ();
}

// Dump this AST_Native node to the ostream o.
void
AST_Native::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Decl::dump (o);
}

int
AST_Native::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_native (this);
}



IMPL_NARROW_FROM_DECL(AST_Native)
