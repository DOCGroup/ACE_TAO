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

// AST_InterfaceFwd nodes denote forward declarations of IDL interfaces.
// AST_InterfaceFwd nodes have a field containing the full declaration
// of the interface, which is initialized when that declaration is
// encountered.

#include "ast_interface_fwd.h"
#include "ast_interface.h"
#include "ast_module.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

AST_Decl::NodeType const
AST_InterfaceFwd::NT = AST_Decl::NT_interface_fwd;

AST_InterfaceFwd::AST_InterfaceFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (dummy->is_local (),
                 dummy->is_abstract ()),
    AST_Decl (AST_Decl::NT_interface_fwd,
              n),
    AST_Type (AST_Decl::NT_interface_fwd,
              n),
    is_defined_ (false)
{
  // Create a dummy placeholder for the forward declared interface. This
  // interface node is not yet defined (n_inherits < 0), so some operations
  // will fail.
  this->pd_full_definition = dummy;

  if (! dummy->is_local ())
    {
      idl_global->non_local_fwd_iface_seen_ = true;
    }
}

AST_InterfaceFwd::~AST_InterfaceFwd (void)
{
}

// Private operations.

bool
AST_InterfaceFwd::is_local (void)
{
  return this->full_definition ()->is_local ();
}

bool
AST_InterfaceFwd::is_valuetype (void)
{
  AST_Decl::NodeType nt = this->full_definition ()->node_type ();
  return nt == AST_Decl::NT_valuetype || nt == AST_Decl::NT_eventtype;
}

bool
AST_InterfaceFwd::is_abstract_valuetype (void)
{
  return (this->full_definition ()->is_abstract ()
          && this->is_valuetype ());
}

bool
AST_InterfaceFwd::full_def_seen (void)
{
  UTL_Scope *s = this->defined_in ();
  AST_Interface *i = 0;

  // If a full definition is seen in a previous module opening
  // or anywhere in the current scope (before or after our
  // declaration, reture TRUE.

  if (AST_Decl::NT_module == s->scope_node_type ())
    {
      AST_Module *m = AST_Module::narrow_from_scope (s);
      AST_Decl *d =
        m->look_in_prev_mods_local (this->local_name (),
                                    false);

      if (0 != d)
        {
          i = AST_Interface::narrow_from_decl (d);

          if (0 != i && i->is_defined ())
            {
              return true;
            }
        }
    }

  for (UTL_ScopeActiveIterator iter (s, UTL_Scope::IK_decls);
        !iter.is_done ();
        iter.next ())
    {
      i = AST_Interface::narrow_from_decl (iter.item ());

      if (0 != i && this->local_name ()->compare (i->local_name ()))
        {
          if (i->is_defined ())
            {
              return true;
            }
        }
    }

  return false;
}

// Redefinition of inherited virtual operations.

// Dump this AST_InterfaceFwd node to the ostream o.
void
AST_InterfaceFwd::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_abstract ())
    {
      this->dump_i (o, "abstract ");
    }
  else if (this->is_local ())
    {
      this->dump_i (o, "local ");
    }

  this->dump_i (o, "interface ");

  this->local_name ()->dump (o);
}

int
AST_InterfaceFwd::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_interface_fwd (this);
}

// Data accessors.

AST_Interface *
AST_InterfaceFwd::full_definition (void)
{
  return this->pd_full_definition;
}

void
AST_InterfaceFwd::set_full_definition (AST_Interface *nfd)
{
  this->pd_full_definition->destroy ();
  delete this->pd_full_definition;
  this->pd_full_definition = nfd;
}

bool
AST_InterfaceFwd::is_defined (void)
{
  // Look for the one instance of the fwd decl
  // that may have a full definition.
  if (!this->is_defined_)
    {
      AST_Module *m =
        AST_Module::narrow_from_scope (this->defined_in ());

      if (0 != m)
        {
          AST_Decl *d =
            m->look_in_prev_mods_local (this->local_name ());

          if (0 != d)
            {
              // We could be looking at a superfluous forward decl
              // of an interface already defined.
              AST_Interface *full = AST_Interface::narrow_from_decl (d);

              if (0 != full)
                {
                  this->is_defined_ = true;
                }

              AST_InterfaceFwd *fwd =
                AST_InterfaceFwd::narrow_from_decl (d);

              // Since fwd_redefinition_helper() is called
              // before fe_add_interface(), we can't check
              // n_inherits() or is_defined(), but added()
              // is a sufficient way to tell if our full
              // definition has already gone through the
              // add_to_scope process.
              if (0 != fwd && fwd->is_defined ())
                {
                  this->is_defined_ = true;
                }
            }
        }
    }

  return this->is_defined_;
}

void
AST_InterfaceFwd::set_as_defined (void)
{
  this->is_defined_ = true;
}

void
AST_InterfaceFwd::destroy (void)
{
  // The implementation of is_defined() accomodates
  // code generation issues and doesn't have the
  // correct semantics here. The older implementation
  // of is_defined is used in the IF block below to
  // check if our full definition allocation must be
  // destroyed.
  if (!this->is_defined_)
    {
      // If our full definition is not defined, it
      // means that there was no full definition
      // for us in this compilation unit, so we
      // have to destroy this allocation.
      this->pd_full_definition->destroy ();
      delete this->pd_full_definition;
      this->pd_full_definition = 0;
    }

  this->AST_Type::destroy ();
}

bool
AST_InterfaceFwd::is_fwd (void)
{
  return true; // This is a fwd declared type
}

// We don't actually want the forward declaration,
// but want to return the full definition member,
// whether defined yet or not.
AST_Decl *
AST_InterfaceFwd::adjust_found (
  bool ignore_fwd,
  bool full_def_only)
{
  if (ignore_fwd)
    {
      AST_Interface *i = this->full_definition ();
      return (full_def_only && !i->is_defined () ? 0 : i);
    }

  return this;
}

IMPL_NARROW_FROM_DECL (AST_InterfaceFwd)
