// $Id$

#include "ast_home.h"
#include "ast_component.h"
#include "ast_valuetype.h"
#include "ast_operation.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast, 
           ast_home, 
           "$Id$")

AST_Home::AST_Home (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    pd_base_home (0),
    pd_managed_component (0),
    pd_primary_key (0)
{
}

AST_Home::AST_Home (UTL_ScopedName *n,
                    AST_Home *base_home,
                    AST_Component *managed_component,
                    AST_ValueType *primary_key,
                    AST_Interface **supports,
                    long n_supports,
                    AST_Interface **supports_flat,
                    long n_supports_flat)
  : COMMON_Base (I_FALSE,
                 I_FALSE),
    AST_Decl (AST_Decl::NT_home,
              n),
    AST_Type (AST_Decl::NT_home,
              n),
    UTL_Scope (AST_Decl::NT_home),
    AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   I_FALSE,
                   I_FALSE),
    pd_base_home (base_home),
    pd_managed_component (managed_component),
    pd_primary_key (primary_key)
{
}

AST_Home::~AST_Home (void)
{
}

AST_Home *
AST_Home::base_home (void) const
{
  return this->pd_base_home;
}

// These next two look ugly, but it is to keep from having to
// create separate visitors for homes in the back end.

AST_Interface **
AST_Home::supports (void) const
{
  return this->pd_base_home ? this->inherits () + 1 : this->inherits ();
}

long 
AST_Home::n_supports (void) const
{
  return this->n_inherits ();
}

AST_Component *
AST_Home::managed_component (void) const
{
  return this->pd_managed_component;
}

AST_ValueType *
AST_Home::primary_key (void) const
{
  return this->pd_primary_key;
}

ACE_Unbounded_Queue<AST_Operation *> &
AST_Home::factories (void)
{
  return this->pd_factories;
}

ACE_Unbounded_Queue<AST_Operation *> &
AST_Home::finders (void)
{
  return this->pd_finders;
}

void
AST_Home::destroy (void)
{
}

void
AST_Home::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "home ");

  this->local_name ()->dump (o);

  this->dump_i (o, " ");

  if (this->pd_base_home != 0)
    {
      this->dump_i (o, ": ");
      this->pd_base_home->local_name ()->dump (o);
    }

  if (this->pd_managed_component != 0)
    {
      this->dump_i (o, "\n");
      this->dump_i (o, "manages ");
      this->pd_managed_component->local_name ()->dump (o);
    }

  if (this->pd_primary_key != 0)
    {
      this->dump_i (o, "\n");
      this->dump_i (o, "primary key ");
      this->pd_primary_key->local_name ()->dump (o);
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

int
AST_Home::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_home (this);
}

  // Narrowing.
IMPL_NARROW_METHODS1(AST_Home, AST_Interface)
IMPL_NARROW_FROM_DECL(AST_Home)
IMPL_NARROW_FROM_SCOPE(AST_Home)

