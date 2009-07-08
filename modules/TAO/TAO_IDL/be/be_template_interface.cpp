// $Id$

#include "be_template_interface.h"
#include "be_visitor.h"

ACE_RCSID (be,
           be_template_interface,
           "$Id$")

be_template_interface::be_template_interface (
      UTL_ScopedName *n,
      AST_Interface **ih,
      long nih,
      AST_Interface **ih_flat,
      long nih_flat,
      const FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_interface,
              n),
    AST_Type (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    AST_Interface (n,
                   ih,
                   nih,
                   ih_flat,
                   nih_flat,
                   false,
                   false),
    AST_Template_Interface (n,
                            ih,
                            nih,
                            ih_flat,
                            nih_flat,
                            template_params),
    be_scope (AST_Decl::NT_interface),
    be_type (AST_Decl::NT_interface,
             n)
{
}

be_template_interface::~be_template_interface (void)
{
}

void
be_template_interface::destroy (void)
{
  this->AST_Template_Interface::destroy ();
  this->be_scope::destroy ();
  this->be_type::destroy ();
}

int
be_template_interface::accept(be_visitor *visitor)
{
  return visitor->visit_template_interface (this);
}

