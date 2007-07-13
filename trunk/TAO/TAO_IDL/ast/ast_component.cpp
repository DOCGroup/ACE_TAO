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
  : COMMON_Base (false,
                 false),
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
                   false,
                   false),
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

AST_Decl *
AST_Component::look_in_inherited (UTL_ScopedName *e,
                                  bool treat_as_ref)
{
  AST_Decl *d = 0;

  if (this->pd_base_component != 0)
    {
      d = this->pd_base_component->lookup_by_name (e, treat_as_ref);
    }

  return d;
}

// Look through supported interface list.
AST_Decl *
AST_Component::look_in_supported (UTL_ScopedName *e,
                                  bool treat_as_ref)
{
  AST_Decl *d = 0;
  AST_Interface **is = 0;
  long nis = -1;

  // Can't look in an interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->fwd_decl_lookup (this,
                                           e);
      return 0;
    }

  // OK, loop through supported interfaces.

  // (Don't leave the inheritance hierarchy, no module or global ...)
  // Find all and report ambiguous results as error.

  for (nis = this->n_supports (), is = this->supports ();
       nis > 0;
       nis--, is++)
    {
      d = (*is)->lookup_by_name (e,
                                 treat_as_ref,
                                 0 /* not in parent */);
      if (d != 0)
        {
          break;
        }
    }

  return d;
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
  this->dump_i (o, "component ");

  this->local_name ()->dump (o);

  this->dump_i (o, " ");

  if (this->pd_base_component != 0)
    {
      this->dump_i (o, ": ");
      this->pd_base_component->local_name ()->dump (o);
    }

  if (this->pd_n_inherits > 0)
    {
      this->dump_i (o, "supports ");

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              this->dump_i (o, ", ");
            }
        }
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

int
AST_Component::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_component (this);
}

bool
AST_Component::redef_clash (void)
{
  if (this->AST_Interface::redef_clash ())
    {
      return 1;
    }

  return 0;
}



IMPL_NARROW_FROM_DECL(AST_Component)
IMPL_NARROW_FROM_SCOPE(AST_Component)
