// $Id$

#include "ast_home.h"
#include "ast_component.h"
#include "ast_valuetype.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast, 
           ast_home, 
           "$Id$")

AST_Home::AST_Home (void)
  : pd_base_home (0),
    pd_managed_component (0),
    pd_primary_key (0)
{
}

AST_Home::AST_Home (UTL_ScopedName *n,
                    AST_Home *base_home,
                    AST_Component *managed_component,
                    AST_ValueType *primary_key)
  : AST_Interface (n,
                   0,
                   0,
                   0,
                   0,
                   I_TRUE,
                   I_FALSE),
    AST_Type (AST_Decl::NT_home,
              n),
    AST_Decl (AST_Decl::NT_home,
              n),
    UTL_Scope (AST_Decl::NT_home),
    COMMON_Base (I_TRUE,
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

void 
AST_Home::factories (AST_Operation *d)
{
  this->pd_factories.enqueue_tail (d);
}

ACE_Unbounded_Queue<AST_Operation *> &
AST_Home::finders (void)
{
  return this->pd_finders;
}

void 
AST_Home::finders (AST_Operation *d)
{
  this->pd_finders.enqueue_tail (d);
}

void
AST_Home::destroy (void)
{
  this->AST_Interface::destroy ();
}

void
AST_Home::dump (ACE_OSTREAM_TYPE &o)
{
  o << "home ";

  this->local_name ()->dump (o);

  o << " ";

  if (this->pd_base_home != 0)
    {
      o << ": ";
      this->pd_base_home->local_name ()->dump (o);
    }

  if (this->pd_managed_component != 0)
    {
      o << "\n";
      o << "manages ";
      this->pd_managed_component->local_name ()->dump (o);
    }

  if (this->pd_primary_key != 0)
    {
      o << "\n";
      o << "primary key ";
      this->pd_primary_key->local_name ()->dump (o);
    }

  o << " {\n";

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  o << "}";
}

int
AST_Home::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_home (this);
}

  // Narrowing.
IMPL_NARROW_METHODS1(AST_Home, AST_Interface);
IMPL_NARROW_FROM_DECL(AST_Home);
IMPL_NARROW_FROM_SCOPE(AST_Home);

