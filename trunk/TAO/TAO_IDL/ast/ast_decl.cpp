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
 * ast_decl.cc - Implementation of class AST_Decl
 *
 * AST_Decl is the base class for all AST nodes except AST_Expression.
 * AST_Decls have a node type (a value from the enum AST_Decl::NodeType)
 * a name (a UTL_ScopedName) and a list of pragmas (a UTL_StrList).
 * Additionally AST_Decl nodes record the scope of definition, the
 * file name in which they were defined, the line on which they were
 * defined in that file, and a boolean denoting whether this is the
 * main file or an #include'd file.
 */

#include "idl.h"
#include "idl_extern.h"
#include "ace/SString.h"

ACE_RCSID(ast, ast_decl, "$Id$")

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
    pd_imported (I_FALSE),
    pd_in_main_file (I_FALSE),
    pd_defined_in (0),
    pd_node_type (NT_module),
    pd_line (-1),
    pd_file_name (0),
    pd_name (0),
    pd_local_name (0),
    pd_original_local_name (0),
    pd_pragmas (0),
    pd_added (I_FALSE),
    full_name_ (0),
    prefix_ (0),
    anonymous_ (I_FALSE)
{
}

AST_Decl::AST_Decl (NodeType nt,
                    UTL_ScopedName *n,
                    UTL_StrList *p,
                    idl_bool anonymous)
  : repoID_ (0),
    pd_imported (idl_global->imported ()),
    pd_in_main_file (idl_global->in_main_file ()),
    pd_defined_in (idl_global->scopes ()->depth () > 0
                     ? idl_global->scopes ()->top ()
                     : 0),
    pd_node_type (nt),
    pd_line (idl_global->lineno ()),
    pd_file_name (idl_global->filename ()),
    pd_name (0),
    pd_local_name (n == 0 ? 0 : n->last_component ()->copy ()),
    pd_pragmas (p),
    pd_added (I_FALSE),
    full_name_ (0),
    prefix_ (0),
    anonymous_ (anonymous)
{
  compute_full_name (n);

  // Keep the name _cxx_ removed, if any.
  if (n != 0)
    {
      this->original_local_name (n->last_component ()->copy ());
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
  UTL_ScopedName *cn = 0;
  AST_Decl *d = 0;

  // Initialize this name to 0.
  this->pd_name = 0;

  // Global scope?
  if (defined_in () == 0)
    {
      this->pd_name = n;
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
      this->pd_name = (UTL_ScopedName *) cn->copy ();
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
  else
    {
      if (this->pd_name == 0)
        {
          ACE_NEW (this->pd_name,
                   UTL_ScopedName (0,
                                   0));
        }
      else
        {
          UTL_ScopedName *conc_name = 0;
          ACE_NEW (conc_name,
                   UTL_ScopedName (0,
                                   0));

          this->pd_name->nconc (conc_name);
        }
    }
}

// Compoute stringified prefix.
void
AST_Decl::compute_prefix (void)
{
  const char* pragma = 0;

  if (this->pragmas () != 0)
    {
      for (UTL_StrlistActiveIterator i (this->pragmas ());
           !i.is_done ();
           i.next ())
        {
          const char* s = i.item ()->get_string ();

          if (ACE_OS::strncmp (s, "#pragma prefix", 14) == 0)
            {
              pragma = s;
            }
        }
    }

  if (pragma != 0)
    {
      // Get pointers to each end of the substring between the quotes.
      const char* start = ACE_OS::strchr (pragma, '"') + 1;
      const char* end = ACE_OS::strchr (start, '"');

      if (end == 0)
        {
          idl_global->err ()->syntax_error (
              IDL_GlobalData::PS_PragmaPrefixSyntax
            );
          this->prefix_ = ACE::strnew ("");
          return;
        }

      int len = end - start;

      ACE_NEW (this->prefix_,
               char[len + 1]);

      ACE_OS::strncpy (this->prefix_, 
                       start, 
                       len);

      this->prefix_[len] = 0;
      return;
    }

  // Could not find it in the local scope, try to recurse to the top
  // scope...
  if (this->defined_in () == 0)
    {
      this->prefix_ = ACE::strnew ("");
    }
  else
    {
      UTL_Scope *scope = this->defined_in ();

      if (scope == 0)
        {
          this->prefix_ = ACE::strnew ("");
        }
      else
        {
          AST_Decl *d = ScopeAsDecl (scope);

          if (d != 0)
            {
              this->prefix_ = ACE::strnew (d->prefix ());
            }
          else
            {
              this->prefix_ = ACE::strnew ("");
            }
        }
    }
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
      long namelen = 0;
      UTL_IdListActiveIterator *i = 0;
      long first = I_TRUE;
      long second = I_FALSE;

      // In the first loop, compute the total length.
      ACE_NEW (i,
               UTL_IdListActiveIterator (this->name ()));

      while (!i->is_done ())
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
          namelen += ACE_OS::strlen (i->item ()->get_string ());

          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }

          i->next ();
        }

      delete i;

      ACE_NEW (this->full_name_,
               char[namelen + 1]);

      this->full_name_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;

      ACE_NEW (i,
               UTL_IdListActiveIterator (this->name ()));

      while (!(i->is_done ()))
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
          ACE_OS::strcat (this->full_name_, i->item ()->get_string ());

          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }

          i->next ();
        }

      delete i;
    }

  return;
}

// Compute stringified repository ID.
void
AST_Decl::compute_repoID (void)
{
  if (this->repoID_ != 0)
    {
      return;
    }
  else
    {
      long namelen = 8; // for the prefix "IDL:" and suffix ":1.0"
      UTL_IdListActiveIterator *i = 0;
      long first = I_TRUE;
      long second = I_FALSE;

      // in the first loop compute the total length
      namelen += ACE_OS::strlen (this->prefix ()) + 1;

      ACE_NEW (i,
               UTL_IdListActiveIterator (this->name ()));

      while (!(i->is_done ()))
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
          namelen += ACE_OS::strlen (i->item ()->get_string ());

          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }

          i->next ();
        }

      delete i;

      ACE_NEW (this->repoID_,
               char[namelen + 1]);

      this->repoID_[0] = '\0';

      ACE_OS::sprintf (this->repoID_, 
                       "%s", 
                       "IDL:");

      ACE_OS::strcat (this->repoID_, 
                      this->prefix ());

      // Add the "/" only if there is a prefix.
      if (ACE_OS::strcmp (this->prefix (), "") != 0)
        {
          ACE_OS::strcat (this->repoID_, "/");
        }

      ACE_NEW (i,
               UTL_IdListActiveIterator (this->name ()));

      first = I_TRUE;
      second = I_FALSE;

      while (!(i->is_done ()))
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
          ACE_OS::strcat (this->repoID_, i->item ()->get_string ());

          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                {
                  // Does not start with a "".
                  first = I_FALSE;
                }
              else
                {
                  second = I_TRUE;
                }
            }

          i->next ();
        }

      delete i;
      ACE_OS::strcat (this->repoID_, 
                      ":1.0");
    }

  return;
}

// Public operations.

// Return TRUE if one of my ancestor scopes is "s"
// and FALSE otherwise.
idl_bool
AST_Decl::has_ancestor (AST_Decl *s)
{
  if (this == s)
    {
      return I_TRUE;
    }

  if (pd_defined_in == NULL)
    {
      return I_FALSE;
    }

  return ScopeAsDecl (pd_defined_in)->has_ancestor (s);
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

// Dump this AST_Decl to the ostream o.
void
AST_Decl::dump (ostream &o)
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
  this->pd_name->destroy ();
  delete this->pd_name;
  this->pd_name = 0;

  this->pd_local_name->destroy ();
  delete this->pd_local_name;
  this->pd_local_name = 0;

  this->pd_original_local_name->destroy ();
  delete this->pd_original_local_name;
  this->pd_original_local_name = 0;

  if (this->full_name_ != 0)
    {
      delete [] this->full_name_;
      this->full_name_ = 0;
    }

  if (this->repoID_ != 0)
    {
      delete [] this->repoID_;
      this->repoID_ = 0;
    }

  if (this->prefix_ != 0)
    {
      delete [] this->prefix_;
      this->prefix_ = 0;
    }

  // Pragmas will be done in IDL_GlobalData
  // because they're not copied.
}

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

const char *
AST_Decl::prefix (void)
{
  if (this->prefix_ == 0)
    {
      this->compute_prefix ();
    }

  return this->prefix_;
}

idl_bool
AST_Decl::anonymous (void) const
{
  return this->anonymous_;
}

// Data accessors.

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
AST_Decl::set_defined_in(UTL_Scope *s)
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
  this->pd_name = n;

  if (n != NULL)
    {
      this->pd_local_name = n->last_component ()->copy ();

      // The name without _cxx_ prefix removed, if there was any.
      this->original_local_name (n->last_component ()->copy ());
    }
}

Identifier *
AST_Decl::local_name (void)
{
  return this->pd_local_name;
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
      ACE_CString name_str (local_name->get_string ());

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

void
AST_Decl::add_pragmas (UTL_StrList *p)
{
  if (p != 0)
    {
      if (this->pd_pragmas != 0)
        {
          this->pd_pragmas->nconc (p);
        }
      else
        {
          this->pd_pragmas = p;
        }
    }
}

UTL_StrList *
AST_Decl::pragmas (void)
{
  return this->pd_pragmas;
}

void
AST_Decl:: pragmas (UTL_StrList *p)
{
  this->pd_pragmas = p;
}

//Narrowing methods for AST_Decl.
IMPL_NARROW_METHODS0(AST_Decl)
IMPL_NARROW_FROM_DECL(AST_Decl)

// Narrowing methods for COMMON_Base.
IMPL_NARROW_METHODS0(COMMON_Base)
