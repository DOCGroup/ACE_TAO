// $Id$

#include "ast_component.h"

ACE_RCSID (ast, 
           ast_component, 
           "$Id$")

// Constructor(s) and destructor.

AST_Component::AST_Component (void)
  : parent_ (0)
{
}

AST_Component::AST_Component (UTL_ScopedName *n,
                              AST_Component *parent,
                              AST_Interface **supports,
                              long n_supports,
                              AST_Interface **supports_flat,
                              long n_supports_flat)
  : AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   I_FALSE,
                   I_FALSE),
    AST_Type (AST_Decl::NT_component,
              n),
    AST_Decl (AST_Decl::NT_component,
              n),
    UTL_Scope (AST_Decl::NT_component),
    COMMON_Base (I_FALSE,
                 I_FALSE),
    parent_ (parent)
{
}

AST_Component::~AST_Component (void)
{
}

AST_Component *
AST_Component::parent (void)
{
  return this->parent_;
}
