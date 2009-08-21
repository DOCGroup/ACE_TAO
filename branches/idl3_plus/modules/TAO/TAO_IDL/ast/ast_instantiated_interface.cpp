// $Id$

#include "ast_instantiated_interface.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_namelist.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_instantiated_interface,
           "$Id$")

AST_Instantiated_Interface::AST_Instantiated_Interface (
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
    template_ref_ (template_ref),
    template_args_ (template_args)
{
}

AST_Instantiated_Interface::~AST_Instantiated_Interface (void)
{
}

void
AST_Instantiated_Interface::destroy (void)
{
  delete this->template_args_;
  this->template_args_ = 0;
  
  this->AST_Interface::destroy ();
}

void
AST_Instantiated_Interface::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "interface ");

  this->local_name ()->dump (o);
  this->dump_i (o, "<");
//  unsigned long index = 0UL;

  // TODO - template args

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}\n");
}

int
AST_Instantiated_Interface::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_instantiated_interface (this);
}

IMPL_NARROW_FROM_DECL (AST_Instantiated_Interface)
IMPL_NARROW_FROM_SCOPE (AST_Instantiated_Interface)

