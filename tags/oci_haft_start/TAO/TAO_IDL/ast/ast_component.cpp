// $Id$

#include "ast_component.h"
#include "ast_attribute.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "global_extern.h"

ACE_RCSID (ast, 
           ast_component, 
           "$Id$")

AST_Component::AST_Component (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    AST_Interface (),
    pd_base_component (0)
{
}

AST_Component::AST_Component (UTL_ScopedName *n,
                              AST_Component *base_component,
                              AST_Interface **supports,
                              long n_supports,
                              AST_Interface **supports_flat,
                              long n_supports_flat)
  : COMMON_Base (I_FALSE,
                 I_FALSE),
    AST_Decl (AST_Decl::NT_component,
              n),
    AST_Type (AST_Decl::NT_component,
              n),
    UTL_Scope (AST_Decl::NT_component),
    AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   I_FALSE,
                   I_FALSE),
    pd_base_component (base_component)
{
}

AST_Component::~AST_Component (void)
{
}

void
AST_Component::redefine (AST_Interface *from)
{
  AST_Component *c = AST_Component::narrow_from_decl (from);

  if (c == 0)
    {
      idl_global->err ()->redef_error (from->local_name ()->get_string (),
                                       this->local_name ()->get_string ());
      return;
    }

  // Copy over all the base class members.
  this->AST_Interface::redefine (from);

  this->pd_base_component = c->pd_base_component;
  this->pd_provides = c->pd_provides;
  this->pd_uses = c->pd_uses;
  this->pd_emits = c->pd_emits;
  this->pd_publishes = c->pd_publishes;
  this->pd_consumes = c->pd_consumes;
}

AST_Component *
AST_Component::base_component (void) const
{
  return this->pd_base_component;
}

AST_Interface **
AST_Component::supports (void) const
{
  return this->inherits ();
}

long 
AST_Component::n_supports (void) const
{
  return this->n_inherits ();
}

ACE_Unbounded_Queue<AST_Component::port_description> &
AST_Component::provides (void)
{
  return this->pd_provides;
}

ACE_Unbounded_Queue<AST_Component::port_description> &
AST_Component::uses (void)
{
  return this->pd_uses;
}

ACE_Unbounded_Queue<AST_Component::port_description> &
AST_Component::emits (void)
{
  return this->pd_emits;
}

ACE_Unbounded_Queue<AST_Component::port_description> &
AST_Component::publishes (void)
{
  return this->pd_publishes;
}

ACE_Unbounded_Queue<AST_Component::port_description> &
AST_Component::consumes (void)
{
  return this->pd_consumes;
}

void
AST_Component::destroy (void)
{
  this->AST_Interface::destroy ();
}

void
AST_Component::dump (ACE_OSTREAM_TYPE &o)
{
  o << "component ";

  this->local_name ()->dump (o);

  o << " ";

  if (this->pd_base_component != 0)
    {
      o << ": ";
      this->pd_base_component->local_name ()->dump (o);
    }

  if (this->pd_n_inherits > 0)
    {
      o << "supports ";

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              o << ", ";
            }
        }
    }

  o << " {\n";

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  o << "}";
}

int
AST_Component::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_component (this);
}

idl_bool
AST_Component::redef_clash (void)
{
  if (this->AST_Interface::redef_clash ())
    {
      return 1;
    }

  return 0;
}

// Narrowing methods.
IMPL_NARROW_METHODS1(AST_Component, AST_Interface)
IMPL_NARROW_FROM_DECL(AST_Component)
IMPL_NARROW_FROM_SCOPE(AST_Component)
