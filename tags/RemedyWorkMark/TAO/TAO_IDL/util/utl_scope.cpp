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

#include "utl_scope.h"
#include "utl_identifier.h"
#include "utl_err.h"
#include "utl_indenter.h"
#include "utl_string.h"
#include "utl_strlist.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_porttype.h"
#include "ast_template_module.h"
#include "ast_template_module_ref.h"
#include "ast_template_module_inst.h"
#include "ast_typedef.h"
#include "ast_type.h"
#include "ast_root.h"
#include "ast_array.h"
#include "ast_enum.h"
#include "ast_concrete_type.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_exception.h"
#include "ast_constant.h"
#include "ast_attribute.h"
#include "ast_operation.h"
#include "ast_argument.h"
#include "ast_param_holder.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_union_branch.h"
#include "ast_field.h"
#include "ast_enum_val.h"
#include "ast_native.h"
#include "ast_factory.h"
#include "ast_visitor.h"
#include "ast_generator.h"
#include "ast_extern.h"

#include "fe_extern.h"
#include "nr_extern.h"
#include "global_extern.h"

#include "ace/OS_NS_strings.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#undef  INCREMENT
#define INCREMENT 64

//  Constructors.

UTL_Scope::UTL_Scope (void)
  : pd_scope_node_type (AST_Decl::NT_module),
    pd_decls (0),
    pd_decls_allocated (0),
    pd_decls_used (0),
    pd_local_types (0),
    pd_locals_allocated (0),
    pd_locals_used (0),
    pd_referenced (0),
    pd_referenced_allocated (0),
    pd_referenced_used (0),
    pd_name_referenced (0),
    pd_name_referenced_allocated (0),
    pd_name_referenced_used (0),
    has_prefix_ (0)
{
}

UTL_Scope::UTL_Scope (AST_Decl::NodeType nt)
  : pd_scope_node_type (nt),
    pd_decls (0),
    pd_decls_allocated (0),
    pd_decls_used (0),
    pd_local_types (0),
    pd_locals_allocated (0),
    pd_locals_used (0),
    pd_referenced (0),
    pd_referenced_allocated (0),
    pd_referenced_used (0),
    pd_name_referenced (0),
    pd_name_referenced_allocated (0),
    pd_name_referenced_used (0),
    has_prefix_ (0)
{
}

// Destructor.
UTL_Scope::~UTL_Scope (void)
{
  for (UTL_ScopeActiveIterator iter (this, IK_both);
       !iter.is_done ();
       iter.next ())
    {
      AST_Decl *d = iter.item ();
      d->destroy ();
      delete d;
    }

  delete [] this->pd_decls;
  delete [] this->pd_local_types;
  delete [] this->pd_referenced;

  for (long i = this->pd_name_referenced_used; i--;) // NOTE test with i--
    {
      Identifier *id = this->pd_name_referenced[i];
      id->destroy ();
      delete id;
    }

  delete [] this->pd_name_referenced;
}

void
UTL_Scope::destroy (void)
{
  for (UTL_ScopeActiveIterator iter (this, IK_both);
       !iter.is_done ();
       iter.next ())
    {
      AST_Decl *d = iter.item ();
      d->destroy ();
      delete d;
    }

  delete [] this->pd_decls;
  this->pd_decls = 0;
  this->pd_decls_allocated = 0;
  this->pd_decls_used = 0;

  delete [] this->pd_local_types;
  this->pd_local_types = 0;
  this->pd_locals_allocated = 0;
  this->pd_locals_used = 0;

  delete [] this->pd_referenced;
  this->pd_referenced = 0;
  this->pd_referenced_allocated = 0;
  this->pd_referenced_used = 0;

  for (long i = this->pd_name_referenced_used; i--;) // NOTE test with i--
    {
      Identifier *id = this->pd_name_referenced[i];
      id->destroy ();
      delete id;
    }

  delete [] this->pd_name_referenced;
  this->pd_name_referenced = 0;
  this->pd_name_referenced_allocated = 0;
  this->pd_name_referenced_used = 0;
}

// Protected operations.

// Special version of lookup which only looks at the local name instead of
// the fully scoped name, when doing lookups. This version is intended to
// be used only by the frontend add_xxx functions.
AST_Decl *
UTL_Scope::lookup_for_add (AST_Decl *d)
{
  if (!d)
    {
      return 0;
    }

  Identifier *id = d->local_name ();
  if (this->idl_keyword_clash (id))
    {
      return 0;
    }

  return this->lookup_by_name_local (id, false);
}

int
UTL_Scope::idl_keyword_clash (Identifier *e)
{
  if (e->escaped ())
    {
      return 0;
    }

  // Convert the identifier string into a
  // canonical (uppercase) form as a ACE_CString
  char *tmp = e->get_string ();
  ACE_CString ext_id (tmp);
  UTL_String::get_canonical_rep (ext_id);

  if (!idl_global->idl_keywords ().find (ext_id))
    {
      if (idl_global->case_diff_error ())
        {
          idl_global->err ()->idl_keyword_error (tmp);
        }
      else
        {
          idl_global->err ()->idl_keyword_warning (tmp);
        }

      return -1;
    }

  return 0;
}

bool
UTL_Scope::redef_clash (AST_Decl::NodeType new_nt,
                        AST_Decl::NodeType scope_elem_nt)
{
  switch (new_nt)
    {
    case AST_Decl::NT_module:
      return scope_elem_nt != AST_Decl::NT_module;

    case AST_Decl::NT_struct:
    case AST_Decl::NT_struct_fwd:
      return scope_elem_nt != AST_Decl::NT_struct_fwd;

    case AST_Decl::NT_union:
    case AST_Decl::NT_union_fwd:
      return scope_elem_nt != AST_Decl::NT_union_fwd;

    case AST_Decl::NT_interface:
      return scope_elem_nt != AST_Decl::NT_interface_fwd;

    case AST_Decl::NT_component:
      return scope_elem_nt != AST_Decl::NT_component_fwd;

    case AST_Decl::NT_interface_fwd:
      return (   scope_elem_nt != AST_Decl::NT_interface_fwd
              && scope_elem_nt != AST_Decl::NT_interface);

    case AST_Decl::NT_component_fwd:
      return (   scope_elem_nt != AST_Decl::NT_component_fwd
              && scope_elem_nt != AST_Decl::NT_component);

    case AST_Decl::NT_valuetype:
      return scope_elem_nt != AST_Decl::NT_valuetype_fwd;

    case AST_Decl::NT_eventtype:
      return scope_elem_nt != AST_Decl::NT_eventtype_fwd;

    case AST_Decl::NT_valuetype_fwd:
      return (   scope_elem_nt != AST_Decl::NT_valuetype_fwd
              && scope_elem_nt != AST_Decl::NT_valuetype);

    case AST_Decl::NT_eventtype_fwd:
      return (   scope_elem_nt != AST_Decl::NT_eventtype_fwd
              && scope_elem_nt != AST_Decl::NT_eventtype);

    default:
      return true;
    }
}

void
UTL_Scope::check_for_predef_seq (AST_Decl *d)
{
  // Right now, we're interested only in predefined sequences
  // referenced in the main IDL file. If we are processing
  // multiple IDL files in one execution, the bits will
  // stay set, and be accumulated as each file is parsed.
  if (!idl_global->in_main_file ())
    {
      return;
    }

  AST_Type *bt = 0;
  AST_Decl::NodeType nt = d->node_type ();

  // We are interested only in members, arguments and typedefs.
  switch (nt)
    {
    case AST_Decl::NT_field:
    case AST_Decl::NT_union_branch:
    case AST_Decl::NT_attr:
    case AST_Decl::NT_argument:
      bt = AST_Field::narrow_from_decl (d)->field_type ();
      break;

    case AST_Decl::NT_typedef:
      bt = AST_Typedef::narrow_from_decl (d)->base_type ();
      break;

    default:
      return;
    }

  // Check to eliminate more candidates.
  if (!bt->imported () || bt->node_type () != AST_Decl::NT_typedef)
    {
      return;
    }

  bt = AST_Typedef::narrow_from_decl (bt)->base_type ();
  nt = bt->node_type ();

  // Must be a sequence with only one level of typedef.
  if (nt != AST_Decl::NT_sequence)
    {
      return;
    }

  // Must be defined in the CORBA module.
  AST_Decl *p = ScopeAsDecl (bt->defined_in ());
  if (ACE_OS::strcmp (p->local_name ()->get_string (), "CORBA"))
    {
      return;
    }

  // We know this narrowing will be successful.
  bt = AST_Sequence::narrow_from_decl (bt)->base_type ();
  nt = bt->node_type ();

  // First check for string or wstring base type.
  if (nt == AST_Decl::NT_string)
    {
      idl_global->string_seq_seen_ = true;
      return;
    }

  if (nt == AST_Decl::NT_wstring)
    {
      idl_global->wstring_seq_seen_ = true;
      return;
    }

  // Now check for predefined base type.
  AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (bt);
  if (!pdt)
    {
      return;
    }

  switch (pdt->pt ())
    {
    case AST_PredefinedType::PT_long:
      idl_global->long_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_ulong:
      idl_global->ulong_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_longlong:
      idl_global->longlong_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_ulonglong:
      idl_global->ulonglong_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_short:
      idl_global->short_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_ushort:
      idl_global->ushort_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_float:
      idl_global->float_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_double:
      idl_global->double_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_longdouble:
      idl_global->longdouble_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_char:
      idl_global->char_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_wchar:
      idl_global->wchar_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_boolean:
      idl_global->boolean_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_octet:
      idl_global->octet_seq_seen_ = true;
      break;

    case AST_PredefinedType::PT_any:
      idl_global->any_seq_seen_ = true;
      break;

    default:
      break;
    }
}

// Protected Front End Scope Management Protocol.
//
// All members of the protocol defined in UTL_Scope simply return NULL
// and don't do a thing. This ensures that runtime errors will discover
// operations which should have been redefined to allow certain kinds of
// AST nodes to appear in a given context.

AST_Decl *
UTL_Scope::fe_add_decl (AST_Decl *t)
{
  // Already defined and cannot be redefined? Or already used?
  AST_Decl *d = this->lookup_for_add (t);

  if (d)
    {
      if (!FE_Utils::can_be_redefined (d, t))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      ScopeAsDecl (this),
                                      d);
          return 0;
        }

      // For convenience, AST_Template_Module_Inst inherits
      // from AST_Field, but has a node type of NT_module.
      // Since we then can't add it using fe_add_module(), a
      // check is needed here to avoid a redefinition error,
      // if the instantiated module is meant to reopen a
      // previous one.

      AST_Decl::NodeType lnt = d->node_type ();
      AST_Decl::NodeType ant = t->node_type ();
      bool need_ref_check =
        (   lnt != AST_Decl::NT_module
         || ant != AST_Decl::NT_module);

      if (need_ref_check && this->referenced (d, t->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      ScopeAsDecl (this),
                                      d);
          return 0;
        }

      if (t->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope (t, d);
          return 0;
        }
    }
  else if (this->inherited_op_attr_clash (t))
    {
      return 0;
    }

  if (this->arg_specific_error (t))
    {
      return 0;
    }

  this->smart_local_add (t);

  // Add it to set of locally referenced symbols, unless it is
  // a home, in which case it will clash when the equivalent
  // interface is created.
  if (t->node_type () != AST_Decl::NT_home)
    {
      this->add_to_referenced (t,
                               false,
                               t->local_name ());
    }

  return t;
}

AST_Field *
UTL_Scope::fe_add_ref_decl (AST_Field *t)
{
  AST_Decl *d = this->fe_add_decl (t);
  if (d)
    {
      AST_Type *ft = t->field_type ();
      UTL_ScopedName *mru = ft->last_referenced_as ();
      if (mru)
        {
          this->add_to_referenced (ft,
                                   false,
                                   mru->first_component ());
        }
    }

  // Catches struct/union/exception which all maintain a queue
  // for fields as distinct from decls and enum values.
  AST_Structure *s = AST_Structure::narrow_from_scope (this);
  if (s)
    {
      s->fields ().enqueue_tail (t);
    }

  return AST_Field::narrow_from_decl (d);
}

AST_Structure *
UTL_Scope::fe_add_full_struct_type (AST_Structure *t)
{
  AST_Decl *predef = this->lookup_for_add (t);

  if (predef)
    {
      if (!FE_Utils::can_be_redefined (predef, t))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      ScopeAsDecl (this),
                                      predef);
          return 0;
        }

      if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      ScopeAsDecl (this),
                                      predef);
          return 0;
        }
    }

  AST_Decl::NodeType nt = ScopeAsDecl (this)->node_type ();

  // Decls inside a struct or union are also referenced by
  // fields, and so must be handled differently.
  if (   nt == AST_Decl::NT_struct
      || nt == AST_Decl::NT_union
      || nt == AST_Decl::NT_except)
    {
      this->add_to_local_types (t);
    }
  else
    {
      this->add_to_scope (t);
    }

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t,
                           false,
                           t->local_name ());
  return t;
}

AST_StructureFwd *
UTL_Scope::fe_add_fwd_struct_type (AST_StructureFwd *t)
{
  // Already defined and cannot be redefined? Or already used?
  AST_Decl *d = this->lookup_for_add (t);
  if (d)
    {
      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add() calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_prev_mods() for modules. If look_in_prev_mods()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (d->node_type () == AST_Decl::NT_struct)
        {
          t->set_full_definition (AST_Structure::narrow_from_decl (d));
        }
      else if (!FE_Utils::can_be_redefined (d, t))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      ScopeAsDecl (this),
                                      d);
          return 0;
        }
      else if (this->referenced (d, t->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      ScopeAsDecl (this),
                                      d);
          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           false,
                           t->local_name ());

  // Must check later that all struct and union forward declarations
  // are defined in the same IDL file.
  AST_record_fwd_decl (t);
  return t;
}

AST_PredefinedType *
UTL_Scope::fe_add_predefined_type (AST_PredefinedType *)
{
  return 0;
}

AST_Module *
UTL_Scope::fe_add_module (AST_Module *)
{
  return 0;
}

AST_Template_Module_Inst *
UTL_Scope::fe_add_template_module_inst (AST_Template_Module_Inst *)
{
  return 0;
}

AST_Template_Module_Ref *
UTL_Scope::fe_add_template_module_ref (AST_Template_Module_Ref *)
{
  return 0;
}

AST_Interface *
UTL_Scope::fe_add_interface (AST_Interface *)
{
  return 0;
}

AST_InterfaceFwd *
UTL_Scope::fe_add_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

AST_ValueBox *
UTL_Scope::fe_add_valuebox (AST_ValueBox *)
{
  return 0;
}

AST_ValueType *
UTL_Scope::fe_add_valuetype (AST_ValueType *)
{
  return 0;
}

AST_ValueTypeFwd *
UTL_Scope::fe_add_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

AST_EventType *
UTL_Scope::fe_add_eventtype (AST_EventType *)
{
  return 0;
}

AST_EventTypeFwd *
UTL_Scope::fe_add_eventtype_fwd (AST_EventTypeFwd *)
{
  return 0;
}

AST_Component *
UTL_Scope::fe_add_component (AST_Component *)
{
  return 0;
}

AST_ComponentFwd *
UTL_Scope::fe_add_component_fwd (AST_ComponentFwd *)
{
  return 0;
}

AST_Home *
UTL_Scope::fe_add_home (AST_Home *)
{
  return 0;
}

AST_Exception *
UTL_Scope::fe_add_exception (AST_Exception *)
{
  return 0;
}

AST_Constant *
UTL_Scope::fe_add_constant (AST_Constant *)
{
  return 0;
}

UTL_StrList *
UTL_Scope::fe_add_context (UTL_StrList *)
{
  return 0;
}

UTL_NameList *
UTL_Scope::fe_add_exceptions (UTL_NameList *)
{
  return 0;
}

AST_Attribute *
UTL_Scope::fe_add_attribute (AST_Attribute *)
{
  return 0;
}

AST_Operation *
UTL_Scope::fe_add_operation (AST_Operation *)
{
  return 0;
}

AST_Argument *
UTL_Scope::fe_add_argument (AST_Argument *)
{
  return 0;
}

AST_Union *
UTL_Scope::fe_add_union (AST_Union *)
{
  return 0;
}

AST_UnionFwd *
UTL_Scope::fe_add_union_fwd (AST_UnionFwd *)
{
  return 0;
}

AST_UnionBranch *
UTL_Scope::fe_add_union_branch (AST_UnionBranch *)
{
  return 0;
}

AST_Structure *
UTL_Scope::fe_add_structure (AST_Structure *)
{
  return 0;
}

AST_StructureFwd *
UTL_Scope::fe_add_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

AST_Field *
UTL_Scope::fe_add_field (AST_Field *)
{
  return 0;
}

AST_Enum *
UTL_Scope::fe_add_enum (AST_Enum *)
{
  return 0;
}

AST_EnumVal *
UTL_Scope::fe_add_enum_val (AST_EnumVal *)
{
  return 0;
}

AST_Typedef *
UTL_Scope::fe_add_typedef (AST_Typedef *)
{
  return 0;
}

AST_Sequence *
UTL_Scope::fe_add_sequence (AST_Sequence *)
{
  return 0;
}

AST_String *
UTL_Scope::fe_add_string (AST_String *)
{
  return 0;
}

AST_Array *
UTL_Scope::fe_add_array (AST_Array *)
{
  return 0;
}

AST_Native *
UTL_Scope::fe_add_native (AST_Native *)
{
  return 0;
}

AST_Factory *
UTL_Scope::fe_add_factory (AST_Factory *)
{
  return 0;
}

AST_Finder *
UTL_Scope::fe_add_finder (AST_Finder *)
{
  return 0;
}

AST_PortType *
UTL_Scope::fe_add_porttype (AST_PortType *)
{
  return 0;
}

AST_Provides *
UTL_Scope::fe_add_provides (AST_Provides *)
{
  return 0;
}

AST_Uses *
UTL_Scope::fe_add_uses (AST_Uses *)
{
  return 0;
}

AST_Publishes *
UTL_Scope::fe_add_publishes (AST_Publishes *)
{
  return 0;
}

AST_Emits *
UTL_Scope::fe_add_emits (AST_Emits *)
{
  return 0;
}

AST_Consumes *
UTL_Scope::fe_add_consumes (AST_Consumes *)
{
  return 0;
}

AST_Extended_Port *
UTL_Scope::fe_add_extended_port (AST_Extended_Port *)
{
  return 0;
}

AST_Mirror_Port *
UTL_Scope::fe_add_mirror_port (AST_Mirror_Port *)
{
  return 0;
}

AST_Connector *
UTL_Scope::fe_add_connector (AST_Connector *)
{
  return 0;
}

// Private lookup mechanism.

// For dealing with lookups of 'Object' and 'TypeCode'.
AST_Decl *
UTL_Scope::lookup_pseudo (Identifier *e)
{
  if (e->escaped ())
    {
      return 0;
    }

  bool *seen = 0;
  char *name_string = e->get_string ();
  UTL_Scope *start_scope = idl_global->corba_module ();

  if (ACE_OS::strcasecmp (name_string, "TypeCode") == 0
      || ACE_OS::strcasecmp (name_string, "TCKind") == 0)
    {
      this->which_pseudo_ = PSEUDO_TYPECODE;
      start_scope = this;
      seen = &idl_global->typecode_seen_;
    }
  else if (ACE_OS::strcasecmp (name_string, "Object") == 0)
    {
      this->which_pseudo_ = PSEUDO_OBJECT;
      seen = &idl_global->base_object_seen_;
    }
  else if (ACE_OS::strcasecmp (name_string, "ValueBase") == 0)
    {
      this->which_pseudo_ = PSEUDO_VALUEBASE;
      seen = &idl_global->valuebase_seen_;
    }
  else if (ACE_OS::strcasecmp (name_string, "AbstractBase") == 0)
    {
      this->which_pseudo_ = PSEUDO_ABSTRACTBASE;
      seen = &idl_global->abstractbase_seen_;
    }
  else
    {
      return 0;
    }

  for (UTL_ScopeActiveIterator i (start_scope, IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();

      if (e->case_compare (d->local_name ()))
        {
          // These have to be located here because we are just looking
          // up a scoped name - skip for imported nodes.
          if (idl_global->in_main_file ())
            {
              *seen = true;
            }

          return d;
        }
    }

  if (this->which_pseudo_ == PSEUDO_TYPECODE)
    {
      AST_Decl *d = this->look_in_prev_mods_local (e);

      if (d != 0)
        {
          // Generation of #includes for Typecode.h
          // checks this bit, so we set it for TCKind as well.
          *seen = true;
          return d;
        }
    }

  return 0;
}

AST_Decl *
UTL_Scope::look_in_prev_mods_local (Identifier *,
                                    bool /* ignore_fwd */)
{
  return 0;
}

AST_Decl *
UTL_Scope::special_lookup (UTL_ScopedName *,
                           bool /* full_def_only */,
                           AST_Decl *&/*final_parent_decl*/)
{
  return 0;
}

// Lookup the node for a primitive (built-in) type.
AST_Decl *
UTL_Scope::lookup_primitive_type (AST_Expression::ExprType et)
{
  UTL_Scope *search = idl_global->corba_module ();

  AST_PredefinedType::PredefinedType pdt =
    FE_Utils::ExprTypeToPredefinedType (et);

  /// This return value means there was no PredefinedType match
  /// for the ExprType.
  if (pdt == AST_PredefinedType::PT_pseudo)
    {
      return 0;
    }

  /// The only 'predefined type' not in the CORBA module.
  if (pdt == AST_PredefinedType::PT_void)
    {
      search = idl_global->root ();
    }

  for (UTL_ScopeActiveIterator i (search, IK_decls);
       !i.is_done();
       i.next ())
    {
      AST_Decl *as_decl = i.item ();

      if (as_decl->node_type () == AST_Decl::NT_pre_defined)
        {
          AST_PredefinedType *t =
            AST_PredefinedType::narrow_from_decl (as_decl);

          if (t->pt () == pdt)
            {
              if (idl_global->in_main_file ())
                {
                  switch (pdt)
                    {
                    case AST_PredefinedType::PT_any:
                      idl_global->any_seen_ = true;
                      break;
                    case AST_PredefinedType::PT_object:
                      idl_global->base_object_seen_ = true;
                      break;
                    default:
                      break;
                    }
                }

              return t;
            }
        }
    }

  return 0;
}

// Look through inherited list. Overridden in AST_Interface.
AST_Decl *
UTL_Scope::look_in_inherited (UTL_ScopedName *,
                              bool )
{
  return 0;
}

// Look through supported interface list. Overridden where necessary.
AST_Decl *
UTL_Scope::look_in_supported (UTL_ScopedName *,
                              bool)
{
  return 0;
}

AST_Decl *
UTL_Scope::lookup_by_name_local (Identifier *e,
                                 bool full_def_only)
{
  // Will catch Object, TypeCode, TCKind, ValueBase and
  // AbstractBase. A non-zero result of this lookup determines the
  // generation of some #includes and, whether successful or not,
  // incurs no extra overhead.
  AST_Decl *d = this->lookup_pseudo (e);
  if (d)
    {
      return d;
    }

  if (this->idl_keyword_clash (e))
    {
      return 0;
    }

  bool in_corba = !ACE_OS::strcmp (e->get_string (), "CORBA");

  // We search only the decls here, the local types are done
  // below as a last resort.
  for (UTL_ScopeActiveIterator i (this, IK_decls);
       !i.is_done ();
       i.next ())
    {
      d = i.item ()->adjust_found (true, full_def_only);
      if (d)
        {
          Identifier *item_name = d->local_name ();
          if (item_name
          // Right now we populate the global scope with all the CORBA basic
          // types, so something like 'ULong' in an IDL file will find a
          // match, unless we skip over these items. This is a workaround until
          // there's time to fix the code generation for CORBA basic types.
              && (in_corba || ACE_OS::strcmp (
                  d->name ()->head ()->get_string (), "CORBA"))
              && e->case_compare (item_name))
            {
              return d; // We have found the one and only one we are looking for.
            }
        }
    }

  // Ok the name wasn't found in the current scope, if this
  // scope is a module, we can check it's previous openings!
  AST_Module *m = AST_Module::narrow_from_scope (this);
  if (m)
    {
      d = m->look_in_prev_mods_local (e);
      if (d && (!full_def_only || d->is_defined ()))
        {
          return d;
        }
    }
  else
    {
      // Or if this scope is an interface, we can check if
      // it was inherited!
      AST_Interface *i = AST_Interface::narrow_from_scope (this);
      if (i)
        {
          d = i->look_in_inherited_local (e);
          if (d)
            {
              return d;
            }
        }
    }

  // There are some cases where the iteration over IK_decls
  // above will fail because what we're searching for has been
  // added only to the local types. It's less complicated to
  // do the iteration below only as a last resort.
  for (UTL_ScopeActiveIterator i (this, IK_localtypes);
       !i.is_done ();
       i.next ())
    {
      d = i.item ();
      if (e->case_compare (d->local_name ()))
        {
          return d;
        }
    }

  return 0;
}

AST_Decl *
UTL_Scope::lookup_by_name (UTL_ScopedName *e,
                           bool full_def_only,
                           bool for_add)
{
  // Empty name? Exit immediately.
  if (!e)
    {
      return 0;
    }

  UTL_Scope *work = this;

  // If name starts with "::" or "" start lookup in global scope,
  // if we're not there already, short_circuiting the
  // scope-expanding iteration below.
  Identifier *name = e->head ();
  const bool global_scope_name = work->is_global_name (name);
  if (global_scope_name)
    {
      // Remove the preceeding "::" or "" from the scopename
      e = static_cast<UTL_ScopedName *> (e->tail ());
      name = e->head ();

      // Move directly to the root scope
      work = idl_global->root ();
    }

  AST_Decl *first_found_final_parent_decl= 0;
  const bool searching_module_path= (e->length () != 1);
  AST_Decl *d = searching_module_path ?
    work->lookup_by_name_r (e, full_def_only, first_found_final_parent_decl) :
    work->lookup_by_name_r (e, full_def_only);
  if (d == 0)
    {
      // If all else fails, look though each outer scope.
      for (UTL_Scope *outer = ScopeAsDecl (work)->defined_in ();
           outer;
           outer = ScopeAsDecl (outer)->defined_in ())
        {
          AST_Decl *next_found_final_parent_decl= 0;
          d = outer->lookup_by_name_r (e, full_def_only, next_found_final_parent_decl);
          if (d != 0)
            {
              work = outer;
              if (first_found_final_parent_decl)
                {
                  // Hidden scopes can't be used indirectly, therefore we didn't actually
                  // find this one because the "first_found_final_parent_decl" was found and
                  // this one just found is hidden by it.
                  idl_global->err ()->scope_masking_error (d, first_found_final_parent_decl);
                  d = 0; // Ignore this one; continue searching to report other ambiguous matches.
                }
              else
                {
                  break; // Ok found it, stop searching.
                }
            }
          else if (searching_module_path && !first_found_final_parent_decl)
            {
              first_found_final_parent_decl = next_found_final_parent_decl;
            }
        }
    }

  if (for_add)
    {
      /// Doesn't add if d == 0.
      work->add_to_referenced (d, false, name);
    }

  return d;
}

AST_Decl *
UTL_Scope::lookup_by_name_r (UTL_ScopedName *e,
                             bool full_def_only)
{
  AST_Decl *ignored= 0;
  return UTL_Scope::lookup_by_name_r (e, full_def_only, ignored);
}

AST_Decl *
UTL_Scope::lookup_by_name_r (UTL_ScopedName *e,
                             bool full_def_only,
                             AST_Decl *&final_parent_decl)
{
  bool work_another_level;
  UTL_Scope *work = this;
  final_parent_decl= (e->length () == 1) ? ScopeAsDecl (work) : 0;

  do
    {
      // Will catch Object, TypeCode, TCKind, ValueBase and
      // AbstractBase. A non-zero result of this lookup determines the
      // generation of some #includes and, whether successful or not,
      // incurs no extra overhead.
      AST_Decl *d = work->lookup_pseudo (e->head ());
      if (d)
        {
          return d;
        }

      if (work->idl_keyword_clash (e->head ()))
        {
          return 0;
        }

      // Before proceeding to normal lookup, check if the name
      // matches a template module parameter. If so, the return
      // value is created on the heap and is owned by the caller
      // of this lookup.
      if (final_parent_decl)
        {
          // Since we are inside the scope of a template module, any
          // single-segment scoped name that matches a template
          // parameter name has to be a reference to that parameter,
          // so we return the created placeholder. If there's no
          // match, 0 is returned, and we proceed with the regular
          // lookup.
          AST_Param_Holder *param_holder =
            UTL_Scope::match_param (e);

          if (param_holder)
            {
              return param_holder;
            }
        }

      work_another_level = false; // Until we find something.
      bool in_corba =
        (ACE_OS::strcmp (e->head ()->get_string (), "CORBA") == 0);

      for (UTL_ScopeActiveIterator i (work, IK_decls);
           !i.is_done ();
           i.next ())
        {
          d = i.item ()->adjust_found (true, full_def_only);
          if (d
          // Right now we populate the global scope with all the CORBA basic
          // types, so something like 'ULong' in an IDL file will find a
          // match, unless we skip over these items. This is a workaround until
          // there's time to fix the code generation for CORBA basic types.
              && (in_corba ||
                  ACE_OS::strcmp (d->name ()->head ()->get_string (), "CORBA"))
              && d->local_name ()->case_compare (e->head ()))
            {
              // Ok we found a match, is there any more to find?
              if (final_parent_decl)
                {
                  return d; // Last scope name matched
                }

              UTL_Scope *next = DeclAsScope (d); // The next scope to search

              if (next)
                {
                  work = next;
                  work_another_level = true;
                  e = static_cast<UTL_ScopedName *> (e->tail ());
                  final_parent_decl= (e->length () == 1) ? d : 0;
                  break;
                }

              // Template weirdness, actual one we want should be
              // found next in this search, keep going.
            }
        }
    } while (work_another_level);

  // A rare enough case that it's worth it to separate it and
  // do it as a last resort. Catches anonymnous types, enums
  // and members with their types defined all in one statement.
  if (final_parent_decl)
    {
      for (UTL_ScopeActiveIterator i (work, IK_localtypes);
           !i.is_done ();
           i.next ())
        {
          AST_Decl *d = i.item ();

          if (d->local_name ()->case_compare (e->head ()))
            {
              return d;
            }
        }
    }

  // Last resort, check other module openings of working scope.
  return work->special_lookup (e, full_def_only, final_parent_decl);
}

// Add a node to set of nodes referenced in this scope.
void
UTL_Scope::add_to_referenced (AST_Decl *e,
                              bool recursive,
                              Identifier *id,
                              AST_Decl *ex)
{
  // Empty name? Exit immediately.
  if (!e)
    {
      return;
    }

  // Special case for forward declared interfaces in the
  // scope in which they're defined. Cannot add before full
  // definition is seen.
  AST_Decl::NodeType nt = e->node_type ();
  if (   nt == AST_Decl::NT_interface
      || nt == AST_Decl::NT_component)
    {
      AST_Interface *itf = AST_Interface::narrow_from_decl (e);
      if (itf
          && itf->defined_in () == this
          && !itf->is_defined ())
        {
          return;
        }
    }

  // Only insert if it is not there already.
  if (this->referenced (e, id))
    {
      return;
    }

  // Sets the appropriate *_seen_ flag for future use.
  this->check_for_predef_seq (e);

  // Make sure there's space for one more decl.
  if (this->pd_referenced_allocated == this->pd_referenced_used)
    {
      long oreferenced_allocated = this->pd_referenced_allocated;
      this->pd_referenced_allocated += INCREMENT;

      AST_Decl **tmp = 0;
      ACE_NEW (tmp, AST_Decl *[this->pd_referenced_allocated]);

      for (long i = 0; i < oreferenced_allocated; ++i)
        {
          tmp [i] = this->pd_referenced [i];
        }

      delete [] this->pd_referenced;
      this->pd_referenced = tmp;
    }

  // Insert new reference.
  if (!ex)
    {
      this->pd_referenced [this->pd_referenced_used++] = e;
    }
  else if (this->referenced (ex))
    {
      long i;
      for (i = this->pd_referenced_used;
           0 < i && this->pd_referenced [i - 1] != ex;
           --i)
        {
          this->pd_referenced [i] = this->pd_referenced [i - 1];
        }
      this->pd_referenced [i] = e;
      ++this->pd_referenced_used;
    }

  // Now, if recursive is specified and "this" is not a common ancestor
  // of the referencing scope and the scope of definition of "e" then
  // add "e" to the set of referenced nodes in the parent of "this".
  if (recursive && !(e->has_ancestor (ScopeAsDecl (this))))
    {
      UTL_Scope *s = e->defined_in ();
      if (s)
        {
          s->add_to_referenced (e, recursive, id);
        }
    }

  // Add the identifier arg, if non-null, to the identifier list.
  if (id)
    {
      this->add_to_name_referenced (id);
    }
}

void
UTL_Scope::add_to_name_referenced (Identifier *id)
{
  // Make sure we have enough space.
  if (this->pd_name_referenced_allocated == this->pd_name_referenced_used)
    {
      long name_referenced_allocated = this->pd_name_referenced_allocated;
      this->pd_name_referenced_allocated += INCREMENT;

      Identifier **name_tmp = 0;
      ACE_NEW (name_tmp, Identifier *[this->pd_name_referenced_allocated]);

      for (long i = 0; i < name_referenced_allocated; ++i)
        {
          name_tmp[i] = this->pd_name_referenced[i];
        }

      delete [] this->pd_name_referenced;
      this->pd_name_referenced = name_tmp;
    }

  // Insert new identifier.
  this->pd_name_referenced[this->pd_name_referenced_used++] = id->copy ();
}

void
UTL_Scope::replace_referenced (AST_Decl *old_decl,
                               AST_Decl *new_decl)
{
  for (int i = 0; i < this->pd_referenced_used; ++i)
    {
      if (this->pd_referenced[i] == old_decl)
        {
          this->pd_referenced[i] = new_decl;
          break;
        }
    }
}

void
UTL_Scope::replace_scope (AST_Decl *old_decl,
                          AST_Decl *new_decl)
{
  for (int i = 0; i < pd_decls_used; ++i)
    {
      if (this->pd_decls[i] == old_decl)
        {
          this->pd_decls[i] = new_decl;
          break;
        }
    }
}

// Add a node to set of nodes declared in this scope.
void
UTL_Scope::add_to_scope (AST_Decl *e,
                         AST_Decl *ex)
{
  if (!e)
    {
      return;
    }

  Identifier *decl_name = e->local_name ();
  char *decl_string = decl_name->get_string ();
  AST_Decl::NodeType new_nt = e->node_type ();

  // First, make sure there's no clash between e, that was
  // just declared, and some other identifier referenced
  // in this scope.
  AST_Decl **tmp = this->pd_decls;
  for (long i = this->pd_decls_used;
       i--; // NOTE i-- test
       ++tmp)
    {
      // A local declaration doesn't use a scoped name.
      Identifier *ref_name = (*tmp)->local_name ();
      char *ref_string = ref_name->get_string ();

      // If the names compare exactly, it's a redefini8tion
      // error, unless they're both modules (which can be
      // reopened) or we have a belated definition of a
      // forward-declared interface.
      AST_Decl::NodeType scope_elem_nt = (*tmp)->node_type ();

      if (this->redef_clash (new_nt, scope_elem_nt)
          && decl_name->compare (ref_name))
        {
          idl_global->err ()->redef_error (decl_string,
                                           ref_string);

          // if we try to continue from here, we risk a crash.
          throw Bailout ();
        }
      // If the spellings differ only by case, it's also an error,
      // unless one, but not both of the identifiers were escaped.
      else if (!(decl_name->escaped () ^ ref_name->escaped ())
               && decl_name->case_compare_quiet (ref_name))
        {
          if (idl_global->case_diff_error ())
            {
              idl_global->err ()->name_case_error (decl_string,
                                                   ref_string);

              // if we try to continue from here, we risk a crash.
              throw Bailout ();
            }
          else
            {
              idl_global->err ()->name_case_warning (decl_string,
                                                     ref_string);
            }
        }
    }

  // The name of any scope except the unnamed scope formed by an operation
  // may not be redefined immediately within (and the root scope has no name).
  // As well as OBV factory construct.
  AST_Decl *d = ScopeAsDecl (this);
  AST_Decl::NodeType nt = d->node_type ();
  if (   nt != AST_Decl::NT_root
      && nt != AST_Decl::NT_op
      && nt != AST_Decl::NT_factory)
    {
      Identifier *parent_name = d->local_name ();
      if (decl_name->compare (parent_name))
        {
          idl_global->err ()->redef_error (
                                  decl_name->get_string (),
                                  parent_name->get_string ()
                                );

          // if we try to continue from here, we risk a crash.
          throw Bailout ();
        }
      else if (decl_name->case_compare_quiet (parent_name))
        {
          if (idl_global->case_diff_error ())
            {
              idl_global->err ()->name_case_error (
                                      decl_name->get_string (),
                                      parent_name->get_string ()
                                    );

              // if we try to continue from here, we risk a crash.
              throw Bailout ();
           }
          else
            {
              idl_global->err ()->name_case_warning (
                                      decl_name->get_string (),
                                      parent_name->get_string ()
                                    );
            }
        }
    }

  // Now make sure there's space for one more.
  if (this->pd_decls_allocated == this->pd_decls_used)
    {
      long odecls_allocated = this->pd_decls_allocated;
      this->pd_decls_allocated += INCREMENT;

      ACE_NEW (tmp, AST_Decl *[pd_decls_allocated]);
      for (long i = 0; i < odecls_allocated; ++i)
        {
          tmp [i] = this->pd_decls [i];
        }

      delete [] this->pd_decls;
      this->pd_decls = tmp;
    }

  // Insert new decl.
  if (!ex)
    {
      this->pd_decls [this->pd_decls_used++] = e;
    }
  else
    {
      long i;
      for (i = this->pd_decls_used;
           0 < i && this->pd_decls [i - 1] != ex;
           --i)
        {
          this->pd_decls [i] = this->pd_decls [i - 1];
        }
      this->pd_decls [i] = e;
      ++this->pd_decls_used;
    }
}

// Add a node to set of nodes representing manifest
// types defined in this scope.
void
UTL_Scope::add_to_local_types (AST_Decl *e)
{
  if (!e)
    {
      return;
    }

  // Make sure there's space for one more.
  if (this->pd_locals_allocated == this->pd_locals_used)
    {
      long olocals_allocated = this->pd_locals_allocated;
      this->pd_locals_allocated += INCREMENT;

      AST_Decl **tmp = 0;
      ACE_NEW (tmp, AST_Decl *[this->pd_locals_allocated]);
      for (long i = 0; i < olocals_allocated; ++i)
        {
          tmp[i] = this->pd_local_types[i];
        }

      delete [] this->pd_local_types;
      this->pd_local_types = tmp;
    }

  // Insert new decl.
  this->pd_local_types[this->pd_locals_used++] = e;
}

// Has this node been referenced here before?
bool
UTL_Scope::referenced (AST_Decl *e,
                       Identifier *id)
{
  Identifier *test = e->local_name ();
  AST_Decl::NodeType nt = e->node_type ();

  AST_Decl **tmp = this->pd_referenced;
  for (long i = this->pd_referenced_used;
       i--; // NOTE test with i--
       ++tmp)
    {
      // Same node?
      if (*tmp == e)
        {
          return true;
        }

      // Are we definging a forward declared struct, union, or interface,
      // or reopening a module?
      if (!this->redef_clash (nt, (*tmp)->node_type ())
          && (*tmp)->local_name ()->compare (test))
        {
          return false;
        }
    }

  // pd_referenced is a list of decls, and so there's no
  // way of telling how much of its scoped name was used
  // when it was referenced in this scope. pd_name_referenced
  // is a list of Identifiers that store the identifier (or
  // the first segment of a scoped name) used in the reference,
  // so we can catch these name reolution clashes.
  if (id)
    {
      Identifier **name_tmp = this->pd_name_referenced;
      for (long j = this->pd_name_referenced_used;
           j--; // NOTE test with j--
           ++name_tmp)
        {
          // If we are a module, there is no clash, if we
          // are an interface, this is not the right place to
          // catch a clash, and if it wasn't defined in this
          // scope, then it's a type name for something else
          // that was, and it can appear any number of times
          // in this scope without a clash.
          AST_Decl::NodeType nt = e->node_type ();
          if (   nt != AST_Decl::NT_module
              && nt != AST_Decl::NT_param_holder
              && e->defined_in () == this
              && id->compare (*name_tmp))
            {
              idl_global->err ()->redef_error (id->get_string (),
                                               (*name_tmp)->get_string ());
              return true;
            }
          // No clash if one or the other of the identifiers was escaped.
          else if (!(id->escaped () ^ (*name_tmp)->escaped ())
                   && id->case_compare_quiet (*name_tmp))
            {
              if (idl_global->case_diff_error ())
                {
                  idl_global->err ()->name_case_error (
                                          id->get_string (),
                                          (*name_tmp)->get_string ()
                                        );
                }
              else
                {
                  idl_global->err ()->name_case_warning (
                                          id->get_string (),
                                          (*name_tmp)->get_string ()
                                        );
                }

              return true;
            }
        }
    }

  // Not found
  return false;
}

bool
UTL_Scope::has_prefix (void)
{
  return this->has_prefix_;
}

void
UTL_Scope::has_prefix (bool val)
{
  this->has_prefix_ = val;
}

// Redefinition of inherited virtual operations.

// AST Dumping.
void
UTL_Scope::dump (ACE_OSTREAM_TYPE &o)
{
  if (!idl_global->indent ())
    {
      UTL_Indenter *idnt = 0;
      ACE_NEW (idnt, UTL_Indenter);
      idl_global->set_indent (idnt);
    }
  idl_global->indent ()->increase ();

  if (0 < this->pd_locals_used)
    {
      o << ACE_TEXT ("\n/* Locally defined types: */\n");

      for (UTL_ScopeActiveIterator i (this, IK_localtypes);
           !i.is_done ();
           i.next ())
        {
          AST_Decl *d = i.item ();

          if (!d->imported ())
            {
              idl_global->indent ()->skip_to (o);
              d->dump (o);
              o << ACE_TEXT ("\n");
            }
        }
    }

  if (0 < this->pd_decls_used)
    {
      o << ACE_TEXT ("\n/* Declarations: */\n");

      for (UTL_ScopeActiveIterator j (this, IK_decls);
           !j.is_done ();
           j.next ())
        {
          AST_Decl *d = j.item ();

          if (!d->imported ())
            {
              idl_global->indent ()->skip_to (o);
              d->dump (o);
              o << ACE_TEXT (";\n");
            }
        }
    }

  idl_global->indent ()->decrease ();
}

int
UTL_Scope::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_scope (this);
}

// How many entries are defined?
unsigned long
UTL_Scope::nmembers (void)
{
  return this->pd_decls_used;
}

AST_Param_Holder *
UTL_Scope::match_param (UTL_ScopedName *e)
{
  // If this call returns a zero value, we are not in the scope
  // of a template module.
  FE_Utils::T_PARAMLIST_INFO const *params =
    idl_global->current_params ();

  if (params == 0)
    {
      return 0;
    }

  const char *name = e->first_component ()->get_string ();
  FE_Utils::T_Param_Info *param = 0;
  unsigned long index = 0;

  UTL_StrList *alias_params =
    const_cast<UTL_StrList *> (idl_global->for_new_holder ());

  if (alias_params == 0)
    {
      alias_params =
        const_cast<UTL_StrList *> (idl_global->alias_params ());
    }

  UTL_String *alias_param = 0;

  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*params);
       i.next (param);
       i.advance (), ++index)
    {
      if (param->name_ == name)
        {
          /// If we are parsing this template module as a
          /// reference, the param holder we create must have
          /// the name of the corresponding aliased param.
          if (alias_params != 0)
            {
              unsigned long slot = 0;

              for (UTL_StrlistActiveIterator iter (alias_params);
                   !iter.is_done ();
                   iter.next (), ++slot)
                {
                  if (slot == index)
                    {
                      alias_param = iter.item ();
                      break;
                    }
                }

              Identifier id (alias_param->get_string ());
              UTL_ScopedName sn (&id, 0);

              return
                idl_global->gen ()->create_param_holder (
                  &sn,
                  param);
            }
          else
            {
              return
                idl_global->gen ()->create_param_holder (
                  e,
                  param);
            }
        }
    }

  return 0;
}

bool
UTL_Scope::inherited_op_attr_clash (AST_Decl *t)
{
  AST_Interface *i = AST_Interface::narrow_from_scope (this);
  if (!i)
    {
      return false;
    }

  AST_Decl *d = i->look_in_inherited (t->name (), false);
  if (d)
    {
      AST_Decl::NodeType nt = d->node_type ();
      if (   nt == AST_Decl::NT_attr
          || nt == AST_Decl::NT_op)
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      i,
                                      d);
          return true;
        }
    }

  return false;
}

bool
UTL_Scope::arg_specific_error (AST_Decl *t)
{
  AST_Operation *op = AST_Operation::narrow_from_scope (this);
  if (!op)
    {
      return false;
    }

  AST_Argument *arg = AST_Argument::narrow_from_decl (t);
  AST_Argument::Direction d = arg->direction ();

  // Cannot add OUT or INOUT argument to oneway operation.
  if ((   d == AST_Argument::dir_OUT
       || d == AST_Argument::dir_INOUT)
      && op->flags () == AST_Operation::OP_oneway)
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_ONEWAY_CONFLICT,
                                  t,
                                  op);
      return true;
    }

  // This error is not caught in y.tab.cpp so we check for it here.
  AST_Type *arg_type = arg->field_type ();
  if (   arg_type->node_type () == AST_Decl::NT_array
      && arg_type->anonymous ())
    {
      idl_global->err ()->syntax_error (idl_global->parse_state ());
      return true;
    }

  return false;
}

void
UTL_Scope::smart_local_add (AST_Decl *t)
{
  // Catches struct, union * exception
  AST_Structure *s = AST_Structure::narrow_from_scope (this);

  // Catches AST_Field and AST_UnionBranch.
  AST_Field *f = AST_Field::narrow_from_decl (t);

  // Decls inside a struct/union/exception are also referenced by
  // fields, and so must be handled differently.
  if (s && !f)
    {
      this->add_to_local_types (t);
    }
  else
    {
      this->add_to_scope (t);
    }

  // If we have an enum discriminator, add the label names to
  // the name_referenced list before we add the union branch,
  // so a branch name clash with a label name will be caught.
  AST_Union *u = AST_Union::narrow_from_scope (this);
  AST_UnionBranch *ub = AST_UnionBranch::narrow_from_decl (t);
  if (u && ub)
    {
      if (u->udisc_type () == AST_Expression::EV_enum)
        {
          ub->add_labels (u);
        }
      else
        {
          ub->coerce_labels (u);
        }
    }
}

// Determines if a name is global.
bool
UTL_Scope::is_global_name (Identifier *i)
{
  if (!i)
    {
      return false;
    }

  ACE_CString cmp (i->get_string (), 0, false);
  return (cmp == "" || cmp == "::");
}

AST_Decl *
UTL_Scope::iter_lookup_by_name_local (AST_Decl *d,
                                      UTL_ScopedName *e,
                                      bool full_def_only)
{
  // Remove all the layers of typedefs.
  while (d && d->node_type () == AST_Decl::NT_typedef)
    {
      AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
      if (!td)
        {
          return 0;
        }

      d = td->base_type ();
    }

  if (!d)
    {
      return 0;
    }

  // Try to convert the AST_Decl to a UTL_Scope.
  UTL_Scope *sc = DeclAsScope (d);
  if (!sc)
    {
      return 0;
    }

  AST_Decl *result = 0;
  if (0 < sc->nmembers ())
    {
      // Look up the first component of the scoped name.
      result = sc->lookup_by_name_local (e->head (),
                                         full_def_only);
    }
  else
    {
      AST_Interface *i = AST_Interface::narrow_from_decl (d);
      result = i ? i->look_in_inherited_local (e->head ())
                 : sc->look_in_prev_mods_local (e->head (), true);
    }

  UTL_ScopedName *sn = (UTL_ScopedName *) e->tail ();
  if (!result)
    {
      if (!sn)
        {
          result = UTL_Scope::match_param (e);
        }
    }
  else if (sn)
    {
      // Look up the next component of the scoped name.
      result = iter_lookup_by_name_local (result,
                                          sn,
                                          full_def_only);
    }

  return result;
}

IMPL_NARROW_FROM_SCOPE(UTL_Scope)

// UTL_SCOPE_ACTIVE_ITERATOR

// Constructor.
UTL_ScopeActiveIterator::UTL_ScopeActiveIterator (
    UTL_Scope *s,
    UTL_Scope::ScopeIterationKind i
  )
  : iter_source (s),
    ik (i),
    stage (i == UTL_Scope::IK_both ? UTL_Scope::IK_localtypes : i),
    il (0)
{
}

// Public operations.

// Advance to next item.
void
UTL_ScopeActiveIterator::next (void)
{
  this->il++;
}

// Get current item.
AST_Decl *
UTL_ScopeActiveIterator::item (void)
{
  if (this->is_done ())
    {
      return 0;
    }

  if (stage == UTL_Scope::IK_decls)
    {
      return this->iter_source->pd_decls[il];
    }

  if (stage == UTL_Scope::IK_localtypes)
    {
      return this->iter_source->pd_local_types[il];
    }

  return 0;
}

// Is this iteration done?
bool
UTL_ScopeActiveIterator::is_done (void)
{
  long limit =
    (stage == UTL_Scope::IK_decls)
      ? iter_source->pd_decls_used
      : iter_source->pd_locals_used;

  for (;;)
    {
      // Last element?
      if (this->il < limit)
        {
          return false;
        }

      // Only want decls?
      if (this->stage == UTL_Scope::IK_decls)
        {
          return true;
        }

      // Already done local types?
      if (this->ik == UTL_Scope::IK_localtypes)
        {
          return true;
        }

      // Switch to next stage.
      this->stage = UTL_Scope::IK_decls;
      this->il = 0;
      limit = this->iter_source->pd_decls_used;
    }
}

// What kind of iterator is this?
UTL_Scope::ScopeIterationKind
UTL_ScopeActiveIterator::iteration_kind (void)
{
  return this->ik;
}

// And where are we in the iteration?
UTL_Scope::ScopeIterationKind
UTL_ScopeActiveIterator::iteration_stage (void)
{
  return this->stage;
}
