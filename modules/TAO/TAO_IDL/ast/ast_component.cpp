// $Id$

#include "ast_component.h"
#include "ast_attribute.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
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
  if (!this->imported ())
    {
      idl_global->component_seen_ = true;
    }
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

AST_Component::PORTS &
AST_Component::provides (void)
{
  return this->pd_provides;
}

AST_Component::PORTS &
AST_Component::uses (void)
{
  return this->pd_uses;
}

AST_Component::PORTS &
AST_Component::emits (void)
{
  return this->pd_emits;
}

AST_Component::PORTS &
AST_Component::publishes (void)
{
  return this->pd_publishes;
}

AST_Component::PORTS &
AST_Component::consumes (void)
{
  return this->pd_consumes;
}

void
AST_Component::destroy (void)
{
  port_description *pd = 0;
  
  for (PORTS::ITERATOR i = this->pd_provides.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }
    
  for (PORTS::ITERATOR i = this->pd_uses.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }
    
  for (PORTS::ITERATOR i = this->pd_publishes.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }
    
  for (PORTS::ITERATOR i = this->pd_consumes.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }
    
  for (PORTS::ITERATOR i = this->pd_emits.begin ();
       !i.done ();
       i.advance ())
    {
      i.next (pd);
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }
    
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

AST_Provides *
AST_Component::fe_add_provides (AST_Provides *p)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (p, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      p,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, p->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      p,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (p);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (p,
                           false,
                           p->local_name ());

  return p;
}

AST_Uses *
AST_Component::fe_add_uses (AST_Uses *u)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (u, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      u,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, u->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      u,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (u);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (u,
                           false,
                           u->local_name ());

  return u;
}

AST_Publishes *
AST_Component::fe_add_publishes (AST_Publishes *p)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (p, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      p,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, p->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      p,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (p);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (p,
                           false,
                           p->local_name ());

  return p;
}

AST_Emits *
AST_Component::fe_add_emits (AST_Emits *e)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (e, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      e,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, e->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      e,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (e);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (e,
                           false,
                           e->local_name ());

  return e;
}

AST_Consumes *
AST_Component::fe_add_consumes (AST_Consumes *c)
{
  AST_Decl *d = 0;

  // Already defined? Or already used?
  if ((d = this->lookup_for_add (c, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      c,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, c->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      c,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (c);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (c,
                           false,
                           c->local_name ());

  return c;
}

IMPL_NARROW_FROM_DECL(AST_Component)
IMPL_NARROW_FROM_SCOPE(AST_Component)
