// $Id$

#include "fe_obv_header.h"

#include "utl_err.h"
#include "utl_namelist.h"

#include "fe_extern.h"
#include "nr_extern.h"
#include "global_extern.h"

#include "ast_valuetype.h"
#include "ast_module.h"
#include "ast_param_holder.h"

// @@@ (JP) Here are the rules for interface inheritance and
// value type inheritance and supports, straight from Jonathan
// Biggar <jon@floorboard.com> as of 3/28/02. The following was
// resolved by the OMG, but is not yet part of an official spec.

/*
An interface can inherit from any number of other interfaces, abstract
or not.

An abstract interface can only inherit from other abstract interfaces.

An abstract valuetype can inherit from any number of abstract
valuetypes.  It may support one interface, and in addition, any number
of abstract interfaces.

A concrete valuetype can inherit from only one concrete valuetype.  It
may inherit from any number of abstract valuetypes.  It may support one
interface, and any number of abstract interfaces.

The single concrete inherited valuetype must be the first one in the
inheritance list.

The single supported interface (for valuetypes) must also be the first
in the "supports" list.

And one more important clarification, if a base valuetype supports an
interface, a derived valuetype may also be declared to support an
interface, as long as it is derived from all interfaces that are
supported by any base valuetypes.  Here is an example:

interface I1 { };
interface I2 { };
interface I3: I1, I2 { };

abstract valuetype V1 supports I1 { };
abstract valuetype V2 supports I2 { };
valuetype V3: V1, V2 supports I3 { }; // legal
valuetype V4: V1 supports I2 { }; // illegal

This last rule was made to guarantee that any given valuetype supported
at most one most-derived interface.  We didn't want valuetypes to extend
the OMG model through the backdoor by providing multiple non-related
interfaces.
*/

FE_OBVHeader::FE_OBVHeader (UTL_ScopedName *n,
                            UTL_NameList *inherits,
                            UTL_NameList *supports,
                            bool truncatable,
                            bool is_eventtype)
  : FE_InterfaceHeader (n,
                        inherits,
                        false,
                        false,
                        false),
    supports_ (0),
    n_supports_ (0),
    inherits_concrete_ (0),
    supports_concrete_ (0),
    truncatable_ (truncatable)
{
  this->compile_inheritance (inherits,
                             is_eventtype);

  if (idl_global->err_count () == 0)
    {
      this->compile_supports (supports);
    }
}

FE_OBVHeader::~FE_OBVHeader (void)
{
}

AST_Type **
FE_OBVHeader::supports (void) const
{
  return this->supports_;
}

long
FE_OBVHeader::n_supports (void) const
{
  return this->n_supports_;
}

AST_Type *
FE_OBVHeader::inherits_concrete (void) const
{
  return this->inherits_concrete_;
}

AST_Type *
FE_OBVHeader::supports_concrete (void) const
{
  return this->supports_concrete_;
}

bool
FE_OBVHeader::truncatable (void) const
{
  return this->truncatable_;
}

void
FE_OBVHeader::destroy (void)
{
  this->FE_InterfaceHeader::destroy ();
}

void
FE_OBVHeader::compile_inheritance (UTL_NameList *vtypes,
                                   bool is_eventtype)
{
  this->FE_InterfaceHeader::compile_inheritance (vtypes,
                                                 true);

  if (this->n_inherits_ > 0)
    {
      AST_Type *t = this->inherits_[0];
      AST_ValueType *vt = AST_ValueType::narrow_from_decl (t);

      if (vt != 0
          && vt->is_abstract () == false)
        {
          this->inherits_concrete_ = vt;
        }

      if (! is_eventtype
          && this->inherits_[0]->node_type () == AST_Decl::NT_eventtype)
        {
          idl_global->err ()->valuetype_expected (this->inherits_[0]);
        }

      for (long i = 1; i < this->n_inherits_; ++i)
        {
          t = this->inherits_[i];

          if (!t->is_abstract ())
            {
              idl_global->err ()->abstract_expected (t);
            }

          if (! is_eventtype
              && t->node_type () == AST_Decl::NT_eventtype)
            {
              idl_global->err ()->valuetype_expected (t);
            }
        }
    }
}

void
FE_OBVHeader::compile_supports (UTL_NameList *supports)
{
  if (supports == 0)
    {
      this->supports_ = 0;
      this->n_supports_ = 0;
      return;
    }

  long length = supports->length ();
  this->n_supports_ = length;

  ACE_NEW (this->supports_,
           AST_Type *[length]);

  AST_Decl *d = 0;
  UTL_ScopedName *item = 0;
  AST_Interface *iface = 0;
  AST_Type *t = 0;
  int i = 0;

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
          idl_global->err ()->supports_error (this->interface_name_,
                                              d);
          continue;
        }

      // Forward declared interface?
      if (iface != 0 && !iface->is_defined ())
        {
          idl_global->err ()->supports_fwd_error (this->interface_name_,
                                                  iface);
          continue;
        }

      if (iface != 0 && !iface->is_abstract ())
        {
          if (i == 0)
            {
              this->supports_concrete_ = iface;

              if (!this->check_concrete_supported_inheritance (iface))
                {
                  idl_global->err ()->concrete_supported_inheritance_error (
                                          this->name (),
                                          iface->name ()
                                        );
                }
            }
          else
            {
              idl_global->err ()->abstract_expected (iface);
              continue;
            }
        }

      this->supports_[i++] = t;
    }
}

bool
FE_OBVHeader::check_concrete_supported_inheritance (AST_Interface *d)
{
  if (this->n_inherits_ == 0)
    {
      return true;
    }

  AST_ValueType *vt = 0;
  AST_Type *concrete = 0;
  AST_Interface *ancestor = 0;

  for (long i = 0; i < this->n_inherits_; ++i)
    {
      vt = AST_ValueType::narrow_from_decl (this->inherits_[i]);
      concrete = vt->supports_concrete ();

      if (0 == concrete)
        {
          return true;
        }

      if (d == concrete)
        {
          return true;
        }

      for (long j = 0; j < d->n_inherits_flat (); ++j)
        {
          ancestor = d->inherits_flat ()[j];

          if (ancestor == concrete)
            {
              return true;
            }
        }
    }

  return false;
}

