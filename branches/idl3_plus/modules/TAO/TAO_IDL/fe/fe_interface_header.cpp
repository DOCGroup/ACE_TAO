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
#include "ast_interface_fwd.h"
#include "ast_valuetype.h"
#include "ast_component.h"
#include "ast_home.h"
#include "ast_module.h"
#include "utl_namelist.h"
#include "utl_err.h"
#include "fe_extern.h"
#include "global_extern.h"
#include "nr_extern.h"

ACE_RCSID (fe,
           fe_interface_header,
           "$Id$")

#undef  INCREMENT
#define INCREMENT 512

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
                                        bool is_local,
                                        bool is_abstract,
                                        bool compile_now)
  : interface_name_ (n),
    inherits_ (0),
    n_inherits_ (0),
    inherits_flat_ (0),
    n_inherits_flat_ (0),
    is_local_ (is_local),
    is_abstract_ (is_abstract),
    iseen_ (0),
    iseen_flat_ (0),
    iallocated_ (0),
    iused_ (0),
    iallocated_flat_ (0),
    iused_flat_ (0)
{
  if (compile_now)
    {
      this->compile_inheritance (inherits,
                                 false);
    }
}

FE_InterfaceHeader::~FE_InterfaceHeader (void)
{
}

bool
FE_InterfaceHeader::is_local (void) const
{
  return this->is_local_;
}

bool
FE_InterfaceHeader::is_abstract (void) const
{
  return this->is_abstract_;
}

void
FE_InterfaceHeader::destroy (void)
{
  if (0 != this->interface_name_)
    {
      this->interface_name_->destroy ();
      delete this->interface_name_;
      this->interface_name_ = 0;
    }
    
  delete [] this->iseen_;
  delete [] this->iseen_flat_;
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
  this->add_inheritance (i);

  // And add i to the flat list as well.
  if (!this->already_seen_flat (i))
    {
      this->add_inheritance_flat (i);
    }

  // Add i's parents to the flat list.
  AST_Interface **parents = i->inherits_flat ();
  long num_parents = i->n_inherits_flat ();

  for (long j = 0; j < num_parents; ++j)
    {
      AST_Interface *tmp = parents[j];

      if (this->already_seen_flat (tmp))
        {
          continue;
        }

      this->add_inheritance_flat (tmp);
    }
}

// Compute the list of top-level interfaces this one inherits from.
void
FE_InterfaceHeader::compile_inheritance (UTL_NameList *ifaces,
                                         bool for_valuetype)
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

  // Compute expanded flattened non-repeating list of interfaces
  // which this one inherits from.

  for (UTL_NamelistActiveIterator l (ifaces); !l.is_done (); l.next ())
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

      d = s->lookup_by_name  (item,
                              true,
                              true,
                              true); // full_def_only

       // Undefined interface?
      if (0 == d)
        {
          // If the lookup now succeeds, without the full_def_only
          // constraint, it's an error.
          d = s->lookup_by_name (item, true, true);

          if (0 != d)
            {
              idl_global->err ()->inheritance_fwd_error (
                                      this->interface_name_,
                                      AST_Interface::narrow_from_decl (d)
                                    );
              break;
            }
        }

     if (0 == d)
        {
          AST_Decl *sad = ScopeAsDecl (s);

          if (sad->node_type () == AST_Decl::NT_module)
            {
              AST_Module *m = AST_Module::narrow_from_decl (sad);

              d = m->look_in_previous (item->last_component ());
            }
        }

      // Not found?
      if (0 == d)
        {
          idl_global->err ()->lookup_error (item);

          // This is probably the result of bad IDL.
          // We will crash if we continue from here.
          throw Bailout ();
        }

      // Not an appropriate interface?
      if (d->node_type () == AST_Decl::NT_typedef)
        {
          d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
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

      if (!for_valuetype && this->is_abstract_ && !i->is_abstract ())
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
  if (this->iused_flat_ > 0)
    {
      ACE_NEW (this->inherits_flat_,
               AST_Interface *[this->iused_flat_]);

      for (j = 0; j < this->iused_flat_; ++j)
        {
          this->inherits_flat_[j] = this->iseen_flat_[j];
        }

      this->n_inherits_flat_ = iused_flat_;
    }

  // Then the list of immediate ancestors.
  if (this->iused_ > 0)
    {
      ACE_NEW (this->inherits_,
               AST_Interface *[this->iused_]);

      for (k = 0; k < this->iused_; ++k)
        {
          this->inherits_[k] = this->iseen_[k];
        }

      this->n_inherits_ = this->iused_;
    }
}

int
FE_InterfaceHeader::check_inherit (AST_Interface *i,
                                   bool for_valuetype)
{
  // We use the narrow instead of node_type() here so we can get a
  // match with both valuetypes and eventtypes.
  bool is_valuetype = (AST_ValueType::narrow_from_decl (i) != 0);

  if (
      // Non-local interfaces may not inherit from local ones.
      (! this->is_local_ && i->is_local ())
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

// Add an interface to an inheritance spec.
void
FE_InterfaceHeader::add_inheritance (AST_Interface *i)
{
  AST_Interface  **oiseen;

  // Make sure there's space for one more.
  if (this->iallocated_ == this->iused_)
    {
      if (this->iallocated_ == 0)
        {
          this->iallocated_ = INCREMENT;

          ACE_NEW (this->iseen_,
                   AST_Interface *[this->iallocated_]);
        }
      else
        {
          oiseen = this->iseen_;
          this->iallocated_ += INCREMENT;

          ACE_NEW (this->iseen_,
                   AST_Interface *[this->iallocated_]);

          for (long k = 0; k < this->iused_; ++k)
            {
              this->iseen_[k] = oiseen[k];
            }

          delete [] oiseen;
        }
    }

  // OK, now insert it.
  this->iseen_[this->iused_++] = i;
}

// Add an interface to the flat list.
void
FE_InterfaceHeader::add_inheritance_flat (AST_Interface *i)
{
  AST_Interface **oiseen_flat;

  // Make sure there's space for one more.
  if (this->iallocated_flat_ == this->iused_flat_)
    {
      if (this->iallocated_flat_ == 0)
        {
          this->iallocated_flat_ = INCREMENT;

          ACE_NEW (this->iseen_flat_,
                   AST_Interface *[this->iallocated_flat_]);
        }
      else
        {
          oiseen_flat = this->iseen_flat_;
          this->iallocated_flat_ += INCREMENT;

          ACE_NEW (this->iseen_flat_,
                   AST_Interface *[this->iallocated_flat_]);

          for (long k = 0; k < this->iused_flat_; k++)
            {
              this->iseen_flat_[k] = oiseen_flat[k];
            }

          delete [] oiseen_flat;
        }
    }

  // OK, now insert it.
  this->iseen_flat_[this->iused_flat_++] = i;
}

// Have we already seen this interface?
bool
FE_InterfaceHeader::already_seen (AST_Interface *ip)
{
  for (long i = 0; i < this->iused_; ++i)
    {
      if (this->iseen_[i] == ip)
        {
          return true;
        }
    }

  return false;
}

// Have we already seen this interface in the flat list?
bool
FE_InterfaceHeader::already_seen_flat (AST_Interface *ip)
{
  for (long i = 0; i < this->iused_flat_; ++i)
    {
      if (this->iseen_flat_[i] == ip)
        {
          return true;
        }
    }

  return false;
}

// Data accessors.

UTL_ScopedName *
FE_InterfaceHeader::name (void) const
{
  return this->interface_name_;
}

AST_Interface **
FE_InterfaceHeader::inherits (void) const
{
  return this->inherits_;
}

long
FE_InterfaceHeader::n_inherits (void) const
{
  return this->n_inherits_;
}

AST_Interface **
FE_InterfaceHeader::inherits_flat (void) const
{
  return this->inherits_flat_;
}

long
FE_InterfaceHeader::n_inherits_flat (void) const
{
  return this->n_inherits_flat_;
}

