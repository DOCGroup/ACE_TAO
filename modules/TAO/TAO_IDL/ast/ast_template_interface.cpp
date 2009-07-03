// $Id$

#include "ast_template_interface.h"

ACE_RCSID (ast,
           ast_template_interface,
           "$Id$")

AST_Template_Interface::AST_Template_Interface (
      UTL_ScopedName *n,
      AST_Interface **ih,
      long nih,
      AST_Interface **ih_flat,
      long nih_flat,
      const PARAMLIST_INFO &template_params)
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
    template_params_ (template_params)
{
}

AST_Template_Interface::~AST_Template_Interface (void)
{
}

void
AST_Template_Interface::destroy (void)
{
  this->AST_Interface::destroy ();
}
