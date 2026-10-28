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

// AST_Type is the base class for all AST classes which represent
// IDL type constructs.

#include "ast_type.h"
#include "ast_typedef.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

AST_Type::AST_Type (AST_Decl::NodeType nt,
                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (nt,
              n)
{
}

AST_Type::~AST_Type ()
{
  destroy ();
}

// Public operations.

// Return our size type.
AST_Type::SIZE_TYPE
AST_Type::size_type ()
{
  if (this->size_type_ == AST_Type::SIZE_UNKNOWN)
    {
      (void) this->compute_size_type ();
    }

  return this->size_type_;
}

// Set our size type and that of all our ancestors.
void
AST_Type::size_type (AST_Type::SIZE_TYPE st)
{
  // Size type can be VARIABLE or FIXED.
  if (this->size_type_ == AST_Type::SIZE_UNKNOWN) // not set yet
    {
      this->size_type_ = st; // set it
    }
  else if ((this->size_type_ == AST_Type::FIXED)
           && (st == AST_Type::VARIABLE))
    {
      // Once we are VARIABLE, we cannot be FIXED. But if we were FIXED and then
      // get overwritten to VARIABLE, it is fine. Such a situation occurs only
      // when setting the sizes of structures and unions.
      this->size_type_ = st;
    }
}

// Compute the size type of the node in question
int
AST_Type::compute_size_type ()
{
  return 0;
}

bool
AST_Type::in_recursion (ACE_Unbounded_Queue<AST_Type *> & /*list*/)
{
  // By default we are not involved in recursion.
  return false;
}

bool
AST_Type::ifr_added () const
{
  return this->ifr_added_;
}

void
AST_Type::ifr_added (bool val)
{
  this->ifr_added_ = val;
}

bool
AST_Type::ifr_fwd_added () const
{
  return this->ifr_fwd_added_;
}

void
AST_Type::ifr_fwd_added (bool val)
{
  this->ifr_fwd_added_ = val;
}

bool
AST_Type::has_constructor () const
{
  return this->has_constructor_;
}

void
AST_Type::has_constructor (bool value)
{
  // Similarly to be_decl::size_type_, once this
  // gets set to true, we don't want it to
  // change back.
  if (!this->has_constructor_)
    {
      this->has_constructor_ = value;
    }
}

// Return the type name.
std::string
AST_Type::nested_type_name (AST_Decl *use_scope,
                            std::string_view suffix,
                            std::string_view prefix) const
{
  return this->nested_name (
                            this->local_name ()->get_string_view_or_empty (),
                            this->full_name (),
                            use_scope,
                            suffix,
                            prefix);
}

AST_Type *
AST_Type::unaliased_type ()
{
  return const_cast<AST_Type *> (static_cast<AST_Type const *> (this)->unaliased_type ());
}

AST_Type const *
AST_Type::unaliased_type () const
{
  AST_Type const *t = this;
  AST_Typedef const *td = nullptr;
  AST_Decl::NodeType nt = this->node_type ();

  while (nt == AST_Decl::NT_typedef)
    {
      td = dynamic_cast<AST_Typedef const*> (t);
      t = td->base_type ();
      nt = t->node_type ();
    }

  return t;
}

bool
AST_Type::legal_for_primary_key () const
{
  return true;
}

// This is the real thing used by the method above.
std::string
AST_Type::nested_name (std::string_view local_name,
                       std::string_view full_name,
                       AST_Decl *use_scope,
                       std::string_view suffix,
                       std::string_view prefix) const
{
  // Some compilers do not like generating a fully scoped name for a type that
  // was defined in the same enclosing scope in which it was defined. For such,
  // we emit a macro defined in the ACE library.

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well.

  std::string nested_name;

  // Traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed.

  // This adds the global double colon for type names using the canonical
  // CORBA namespace, replacing the ad hoc spot
  // generations of "::" here and there, which have now been removed.
  UTL_Scope * const s = this->defined_in ();
  AST_Decl * const def_scope = s != nullptr ? ScopeAsDecl (s) : nullptr;

  // TypeCode is a special case for predefined types, since it's
  // defined in the CORBA module.
  bool const in_root =
    (def_scope != nullptr && def_scope->node_type () == AST_Decl::NT_root)
    || ((this->node_type () == AST_Decl::NT_pre_defined
        && this->flat_name () == "CORBA_TypeCode"));

  std::string_view const fname = this->full_name ();
  bool const corba_type = fname.substr (0, 7) == "CORBA::";

  if (in_root && corba_type)
    {
      nested_name.append ("::");
    }

  if (def_scope != nullptr && !in_root && use_scope != nullptr)
    // If both scopes exist and that we are not in the root scope.
    {
      std::string const def_name (def_scope->full_name ());
      std::string const use_name (use_scope->full_name ());

      // If the scopes are identical, don't supply them.
      if (def_name == use_name)
        {
          if (!prefix.empty ())
            {
              nested_name.append (prefix);
            }

          nested_name.append (local_name);
          if (!suffix.empty ())
            {
              nested_name.append (suffix);
            }

          return nested_name;
        }

      std::string::size_type def_pos = 0;
      std::string::size_type use_pos = 0;
      std::string::size_type common_end = 0;

      while (def_pos < def_name.length () && use_pos < use_name.length ())
        {
          std::string::size_type const def_next = def_name.find ("::", def_pos);
          std::string::size_type const use_next = use_name.find ("::", use_pos);
          std::string::size_type const def_end =
            def_next == std::string::npos ? def_name.length () : def_next;
          std::string::size_type const use_end =
            use_next == std::string::npos ? use_name.length () : use_next;

          if (def_name.compare (def_pos, def_end - def_pos,
                                use_name, use_pos, use_end - use_pos) != 0)
            {
              break;
            }

          common_end = def_end;

          if (def_next == std::string::npos || use_next == std::string::npos)
            {
              break;
            }

          def_pos = def_next + 2;
          use_pos = use_next + 2;
        }

      if (common_end != 0)
        {
          nested_name.append (def_name, 0, common_end);
          nested_name.append ("::");

          std::string::size_type const def_remainder =
            common_end + (common_end < def_name.length () ? 2 : 0);
          if (def_remainder < def_name.length ())
            {
              nested_name.append (def_name, def_remainder, std::string::npos);
              nested_name.append ("::");
            }

          if (!prefix.empty ())
            {
              nested_name.append (prefix);
            }

          nested_name.append (local_name);

          if (!suffix.empty ())
            {
              nested_name.append (suffix);
            }

          return nested_name;
        } // End of if the root prefixes match.
    }

  // Otherwise just emit our full_name.
  if (!prefix.empty ())
    {
      nested_name.append (prefix);
    }

  nested_name.append (full_name);

  if (!suffix.empty ())
    {
      nested_name.append (suffix);
    }

  return nested_name;
}

bool
AST_Type::match_names (AST_Type *t,
                       ACE_Unbounded_Queue<AST_Type *> const &list) const
{
  for (ACE_Unbounded_Queue_Const_Iterator<AST_Type *> iter (list);
       !iter.done ();
       (void) iter.advance ())
    {
      // Queue element.
      AST_Type **temp = nullptr;

      (void) iter.next (temp);

      if (t->full_name () == (*temp)->full_name ())
        {
          return true;
        }
    }

  return false;
}

int
AST_Type::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_type (this);
}

void
AST_Type::destroy ()
{
  this->AST_Decl::destroy ();
}
