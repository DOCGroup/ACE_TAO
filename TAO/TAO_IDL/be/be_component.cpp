
//=============================================================================
/**
 *  @file    be_component.cpp
 *
 *  Extension of class AST_Component that provides additional means for C++
 *  mapping of an interface.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_component.h"
#include "be_visitor.h"
#include "be_helper.h"

#include "ast_mirror_port.h"
#include "ast_uses.h"
#include "ast_provides.h"
#include "ast_attribute.h"

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
    n_provides_ (0UL),
    n_remote_provides_ (0UL),
    n_uses_ (0UL),
    n_remote_uses_ (0UL),
    has_uses_multiple_ (false),
    n_publishes_ (0UL),
    n_consumes_ (0UL),
    n_emits_ (0UL)
{
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (true);
}

be_component::~be_component ()
{
}

be_component *
be_component::base_component () const
{
  return dynamic_cast<be_component*> (this->AST_Component::base_component ());
}

void
be_component::redefine (AST_Interface *from)
{
  be_component *bc = dynamic_cast<be_component*> (from);
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

ACE_CDR::ULong
be_component::n_provides () const
{
  return this->n_provides_;
}

ACE_CDR::ULong
be_component::n_remote_provides () const
{
  return this->n_remote_provides_;
}

ACE_CDR::ULong
be_component::n_uses () const
{
  return this->n_uses_;
}

ACE_CDR::ULong
be_component::n_remote_uses () const
{
  return this->n_remote_uses_;
}

bool
be_component::has_uses_multiple () const
{
  return this->has_uses_multiple_;
}

ACE_CDR::ULong
be_component::n_publishes () const
{
  return this->n_publishes_;
}

ACE_CDR::ULong
be_component::n_consumes () const
{
  return this->n_consumes_;
}

ACE_CDR::ULong
be_component::n_emits () const
{
  return this->n_emits_;
}

void
be_component::scan (UTL_Scope *s)
{
  if (s == nullptr)
    {
      return;
    }

  AST_Extended_Port *ep = nullptr;
  AST_Mirror_Port *mp = nullptr;
  AST_Uses *u = nullptr;
  AST_Provides *p = nullptr;
  AST_Attribute *a = nullptr;
  AST_Decl::NodeType my_nt;
  AST_Decl::NodeType scope_nt;

  for (UTL_ScopeActiveIterator i (s, UTL_Scope::IK_both);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            ++this->n_provides_;
            p = dynamic_cast<AST_Provides*> (d);

            if (!p->provides_type ()->is_local ())
              {
                ++this->n_remote_provides_;
              }

            continue;
          case AST_Decl::NT_uses:
            ++this->n_uses_;
            u = dynamic_cast<AST_Uses*> (d);

            if (u->is_multiple ())
              {
                this->has_uses_multiple_ = true;
              }

            if (!u->uses_type ()->is_local ())
              {
                ++this->n_remote_uses_;
              }

            continue;
          case AST_Decl::NT_publishes:
            ++this->n_publishes_;
            continue;
          case AST_Decl::NT_consumes:
            ++this->n_consumes_;
            continue;
          case AST_Decl::NT_emits:
            ++this->n_emits_;
            continue;
          case AST_Decl::NT_ext_port:
            ep = dynamic_cast<AST_Extended_Port*> (d);
            this->scan (ep->port_type ());
            continue;
          case AST_Decl::NT_mirror_port:
            mp = dynamic_cast<AST_Mirror_Port*> (d);
            this->mirror_scan (mp->port_type ());
            continue;
          case AST_Decl::NT_attr:
            a = dynamic_cast<AST_Attribute*> (d);

            if (!a->readonly ())
              {
                my_nt = this->node_type ();
                scope_nt =
                  ScopeAsDecl (a->defined_in ())->node_type ();

                /// Attributes coming from a porttype appear
                /// only on connectors.
                if (my_nt == AST_Decl::NT_component
                    && scope_nt == AST_Decl::NT_porttype)
                  {
                    continue;
                  }

                this->has_rw_attributes_ = true;
              }

            continue;
          default:
            continue;
        }
    }

  AST_Component *c = dynamic_cast<AST_Component*> (s);
  AST_Interface *iface = nullptr;

  if (c != nullptr)
    {
      for (long i = 0; i < c->n_supports (); ++i)
        {
          // See if the supported interfaces (if any) have attributes.
          // If CORBA::Object is supported, DeclAsScope will evaluate
          // to 0 and the call to scan() will return immediately.
          this->scan (DeclAsScope (c->supports ()[i]));
        }

      // Check the base component. If there is none, the arg to scan()
      // will be 0 and the call will return immediately.
      this->scan (c->base_component ());
    }
  else if ((iface = dynamic_cast<AST_Interface*> (s)) != nullptr)
    {
      for (long i = 0; i < iface->n_inherits (); ++i)
        {
          // Will pick up a chain of inheritance,
          // no need to use inherits_flat().
          this->scan (DeclAsScope (iface->inherits ()[i]));
        }
    }
}

void
be_component::gen_stub_inheritance (TAO_OutStream *os)
{
  this->analyze_parentage ();

  *os << "public virtual ::";

  AST_Component *parent = this->base_component ();

  if (parent != nullptr)
    {
      *os << parent->name ();
    }
  else
    {
      *os << "Components::CCMObject";
    }

  long nsupports = this->n_inherits ();

  if (nsupports > 0)
    {
      *os << be_idt;

      AST_Type **supp_list = this->supports ();

      for (long i = 0; i < nsupports; ++i)
        {
          *os << "," << be_nl
              << "public virtual ::"
              << supp_list[i]->name ();
        }

      *os << be_uidt;
    }

  *os << be_uidt;
}

void
be_component::gen_skel_inheritance (TAO_OutStream *os)
{
  AST_Component *base = this->base_component ();

  if (base != nullptr)
    {
      *os << "public virtual POA_" << base->name ();
    }
  else
    {
      *os << "public virtual POA_Components::CCMObject";
    }

  long nsupports = this->n_inherits ();
  AST_Type **supports = this->supports ();
  AST_Type *supported = nullptr;

  for (long i = 0; i < nsupports; ++i)
    {
      supported = supports[i];

      if (supported->is_abstract ())
        {
          continue;
        }

      *os << "," << be_nl;
      *os << "public virtual POA_" << supported->name ();
    }
}

int
be_component::gen_is_a_ancestors (TAO_OutStream *os)
{
  AST_Component *ancestor = this;

  while (ancestor != nullptr)
    {
      *os << "std::strcmp (value, \"" << ancestor->repoID () << "\") == 0 ||" << be_nl;

      ancestor = ancestor->base_component ();
    }

  *os << "std::strcmp (value, \"IDL:omg.org/Components/CCMObject:1.0\") == 0" << be_nl;

  return 0;
}

void
be_component::mirror_scan (AST_PortType *pt)
{
  AST_Uses *u = nullptr;
  AST_Provides *p = nullptr;
  AST_Attribute *a = nullptr;

  for (UTL_ScopeActiveIterator i (pt, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();

      switch (d->node_type ())
        {
          case AST_Decl::NT_provides:
            ++this->n_uses_;
            p = dynamic_cast<AST_Provides*> (d);

            if (!p->provides_type ()->is_local ())
              {
                ++this->n_remote_uses_;
              }

            continue;
          case AST_Decl::NT_uses:
            ++this->n_provides_;
            u = dynamic_cast<AST_Uses*> (d);

            if (!u->uses_type ()->is_local ())
              {
                ++this->n_remote_provides_;
              }

            continue;
          case AST_Decl::NT_attr:
            a = dynamic_cast<AST_Attribute*> (d);

            if (!a->readonly ())
              {
                this->has_rw_attributes_ = true;
              }

            continue;
          default:
            continue;
        }
    }
}

