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
#include "ace/OS_Memory.h"

AST_Type::AST_Type (AST_Decl::NodeType nt,
                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (nt,
              n),
    ifr_added_ (0),
    ifr_fwd_added_ (0),
    size_type_ (AST_Type::SIZE_UNKNOWN),
    has_constructor_ (0),
    nested_type_name_ (0),
    in_recursion_ (-1),
    recursing_in_legal_pk_ (false)
{
}

AST_Type::~AST_Type (void)
{
}

// Public operations.

// Return our size type.
AST_Type::SIZE_TYPE
AST_Type::size_type (void)
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
AST_Type::compute_size_type (void)
{
  return 0;
}

bool
AST_Type::in_recursion (ACE_Unbounded_Queue<AST_Type *> &)
{
  // By default we are not involved in recursion.
  return 0;
}

bool
AST_Type::ifr_added (void)
{
  return this->ifr_added_;
}

void
AST_Type::ifr_added (bool val)
{
  this->ifr_added_ = val;
}

bool
AST_Type::ifr_fwd_added (void)
{
  return this->ifr_fwd_added_;
}

void
AST_Type::ifr_fwd_added (bool val)
{
  this->ifr_fwd_added_ = val;
}

bool
AST_Type::has_constructor (void)
{
  return this->has_constructor_;
}

void
AST_Type::has_constructor (bool value)
{
  // Similarly to be_decl::size_type_, once this
  // gets set to true, we don't want it to
  // change back.
  if (this->has_constructor_ == 0)
    {
      this->has_constructor_ = value;
    }
}

// This code works. However, whether we should generate the
// ACE_NESTED_CLASS macro or not should be based on an option to the
// compiler. The previous version generated a relative path.
// This version always generates ACE_NESTED_CLASS, (leave ace/ACE.h and friends
// do the porting)
//
// Caution: returns the same buffer pointer even if the contents may change
// in the next call.  (return std::string anyone?)
//
// Return the type name using the ACE_NESTED_CLASS macro

const char *
AST_Type::nested_type_name (AST_Decl *use_scope,
                            const char *suffix,
                            const char *prefix)
{
  return this->nested_name (this->local_name ()->get_string (),
                            this->full_name (),
                            use_scope,
                            suffix,
                            prefix);
}

AST_Type *
AST_Type::unaliased_type (void)
{
  AST_Type *t = this;
  AST_Typedef *td = 0;
  AST_Decl::NodeType nt = this->node_type ();

  while (nt == AST_Decl::NT_typedef)
    {
      td = AST_Typedef::narrow_from_decl (t);
      t = td->base_type ();
      nt = t->node_type ();
    }

  return t;
}

bool
AST_Type::legal_for_primary_key (void) const
{
  return true;
}

// This is the real thing used by the method above.
const char *
AST_Type::nested_name (const char* local_name,
                       const char* full_name,
                       AST_Decl *use_scope,
                       const char *suffix,
                       const char *prefix)
{
  // Some compilers do not like generating a fully scoped name for a type that
  // was defined in the same enclosing scope in which it was defined. For such,
  // we emit a macro defined in the ACE library.
  //

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well. If that is the case, then we can generate a fully
  // scoped name for that type, else we use the ACE_NESTED_CLASS macro.

  // Thus we need some sort of relative name to be generated.

  if (this->nested_type_name_ == 0)
    {
      ACE_NEW_RETURN (this->nested_type_name_,
                      char[NAMEBUFSIZE],
                      0);
    }

  // Hold the fully scoped name.
  char def_name [NAMEBUFSIZE];
  char use_name [NAMEBUFSIZE];

  // These point to the prev, curr and next component in the scope.
  char *def_curr = def_name;
  char *def_next = 0;
  char *use_curr = use_name;
  char *use_next = 0;

  // How many chars to compare.
  int len_to_match = 0;

  // Initialize the buffers.
  ACE_OS::memset (this->nested_type_name_,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::memset (def_name,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::memset (use_name,
                  '\0',
                  NAMEBUFSIZE);

  // Traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed. This forms the first argument to the ACE_NESTED_CLASS
  // macro. Whenever there is no match, the remaining components of the
  // def_scope form the second argument.

  // This adds the global double colon for type names using the canonical
  // CORBA namespace, replacing the ad hoc spot
  // generations of "::" here and there, which have now been removed.
  UTL_Scope *s = this->defined_in ();
  AST_Decl *def_scope = s != 0 ? ScopeAsDecl (s) : 0;

  // TypeCode is a special case for predefined types, since it's
  // defined in the CORBA module.
  bool in_root =
    (def_scope != 0 && def_scope->node_type () == AST_Decl::NT_root)
    || ((this->node_type () == AST_Decl::NT_pre_defined
        && ACE_OS::strcmp (this->flat_name (), "CORBA_TypeCode") == 0));

  ACE_CString fname (this->full_name ());
  bool corba_type = fname.find ("CORBA::") == 0;

  if (in_root && corba_type)
    {
      ACE_OS::strcat (this->nested_type_name_, "::");
    }

  if (def_scope != 0 && !in_root && use_scope != 0)
    // If both scopes exist and that we are not in the root scope.
    {
      ACE_OS::strcpy (def_name,
                      def_scope->full_name ());

      ACE_OS::strcpy (use_name,
                      use_scope->full_name ());

      // Find the first occurrence of a :: and advance
      // the next pointers accordingly.
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      // If the scopes are identical, don't supply them.
      if  (ACE_OS::strcmp (def_name, use_name) == 0)
        {
          if (prefix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_,
                              prefix);
            }

          ACE_OS::strcat (this->nested_type_name_,
                          local_name);
          if (suffix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_,
                              suffix);
            }

          return this->nested_type_name_;
        }

      if (def_next != 0)
        {
          len_to_match =
            static_cast<int> (ACE_OS::strlen (def_curr)) -
            static_cast<int> (ACE_OS::strlen (def_next));
        }
      else
        {
          len_to_match = static_cast<int> (ACE_OS::strlen (def_curr));
        }

      if (use_next != 0)
        {
          const int len =
            static_cast<int> (ACE_OS::strlen (use_curr)) -
            static_cast<int> (ACE_OS::strlen (use_next));

          if (len > len_to_match)
            {
              len_to_match = len;
            }
        }
      else
        {
          const int len = static_cast<int> (ACE_OS::strlen (use_curr));

          if (len > len_to_match)
            {
              len_to_match = len;
            }
        }

      if (ACE_OS::strncmp (def_curr,
                           use_curr,
                           len_to_match)
            == 0)
        {
          // Initialize the first argument.
          ACE_OS::strncat (this->nested_type_name_,
                           def_curr,
                           len_to_match);

          // Shift the current scopes to the next level.
          def_curr = (def_next ? (def_next + 2) : 0); // Skip the ::
          use_curr = (use_next ? (use_next + 2) : 0); // Skip the ::

          while (def_curr && use_curr)
            {
              // Find the first occurrence of a :: and advance the
              // next pointers accordingly.
              def_next = ACE_OS::strstr (def_curr, "::");
              use_next = ACE_OS::strstr (use_curr, "::");

              if (def_next != 0)
                {
                  len_to_match =
                    static_cast<int> (ACE_OS::strlen (def_curr)) -
                    static_cast<int> (ACE_OS::strlen (def_next));
                }
              else
                {
                  len_to_match = static_cast<int> (ACE_OS::strlen (def_curr));
                }

              if (use_next != 0)
                {
                  int len  =
                    static_cast<int> (ACE_OS::strlen (use_curr)) -
                    static_cast<int> (ACE_OS::strlen (use_next));

                  if (len > len_to_match)
                    {
                      len_to_match = len;
                    }
                }
              else
                {
                  const int len = static_cast<int> (ACE_OS::strlen (use_curr));

                  if (len > len_to_match)
                    {
                      len_to_match = len;
                    }
                }

              if (ACE_OS::strncmp (def_curr,
                                   use_curr,
                                   len_to_match)
                    == 0)
                {
                  // They have same prefix, append to arg1.
                  ACE_OS::strcat (this->nested_type_name_,
                                  "::");

                  ACE_OS::strncat (this->nested_type_name_,
                                   def_curr,
                                   len_to_match);

                  def_curr = (def_next ? (def_next + 2) : 0); // Skip the ::
                  use_curr = (use_next ? (use_next + 2) : 0); // Skip the ::
                }
              else
                {
                  // No match. This is the end of the first argument. Get out
                  // of the loop as no more comparisons are necessary.
                  break;
                }
            }

          // Start the 2nd argument of the macro.
          ACE_OS::strcat (this->nested_type_name_, "::");

          // Copy the remaining def_name (if any are left).
          if (def_curr != 0)
            {
              ACE_OS::strcat (this->nested_type_name_,
                              def_curr);

              ACE_OS::strcat (this->nested_type_name_,
                              "::");
            }

          // Append our local name.
          if (prefix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_, prefix);
            }

          ACE_OS::strcat (this->nested_type_name_,
                          local_name);

          if (suffix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_,
                              suffix);
            }

          return this->nested_type_name_;
        } // End of if the root prefixes match.
    }

  // Otherwise just emit our full_name.
  if (prefix != 0)
    {
      ACE_OS::strcat (this->nested_type_name_, prefix);
    }

  ACE_OS::strcat (this->nested_type_name_,
                  full_name);

  if (suffix != 0)
    {
      ACE_OS::strcat (this->nested_type_name_,
                      suffix);
    }

  return this->nested_type_name_;
}

bool
AST_Type::match_names (AST_Type *t, ACE_Unbounded_Queue<AST_Type *> &list)
{
  for (ACE_Unbounded_Queue_Iterator<AST_Type *> iter (list);
       !iter.done ();
       (void) iter.advance ())
    {
      // Queue element.
      AST_Type **temp = 0;

      (void) iter.next (temp);

      if (!ACE_OS::strcmp (t->full_name (),
                           (*temp)->full_name ()))
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
AST_Type::destroy (void)
{
  delete [] this->nested_type_name_;
  this->nested_type_name_ = 0;

  this->AST_Decl::destroy ();
}



IMPL_NARROW_FROM_DECL(AST_Type)
