// $Id$

#include "fe_component_header.h"

#include "fe_extern.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "utl_err.h"
#include "utl_namelist.h"

#include "ast_component.h"
#include "ast_module.h"
#include "ast_param_holder.h"

FE_ComponentHeader::FE_ComponentHeader (UTL_ScopedName *n,
                                        UTL_ScopedName *base_component,
                                        UTL_NameList *supports,
                                        bool /* compile_now */)
  : FE_InterfaceHeader (n,
                        supports,
                        false,
                        false,
                        false),
    base_component_ (0)
{
  this->compile_inheritance (base_component);
  this->compile_supports (supports);
}

FE_ComponentHeader::~FE_ComponentHeader (void)
{
}

AST_Component *
FE_ComponentHeader::base_component (void) const
{
  return this->base_component_;
}

AST_Type **
FE_ComponentHeader::supports (void) const
{
  return this->inherits_;
}

long
FE_ComponentHeader::n_supports (void) const
{
  return this->n_inherits_;
}

AST_Interface **
FE_ComponentHeader::supports_flat (void) const
{
  return this->inherits_flat_;
}

long
FE_ComponentHeader::n_supports_flat (void) const
{
  return this->n_inherits_flat_;
}

void
FE_ComponentHeader::compile_inheritance (UTL_ScopedName *base_component)
{
  // If there is a base component, look up the decl and assign our member.
  // We also inherit its supported interfaces.
  if (base_component == 0)
    {
      return;
    }

  UTL_Scope *s = idl_global->scopes ().top_non_null ();
  AST_Decl *d = s->lookup_by_name (base_component,
                                   true);

  if (d == 0)
    {
      idl_global->err ()->lookup_error (base_component);

      // This is probably the result of bad IDL.
      // We will crash if we continue from here.
      throw Bailout ();
    }

  if (d->node_type () == AST_Decl::NT_typedef)
    {
      d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
    }

  this->base_component_ = AST_Component::narrow_from_decl (d);

  if (this->base_component_ == 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_USE,
                                  d);
    }
  else if (!this->base_component_->is_defined ())
    {
      idl_global->err ()->inheritance_fwd_error (
                              this->name (),
                              this->base_component_
                            );
      this->base_component_ = 0;
    }
}

void
FE_ComponentHeader::compile_supports (UTL_NameList *supports)
{
  if (supports == 0)
    {
      return;
    }

  AST_Decl *d = 0;
  UTL_ScopedName *item = 0;
  AST_Interface *iface = 0;
  AST_Type *t = 0;
  long j = 0;
  long k = 0;

  // Compute expanded flattened non-repeating list of interfaces
  // which this one inherits from.

  for (UTL_NamelistActiveIterator l (supports); !l.is_done (); l.next ())
    {
      item = l.item ();

      // Check that scope stack is valid.
      if (idl_global->scopes ().top () == 0)
        {
          idl_global->err ()->lookup_error (item);

          // This is probably the result of bad IDL.
          // We will crash if we continue from here.
          throw Bailout ();
        }

      // Look it up.
      UTL_Scope *s = idl_global->scopes ().top ();

      d = s->lookup_by_name  (item, true);

      if (d == 0)
        {
          AST_Decl *sad = ScopeAsDecl (s);

          if (sad->node_type () == AST_Decl::NT_module)
            {
              AST_Module *m = AST_Module::narrow_from_decl (sad);

              d = m->look_in_prev_mods_local (item->last_component ());
            }
        }

      // Not found?
      if (d == 0)
        {
          idl_global->err ()->lookup_error (item);

          // This is probably the result of bad IDL.
          // We will crash if we continue from here.
          throw Bailout ();
        }

      // Remove typedefs, if any.
      if (d->node_type () == AST_Decl::NT_typedef)
        {
          d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
        }

      AST_Decl::NodeType nt = d->node_type ();
      t = AST_Type::narrow_from_decl (d);

      if (nt == AST_Decl::NT_interface)
        {
          iface = AST_Interface::narrow_from_decl (d);

          // Undefined interface?
          if (!iface->is_defined ())
            {
              idl_global->err ()->inheritance_fwd_error (
                this->interface_name_,
                iface);

              continue;
            }

          // Local interface? (illegal for components to support).
          if (iface->is_local ())
            {
              idl_global->err ()->unconstrained_interface_expected (
                this->name (),
                iface->name ());

              continue;
           }
        }
      else if (nt == AST_Decl::NT_param_holder)
        {
          AST_Param_Holder *ph =
            AST_Param_Holder::narrow_from_decl (d);

          nt = ph->info ()->type_;

          if (nt != AST_Decl::NT_type
              && nt != AST_Decl::NT_interface)
            {
              idl_global->err ()->mismatched_template_param (
                ph->info ()->name_.c_str ());

              continue;
            }
        }
      else
        {
          idl_global->err ()->interface_expected (d);
          continue;
        }

      // OK, see if we have to add this to the list of interfaces
      // inherited from.
      this->compile_one_inheritance (t);
    }

  // OK, install in interface header.
  // First the flat list (all ancestors).
  if (this->iused_flat_ > 0)
    {
      ACE_NEW (this->inherits_flat_,
               AST_Interface *[this->iused_flat_]);

      for (j = 0; j < this->iused_flat_; ++j)
        {
          this->inherits_flat_[j] = this->iseen_flat_[j];
        }

      this->n_inherits_flat_ = this->iused_flat_;
    }

  // Then the list of immediate ancestors.
  if (this->iused_ > 0)
    {
      ACE_NEW (this->inherits_,
               AST_Type *[this->iused_]);

      for (k = 0; k < this->iused_; ++k)
        {
          this->inherits_[k] = this->iseen_[k];
        }

      this->n_inherits_ = this->iused_;
    }
}

