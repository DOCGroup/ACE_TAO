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
#include "utl_string.h"
#include "utl_scope.h"
#include "utl_err.h"
#include "ace/OS_NS_stdio.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

COMMON_Base::COMMON_Base (bool local,
                          bool abstract)
  : is_local_ (local),
    is_abstract_ (abstract)
{
}

COMMON_Base::~COMMON_Base (void)
{
}

void
COMMON_Base::destroy (void)
{
}

bool
COMMON_Base::is_local (void)
{
  return this->is_local_;
}

void
COMMON_Base::is_local (bool val)
{
  this->is_local_ = val;
}

bool
COMMON_Base::is_abstract (void) const
{
  return this->is_abstract_;
}

void
COMMON_Base::is_abstract (bool val)
{
  this->is_abstract_ = val;
}

AST_Decl::AST_Decl (NodeType nt,
                    UTL_ScopedName *n,
                    bool anonymous)
  : COMMON_Base (),
    repoID_ (0),
    flat_name_ (0),
    contains_wstring_ (-1),
    pd_imported (idl_global->imported ()),
    pd_in_main_file (idl_global->in_main_file ()),
    pd_defined_in (idl_global->scopes ().depth () > 0
                     ? idl_global->scopes ().top ()
                     : 0),
    pd_node_type (nt),
    pd_line (idl_global->lineno ()),
    pd_name (0),
    pd_local_name (n ? n->last_component ()->copy () : 0),
    pd_original_local_name (0),
    full_name_ (0),
    prefix_ (0),
    version_ (0),
    anonymous_ (anonymous),
    typeid_set_ (false),
    last_referenced_as_ (0),
    prefix_scope_ (0),
    in_tmpl_mod_not_aliased_ (idl_global->in_tmpl_mod_no_alias ())
{
  // If this is the root node, the filename won't have been set yet.
  UTL_String *fn = idl_global->filename ();
  this->pd_file_name = (fn ? fn->get_string () : "");

  this->compute_full_name (n);

  char *prefix = 0;
  idl_global->pragma_prefixes ().top (prefix);
  this->prefix_ = ACE::strnew (prefix ? prefix : "");

  if (n)
    {
      // The function body creates its own copy.
      this->original_local_name (n->last_component ());
    }

  this->compute_repoID ();
}

AST_Decl::~AST_Decl (void)
{
}

void
AST_Decl::destroy (void)
{
  // These are not set for the root node.
  if (this->pd_name)
    {
      this->pd_name->destroy ();
      delete this->pd_name;
      this->pd_name = 0;
    }

  if (this->pd_local_name)
    {
      this->pd_local_name->destroy ();
      delete this->pd_local_name;
      this->pd_local_name = 0;
    }

  if (this->pd_original_local_name)
    {
      this->pd_original_local_name->destroy ();
      delete this->pd_original_local_name;
      this->pd_original_local_name = 0;
    }

  if (this->last_referenced_as_)
    {
      this->last_referenced_as_->destroy ();
      delete this->last_referenced_as_;
      this->last_referenced_as_ = 0;
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

AST_Decl *
AST_Decl::adjust_found (
  bool /*ignore_fwd*/,
  bool /*full_def_only*/)
{
  return this; // Defaults to no adjustment
}

bool
AST_Decl::is_fwd (void)
{
  return false; // Not a fwd declared type (by default)
}

// Private operations.

// Compute our private UTL_ScopedName member.
void
AST_Decl::compute_full_name (UTL_ScopedName *n)
{
  // This should happen only when we are a non-void predefined type,
  // in which case our scoped name has already been created by the
  // AST_PredefinedType constructor.
  if (!n)
    {
      return;
    }

  // Global scope?
  if (!this->defined_in ())
    {
      this->pd_name = (UTL_IdList *) n->copy ();
      return;
    }

  // Initialize this name to 0.
  this->pd_name = 0;

  // OK, not global. So copy name of containing scope, then
  // smash last cdr of copy with new component
  UTL_ScopedName *cn = 0;
  AST_Decl *d = ScopeAsDecl (this->defined_in ());
  if (d)
    {
      cn = d->name ();
      if (cn)
        {
          this->pd_name = (UTL_IdList *) cn->copy ();
        }
    }

  if (this->pd_local_name)
    {
      if (this->pd_name)
        {
          UTL_ScopedName *conc_name = 0;
          ACE_NEW (conc_name,
                   UTL_ScopedName (this->pd_local_name->copy (), 0));
          this->pd_name->nconc (conc_name);
        }
      else
        {
          ACE_NEW (this->pd_name,
                   UTL_ScopedName (this->pd_local_name->copy (), 0));
        }
    }
}

void
AST_Decl::set_prefix_with_typeprefix_r (const char *value,
                                        UTL_Scope *appeared_in)
{
  if (this->typeid_set_)
    {
      return;
    }

  if (this->prefix_scope_)
    {
      AST_Decl *decl = ScopeAsDecl (this->prefix_scope_);
      bool const overridden =
        decl->has_ancestor (ScopeAsDecl (appeared_in));
      if (overridden)
        {
          return;
        }
    }

  delete [] this->repoID_;
  this->repoID_ = 0;
  this->prefix (value);
  this->prefix_scope_ = appeared_in;

  UTL_Scope *s = DeclAsScope (this);
  if (s)
    {
      for (UTL_ScopeActiveIterator i (s, UTL_Scope::IK_decls);
           !i.is_done ();
           i.next ())
        {
          AST_Decl *tmp = i.item ();
          UTL_Scope *s_tmp = DeclAsScope (tmp);
          if (s_tmp)
            {
              tmp->set_prefix_with_typeprefix_r (value, appeared_in);
            }
        }
    }

  // This will recursively catch all previous openings of a module.
  if (this->node_type () == AST_Decl::NT_module)
    {
      AST_Module *m = AST_Module::narrow_from_decl (this);
      while (!!(m = m->previous_opening ()))
        {
          for (UTL_ScopeActiveIterator si (m, UTL_Scope::IK_decls);
               !si.is_done ();
               si.next ())
            {
              AST_Decl *d = si.item ();
              if (d->node_type () != AST_Decl::NT_pre_defined)
                {
                  d->set_prefix_with_typeprefix_r (value, appeared_in);
                }
            }
        }
    }

  this->compute_repoID ();
}

// Protected operations.

// Compute stringified fully scoped name.
void
AST_Decl::compute_full_name (void)
{
  if (!this->full_name_)
    {
      size_t namelen = 0;
      long first = true;
      long second = false;
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
              first = second = false;
            }

          // Print the identifier.
          name = i.item ()->get_string ();
          namelen += ACE_OS::strlen (name);
          if (first)
            {
              if (ACE_OS::strcmp (name, ""))
                {
                  // Does not start with a "".
                  first = false;
                }
              else
                {
                  second = true;
                }
            }
        }

      ACE_NEW (this->full_name_, char [namelen + 1]);
      this->full_name_ [0] = '\0';
      first = true;
      second = false;

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
              first = second = false;
            }

          // Print the identifier.
          name = j.item ()->get_string ();
          ACE_OS::strcat (this->full_name_, name);
          if (first)
            {
              if (ACE_OS::strcmp (name, ""))
                {
                  // Does not start with a "".
                  first = false;
                }
              else
                {
                  second = true;
                }
            }
        }
    }
}

// Compute stringified repository ID.
void
AST_Decl::compute_repoID (void)
{
  if (this->repoID_)
    {
      return;
    }

  size_t namelen = 4; // for the prefix "IDL:"
  long first = true;
  long second = false;
  char *name = 0;
  const char *prefix = (this->prefix_ ? this->prefix_ : "");
  UTL_Scope *scope = this->defined_in ();
  const char *parent_prefix = 0;

  // If our prefix is empty, we check to see if an ancestor has one.
  while (scope && !ACE_OS::strcmp (prefix, ""))
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      if (parent->node_type () == AST_Decl::NT_root
          && parent->imported ())
        {
          break;
        }

      parent_prefix = parent->prefix ();
      prefix = (parent_prefix ? parent_prefix : "");
      scope = parent->defined_in ();
    }

  // in the first loop compute the total length
  namelen += ACE_OS::strlen (prefix) + 1;

  const char *version = this->version_;
  scope = this->defined_in ();

  // If our version is has not bee set, we use the parent's, if any.
  while (!version && scope)
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      version = parent->version_;
      scope = parent->defined_in ();
    }

  if (version)
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
          first = second = false;
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
          if (ACE_OS::strcmp (name, ""))
            {
              // Does not start with a "".
              first = false;
            }
          else
            {
              second = true;
            }
        }
    }

  ACE_NEW (this->repoID_, char [namelen + 1]);
  ACE_OS::sprintf (this->repoID_, "IDL:");

  if (ACE_OS::strcmp (prefix, ""))
    {
      ACE_OS::strcat (this->repoID_, prefix);
      ACE_OS::strcat (this->repoID_, "/");
    }

  first = true;
  second = false;

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
          first = second = false;
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
          ACE_OS::strcat (this->repoID_, name);
        }

      if (first)
        {
          if (ACE_OS::strcmp (name, ""))
            {
              // Does not start with a "".
              first = false;
            }
          else
            {
              second = true;
            }
        }
    }

  if (version)
    {
      ACE_OS::strcat (this->repoID_, ":");
      ACE_OS::strcat (this->repoID_, version);
    }
  else
    {
      ACE_OS::strcat (this->repoID_, ":1.0");
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
  if (this->flat_name_ == 0)
    {
      size_t namelen = 0;
      long first = true;
      long second = false;
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
              first = second = false;
            }

          // Print the identifier, leaving out
          // _cxx_ prefix, if any.
          Identifier *tmp = i.item ()->copy ();
          FE_Utils::original_local_name (tmp);
          item_name = tmp->get_string ();
          namelen += ACE_OS::strlen (item_name);

          if (first)
            {
              if (ACE_OS::strcmp (item_name, "") != 0)
                {
                  // Does not start with a "".
                  first = false;
                }
              else
                {
                  second = true;
                }
            }

          tmp->destroy ();
          delete tmp;
          tmp = 0;
        }

      ACE_NEW (this->flat_name_, char [namelen + 1]);
      this->flat_name_ [0] = '\0';
      first = true;
      second = false;

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
              first = second = false;
            }

          // Print the identifier, leaving out
          // _cxx_ prefix, if any.
          Identifier *tmp = j.item ()->copy ();
          FE_Utils::original_local_name (tmp);
          item_name = tmp->get_string ();
          ACE_OS::strcat (this->flat_name_, item_name);

          if (first)
            {
              if (ACE_OS::strcmp (item_name, ""))
                {
                  // Does not start with a "".
                  first = false;
                }
              else
                {
                  second = true;
                }
            }

          tmp->destroy ();
          delete tmp;
          tmp = 0;
        }
    }
}

const char *
AST_Decl::node_type_to_string (NodeType nt)
{
  switch (nt)
    {
    case NT_module:
      return "module";

    case NT_interface:
    case NT_interface_fwd:
      return "interface";

    case NT_valuetype:
    case NT_valuetype_fwd:
    case NT_valuebox:
      return "valuetype";

    case NT_const:
      return "const";

    case NT_except:
      return "exception";

    case NT_attr:
      return "attribute";

    case NT_union:
    case NT_union_fwd:
      return "union";

    case NT_struct:
    case NT_struct_fwd:
      return "struct";

    case NT_enum:
      return "enum";

    case NT_string:
      return "string";

    case NT_wstring:
      return "wstring";

    case NT_array:
      return "array";

    case NT_sequence:
      return "sequence";

    case NT_typedef:
      return "typedef";

    case NT_pre_defined:
      return "primitive";

    case NT_native:
      return "native";

    case NT_factory:
      return "factory";

    case NT_component:
    case NT_component_fwd:
      return "component";

    case NT_home:
      return "home";

    case NT_eventtype:
    case NT_eventtype_fwd:
      return "eventtype";

    case NT_type:
      return "typename";

    case NT_fixed:
      return "fixed";

    case NT_porttype:
      return "porttype";

    case NT_provides:
      return "provides";

    case NT_uses:
      return "uses";

    case NT_publishes:
      return "publishes";

    case NT_emits:
      return "emits";

    case NT_consumes:
      return "consumes";

    // No useful output for these.
    case NT_enum_val:
    case NT_field:
    case NT_union_branch:
    case NT_op:
    case NT_argument:
    case NT_root:
    default:
      return "";
    }
}

// Return TRUE if one of my ancestor scopes is "s"
// and FALSE otherwise.
bool
AST_Decl::has_ancestor (AST_Decl *s)
{
  AST_Decl *work = this;
  do
    {
      if (work == s)
        {
          return true;
        }

      AST_Module *m = AST_Module::narrow_from_decl (s);
      if (m)
        {
          while (!!(m = m->previous_opening ()))
            {
              if (static_cast<AST_Decl *> (m) == s)
                {
                  return true;
                }
            }
        }

      work = work->pd_defined_in ?
             ScopeAsDecl (work->pd_defined_in) :
             0;
    } while (work);

  return false;
}

bool
AST_Decl::is_child (AST_Decl *s)
{
  if (this->defined_in ())
    {
      AST_Decl *d = ScopeAsDecl (this->defined_in ());
      if (!d)
        {
          return 0;
        }

      if (!ACE_OS::strcmp (d->full_name (), s->full_name ()))
        {
          return 1;
        }
    }

  return 0; // Not a child.
}

bool
AST_Decl::is_nested (void)
{
  AST_Decl *d = ScopeAsDecl (this->defined_in ());

  // If we have an outermost scope and if that scope is not that of the Root,
  // then we are defined at some nesting level.
  return (d && d->node_type () != AST_Decl::NT_root);
}

// Dump this AST_Decl to the ostream o.
void
AST_Decl::dump (ACE_OSTREAM_TYPE &o)
{
  this->pd_local_name->dump (o);
}

void
AST_Decl::dump_i (ACE_OSTREAM_TYPE &o, const char *s) const
{
  // Have to use ACE_CString here to avoid ambiguous overload error, see
  // SString.h for an the overloaded operator << () methods.
  o << ACE_CString(s);
}

int
AST_Decl::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_decl (this);
}

// Data accessors.

const char *
AST_Decl::full_name (void)
{
  if (!this->full_name_)
    {
      this->compute_full_name ();
    }

  return this->full_name_;
}

const char *
AST_Decl::repoID (void)
{
  if (this->pd_node_type == NT_root)
    {
      delete [] this->repoID_;
      this->repoID_ = ACE::strnew ("");
    }

  if (!this->repoID_)
    {
      this->compute_repoID ();
    }

  return this->repoID_;
}

void
AST_Decl::repoID (char *value)
{
  delete [] this->repoID_;
  this->repoID_ = value;
}

const char *
AST_Decl::prefix (void)
{
  return this->prefix_;
}

void
AST_Decl::prefix (const char *value)
{
  delete [] this->prefix_;
  this->prefix_ = ACE::strnew (value);
}

const char *
AST_Decl::version (void)
{
  if (!this->version_)
    {
      // Calling the method will compute if necessary.
      const char *repo_id = this->repoID ();

      // All forms of repo id should contain two colons, the
      // version coming after the second one.
      const char *tail1 = 0;
      const char *tail2 = 0;

      if (repo_id)
        {
          tail1 = ACE_OS::strchr (repo_id, ':');
        }

      if (tail1)
        {
          tail2 = ACE_OS::strchr (tail1 + 1, ':');
        }

      if (!this->typeid_set_ && tail2)
        {
          this->version_ = ACE::strnew (tail2 + 1);
        }
      else
        {
          this->version_ = ACE::strnew ("1.0");
        }
    }

  return this->version_;
}

void
AST_Decl::version (char *value)
{
  // Previous #pragma version or #pragma id make this illegal.
  if ((!this->version_ || !ACE_OS::strcmp (this->version_, value))
      && !this->typeid_set_)
    {
      delete [] this->version_;
      this->version_ = value;

      // Repo id is now computed eagerly, so a version set must update
      // is as well.
      if (this->repoID_)
        {
          ACE_CString tmp (this->repoID_);
          ACE_CString::size_type const pos = tmp.rfind (':');
          if (pos != ACE_CString::npos)
            {
              tmp = tmp.substr (0, pos + 1) + value;
              delete [] this->repoID_;
              this->repoID_ = ACE::strnew (tmp.fast_rep ());
            }
        }
    }
  else
    {
      idl_global->err ()->version_reset_error ();
    }
}

bool
AST_Decl::anonymous (void) const
{
  return this->anonymous_;
}

void
AST_Decl::anonymous (bool val)
{
  this->anonymous_ = val;
}

bool
AST_Decl::typeid_set (void) const
{
  return this->typeid_set_;
}

void
AST_Decl::typeid_set (bool val)
{
  this->typeid_set_ = val;
}

void
AST_Decl::set_id_with_typeid (char *value)
{
  // Can't call 'typeid' twice, even with the same value.
  if (this->typeid_set ())
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_TYPEID_RESET, this);
    }

  // Are we a legal type for 'typeid'?
  switch (this->pd_node_type)
    {
    case AST_Decl::NT_field:
      {
        AST_Decl::NodeType nt =
          ScopeAsDecl (this->defined_in ())->node_type ();

        if (   nt == AST_Decl::NT_valuetype
            || nt == AST_Decl::NT_eventtype)
          {
            break;
          }
        else
          {
            idl_global->err ()->error1 (UTL_Error::EIDL_INVALID_TYPEID, this);
            return;
          }
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
      idl_global->err ()->error1 (UTL_Error::EIDL_INVALID_TYPEID, this);
      return;
    }

  delete [] this->repoID_;
  this->repoID_ = 0;
  this->repoID (ACE::strnew (value));
  this->typeid_set_ = true;
}

void
AST_Decl::set_prefix_with_typeprefix (const char *value)
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
      idl_global->err ()->error1 (UTL_Error::EIDL_INVALID_TYPEPREFIX, this);
      return;
    }

  this->set_prefix_with_typeprefix_r (value, DeclAsScope (this));
}

bool
AST_Decl::imported (void)
{
  return this->pd_imported;
}

void
AST_Decl::set_imported (bool is_it)
{
  this->pd_imported = is_it;
}

bool
AST_Decl::in_main_file (void)
{
  return this->pd_in_main_file;
}

void
AST_Decl::set_in_main_file (bool is_it)
{
  this->pd_in_main_file = is_it;
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

ACE_CString &
AST_Decl::file_name (void)
{
  return this->pd_file_name;
}

void
AST_Decl::set_file_name (ACE_CString s)
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
  if (!prefix || !suffix)
    {
      return 0;
    }

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
                  UTL_ScopedName (result_local_id, 0),
                  0);

  // Global scope?
  UTL_ScopedName *result_name = 0;
  if (!this->defined_in ())
    {
      result_name = result_local_name;
    }
  else
    {
      // OK, not global. So copy name of containing scope, then
      // smash last cdr of copy with new component.

      AST_Decl *d = ScopeAsDecl (this->defined_in ());
      if (d)
        {
          UTL_ScopedName *cn = d->name ();
          if (cn)
            {
              result_name = (UTL_ScopedName *) cn->copy ();
              if (result_name)
                {
                  result_name->nconc (result_local_name);
                }
              else
                {
                  result_name = result_local_name;
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

  if (this->pd_name)
    {
      this->pd_name->destroy ();
      delete this->pd_name;
    }

  this->pd_name = n;

  if (n)
    {
      if (this->pd_local_name)
        {
          this->pd_local_name->destroy ();
          delete this->pd_local_name;
        }

      this->pd_local_name = n->last_component ()->copy ();

      // The name without _cxx_ prefix removed, if there was any.
      if (this->pd_original_local_name)
        {
          this->pd_original_local_name->destroy ();
          delete this->pd_original_local_name;
        }

      this->original_local_name (n->last_component ());

      // These will be recomputed on demand.
      delete [] this->flat_name_;
      this->flat_name_ = 0;

      delete [] this->full_name_;
      this->full_name_ = 0;

      delete [] this->repoID_;
      this->repoID_ = 0;
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
  if (this->pd_local_name)
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
  if (!prefix || !suffix)
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
  if (ACE_OS::strstr (local_name->get_string (), "_cxx_")
        == local_name->get_string ())
    {
      // AACE_CString class is good to do this stuff.
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

bool
AST_Decl::is_defined (void)
{
  // AST_Interface, AST_Structure, and AST_Union will
  // override this, as will AST_InterfaceFwd, etc.
  return true;
}

UTL_ScopedName *
AST_Decl::last_referenced_as (void) const
{
  return this->last_referenced_as_;
}

void
AST_Decl::last_referenced_as (UTL_ScopedName *n)
{
  if (this->last_referenced_as_)
    {
      this->last_referenced_as_->destroy ();
    }

  delete this->last_referenced_as_;
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
            this->contains_wstring_ = a->base_type ()->contains_wstring ();
            break;
          }

        case AST_Decl::NT_except:
        case AST_Decl::NT_struct:
        case AST_Decl::NT_union:
          {
            AST_Structure *s = AST_Structure::narrow_from_decl (this);
            this->contains_wstring_ = s->contains_wstring ();
            break;
          }

        case AST_Decl::NT_sequence:
          {
            AST_Sequence *s = AST_Sequence::narrow_from_decl (this);
            this->contains_wstring_ = s->base_type ()->contains_wstring ();
            break;
          }

        case AST_Decl::NT_attr:
        case AST_Decl::NT_field:
        case AST_Decl::NT_union_branch:
          {
            AST_Field *f = AST_Field::narrow_from_decl (this);
            this->contains_wstring_ = f->field_type ()->contains_wstring ();
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

bool
AST_Decl::masking_checks (AST_Decl *mod)
{
  if (!this->pd_local_name->case_compare (mod->local_name ()))
    {
      return true;
    }

  AST_Module *me_mod = AST_Module::narrow_from_decl (this);

  if (me_mod != 0)
    {
      AST_Module *po_mod = AST_Module::narrow_from_decl (mod);
      if (po_mod)
        {
          while (!!(po_mod = po_mod->previous_opening ()))
            {
              if (po_mod == me_mod)
                {
                  return true;
                }
            }
        }
    }

  return false;
}

bool
AST_Decl::in_tmpl_mod_not_aliased (void) const
{
  return this->in_tmpl_mod_not_aliased_;
}

void
AST_Decl::in_tmpl_mod_not_aliased (bool val)
{
  this->in_tmpl_mod_not_aliased_ = val;
}

//Narrowing methods for AST_Decl.

IMPL_NARROW_FROM_DECL(AST_Decl)
