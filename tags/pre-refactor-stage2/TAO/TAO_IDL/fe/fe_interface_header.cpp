// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

// These utility classes are used to store information about a
// node type as the node type is being parsed and before the
// node itself is created.

#include "fe_interface_header.h"
#include "ast_valuetype.h"
#include "ast_component.h"
#include "ast_home.h"
#include "ast_module.h"
#include "utl_namelist.h"
#include "utl_err.h"
#include "global_extern.h"
#include "nr_extern.h"

ACE_RCSID (fe, 
           fe_interface_header, 
           "$Id$")

#undef  INCREMENT
#define INCREMENT 512

// Private storage used to store interfaces seen already in the
// computation of the unique inheritance list.
static AST_Interface **iseen = 0;
static long iallocated = 0;
static long iused = 0;

// Same as above, but the list is flattened and
// includes all ancestors, not just immediate ones.
static AST_Interface **iseen_flat = 0;
static long iallocated_flat = 0;
static long iused_flat = 0;

// Add an interface to an inheritance spec.
static void
add_inheritance (AST_Interface *i)
{
  AST_Interface  **oiseen;

  // Make sure there's space for one more.
  if (iallocated == iused)
    {
      if (iallocated == 0)
        {
          iallocated = INCREMENT;

          ACE_NEW (iseen,
                   AST_Interface *[iallocated]);
        }
      else
        {
          oiseen = iseen;
          iallocated += INCREMENT;

          ACE_NEW (iseen,
                   AST_Interface *[iallocated]);

          for (long k = 0; k < iused; ++k)
            {
              iseen[k] = oiseen[k];
            }

          delete oiseen;
        }
    }

  // OK, now insert it.
  iseen[iused++] = i;
}

// Add an interface to the flat list.
static void
add_inheritance_flat (AST_Interface *i)
{
  AST_Interface **oiseen_flat;

  // Make sure there's space for one more.
  if (iallocated_flat == iused_flat)
    {
      if (iallocated_flat == 0)
        {
          iallocated_flat = INCREMENT;

          ACE_NEW (iseen_flat,
                   AST_Interface *[iallocated_flat]);
        }
      else
        {
          oiseen_flat = iseen_flat;
          iallocated_flat += INCREMENT;

          ACE_NEW (iseen_flat,
                   AST_Interface *[iallocated_flat]);

          for (long k = 0; k < iused_flat; k++)
            {
              iseen_flat[k] = oiseen_flat[k];
            }

          delete oiseen_flat;
        }
    }

  // OK, now insert it.
  iseen_flat[iused_flat++] = i;
}

// Have we already seen this interface?
static long
already_seen (AST_Interface *ip)
{
  for (long i = 0; i < iused; ++i)
    {
      if (iseen[i] == ip)
        {
          return I_TRUE;
        }
    }

  return I_FALSE;
}

// Have we already seen this interface in the flat list?
static long
already_seen_flat (AST_Interface *ip)
{
  for (long i = 0; i < iused_flat; ++i)
    {
      if (iseen_flat[i] == ip)
        {
          return I_TRUE;
        }
    }

  return I_FALSE;
}

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

FE_InterfaceHeader::FE_InterfaceHeader (UTL_ScopedName *n,
                                        UTL_NameList *inherits,
                                        idl_bool is_local,
                                        idl_bool is_abstract,
                                        idl_bool compile_now)
  : pd_interface_name (n),
    pd_inherits (0),
    pd_n_inherits (0),
    pd_inherits_flat (0),
    pd_n_inherits_flat (0),
    pd_is_local (is_local),
    pd_is_abstract (is_abstract)
{
  if (compile_now)
    {
      this->compile_inheritance (inherits,
                                 I_FALSE);
    }
}

FE_InterfaceHeader::~FE_InterfaceHeader (void)
{
}

idl_bool
FE_InterfaceHeader::is_local (void) const
{
  return this->pd_is_local;
}

idl_bool
FE_InterfaceHeader::is_abstract (void) const
{
  return this->pd_is_abstract;
}

// Add this interface to the list of inherited if not already there.
void
FE_InterfaceHeader::compile_one_inheritance (AST_Interface *i)
{
  // Check for badly formed interface.
  if (i == 0)
    {
      return;
    }

  // If we've seen it already then don't expand again.
  if (already_seen (i))
    {
      return;
    }

  // OK, add i to the list of inherited interfaces.
  add_inheritance (i);

  // And add i to the flat list as well.
  if (!already_seen_flat (i))
    {
      add_inheritance_flat (i);
    }

  // Add i's parents to the flat list.
  AST_Interface **parents = i->inherits ();
  long num_parents = i->n_inherits ();

  for (long j = 0; j < num_parents; ++j)
    {
      AST_Interface *tmp = parents[j];

      if (already_seen_flat (tmp))
        {
          continue;
        }

      add_inheritance_flat (tmp);
    }
}

// Compute the list of top-level interfaces this one inherits from.
void
FE_InterfaceHeader::compile_inheritance (UTL_NameList *ifaces,
                                         idl_bool for_valuetype)
{
  if (ifaces == 0)
    {
      return;
    }

  AST_Decl *d = 0;
  UTL_ScopedName *item = 0;;
  AST_Interface *i = 0;
  long j = 0;
  long k = 0;
  int inh_err = 0;

  iused = 0;
  iused_flat = 0;

  // Compute expanded flattened non-repeating list of interfaces
  // which this one inherits from.

  for (UTL_NamelistActiveIterator l (ifaces); !l.is_done (); l.next ())
    {
      item = l.item ();

      // Check that scope stack is valid.
      if (idl_global->scopes ().top () == 0)
        {
          idl_global->err ()->lookup_error (item);
          return;
        }

      // Look it up.
      UTL_Scope *s = idl_global->scopes ().top ();

      d = s->lookup_by_name  (item,
                              I_TRUE);

      if (d == 0)
        {
          AST_Decl *sad = ScopeAsDecl (s);

          if (sad->node_type () == AST_Decl::NT_module)
            {
              AST_Module *m = AST_Module::narrow_from_decl (sad);

              d = m->look_in_previous (item->last_component ());
            }
        }

      // Not found?
      if (d == 0)
        {
          idl_global->err ()->lookup_error (item);
          return;
        }

      // Not an appropriate interface?
      while (d->node_type () == AST_Decl::NT_typedef)
        {
          d = AST_Typedef::narrow_from_decl (d)->base_type ();
        }

      i = AST_Interface::narrow_from_decl (d);

      if (i != 0)
        {
          inh_err = this->check_inherit (i,
                                         for_valuetype);
        }
      else
        {
          inh_err = -1;
        }

      if (inh_err == -1)
        {
          idl_global->err ()->interface_expected (d);
          break;
        }

      // Forward declared interface?
      if (!i->is_defined ())
        {
          idl_global->err ()->inheritance_fwd_error (this->pd_interface_name,
                                                     i);
          break;
        }

      if (!for_valuetype && this->pd_is_abstract && !i->is_abstract ())
        {
          idl_global->err ()->abstract_inheritance_error (this->name (),
                                                          i->name ());
        }

      // OK, see if we have to add this to the list of interfaces
      // inherited from.
      this->compile_one_inheritance (i);
    }

  // OK, install in interface header.
  // First the flat list (all ancestors).
  if (iused_flat > 0)
    {
      ACE_NEW (this->pd_inherits_flat,
               AST_Interface *[iused_flat]);

      for (j = 0; j < iused_flat; ++j)
        {
          this->pd_inherits_flat[j] = iseen_flat[j];
        }

      this->pd_n_inherits_flat = iused_flat;
    }

  // Then the list of immediate ancestors.
  if (iused > 0)
    {
      ACE_NEW (this->pd_inherits,
               AST_Interface *[iused]);

      for (k = 0; k < iused; ++k)
        {
          this->pd_inherits[k] = iseen[k];
        }

      this->pd_n_inherits = iused;
    }
}

int
FE_InterfaceHeader::check_inherit (AST_Interface *i,
                                   idl_bool for_valuetype)
{
  // We use the narrow instead of node_type() here so we can get a
  // match with both valuetypes and eventtypes.
  idl_bool is_valuetype = (AST_ValueType::narrow_from_decl (i) != 0);

  if (
      // Non-local interfaces may not inherit from local ones.
      (! this->pd_is_local && i->is_local ())
      // Both valuetype or both interface.
      || (for_valuetype ^ is_valuetype)
     )
    {
      return -1;
    }
  else
    {
      return 0;
    }
}

// Data accessors.

UTL_ScopedName *
FE_InterfaceHeader::name (void) const
{
  return this->pd_interface_name;
}

AST_Interface **
FE_InterfaceHeader::inherits (void) const
{
  return this->pd_inherits;
}

long
FE_InterfaceHeader::n_inherits (void) const
{
  return this->pd_n_inherits;
}

AST_Interface **
FE_InterfaceHeader::inherits_flat (void) const
{
  return this->pd_inherits_flat;
}

long
FE_InterfaceHeader::n_inherits_flat (void) const
{
  return this->pd_n_inherits_flat;
}

//************************************************************************

FE_OBVHeader::FE_OBVHeader (UTL_ScopedName *n,
                            UTL_NameList *inherits,
                            UTL_NameList *supports,
                            idl_bool truncatable,
                            idl_bool is_eventtype)
  : FE_InterfaceHeader (n,
                        inherits,
                        I_FALSE,
                        I_FALSE,
                        I_FALSE),
    pd_supports (0),
    pd_inherits_concrete (0),
    pd_supports_concrete (0),
    pd_truncatable (truncatable)
{
  this->compile_inheritance (inherits,
                             I_TRUE);

  if (this->pd_n_inherits > 0)
    {
      AST_Interface *iface = this->pd_inherits[0];
      AST_ValueType *vt = AST_ValueType::narrow_from_decl (iface);

      if (vt != 0
          && vt->is_abstract () == I_FALSE)
        {
          this->pd_inherits_concrete = vt;
        }

      if (! is_eventtype
          && this->pd_inherits[0]->node_type () == AST_Decl::NT_eventtype)
        {
          idl_global->err ()->valuetype_expected (this->pd_inherits[0]);
        }

      for (long i = 1; i < this->pd_n_inherits; ++i)
        {
          iface = this->pd_inherits[i];

          if (!iface->is_abstract ())
            {
              idl_global->err ()->abstract_expected (iface);
            }

          if (! is_eventtype
              && iface->node_type () == AST_Decl::NT_eventtype)
            {
              idl_global->err ()->valuetype_expected (iface);
            }
        }
    }

  if (idl_global->err_count () == 0)
    {
      this->compile_supports (supports);
    }
}

FE_OBVHeader::~FE_OBVHeader (void)
{
}

AST_Interface **
FE_OBVHeader::supports (void) const
{
  return this->pd_supports;
}

long
FE_OBVHeader::n_supports (void) const
{
  return this->pd_n_supports;
}

AST_ValueType *
FE_OBVHeader::inherits_concrete (void) const
{
  return this->pd_inherits_concrete;
}

AST_Interface *
FE_OBVHeader::supports_concrete (void) const
{
  return this->pd_supports_concrete;
}

idl_bool
FE_OBVHeader::truncatable (void) const
{
  return this->pd_truncatable;
}

void
FE_OBVHeader::compile_supports (UTL_NameList *supports)
{
  if (supports == 0)
    {
      this->pd_supports = 0;
      this->pd_n_supports = 0;
      return;
    }

  long length = supports->length ();
  this->pd_n_supports = length;
  ACE_NEW (this->pd_supports,
           AST_Interface *[length]);

  AST_Decl *d = 0;
  UTL_ScopedName *item = 0;;
  AST_Interface *iface = 0;
  int i = 0;

  for (UTL_NamelistActiveIterator l (supports); !l.is_done (); l.next ())
    {
      item = l.item ();

      // Check that scope stack is valid.
      if (idl_global->scopes ().top () == 0)
        {
          idl_global->err ()->lookup_error (item);
          return;
        }

      // Look it up.
      UTL_Scope *s = idl_global->scopes ().top ();

      d = s->lookup_by_name  (item,
                              I_TRUE);

      if (d == 0)
        {
          AST_Decl *sad = ScopeAsDecl (s);

          if (sad->node_type () == AST_Decl::NT_module)
            {
              AST_Module *m = AST_Module::narrow_from_decl (sad);

              d = m->look_in_previous (item->last_component ());
            }
        }

      // Not found?
      if (d == 0)
        {
          idl_global->err ()->lookup_error (item);
          return;
        }

      // Remove typedefs, if any.
      if (d->node_type () == AST_Decl::NT_typedef)
        {
          d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
        }

      if (d->node_type () == AST_Decl::NT_interface)
        {
          iface = AST_Interface::narrow_from_decl (d);
        }
      else
        {
          idl_global->err ()->supports_error (pd_interface_name,
                                              d);
          continue;
        }

      // Forward declared interface?
      if (!iface->is_defined ())
        {
          idl_global->err ()->supports_fwd_error (pd_interface_name,
                                                  iface);
          continue;
        }

      if (!iface->is_abstract ())
        {
          if (i == 0)
            {
              this->pd_supports_concrete = iface;

              if (this->check_concrete_supported_inheritance (iface) != 0)
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

      this->pd_supports[i++] = iface;
    }
}

idl_bool 
FE_OBVHeader::check_concrete_supported_inheritance (AST_Interface *d)
{
  AST_ValueType *vt = 0;
  AST_Interface *concrete = 0;
  AST_Interface **ancestors = 0;
  AST_Interface *ancestor = 0;
  long n_ancestors = 0;

  for (long i = 0; i < this->pd_n_inherits; ++i)
    {
      vt = AST_ValueType::narrow_from_decl (this->pd_inherits[i]);
      concrete = vt->supports_concrete ();

      if (concrete != 0)
        {
          ancestors = concrete->inherits_flat ();
          n_ancestors = concrete->n_inherits_flat ();

          for (long j = 0; j < n_ancestors; ++j)
            {
              ancestor = ancestors[j];

              if (!d->is_child (ancestor))
                {
                  return 1;
                }
            }
        }
    }

  return 0;
}

//************************************************************************

FE_EventHeader::FE_EventHeader (UTL_ScopedName *n,
                                UTL_NameList *inherits,
                                UTL_NameList *supports,
                                idl_bool truncatable)
  : FE_OBVHeader (n,
                  inherits,
                  supports,
                  truncatable,
                  I_TRUE)
{
}

FE_EventHeader::~FE_EventHeader (void)
{
}

//************************************************************************

FE_ComponentHeader::FE_ComponentHeader (UTL_ScopedName *n, 
                                        UTL_ScopedName *base_component, 
                                        UTL_NameList *supports,
                                        idl_bool /* compile_now */)
  : FE_InterfaceHeader (n,
                        supports,
                        I_FALSE,
                        I_FALSE,
                        I_FALSE),
    pd_base_component (0)
{
  // If there is a base component, look up the decl and assign our member.
  // We also inherit its supported interfaces.
  if (base_component != 0)
    {
      UTL_Scope *s = idl_global->scopes ().top_non_null ();
      AST_Decl *d = s->lookup_by_name (base_component,
                                       I_TRUE);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (base_component);
        }
      else
        {
          this->pd_base_component = AST_Component::narrow_from_decl (d);

          if (this->pd_base_component == 0)
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_USE,
                                          d);
            }
          else if (!this->pd_base_component->is_defined ())
            {
              idl_global->err ()->inheritance_fwd_error (
                                      this->name (),
                                      this->pd_base_component
                                    );
            }
        }
    }

  this->compile_inheritance (supports,
                             I_FALSE);
}

FE_ComponentHeader::~FE_ComponentHeader (void)
{
}

AST_Component *
FE_ComponentHeader::base_component (void) const
{
  return this->pd_base_component;
}

AST_Interface **
FE_ComponentHeader::supports (void) const
{
  return this->pd_inherits;
}

long 
FE_ComponentHeader::n_supports (void) const
{
  return this->pd_n_inherits;
}

AST_Interface **
FE_ComponentHeader::supports_flat (void) const
{
  return this->pd_inherits_flat;
}

long
FE_ComponentHeader::n_supports_flat (void) const
{
  return this->pd_n_inherits_flat;
}

//************************************************************************

FE_HomeHeader::FE_HomeHeader (UTL_ScopedName *n,
                              UTL_ScopedName *base_home,
                              UTL_NameList *supports,
                              UTL_ScopedName *managed_component,
                              UTL_ScopedName *primary_key)
  : FE_ComponentHeader (n,
                        managed_component,
                        supports,
                        I_FALSE),
    pd_base_home (0),
    pd_primary_key (0)
{
  UTL_Scope *s = idl_global->scopes ().top_non_null ();
  AST_Decl *d = 0;

  if (base_home != 0)
    {
      d = s->lookup_by_name (base_home,
                             I_TRUE);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (base_home);
        }
      else
        {
          this->pd_base_home = AST_Home::narrow_from_decl (d);

          if (this->pd_base_home == 0)
            {
              idl_global->err ()->inheritance_error (this->name (),
                                                     d);
            }
        }
    }

  if (managed_component != 0)
    {
      d = s->lookup_by_name (managed_component,
                             I_TRUE);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (managed_component);
        }
      else
        {
          this->pd_base_component = AST_Component::narrow_from_decl (d);

          if (this->pd_base_component == 0)
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_USE,
                                          d);
            }
        }
    }

  if (primary_key != 0)
    {
      d = s->lookup_by_name (primary_key,
                             I_TRUE);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (primary_key);
        }
      else
        {
          this->pd_primary_key = AST_ValueType::narrow_from_decl (d);

          if (this->pd_primary_key == 0)
            {
              idl_global->err ()->valuetype_expected (d);
            }
        }
    }

  this->compile_inheritance (supports,
                             I_FALSE);
}

void
FE_HomeHeader::compile_inheritance (UTL_NameList *supports,
                                    idl_bool for_valuetype)
{
  if (this->pd_base_home != 0)
    {
      UTL_NameList *base_home_name = 0;
      ACE_NEW (base_home_name,
               UTL_NameList (this->pd_base_home->name (),
                             supports));

      supports = base_home_name;
      this->FE_InterfaceHeader::compile_inheritance (supports,
                                                     for_valuetype);
    }
}

FE_HomeHeader::~FE_HomeHeader (void)
{
}

AST_Home *
FE_HomeHeader::base_home (void) const
{
  return this->pd_base_home;
}

AST_Component *
FE_HomeHeader::managed_component (void) const
{
  return this->pd_base_component;
}

AST_ValueType *
FE_HomeHeader::primary_key (void) const
{
  return this->pd_primary_key;
}

