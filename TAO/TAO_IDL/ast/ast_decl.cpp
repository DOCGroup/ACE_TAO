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

#include <ast_interface.h>
#include <ast_module.h>
#include <ast_array.h>
#include <ast_field.h>
#include <ast_structure.h>
#include <ast_sequence.h>
#include <ast_map.h>
#include <ast_string.h>
#include <ast_typedef.h>
#include <ast_visitor.h>
#include <global_extern.h>
#include <nr_extern.h>
#include <utl_identifier.h>
#include <utl_string.h>
#include <utl_scope.h>
#include <utl_err.h>
#include <ast_annotation_appl.h>
#include <utl_indenter.h>

#include <utility>

// FUZZ: disable check_for_streams_include
#include <ace/streams.h>
#include <ace/OS_NS_stdio.h>
#include <ace/OS_NS_stdlib.h>

#include <string>
#include <string_view>

COMMON_Base::COMMON_Base (bool local,
                          bool abstract)
  : is_local_ (local),
    is_abstract_ (abstract)
{
}

COMMON_Base::~COMMON_Base () = default;

void
COMMON_Base::destroy ()
{
}

bool
COMMON_Base::is_local ()
{
  return this->is_local_;
}

void
COMMON_Base::is_local (bool val)
{
  this->is_local_ = val;
}

bool
COMMON_Base::is_abstract () const
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
    builtin_ (idl_global->in_eval_),
    pd_imported (idl_global->imported ()),
    pd_in_main_file (idl_global->in_main_file ()),
    pd_defined_in (idl_global->scopes ().depth () > 0
                     ? idl_global->scopes ().top ()
                     : nullptr),
    pd_node_type (nt),
    pd_line (idl_global->lineno ()),
    pd_local_name (n ? n->last_component ()->copy () : nullptr),
    anonymous_ (anonymous),
    in_tmpl_mod_not_aliased_ (idl_global->in_tmpl_mod_no_alias ())
{
  // If this is the root node, the filename won't have been set yet.
  UTL_String *fn = idl_global->filename ();
  this->pd_file_name = fn != nullptr ? fn->get_string_view_or_empty () : std::string_view {};

  this->compute_full_name (n);

  char *prefix = nullptr;
  idl_global->pragma_prefixes ().top (prefix);
  this->prefix_ = prefix ? prefix : "";

  if (n)
    {
      // The function body creates its own copy.
      this->original_local_name (n->last_component ());
    }

  this->compute_repoID ();
}

AST_Decl::AST_Decl (
  UTL_ScopedName *name,
  AST_Decl *other)
  : COMMON_Base (),
    builtin_ (idl_global->in_eval_),
    pd_imported (idl_global->imported ()),
    pd_in_main_file (idl_global->in_main_file ()),
    pd_defined_in (idl_global->scopes ().depth () > 0
                     ? idl_global->scopes ().top ()
                     : nullptr),
    pd_node_type (other->node_type ()),
    pd_line (idl_global->lineno ()),
    pd_local_name (name ? name->last_component ()->copy () : nullptr),
    anonymous_ (other->anonymous ()),
    in_tmpl_mod_not_aliased_ (idl_global->in_tmpl_mod_no_alias ())
{
  // If this is the root node, the filename won't have been set yet.
  UTL_String *fn = idl_global->filename ();
  this->pd_file_name = fn != nullptr ? fn->get_string_view_or_empty () : std::string_view {};

  this->compute_full_name (name);

  char *prefix = nullptr;
  idl_global->pragma_prefixes ().top (prefix);
  this->prefix_ = prefix ? prefix : "";

  if (name)
    {
      // The function body creates its own copy.
      this->original_local_name (name->last_component ());
    }

  this->compute_repoID ();
}

AST_Decl::~AST_Decl () = default;

void
AST_Decl::destroy ()
{
  // These are not set for the root node.
  if (this->pd_name)
    {
      this->pd_name->destroy ();
      delete this->pd_name;
      this->pd_name = nullptr;
    }

  if (this->pd_local_name)
    {
      this->pd_local_name->destroy ();
      delete this->pd_local_name;
      this->pd_local_name = nullptr;
    }

  if (this->pd_original_local_name)
    {
      this->pd_original_local_name->destroy ();
      delete this->pd_original_local_name;
      this->pd_original_local_name = nullptr;
    }

  if (this->last_referenced_as_)
    {
      this->last_referenced_as_->destroy ();
      delete this->last_referenced_as_;
      this->last_referenced_as_ = nullptr;
    }

  this->full_name_.reset ();
  this->repoID_.reset ();
  this->prefix_.reset ();
  this->version_.reset ();
  this->flat_name_.reset ();

  delete annotation_appls_;
  annotation_appls_ = nullptr;
}

AST_Decl *
AST_Decl::adjust_found (
  bool /*ignore_fwd*/,
  bool /*full_def_only*/)
{
  return this; // Defaults to no adjustment
}

bool
AST_Decl::is_fwd () const
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
  this->pd_name = nullptr;

  // OK, not global. So copy name of containing scope, then
  // smash last cdr of copy with new component
  UTL_ScopedName *cn = nullptr;
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
          UTL_ScopedName *conc_name = nullptr;
          ACE_NEW (conc_name,
                   UTL_ScopedName (this->pd_local_name->copy (), nullptr));
          this->pd_name->nconc (conc_name);
        }
      else
        {
          ACE_NEW (this->pd_name,
                   UTL_ScopedName (this->pd_local_name->copy (), nullptr));
        }
    }
}

void
AST_Decl::set_prefix_with_typeprefix_r (std::string_view value,
                                        UTL_Scope *appeared_in)
{
  std::string const prefix_value (value);

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

  this->repoID_.reset ();
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
              tmp->set_prefix_with_typeprefix_r (prefix_value, appeared_in);
            }
        }
    }

  // This will recursively catch all previous openings of a module.
  if (this->node_type () == AST_Decl::NT_module)
    {
      AST_Module *m = dynamic_cast<AST_Module*> (this);
      while ((m = m->previous_opening ()) != nullptr)
        {
          for (UTL_ScopeActiveIterator si (m, UTL_Scope::IK_decls);
               !si.is_done ();
               si.next ())
            {
              AST_Decl *d = si.item ();
              if (d->node_type () != AST_Decl::NT_pre_defined)
                {
                  d->set_prefix_with_typeprefix_r (prefix_value, appeared_in);
                }
            }
        }
    }

  this->compute_repoID ();
}

// Protected operations.

// Compute stringified fully scoped name.
void
AST_Decl::compute_full_name () const
{
  if (!this->full_name_)
    {
      std::string name;
      bool first = true;
      bool second = false;

      for (UTL_IdListActiveIterator i (this->pd_name);
           !i.is_done ();
           i.next ())
        {
          if (!first)
            {
              name += "::";
            }
          else if (second)
            {
              first = second = false;
            }

          std::string_view const identifier (i.item ()->get_string_view ());
          name.append (identifier);
          if (first)
            {
              if (!identifier.empty ())
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

      this->full_name_ = name;
    }
}

// Compute stringified repository ID.
void
AST_Decl::compute_repoID () const
{
  if (this->repoID_)
    {
      return;
    }

  bool first = true;
  bool second = false;
  std::string_view prefix = this->prefix_ ? *this->prefix_ : std::string_view {};
  UTL_Scope *scope = this->defined_in ();

  // If our prefix is empty, we check to see if an ancestor has one.
  while (scope && prefix.empty ())
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
  std::string_view version = this->version_ ? *this->version_ : std::string_view {};
  scope = this->defined_in ();

  // If our version has not been set, we use the parent's, if any.
  while (version.empty () && scope)
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      version = parent->version_ ? *parent->version_ : std::string_view {};
      scope = parent->defined_in ();
    }

  std::string repo_id ("IDL:");

  if (!prefix.empty ())
    {
      repo_id += prefix;
      repo_id += '/';
    }

  for (UTL_IdListActiveIterator j (this->name ());
       !j.is_done ();
       j.next ())
    {
      if (!first)
        {
          repo_id += '/';
        }
      else if (second)
        {
          first = second = false;
        }

      std::string_view identifier (j.item ()->get_string_view ());
      bool const identifier_empty = identifier.empty ();
      static constexpr std::string_view cxx_prefix ("_cxx_");
      if (identifier.substr (0, cxx_prefix.size ()) == cxx_prefix)
        {
          identifier.remove_prefix (cxx_prefix.size ());
        }
      repo_id.append (identifier);

      if (first)
        {
          if (!identifier_empty)
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

  if (!version.empty ())
    {
      repo_id += ':';
      repo_id += version;
    }
  else
    {
      repo_id += ":1.0";
    }

  this->repoID_ = repo_id;
}

// Public operations.

std::string_view
AST_Decl::flat_name () const
{
  if (!this->flat_name_)
    {
      this->compute_flat_name ();
    }

  return this->flat_name_ ? std::string_view (*this->flat_name_) : std::string_view {};
}

// Compute stringified flattened fully scoped name.
void
AST_Decl::compute_flat_name () const
{
  if (!this->flat_name_)
    {
      std::string flat_name;
      bool first = true;
      bool second = false;

      for (UTL_IdListActiveIterator j (this->name ());
           !j.is_done ();
           j.next ())
        {
          if (!first)
            {
              flat_name += '_';
            }
          else if (second)
            {
              first = second = false;
            }

          // Print the identifier, leaving out
          // _cxx_ prefix, if any.
          Identifier *tmp = j.item ()->copy ();
          FE_Utils::original_local_name (tmp);
          std::string_view const item_name (tmp->get_string_view ());
          flat_name.append (item_name);

          if (first)
            {
              if (!item_name.empty ())
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
        }

      this->flat_name_ = flat_name;
    }
}

std::string_view
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

    case NT_map:
      return "map";

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
    default:
      return "";
    }
}

std::string_view AST_Decl::node_type_name () const
{
  return node_type_to_string (pd_node_type);
}

// Return TRUE if one of my ancestor scopes is "s"
// and FALSE otherwise.
bool
AST_Decl::has_ancestor (AST_Decl const *s) const
{
  AST_Decl const *work = this;
  do
    {
      if (work == s)
        {
          return true;
        }

      AST_Module const *m = dynamic_cast<AST_Module const*> (s);
      if (m)
        {
          while ((m = m->previous_opening ()) != nullptr)
            {
              if (static_cast<AST_Decl const *> (m) == s)
                {
                  return true;
                }
            }
        }

      work = work->pd_defined_in ?
             ScopeAsDecl (work->pd_defined_in) :
             nullptr;
    } while (work);

  return false;
}

bool
AST_Decl::is_child (AST_Decl *s) const
{
  if (this->defined_in ())
    {
      AST_Decl *d = ScopeAsDecl (this->defined_in ());
      if (!d)
        {
          return false;
        }

      if (d->full_name () == s->full_name ())
        {
          return true;
        }
    }

  return false; // Not a child.
}

bool
AST_Decl::is_nested () const
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
AST_Decl::dump_i (ACE_OSTREAM_TYPE &o, std::string_view s) const
{
  o << std::string (s);
}

int
AST_Decl::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_decl (this);
}

// Data accessors.

std::string_view
AST_Decl::full_name () const
{
  if (!this->full_name_)
    {
      this->compute_full_name ();
    }

  return this->full_name_ ? std::string_view (*this->full_name_) : std::string_view {};
}

std::string_view
AST_Decl::repoID () const
{
  if (this->pd_node_type == NT_root)
    {
      this->repoID_ = std::string ();
    }

  if (!this->repoID_)
    {
      this->compute_repoID ();
    }

  return this->repoID_ ? std::string_view (*this->repoID_) : std::string_view {};
}

void
AST_Decl::repoID (std::string_view value)
{
  this->repoID_ = value;
}

void
AST_Decl::clear_repoID ()
{
  this->repoID_.reset ();
}

std::string_view
AST_Decl::prefix () const
{
  return this->prefix_ ? std::string_view (*this->prefix_) : std::string_view {};
}

void
AST_Decl::prefix (std::string_view value)
{
  this->prefix_ = value;
}

std::string_view
AST_Decl::version () const
{
  if (!this->version_)
    {
      // Calling the method will compute if necessary.
      std::string_view const repo_id = this->repoID ();

      // All forms of repo id should contain two colons, the
      // version coming after the second one.
      size_t const tail1 = repo_id.find (':');
      size_t tail2 = std::string_view::npos;

      if (tail1 != std::string_view::npos)
        {
          tail2 = repo_id.find (':', tail1 + 1);
        }

      if (!this->typeid_set_ && tail2 != std::string_view::npos)
        {
          tail2 = repo_id.find (':', tail2 + 1);
        }

      if (!this->typeid_set_ && tail2 != std::string_view::npos)
        {
          this->version_ = repo_id.substr (tail2 + 1);
        }
      else
        {
          this->version_ = "1.0";
        }
    }

  return this->version_ ? std::string_view (*this->version_) : std::string_view {};
}

void
AST_Decl::version (std::string_view value)
{
  // Previous #pragma version or #pragma id make this illegal.
  if ((!this->version_ || *this->version_ == value)
      && !this->typeid_set_)
    {
      std::string const new_version (value);
      this->version_ = new_version;

      // Repo id is now computed eagerly, so a version set must update
      // is as well.
      if (this->repoID_)
        {
          std::string tmp (*this->repoID_);
          std::string::size_type const pos = tmp.rfind (':');
          if (pos != std::string::npos)
            {
              tmp = tmp.substr (0, pos + 1) + new_version;
              this->repoID_ = tmp;
            }
        }
    }
  else
    {
      idl_global->err ()->version_reset_error ();
    }
}

void
AST_Decl::version (char *value)
{
  // The checked-in generated lexer still passes an owned C string.
  std::string version_value (value);
  delete [] value;
  this->version (version_value);
}

bool
AST_Decl::anonymous () const
{
  return this->anonymous_;
}

void
AST_Decl::anonymous (bool val)
{
  this->anonymous_ = val;
}

bool
AST_Decl::typeid_set () const
{
  return this->typeid_set_;
}

void
AST_Decl::typeid_set (bool val)
{
  this->typeid_set_ = val;
}

void
AST_Decl::set_id_with_typeid (std::string_view value)
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

  this->repoID_.reset ();
  this->repoID (value);
  this->typeid_set_ = true;
}

void
AST_Decl::set_prefix_with_typeprefix (std::string_view value)
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
AST_Decl::imported () const
{
  return this->pd_imported;
}

void
AST_Decl::set_imported (bool is_it)
{
  this->pd_imported = is_it;
}

bool
AST_Decl::in_main_file () const
{
  return this->pd_in_main_file;
}

void
AST_Decl::set_in_main_file (bool is_it)
{
  this->pd_in_main_file = is_it;
}

UTL_Scope *
AST_Decl::defined_in () const
{
  return this->pd_defined_in;
}

void
AST_Decl::set_defined_in (UTL_Scope *s)
{
  this->pd_defined_in = s;
}

AST_Decl::NodeType
AST_Decl::node_type () const
{
  return this->pd_node_type;
}

long
AST_Decl::line () const
{
  return this->pd_line;
}

void
AST_Decl::set_line (long l)
{
  this->pd_line = l;
}

std::string_view
AST_Decl::file_name () const
{
  return this->pd_file_name;
}

void
AST_Decl::set_file_name (std::string_view s)
{
  this->pd_file_name = s;
}

UTL_ScopedName *
AST_Decl::name () const
{
  return this->pd_name;
}

// @@ Wherever compute_* are called, we should remember to delete them
//    after use.

// Variation of the <name>. Computes scoped name string, applying
// prefix and suffix to the local name component.
UTL_ScopedName *
AST_Decl::compute_name (std::string_view prefix,
                        std::string_view suffix) const
{
  // Prepare prefix_<local_name>_suffix string.

  std::string result_local_str (prefix);
  result_local_str.append (this->local_name ()->get_string_view ());
  result_local_str.append (suffix);

  // Identifier for the resulting local name.
  Identifier *result_local_id = nullptr;
  ACE_NEW_RETURN (result_local_id,
                  Identifier (result_local_str),
                  nullptr);

  // UTL_Scoped name for the resulting local name.
  UTL_ScopedName *result_local_name = nullptr;
  ACE_NEW_RETURN (result_local_name,
                  UTL_ScopedName (result_local_id, nullptr),
                  nullptr);

  // Global scope?
  UTL_ScopedName *result_name = nullptr;
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
      this->flat_name_.reset ();
      this->full_name_.reset ();
      this->repoID_.reset ();
    }
}

Identifier *
AST_Decl::local_name () const
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
AST_Decl::compute_local_name (std::string_view prefix,
                              std::string_view suffix) const
{
  // Init the result with prefix.
  std::string result_str (prefix);
  result_str.append (this->local_name ()->get_string_view ());
  result_str.append (suffix);

  // Identifier for the resulting local name.
  Identifier *result_id = nullptr;
  ACE_NEW_RETURN (result_id,
                  Identifier (result_str),
                  nullptr);

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
  std::string_view const name (local_name->get_string_view ());
  static constexpr std::string_view cxx_prefix ("_cxx_");
  if (name.substr (0, cxx_prefix.size ()) == cxx_prefix)
    {
      // Assign to the Identifier variable.
      ACE_NEW (this->pd_original_local_name,
               Identifier (name.substr (cxx_prefix.size ())));
    }
  else
    {
      this->pd_original_local_name = local_name->copy ();
    }
}

Identifier *
AST_Decl::original_local_name () const
{
  return this->pd_original_local_name;
}

bool
AST_Decl::is_defined ()
{
  // AST_Interface, AST_Structure, and AST_Union will
  // override this, as will AST_InterfaceFwd, etc.
  return true;
}

UTL_ScopedName *
AST_Decl::last_referenced_as () const
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
AST_Decl::prefix_scope () const
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
AST_Decl::contains_wstring ()
{
  if (this->contains_wstring_ == -1)
    {
      switch (this->node_type ())
        {
        case AST_Decl::NT_array:
          {
            AST_Array *a = dynamic_cast<AST_Array*> (this);
            this->contains_wstring_ = a->base_type ()->contains_wstring ();
            break;
          }

        case AST_Decl::NT_except:
        case AST_Decl::NT_struct:
        case AST_Decl::NT_union:
          {
            AST_Structure *s = dynamic_cast<AST_Structure*> (this);
            this->contains_wstring_ = s->contains_wstring ();
            break;
          }

        case AST_Decl::NT_sequence:
          {
            AST_Sequence *s = dynamic_cast<AST_Sequence*> (this);
            this->contains_wstring_ = s->base_type ()->contains_wstring ();
            break;
          }

        case AST_Decl::NT_map:
          {
            AST_Map *m = dynamic_cast<AST_Map *> (this);
            this->contains_wstring_ = m->key_type ()->contains_wstring () ||
              m->value_type ()->contains_wstring ();
            break;
          }

        case AST_Decl::NT_attr:
        case AST_Decl::NT_field:
        case AST_Decl::NT_union_branch:
          {
            AST_Field *f = dynamic_cast<AST_Field*> (this);
            this->contains_wstring_ = f->field_type ()->contains_wstring ();
            break;
          }

        case AST_Decl::NT_typedef:
          {
            AST_Typedef *td = dynamic_cast<AST_Typedef*> (this);
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

  AST_Module *me_mod = dynamic_cast<AST_Module*> (this);

  if (me_mod != nullptr)
    {
      AST_Module *po_mod = dynamic_cast<AST_Module*> (mod);
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
AST_Decl::in_tmpl_mod_not_aliased () const
{
  return this->in_tmpl_mod_not_aliased_;
}

void
AST_Decl::in_tmpl_mod_not_aliased (bool val)
{
  this->in_tmpl_mod_not_aliased_ = val;
}

void
AST_Decl::annotation_appls (const AST_Annotation_Appls &annotations)
{
  if (annotatable ())
    {
      annotation_appls () = annotations;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("WARNING: %C is annotated but its type can't be annotated!\n"),
        full_name ()
        ));
    }
}

AST_Annotation_Appls&
AST_Decl::annotation_appls ()
{
  if (!annotation_appls_)
    {
      annotation_appls_ = new AST_Annotation_Appls ();
    }
  return *annotation_appls_;
}

void
AST_Decl::dump_annotations (ACE_OSTREAM_TYPE &o, bool print_inline)
{
  AST_Annotation_Appls::iterator
    i = annotation_appls ().begin (),
    finished = annotation_appls ().end ();
  for (; i != finished; ++i)
    {
      AST_Annotation_Appl* a = i->get ();
      a->dump (o);
      if (print_inline)
        {
          dump_i (o, " ");
        }
      else
        {
          dump_i (o, "\n");
          idl_global->indent ()->skip_to (o);
        }
    }
}

void
AST_Decl::dump_with_annotations (ACE_OSTREAM_TYPE &o, bool inline_annotations)
{
  if (annotatable () && auto_dump_annotations())
    {
      dump_annotations (o, inline_annotations);
    }

  dump (o);
}

ACE_OSTREAM_TYPE &
operator<< (ACE_OSTREAM_TYPE &o, AST_Decl &d)
{
  d.dump_with_annotations (o, d.dump_annotations_inline ());

  return o;
}

bool
AST_Decl::annotatable () const
{
  return true;
}

bool
AST_Decl::dump_annotations_inline () const
{
  return false;
}

bool
AST_Decl::auto_dump_annotations () const
{
  return true;
}

bool
AST_Decl::builtin () const
{
  return builtin_;
}

bool
AST_Decl::should_be_dumped () const
{
  return !builtin () || idl_global->dump_builtins_;
}

AST_Annotation_Appls &
AST_Decl::annotations ()
{
  return annotation_appls ();
}

bool
AST_Decl::ami_visit () const
{
  return true;
}
