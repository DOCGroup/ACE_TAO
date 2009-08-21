// $Id$

#include "be_instantiated_interface.h"
#include "be_visitor.h"

ACE_RCSID (be,
           be_instantiated_interface,
           "$Id$")

be_instantiated_interface::be_instantiated_interface (
      UTL_ScopedName *n,
      AST_Template_Interface *template_ref,
      AST_Template_Common::T_ARGLIST *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_interface,
              n),
    AST_Type (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    AST_Interface (n,
                   0,
                   0,
                   0,
                   0,
                   false,
                   false),
    AST_Instantiated_Interface (n,
                                template_ref,
                                template_args)
{
}

be_instantiated_interface::~be_instantiated_interface (void)
{
}

void
be_instantiated_interface::destroy (void)
{
  this->AST_Instantiated_Interface::destroy ();
}

int
be_instantiated_interface::accept (be_visitor *visitor)
{
  return visitor->visit_instantiated_interface (this);
}

IMPL_NARROW_FROM_DECL (be_instantiated_interface)
IMPL_NARROW_FROM_SCOPE (be_instantiated_interface)

