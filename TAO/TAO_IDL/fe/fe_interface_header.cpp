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

// FE_InterfaceHeader instances are used to store information about an
// interface header as the interface is being parsed and before the
// AST_Interface node used to represent this interface is created.

#include "idl.h"
#include "idl_extern.h"
#include "fe_private.h"

ACE_RCSID(fe, fe_interface_header, "$Id$")

// Constructor(s) and destructor

FE_InterfaceHeader::FE_InterfaceHeader (UTL_ScopedName *n,
                                        UTL_NameList *nl,
                                        UTL_NameList *supports,
                                        idl_bool compile_now)
  : pd_interface_name (n)
{
  if (compile_now)
    {
      compile_inheritance (nl,
                           supports);
    }
}

FE_InterfaceHeader::~FE_InterfaceHeader (void)
{
}

// Compute flattened, non-repeating list of inherited interfaces

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

          for (long k = 0; k < iused; k++)
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
  for (long i = 0; i < iused; i++)
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
  for (long i = 0; i < iused_flat; i++)
    {
      if (iseen_flat[i] == ip)
        {
          return I_TRUE;
        }
    }

  return I_FALSE;
}

idl_bool
FE_InterfaceHeader::is_local (void)
{
  return 0;
}

idl_bool
FE_InterfaceHeader::is_abstract (void)
{
  return 0;
}

// Add this interface to the list of inherited if not already there.
void
FE_InterfaceHeader::compile_one_inheritance (AST_Interface *i)
{
  // Check for badly formed interface.
  if (i == NULL)
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

  for (long j = 0; j < num_parents; j++)
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
                                         UTL_NameList *supports)
{
  AST_Decl *d = 0;
  UTL_ScopedName *item;
  AST_Interface *i = 0;
  long j, k;
  UTL_NameList *nl = ifaces;
  long ichecked = 0;
  idl_bool inh_err = 0;
  idl_bool in_supports = 0;

  iused = 0;
  iused_flat = 0;

  // Compute expanded flattened non-repeating list of interfaces
  // which this one inherits from.

  // Loop twice if nl and supports are nonempty.
  for (int loops = 0; loops < 2; ++loops)
    {
      if (nl != NULL)
        {
          for (UTL_NamelistActiveIterator l (nl); !l.is_done (); l.next ())
            {
              item = l.item ();

              // Check that scope stack is valid.
              if (idl_global->scopes ()->top () == 0)
                {
                  idl_global->err ()->lookup_error (item);
                  return;
                }

              // Look it up.
              UTL_Scope *s = idl_global->scopes ()->top ();

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

              if (d->node_type () == AST_Decl::NT_interface)
                {
                  i = AST_Interface::narrow_from_decl (d);
                }
              else
                {
                  i = 0;
                }

              if (i != 0)
                {
                  if (in_supports)
                    {
                      inh_err = !this->check_supports (i);
                    }
                  else
                    {
                      if (ichecked == 0)
                        {
                          inh_err = !this->check_first (i);
                        }
                      else
                        {
                          inh_err = !this->check_further (i);
                        }
                    }
                }
              else
                {
                  inh_err = 1;
                }

              if (inh_err)
                {
                  idl_global->err ()->inheritance_error (pd_interface_name,
                                                         d);
                  return;  // @@ really ? inh_err=0; and test the remaining...?
                }

              // Forward declared interface?
              if (!i->is_defined ())
                {
                  idl_global->err ()->inheritance_fwd_error (pd_interface_name,
                                                             i);
                  return;
                }

              // OK, see if we have to add this to the list of interfaces
              // inherited from.
              this->compile_one_inheritance (i);

              // Next element in header list.
              ++ichecked;
            }
        }

      in_supports = 1;
      nl = supports;
    }

  // OK, install in interface header.
  // First the flat list (all ancestors).
  this->pd_inherits_flat = new AST_Interface *[iused_flat];

  for (j = 0; j < iused_flat; j++)
    {
      this->pd_inherits_flat[j] = iseen_flat[j];
    }

  this->pd_n_inherits_flat = iused_flat;

  // Then the list of immediate ancestors.
  ACE_NEW (this->pd_inherits,
           AST_Interface *[iused]);

  for (k = 0; k < iused; k++)
    {
      this->pd_inherits[k] = iseen[k];
    }

  this->pd_n_inherits = iused;
}

// check_ methods called from compile_inheritance()
// overridden in derived classes.
idl_bool
FE_InterfaceHeader::check_first (AST_Interface *i)
{
  // Nothing special for the first.
  return this->check_further (i);
}

idl_bool
FE_InterfaceHeader::check_further (AST_Interface *i)
{
  if (i != 0
      && !i->is_valuetype ()
      && (this->is_local () || !i->is_local ()))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

idl_bool
FE_InterfaceHeader::check_supports (AST_Interface *)
{
  return 0;
}

// Data accessors.

UTL_ScopedName *
FE_InterfaceHeader::interface_name (void)
{
  return this->pd_interface_name;
}

AST_Interface **
FE_InterfaceHeader::inherits (void)
{
  return this->pd_inherits;
}

long
FE_InterfaceHeader::n_inherits (void)
{
  return this->pd_n_inherits;
}

AST_Interface **
FE_InterfaceHeader::inherits_flat (void)
{
  return this->pd_inherits_flat;
}

long
FE_InterfaceHeader::n_inherits_flat (void)
{
  return this->pd_n_inherits_flat;
}

FE_Local_InterfaceHeader::FE_Local_InterfaceHeader (UTL_ScopedName *n,
                                                    UTL_NameList *nl,
                                                    UTL_NameList *supports)
  : FE_InterfaceHeader (n,
                        nl,
                        supports,
                        0)
{
  compile_inheritance (nl,
                       supports);
}

idl_bool
FE_Local_InterfaceHeader::is_local (void)
{
  return 1;
}

FE_Abstract_InterfaceHeader::FE_Abstract_InterfaceHeader (UTL_ScopedName *n,
                                                          UTL_NameList *nl,
                                                          UTL_NameList *supports)

  : FE_InterfaceHeader (n,
                        nl,
                        supports,
                        0)
{
  compile_inheritance (nl,
                       supports);
}

idl_bool
FE_Abstract_InterfaceHeader::is_abstract (void)
{
  return 1;
}

// FE_obv_header.

FE_obv_header::FE_obv_header (UTL_ScopedName *n,
                              UTL_NameList *nl,
                              UTL_NameList *supports)
  : FE_InterfaceHeader (n,
                        nl,
                        supports,
                        0),
    truncatable_ (0),
    n_concrete_ (0)
{
  compile_inheritance (nl,
                       supports);
}

// check_ methods called from compile_inheritance().
idl_bool
FE_obv_header::check_first (AST_Interface *i)
{
  if (i != 0 && i->is_valuetype ())
    {
      if (!i->is_abstract_valuetype ())
        {
          ++this->n_concrete_;
        }

      return 1;
    }
  else
    {
      return 0;
    }
}

idl_bool
FE_obv_header::check_further (AST_Interface *i)
{
  if (i != 0
      && i->is_valuetype ()
      && i->is_abstract_valuetype ())
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

idl_bool
FE_obv_header::check_supports (AST_Interface *i)
{
  if (i && !i->is_valuetype ())
    {
      return 1;
    }
  else
    {
      return 0;
    }
}


long
FE_obv_header::n_concrete (void)
{
  return this->n_concrete_;
}

