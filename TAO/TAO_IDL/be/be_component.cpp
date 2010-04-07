
//=============================================================================
/**
 *  @file    be_component.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Component that provides additional means for C++
 *  mapping of an interface.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_component.h"
#include "be_visitor.h"

#include "ast_mirror_port.h"
#include "ast_uses.h"

#include "global_extern.h"
#include "utl_err.h"

be_component::be_component (UTL_ScopedName *n,
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
    AST_Component (n,
                   base_component,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat),
    be_scope (AST_Decl::NT_component),
    be_decl (AST_Decl::NT_component,
             n),
    be_type (AST_Decl::NT_component,
             n),
    be_interface (n,
                  supports,
                  n_supports,
                  supports_flat,
                  n_supports_flat,
                  false,
                  false),
    has_provides_ (false),
    has_uses_ (false),
    has_uses_multiple_ (false),
    has_publishes_ (false),
    has_consumes_ (false),
    has_emits_ (false),
    has_attributes_ (false)
{
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (true);
}

be_component::~be_component (void)
{
}

be_component *
be_component::base_component (void) const
{
  return
    be_component::narrow_from_decl (
      this->AST_Component::base_component ());
}

void
be_component::redefine (AST_Interface *from)
{
  be_component *bc = be_component::narrow_from_decl (from);
  this->var_out_seq_decls_gen_ = bc->var_out_seq_decls_gen_;
  AST_Component::redefine (from);
}

void
be_component::destroy ()
{
  this->be_interface::destroy ();
  this->AST_Component::destroy ();
}

int
be_component::accept (be_visitor *visitor)
{
  return (idl_global->ignore_idl3 ()
            ? 0
            : visitor->visit_component (this));
}

AST_Structure *
be_component::be_add_structure (AST_Structure *t)
{
  return this->fe_add_structure (t);
}

AST_Typedef *
be_component::be_add_typedef (AST_Typedef *t)
{
  return this->fe_add_typedef (t);
}

bool
be_component::has_provides (void)
{
  return this->has_provides_;
}

bool
be_component::has_uses (void)
{
  return this->has_uses_;
}

bool
be_component::has_uses_multiple (void)
{
  return this->has_uses_multiple_;
}

bool
be_component::has_publishes (void)
{
  return this->has_publishes_;
}

bool
be_component::has_consumes (void)
{
  return this->has_consumes_;
}

bool
be_component::has_emits (void)
{
  return this->has_emits_;
}

bool
be_component::has_attributes (void)
{
  return this->has_attributes_;
}

IMPL_NARROW_FROM_DECL (be_component)
IMPL_NARROW_FROM_SCOPE (be_component)

void
be_component::scan (UTL_Scope *s)
{
  if (s == 0)
    {
      return;
    }

  for (UTL_ScopeActiveIterator i (s, UTL_Scope::IK_both);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Extended_Port *ep = 0;
      AST_Mirror_Port *mp = 0;
      AST_Uses *u = 0;

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            this->has_provides_ = true;
            continue;
          case AST_Decl::NT_uses:
            this->has_uses_ = true;
            u = AST_Uses::narrow_from_decl (d);
            
            if (u->is_multiple ())
              {
                this->has_uses_multiple_ = true;
              }
              
            continue;
          case AST_Decl::NT_publishes:
            this->has_publishes_ = true;
            continue;
          case AST_Decl::NT_consumes:
            this->has_consumes_ = true;
            continue;
          case AST_Decl::NT_emits:
            this->has_emits_ = true;
            continue;
          case AST_Decl::NT_ext_port:
            ep = AST_Extended_Port::narrow_from_decl (d);
            this->scan (ep->port_type ());
            continue;
          case AST_Decl::NT_mirror_port:
            mp = AST_Mirror_Port::narrow_from_decl (d);
            this->mirror_scan (mp->port_type ());
            continue;
          case AST_Decl::NT_attr:
            this->has_attributes_ = true;
            continue;
          default:
            continue;
        }
    }

  AST_Component *c = AST_Component::narrow_from_scope (s);

  if (c != 0)
    {
      this->scan (c->base_component ());
    }
}

void
be_component::mirror_scan (AST_PortType *p)
{
  for (UTL_ScopeActiveIterator i (p, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            this->has_uses_ = true;
            continue;
          case AST_Decl::NT_uses:
            this->has_provides_ = true;
            continue;
          default:
            continue;
        }
    }
}

