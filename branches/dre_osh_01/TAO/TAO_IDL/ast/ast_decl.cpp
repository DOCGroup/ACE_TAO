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

/*
 * AST_Decl is the base class for all AST nodes except AST_Expression.
 * AST_Decls have a node type (a value from the enum AST_Decl::NodeType)
 * and a name (a UTL_ScopedName).
 * Additionally AST_Decl nodes record the scope of definition, the
 * file name in which they were defined, the line on which they were
 * defined in that file, and a boolean denoting whether this is the
 * main file or an #include'd file.
 */

#include "ast_interface.h"
#include "ast_module.h"
#include "ast_array.h"
#include "ast_field.h"
#include "ast_structure.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_typedef.h"
#include "ast_visitor.h"
#include "global_extern.h"
#include "nr_extern.h"
#include "utl_identifier.h"
#include "utl_scope.h"
#include "utl_err.h"

ACE_RCSID (ast, 
           ast_decl, 
           "$Id$")

COMMON_Base::COMMON_Base (idl_bool local,
                          idl_bool abstract)
  : is_local_ (local),
    is_abstract_ (abstract)
{
}

idl_bool
COMMON_Base::is_local (void)
{
  return this->is_local_;
}

idl_bool
COMMON_Base::is_abstract (void)
{
  return this->is_abstract_;
}

void
COMMON_Base::destroy (void)
{
}

// Constructor(s) and destructor.

AST_Decl::AST_Decl (void)
  : repoID_ (0),
    contains_wstring_ (-1),
    pd_imported (I_FALSE),
    pd_in_main_file (I_FALSE),
    pd_defined_in (0),
    pd_node_type (NT_module),
    pd_line (-1),
    pd_file_name (0),
    pd_local_name (0),
    pd_original_local_name (0),
    pd_added (I_FALSE),
    full_name_ (0),
    prefix_ (0),
    version_ (0),
    anonymous_ (I_FALSE),
    typeid_set_ (I_FALSE),
    flat_name_ (0),
    last_referenced_as_ (0),
    prefix_scope_ (0)
{
}

AST_Decl::AST_Decl (NodeType nt,
                    UTL_ScopedName *n,
                    idl_bool anonymous)
  : repoID_ (0),
    contains_wstring_ (-1),
    pd_imported (idl_global->imported ()),
    pd_in_main_file (idl_global->in_main_file ()),
    pd_defined_in (idl_global->scopes ().depth () > 0
                     ? idl_global->scopes ().top ()
                     : 0),
    pd_node_type (nt),
    pd_line (idl_global->lineno ()),
    pd_file_name (idl_global->filename ()),
    pd_name (0),
    pd_local_name (n == 0 ? 0 : n->last_component ()->copy ()),
    pd_original_local_name (0),
    pd_added (I_FALSE),
    full_name_ (0),
    prefix_ (0),
    version_ (0),
    anonymous_ (anonymous),
    typeid_set_ (I_FALSE),
    flat_name_ (0),
    last_referenced_as_ (0),
    prefix_scope_ (0)
{
  this->compute_full_name (n);

  char *prefix = 0;
  idl_global->pragma_prefixes ().top (prefix);

  if (prefix == 0)
    {
      this->prefix_ = ACE::strnew ("");
    }
  else
    {
      this->prefix_ = ACE::strnew (prefix);
    }

  if (n != 0)
    {
      // The function body creates its own copy.
      this->original_local_name (n->last_component ());
    }
}

AST_Decl::~AST_Decl (void)
{
}

// Private operations.

// Compute our private UTL_ScopedName member.
void
AST_Decl::compute_full_name (UTL_ScopedName *n)
{
  // This should happen only when we are a non-void predefined type,
  // in which case our scoped name has already been created by the
  // AST_PredefinedType constructor.
  if (n == 0)
    {
      return;
    }

  UTL_ScopedName *cn = 0;
  AST_Decl *d = 0;

  // Initialize this name to 0.
  this->pd_name = 0;

  // Global scope?
  if (this->defined_in () == 0)
    {
      this->pd_name = (UTL_IdList *) n->copy ();
      return;
    }

  // OK, not global. So copy name of containing scope, then
  // smash last cdr of copy with new component
  d = ScopeAsDecl (this->defined_in ());

  if (d != 0)
    {
      cn = d->name ();
    }

  if (cn != 0)
    {
      this->pd_name = (UTL_IdList *) cn->copy ();
    }

  if (this->pd_local_name != 0)
    {
      if (this->pd_name == 0)
        {
          ACE_NEW (this->pd_name,
                   UTL_ScopedName (this->pd_local_name->copy (),
                                   0));
        }
      else
        {
          UTL_ScopedName *conc_name = 0;
          ACE_NEW (conc_name,
                   UTL_ScopedName (this->pd_local_name->copy (),
                                   0));

          this->pd_name->nconc (conc_name);
        }
    }
}

void
AST_Decl::set_prefix_with_typeprefix_r (char *value,
                                        UTL_Scope *appeared_in)
{
  if (this->typeid_set_)
    {
      return;
    }

  delete [] this->repoID_;
  this->repoID_ = 0;
  this->prefix (value);
  this->prefix_scope_ = appeared_in;

  // This will recursively catch all previous openings of a module.
  if (this->node_type () == AST_Decl::NT_module)
    {
      AST_Decl **d = 0;
      AST_Module *m = AST_Module::narrow_from_decl (this);

      for (ACE_Unbounded_Set_Iterator<AST_Decl *> iter (m->previous ());
           !iter.done ();
           iter.advance ())
        {
          iter.next (d);

          if ((*d)->node_type () == AST_Decl::NT_pre_defined)
            {
              continue;
            }

          (*d)->set_prefix_with_typeprefix_r (value,
                                              appeared_in);
        }
    }

  this->compute_repoID ();
}

// Protected operations.

// Compute stringified fully scoped name.
void
AST_Decl::compute_full_name (void)
{
 if (this->full_name_ != 0)
    {
      return;
    }
  else
    {
      size_t namelen = 0;
      long first = I_TRUE;
      long second = I_FALSE;
      char *name = 0;

      for (UTL_IdListActiveIterator i (this->name ()); 
           !i.is_done (); 
           i.next ())
        {
          if (!first)
            {
              namelen += 2; // for "::"
            }
          else if (second)
            {
              first = second = I_FALSE;
            }

          // Print the identifier.
          name = i.item ()->get_string ();
          namelen += ACE_OS::strlen (name);

          if (first)
            {
              if (ACE_OS::strcmp (name, "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }
        }

      ACE_NEW (this->full_name_,
               char[namelen + 1]);

      this->full_name_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;

      for (UTL_IdListActiveIterator j (this->name ()); 
           !j.is_done (); 
           j.next ())
        {
          if (!first)
            {
              ACE_OS::strcat (this->full_name_, "::");
            }
          else if (second)
            {
              first = second = I_FALSE;
            }

          // Print the identifier.
          name = j.item ()->get_string ();
          ACE_OS::strcat (this->full_name_, name);

          if (first)
            {
              if (ACE_OS::strcmp (name, "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }
        }
    }
}

// Compute stringified repository ID.
void
AST_Decl::compute_repoID (void)
{
 if (this->repoID_ != 0)
    {
      return;
    }

  size_t namelen = 4; // for the prefix "IDL:"
  long first = I_TRUE;
  long second = I_FALSE;
  char *name = 0;
  const char *prefix = this->prefix_;
  UTL_Scope *scope = this->defined_in ();

  // If our prefix is empty, we check to see if an ancestor has one.
  while (ACE_OS::strcmp (prefix, "") == 0 && scope != 0)
    {
      AST_Decl *parent = ScopeAsDecl (scope);

      if (parent->node_type () == AST_Decl::NT_root
          && parent->imported ())
        {
          break;
        }

      prefix = parent->prefix ();
      scope = parent->defined_in ();
    }

  // in the first loop compute the total length
  namelen += ACE_OS::strlen (prefix) + 1;

  const char *version = this->version_;
  scope = this->defined_in ();

  // If our version is has not bee set, we use the parent's, if any.
  while (version == 0 && scope != 0)
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      version = parent->version ();
      scope = parent->defined_in ();
    }

  if (version != 0)
    {
      // Version member string + ':'
      namelen += ACE_OS::strlen (version) + 1;
    }
  else
    {
      // For ":1.0"
      namelen += 4;
    }

  for (UTL_IdListActiveIterator i (this->name ()); 
       !i.is_done (); 
       i.next ())
    {
      if (!first)
        {
          namelen += 1; // for "/"
        }
      else if (second)
        {
          first = second = I_FALSE;
        }

      // Print the identifier.
      name = i.item ()->get_string ();
      size_t item_len = ACE_OS::strlen (name);

      if (ACE_OS::strstr (name, "_cxx_") == name)
        {
          namelen += (item_len - ACE_OS::strlen ("_cxx_"));
        }
      else
        {
          namelen += item_len;
        }

      if (first)
        {
          if (ACE_OS::strcmp (name, "") != 0)
            {
              // Does not start with a "".
              first = I_FALSE;
            }
          else
            {
              second = I_TRUE;
            }
        }
    }

  ACE_NEW (this->repoID_,
           char[namelen + 1]);

  this->repoID_[0] = '\0';

  ACE_OS::sprintf (this->repoID_,
                   "%s",
                   "IDL:");

  if (ACE_OS::strcmp (prefix, "") != 0)
    {
      ACE_OS::strcat (this->repoID_,
                      prefix);

      ACE_OS::strcat (this->repoID_, "/");
    }
  
  first = I_TRUE;
  second = I_FALSE;

  for (UTL_IdListActiveIterator j (this->name ()); 
       !j.is_done (); 
       j.next ())
    {
      if (!first)
        {
          ACE_OS::strcat (this->repoID_, "/");
        }
      else if (second)
        {
          first = second = I_FALSE;
        }

      // Print the identifier.
      name = j.item ()->get_string ();

      if (ACE_OS::strstr (name, "_cxx_") == name)
        {
          ACE_OS::strcat (this->repoID_,
                          name + ACE_OS::strlen ("_cxx_"));
        }
      else
        {
          ACE_OS::strcat (this->repoID_,
                          name);
        }

      if (first)
        {
          if (ACE_OS::strcmp (name, "") != 0)
            {
              // Does not start with a "".
              first = I_FALSE;
            }
          else
            {
              second = I_TRUE;
            }
        }
    }

  if (version != 0)
    {
      ACE_OS::strcat (this->repoID_,
                      ":");
      ACE_OS::strcat (this->repoID_,
                      version);
    }
  else
    {
      ACE_OS::strcat (this->repoID_,
                      ":1.0");
    }
}

// Public operations.

const char *
AST_Decl::flat_name (void)
{
  if (!this->flat_name_)
    {
      this->compute_flat_name ();
    }

  return this->flat_name_;
}

// Compute stringified flattened fully scoped name.
void
AST_Decl::compute_flat_name (void)
{
  if (this->flat_name_ != 0)
    {
      return;
    }
  else
    {
      size_t namelen = 0;
      long first = I_TRUE;
      long second = I_FALSE;
      char *item_name = 0;

      // In the first loop, compute the total length.
      for (UTL_IdListActiveIterator i (this->name ());
           !i.is_done ();
           i.next ())
        {
          if (!first)
            {
              namelen += 1; // for "_"
            }
          else if (second)
            {
              first = second = I_FALSE;
            }

          // Print the identifier.
          item_name = i.item ()->get_string ();
          namelen += ACE_OS::strlen (item_name);

          if (first)
            {
              if (ACE_OS::strcmp (item_name, "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }
        }

      ACE_NEW (this->flat_name_,
               char[namelen + 1]);

      this->flat_name_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;

      for (UTL_IdListActiveIterator j (this->name ());
           !j.is_done ();
           j.next ())
        {
          if (!first)
            {
              ACE_OS::strcat (this->flat_name_, "_");
            }
          else if (second)
            {
              first = second = I_FALSE;
            }

          // Print the identifier.
          item_name = j.item ()->get_string ();
          ACE_OS::strcat (this->flat_name_, item_name);

          if (first)
            {
              if (ACE_OS::strcmp (item_name, "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }
        }
    }
}

// Return TRUE if one of my ancestor scopes is "s"
// and FALSE otherwise.
idl_bool
AST_Decl::has_ancestor (AST_Decl *s)
{
  if (this == s)
    {
      return I_TRUE;
    }

  if (s->node_type () == AST_Decl::NT_module)
    {
      UTL_Scope *enclosing = s->defined_in ();
      AST_Decl *other_opening = s;

      for (int index = 1; other_opening != 0; ++index)
        {
          if (this == other_opening)
            {
              return I_TRUE;
            }

          other_opening = enclosing->lookup_by_name_local (s->local_name (),
                                                           index);
        }
    }

  if (this->pd_defined_in == 0)
    {
      return I_FALSE;
    }

  return ScopeAsDecl (this->pd_defined_in)->has_ancestor (s);
}

idl_bool
AST_Decl::is_child (AST_Decl *s)
{
  if (this->defined_in ())
    {
      AST_Decl *d = ScopeAsDecl (this->defined_in ());

      if (d == 0)
        {
          return 0;
        }

      if (ACE_OS::strcmp (d->full_name (), s->full_name ()) == 0)
        {
          return 1;
        }
    }

  return 0; // Not a child.
}

idl_bool
AST_Decl::is_nested (void)
{
  AST_Decl *d = ScopeAsDecl (this->defined_in ());

  // If we have an outermost scope and if that scope is not that of the Root,
  // then we are defined at some nesting level.
  if (d != 0 && d->node_type () != AST_Decl::NT_root)
    {
      return I_TRUE;
    }

  return I_FALSE;
}

// Dump this AST_Decl to the ostream o.
void
AST_Decl::dump (ACE_OSTREAM_TYPE &o)
{
  this->pd_local_name->dump (o);
}

int
AST_Decl::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_decl (this);
}

void
AST_Decl::destroy (void)
{
  // These are not set for the root node.
  if (this->pd_name != 0)
    {
      this->pd_name->destroy ();
      delete this->pd_name;
      this->pd_name = 0;
    }

  if (this->pd_local_name != 0)
    {
      this->pd_local_name->destroy ();
      delete this->pd_local_name;
      this->pd_local_name = 0;
    }

  if (this->pd_original_local_name != 0)
    {
      this->pd_original_local_name->destroy ();
      delete this->pd_original_local_name;
      this->pd_original_local_name = 0;
    }

  delete [] this->full_name_;
  this->full_name_ = 0;

  delete [] this->repoID_;
  this->repoID_ = 0;

  delete [] this->prefix_;
  this->prefix_ = 0;

  delete [] this->version_;
  this->version_ = 0;

  delete [] this->flat_name_;
  this->flat_name_ = 0;
}

// Data accessors.

const char *
AST_Decl::full_name (void)
{
  if (this->full_name_ == 0)
    {
      compute_full_name ();
    }

  return this->full_name_;
}

const char *
AST_Decl::repoID (void)
{
  if (this->repoID_ == 0)
    {
      this->compute_repoID ();
    }

  return this->repoID_;
}

void
AST_Decl::repoID (char *value)
{
  if (this->repoID_ == 0)
    {
      this->repoID_ = value;

      // Forces version to be set to the last id component.
      delete [] this->version_;
      this->version_ = 0;
      (void) this->version ();
    }
  else if (ACE_OS::strcmp (this->repoID_, value) != 0)
    {
      idl_global->err ()->id_reset_error (this->repoID_,
                                          value);
    }
}

const char *
AST_Decl::prefix (void)
{
  return this->prefix_;
}

void
AST_Decl::prefix (char *value)
{
  delete [] this->prefix_;
  this->prefix_ = ACE::strnew (value);
}

const char *
AST_Decl::version (void)
{
  if (this->version_ == 0)
    {
      // Calling the method will compute if necessary.
      const char *repo_id = this->repoID ();

      // All forms of repo id should contain two colons, the
      // version coming after the second one.
      const char *tail1 = 0;
      const char *tail2 = 0;

      if (repo_id != 0)
        {
          tail1 = ACE_OS::strchr (repo_id,
                                  ':');
        }

      if (tail1 != 0)
        {
          tail2 = ACE_OS::strchr (tail1 + 1,
                                  ':');
        }

      this->version_ = (tail2 == 0) ? ACE::strnew ("1.0")
                                    : ACE::strnew (tail2 + 1);
    }

  return this->version_;
}

void
AST_Decl::version (char *value)
{
  // Previous #pragma version or #pragma id make this illegal.
  if (this->version_ == 0 && this->repoID_ == 0
      || ACE_OS::strcmp (this->version_, value) == 0)
    {
      delete [] this->version_;
      this->version_ = value;
    }
  else
    {
      idl_global->err ()->version_reset_error ();
    }
}

idl_bool
AST_Decl::anonymous (void) const
{
  return this->anonymous_;
}

idl_bool
AST_Decl::typeid_set (void) const
{
  return this->typeid_set_;
}

void
AST_Decl::typeid_set (idl_bool val)
{
  this->typeid_set_ = val;
}

void
AST_Decl::set_id_with_typeid (char *value)
{
  // Can't call 'typeid' twice, even with the same value.
  if (this->typeid_set ())
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_TYPEID_RESET,
                                  this);
    }

  // Are we a legal type for 'typeid'?
  switch (this->pd_node_type)
  {
    case AST_Decl::NT_field:
      {
        AST_Interface *iface = 
          AST_Interface::narrow_from_scope (this->defined_in ());

        if (iface == 0 || iface->node_type () == AST_Decl::NT_valuetype == 0)
          {
            idl_global->err ()->error1 (UTL_Error::EIDL_INVALID_TYPEID,
                                        this);

            return;
          }

        break;
      }
    case AST_Decl::NT_module:
    case AST_Decl::NT_interface:
    case AST_Decl::NT_const:
    case AST_Decl::NT_typedef:
    case AST_Decl::NT_except:
    case AST_Decl::NT_attr:
    case AST_Decl::NT_op:
    case AST_Decl::NT_enum:
    case AST_Decl::NT_factory:
    case AST_Decl::NT_component:
    case AST_Decl::NT_home:
    case AST_Decl::NT_eventtype:
      break;
    default:
      idl_global->err ()->error1 (UTL_Error::EIDL_INVALID_TYPEID,
                                  this);

      return;
  }

  delete [] this->repoID_;
  this->repoID_ = 0;
  this->repoID (value);
  this->typeid_set (I_TRUE);
}

void
AST_Decl::set_prefix_with_typeprefix (char *value)
{
  // Are we a legal type for 'typeprefix'? This is checked only at
  // the top level.
  switch (this->pd_node_type)
  {
    case AST_Decl::NT_module:
    case AST_Decl::NT_interface:
    case AST_Decl::NT_valuetype:
    case AST_Decl::NT_eventtype:
    case AST_Decl::NT_struct:
    case AST_Decl::NT_union:
    case AST_Decl::NT_except:
      break;
    default:
      idl_global->err ()->error1 (UTL_Error::EIDL_INVALID_TYPEPREFIX,
                                  this);

      return;
  }

  this->set_prefix_with_typeprefix_r (value,
                                      DeclAsScope (this));
}

idl_bool
AST_Decl::imported (void)
{
  return this->pd_imported;
}

void
AST_Decl::set_imported (idl_bool is_it)
{
  this->pd_imported = is_it;
}

idl_bool
AST_Decl::in_main_file (void)
{
  return this->pd_in_main_file;
}

void
AST_Decl::set_in_main_file (idl_bool is_it)
{
  this->pd_in_main_file = is_it;
}

idl_bool
AST_Decl::added (void)
{
  return this->pd_added;
}

void
AST_Decl::set_added (idl_bool is_it)
{
  this->pd_added = is_it;
}

UTL_Scope *
AST_Decl::defined_in (void)
{
  return this->pd_defined_in;
}

void
AST_Decl::set_defined_in (UTL_Scope *s)
{
  this->pd_defined_in = s;
}

AST_Decl::NodeType
AST_Decl::node_type (void)
{
  return this->pd_node_type;
}

long
AST_Decl::line (void)
{
  return this->pd_line;
}

void
AST_Decl::set_line (long l)
{
  this->pd_line = l;
}

UTL_String *
AST_Decl::file_name (void)
{
  return this->pd_file_name;
}

void
AST_Decl::set_file_name (UTL_String *s)
{
  this->pd_file_name = s;
}

UTL_ScopedName *
AST_Decl::name (void)
{
  return this->pd_name;
}

// @@ Wherever compute_* are called, we should remember to delete them
//    after use.

// Variation of the <name>. Computes scoped name string, applying
// prefix and suffix to the local name component.
UTL_ScopedName *
AST_Decl::compute_name (const char *prefix,
                        const char *suffix)
{
  if (prefix == 0 || suffix == 0)
    {
      return 0;
    }

  UTL_ScopedName *result_name = 0;

  // Prepare prefix_<local_name>_suffix string.

  ACE_CString suffix_str (suffix);
  ACE_CString local_str (this->local_name ()->get_string ());

  ACE_CString result_local_str (prefix);
  result_local_str += local_str;
  result_local_str += suffix_str;

  // Identifier for the resulting local name.
  Identifier *result_local_id = 0;
  ACE_NEW_RETURN (result_local_id,
                  Identifier (result_local_str.c_str ()),
                  0);

  // UTL_Scoped name for the resulting local name.
  UTL_ScopedName *result_local_name = 0;
  ACE_NEW_RETURN (result_local_name,
                  UTL_ScopedName (result_local_id,
                                  0),
                  0);

  // Global scope?
  if (this->defined_in () == 0)
    {
      result_name = result_local_name;
    }
  else
    {
      // OK, not global. So copy name of containing scope, then
      // smash last cdr of copy with new component.

      AST_Decl *d = ScopeAsDecl (this->defined_in ());

      if (d != 0)
        {
          UTL_ScopedName *cn = d->name ();

          if (cn != 0)
            {
              result_name = (UTL_ScopedName *) cn->copy ();

              if (result_name == 0)
                {
                  result_name = result_local_name;
                }
              else
                {
                  result_name->nconc (result_local_name);
                }
            }
        }
    }

  return result_name;
}

void
AST_Decl::set_name (UTL_ScopedName *n)
{
  if (this->pd_name == n)
    {
      return;
    }

  if (this->pd_name != 0)
    {
      this->pd_name->destroy ();
      delete this->pd_name;
    }

  this->pd_name = n;

  if (n != 0)
    {
      if (this->pd_local_name != 0)
        {
          this->pd_local_name->destroy ();
          delete this->pd_local_name;
        }

      this->pd_local_name = n->last_component ()->copy ();

      // The name without _cxx_ prefix removed, if there was any.
      if (this->pd_original_local_name != 0)
        {
          this->pd_original_local_name->destroy ();
          delete this->pd_original_local_name;
        }

      this->original_local_name (n->last_component ());
    }
}

Identifier *
AST_Decl::local_name (void)
{
  return this->pd_local_name;
}

void
AST_Decl::local_name (Identifier *id)
{
  if (this->pd_local_name != 0)
    {
      this->pd_local_name->destroy ();
    }

  delete this->pd_local_name;
  this->pd_local_name = id;
}

Identifier *
AST_Decl::compute_local_name (const char *prefix,
                              const char *suffix)
{
  if (prefix == 0 || suffix == 0)
    {
      return 0;
    }

  // Init the result with prefix.
  ACE_CString result_str (prefix);

  // Put local.
  result_str += ACE_CString (this->local_name ()->get_string ());

  // Put suffix.
  result_str += ACE_CString (suffix);

  // Identifier for the resulting local name.
  Identifier *result_id = 0;
  ACE_NEW_RETURN (result_id,
                  Identifier (result_str.c_str ()),
                  0);

  return result_id;
}

// If there is _cxx_ in the beginning, we will remove that and keep
// a copy of the original name. TAO IDL's front end adds _cxx_
// prefix to the all the reserved keywords. But when we invoke the
// operation remotely, we should be sending only the name with out
// "_cxx_" prefix.

void
AST_Decl::original_local_name (Identifier *local_name)
{
  // Remove _cxx_ if it is present.
  if (ACE_OS::strstr (local_name->get_string (),
                      "_cxx_")
        == local_name->get_string ())
    {
      // CSting class is good to do this stuff.
      ACE_CString name_str (local_name->get_string (),
                            0,
                            0);

      // Remove _cxx_.
      name_str = name_str.substr (ACE_OS::strlen ("_cxx_"));

      // Assign to the Identifier variable.
      ACE_NEW (this->pd_original_local_name,
               Identifier (name_str.c_str ()));
    }
  else
    {
      this->pd_original_local_name = local_name->copy ();
    }
}

Identifier *
AST_Decl::original_local_name (void)
{
  return this->pd_original_local_name;
}

UTL_ScopedName *
AST_Decl::last_referenced_as (void) const
{
  return this->last_referenced_as_;
}

void
AST_Decl::last_referenced_as (UTL_ScopedName *n)
{
  this->last_referenced_as_ = n;
}

UTL_Scope *
AST_Decl::prefix_scope (void)
{
  return this->prefix_scope_;
}

void
AST_Decl::prefix_scope (UTL_Scope *s)
{
  this->prefix_scope_ = s;
}

// Container types will override this.
int 
AST_Decl::contains_wstring (void)
{
  if (this->contains_wstring_ == -1)
    {
      switch (this->node_type ())
        {
          case AST_Decl::NT_array:
            {
              AST_Array *a = AST_Array::narrow_from_decl (this);
              this->contains_wstring_ = 
                a->base_type ()->contains_wstring ();
              break;
            }
          case AST_Decl::NT_except:
          case AST_Decl::NT_struct:
          case AST_Decl::NT_union:
            {
              AST_Structure *s = AST_Structure::narrow_from_decl (this);
              this->contains_wstring_ = 
                s->contains_wstring ();
              break;
            }
          case AST_Decl::NT_sequence:
            {
              AST_Sequence *s = AST_Sequence::narrow_from_decl (this);
              this->contains_wstring_ = 
                s->base_type ()->contains_wstring ();
              break;
            }
          case AST_Decl::NT_attr:
          case AST_Decl::NT_field:
          case AST_Decl::NT_union_branch:
            {
              AST_Field *f = AST_Field::narrow_from_decl (this);
              this->contains_wstring_ = 
                f->field_type ()->contains_wstring ();
              break;
            }
          case AST_Decl::NT_typedef:
            {
              AST_Typedef *td = AST_Typedef::narrow_from_decl (this);
              this->contains_wstring_ = 
                td->primitive_base_type ()->contains_wstring ();
              break;
            }
          case AST_Decl::NT_wstring:
            this->contains_wstring_ = 1;
            break;
          default:
            this->contains_wstring_ = 0;
            break;
        }
    }

  return this->contains_wstring_;
}

// Non-virtual - no need to override this one.
void 
AST_Decl::contains_wstring (int val)
{
  this->contains_wstring_ = val;
}

//Narrowing methods for AST_Decl.
IMPL_NARROW_METHODS0(AST_Decl)
IMPL_NARROW_FROM_DECL(AST_Decl)

// Narrowing methods for COMMON_Base.
IMPL_NARROW_METHODS0(COMMON_Base)
