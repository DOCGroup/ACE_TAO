// $Id$

#include "ast_component.h"
#include "ast_attribute.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_mirror_port.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "global_extern.h"

AST_Decl::NodeType const
AST_Component::NT = AST_Decl::NT_component;

AST_Component::AST_Component (UTL_ScopedName *n,
                              AST_Component *base_component,
                              AST_Type **supports,
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
}

AST_Decl *
AST_Component::look_in_inherited (UTL_ScopedName *e,
                                  bool full_def_only)
{
  AST_Decl *d = 0;

  if (this->pd_base_component != 0)
    {
      d =
        this->pd_base_component->lookup_by_name_r (
          e,
          full_def_only);
    }

  return d;
}

// Look through supported interface list.
AST_Decl *
AST_Component::look_in_supported (UTL_ScopedName *e,
                                  bool full_def_only)
{
  AST_Decl *d = 0;
  AST_Type **is = 0;
  long nis = -1;

  // Can't look in an interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->fwd_decl_lookup (this, e);
      return 0;
    }

  // OK, loop through supported interfaces.

  for (nis = this->n_supports (), is = this->supports ();
       nis > 0;
       nis--, is++)
    {
      if ((*is)->node_type () == AST_Decl::NT_param_holder)
        {
          continue;
        }

      AST_Interface *i =
        AST_Interface::narrow_from_decl (*is);

      d = (i)->lookup_by_name_r (e, full_def_only);

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

AST_Type **
AST_Component::supports (void) const
{
  return this->inherits ();
}

long
AST_Component::n_supports (void) const
{
  return this->n_inherits ();
}

AST_Decl *
AST_Component::special_lookup (UTL_ScopedName *e,
                               bool full_def_only)
{
  AST_Decl *d = this->look_in_inherited (e, full_def_only);

  if (d == 0)
    {
      d = this->look_in_supported (e, full_def_only);
    }

  return d;
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

AST_Provides *
AST_Component::fe_add_provides (AST_Provides *p)
{
  return
    AST_Provides::narrow_from_decl (
      this->fe_add_ref_decl (p));
}

AST_Uses *
AST_Component::fe_add_uses (AST_Uses *u)
{
  return
    AST_Uses::narrow_from_decl (
      this->fe_add_ref_decl (u));
}

AST_Publishes *
AST_Component::fe_add_publishes (AST_Publishes *p)
{
  return
    AST_Publishes::narrow_from_decl (
      this->fe_add_ref_decl (p));
}

AST_Emits *
AST_Component::fe_add_emits (AST_Emits *e)
{
  return
    AST_Emits::narrow_from_decl (
      this->fe_add_ref_decl (e));
}

AST_Consumes *
AST_Component::fe_add_consumes (AST_Consumes *c)
{
  return
    AST_Consumes::narrow_from_decl (
      this->fe_add_ref_decl (c));
}

AST_Extended_Port *
AST_Component::fe_add_extended_port (AST_Extended_Port *p)
{
  return
    AST_Extended_Port::narrow_from_decl (
      this->fe_add_ref_decl (p));
}

AST_Mirror_Port *
AST_Component::fe_add_mirror_port (AST_Mirror_Port *p)
{
  return
    AST_Mirror_Port::narrow_from_decl (
      this->fe_add_ref_decl (p));
}

int
AST_Component::be_add_uses (AST_Uses *i,
                            AST_Uses *ix)
{
  // Add it to scope.
  this->add_to_scope (i,
                      ix);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (i,
                           false,
                           i->local_name (),
                           ix);

  return 0;
}

IMPL_NARROW_FROM_DECL (AST_Component)
IMPL_NARROW_FROM_SCOPE (AST_Component)
