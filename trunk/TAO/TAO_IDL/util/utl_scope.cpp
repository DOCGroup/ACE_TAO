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
}

// Protected operations.

// Special version of lookup which only looks at the local name instead of
// the fully scoped name, when doing lookups. This version is intended to
// be used only by the frontend add_xxx functions.
AST_Decl *
UTL_Scope::lookup_for_add (AST_Decl *d)
{
  if (d == 0)
    {
      return 0;
    }

  Identifier *id = d->local_name ();

  if (this->idl_keyword_clash (id) != 0)
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

  char *tmp = e->get_string ();

  UTL_String utl_tmp (tmp);

  ACE_CString ext_id (utl_tmp.get_canonical_rep (),
                      0,
                      false);

  int status = idl_global->idl_keywords ().find (ext_id);

  utl_tmp.destroy ();

  if (status == 0)
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
      return (scope_elem_nt != AST_Decl::NT_interface_fwd
              && scope_elem_nt != AST_Decl::NT_interface);
    case AST_Decl::NT_component_fwd:
      return (scope_elem_nt != AST_Decl::NT_component_fwd
              && scope_elem_nt != AST_Decl::NT_component);
    case AST_Decl::NT_valuetype:
      return scope_elem_nt != AST_Decl::NT_valuetype_fwd;
    case AST_Decl::NT_eventtype:
      return scope_elem_nt != AST_Decl::NT_eventtype_fwd;
    case AST_Decl::NT_valuetype_fwd:
      return (scope_elem_nt != AST_Decl::NT_valuetype_fwd
              && scope_elem_nt != AST_Decl::NT_valuetype);
    case AST_Decl::NT_eventtype_fwd:
      return (scope_elem_nt != AST_Decl::NT_eventtype_fwd
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
  if (ACE_OS::strcmp (p->local_name ()->get_string (), "CORBA") != 0)
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
  else if (nt == AST_Decl::NT_wstring)
    {
      idl_global->wstring_seq_seen_ = true;
      return;
    }

  // Now check for predefined base type.
  AST_PredefinedType *pdt = AST_PredefinedType::narrow_from_decl (bt);
  if (pdt == 0)
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

// Public operations.

// Protected Front End Scope Management Protocol.
//
// All members of the protocol defined in UTL_Scope simply return NULL
// and don't do a thing. This ensures that runtime errors will discover
// operations which should have been redefined to allow certain kinds of
// AST nodes to appear in a given context.

AST_Decl *
UTL_Scope::fe_add_decl (AST_Decl *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t)) != 0)
    {
      if (!can_be_redefined (d))
        {
          AST_Decl::NodeType tnt = t->node_type ();
          AST_Decl::NodeType dnt = d->node_type ();
          
          /// Factories are not inherited, so they can be
          /// redefined, but not in the same scope (home or
          /// valuetype).
          bool inherited_factory =
            (tnt == AST_Decl::NT_factory
             && dnt == AST_Decl::NT_factory
             && t->defined_in () != d->defined_in ());
          
          if (!inherited_factory)
            {  
              idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                          t,
                                          ScopeAsDecl (this),
                                          d);
              return 0;
            }
        }
        
      /// For convenience, AST_Template_Module_Inst inherits
      /// from AST_Field, but has a node type of NT_module.
      /// Since we then can't add it using fe_add_module(), a
      /// check is needed here to avoid a redefinition error,
      /// if the instantiated module is meant to reopen a
      /// previous one.
      
      AST_Decl::NodeType lnt = d->node_type ();
      AST_Decl::NodeType ant = t->node_type ();
      
      bool need_ref_check =
        (lnt != AST_Decl::NT_module
         || ant != AST_Decl::NT_module);

      if (need_ref_check)
        {
          if (this->referenced (d, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          ScopeAsDecl (this),
                                          d);
              return 0;
            }
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

  if (d != 0)
    {
      AST_Type *ft = t->field_type ();
      UTL_ScopedName *mru = ft->last_referenced_as ();

      if (mru != 0)
        {
          this->add_to_referenced (ft,
                                   false,
                                   mru->first_component ());
        }
    }
    
  /// Catches struct/union/exception which all maintain a queue
  /// for fields as distinct from decls and enum values.  
  AST_Structure *s = AST_Structure::narrow_from_scope (this);
  
  if (s != 0)
    {
      s->fields ().enqueue_tail (t);
    }

  return AST_Field::narrow_from_decl (d);
}

AST_Structure *
UTL_Scope::fe_add_full_struct_type (AST_Structure *t)
{
  AST_Decl *predef = 0;

  if ((predef = this->lookup_for_add (t)) != 0)
    {
      if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      ScopeAsDecl (this),
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ())
               && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      ScopeAsDecl (this),
                                      predef);

          return 0;
        }
    }

  AST_Decl::NodeType nt = ScopeAsDecl (this)->node_type ();
  
  /// Decls inside a struct or union are also referenced by
  /// fields, and so must be handled differently.
  if (nt == AST_Decl::NT_struct
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
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add() calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_prev_mods() for modules. If look_in_prev_mods()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_struct)
        {
          AST_Structure *s = AST_Structure::narrow_from_decl (d);
          t->set_full_definition (s);
        }
      else
        {
          if (!can_be_redefined (d))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                          t,
                                          ScopeAsDecl (this),
                                          d);
              return 0;
            }

          if (this->referenced (d, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          ScopeAsDecl (this),
                                          d);
              return 0;
            }
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

  Identifier *item_name = 0;
  AST_Decl *d = 0;
  char *name_string = e->get_string ();
  
  if (ACE_OS::strcasecmp (name_string, "Object") == 0)
    {
      this->which_pseudo_ = PSEUDO_OBJECT;
    }
  else if (ACE_OS::strcasecmp (name_string, "ValueBase") == 0)
    {
      this->which_pseudo_ = PSEUDO_VALUEBASE;
    }
  else if (ACE_OS::strcasecmp (name_string, "AbstractBase") == 0)
    {
      this->which_pseudo_ = PSEUDO_ABSTRACTBASE;
    }
  else if (ACE_OS::strcasecmp (name_string, "TypeCode") == 0
           || ACE_OS::strcasecmp (name_string, "TCKind") == 0)
    {
      this->which_pseudo_ = PSEUDO_TYPECODE;
    }
  else
    {
      return 0;
    }
    
  UTL_Scope *start_scope = 0;
  
  switch (this->which_pseudo_)
    {
      case PSEUDO_TYPECODE:
        start_scope = this;
        break;
      case PSEUDO_OBJECT:
      case PSEUDO_VALUEBASE:
      case PSEUDO_ABSTRACTBASE:
        start_scope = idl_global->root ();
        break;
      default:
        break;
    }

  for (UTL_ScopeActiveIterator i (start_scope, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      d = i.item ();
      item_name = d->local_name ();

      if (e->case_compare (item_name))
        {
          // These have to be located here because we are just looking
          // up a scoped name - skip for imported nodes.
          if (idl_global->in_main_file ())
            {
              switch (this->which_pseudo_)
                {
                  case PSEUDO_TYPECODE:
                    idl_global->typecode_seen_ = true;
                    break;
                  case PSEUDO_OBJECT:
                    idl_global->base_object_seen_ = true;
                    break;
                  case PSEUDO_VALUEBASE:
                    idl_global->valuebase_seen_ = true;
                    break;
                  case PSEUDO_ABSTRACTBASE:
                    idl_global->abstractbase_seen_ = true;
                    break;
                  default:
                    break;
                }
            }

          return d;
        }
    }

  if (this->which_pseudo_ == PSEUDO_TYPECODE)
    {
      d = this->look_in_prev_mods (e);

      if (d != 0)
        {
          // Generation of #includes for Typecode.h
          // checks this bit, so we set it for TCKind as well.
          idl_global->typecode_seen_ = true;
          return d;
        }
    }

  return 0;
}

AST_Decl *
UTL_Scope::look_in_prev_mods (Identifier *, bool /* ignore_fwd */)
{
  return 0;
}

// Lookup the node for a primitive (built in) type.
AST_Decl *
UTL_Scope::lookup_primitive_type (AST_Expression::ExprType et)
{
  AST_PredefinedType::PredefinedType pdt;

  AST_Decl *as_decl = ScopeAsDecl (this);

  if (as_decl == 0)
    {
      return 0;
    }

  UTL_Scope *ancestor = as_decl->defined_in ();

  if (ancestor != 0)
    {
      return ancestor->lookup_primitive_type (et);
    }

  switch (et)
    {
    case AST_Expression::EV_short:
      pdt = AST_PredefinedType::PT_short;
      break;
    case AST_Expression::EV_ushort:
      pdt = AST_PredefinedType::PT_ushort;
      break;
    case AST_Expression::EV_long:
      pdt = AST_PredefinedType::PT_long;
      break;
    case AST_Expression::EV_ulong:
      pdt = AST_PredefinedType::PT_ulong;
      break;
    case AST_Expression::EV_longlong:
      pdt = AST_PredefinedType::PT_longlong;
      break;
    case AST_Expression::EV_ulonglong:
      pdt = AST_PredefinedType::PT_ulonglong;
      break;
    case AST_Expression::EV_float:
      pdt = AST_PredefinedType::PT_float;
      break;
    case AST_Expression::EV_double:
      pdt = AST_PredefinedType::PT_double;
      break;
    case AST_Expression::EV_longdouble:
      pdt = AST_PredefinedType::PT_longdouble;
      break;
    case AST_Expression::EV_char:
      pdt = AST_PredefinedType::PT_char;
      break;
    case AST_Expression::EV_wchar:
      pdt = AST_PredefinedType::PT_wchar;
      break;
    case AST_Expression::EV_octet:
      pdt = AST_PredefinedType::PT_octet;
      break;
    case AST_Expression::EV_bool:
      pdt = AST_PredefinedType::PT_boolean;
      break;
    case AST_Expression::EV_any:
      pdt = AST_PredefinedType::PT_any;
      break;
    case AST_Expression::EV_object:
      pdt = AST_PredefinedType::PT_object;
      break;
    case AST_Expression::EV_void:
      pdt = AST_PredefinedType::PT_void;
      break;
    case AST_Expression::EV_enum:
    case AST_Expression::EV_string:
    case AST_Expression::EV_wstring:
    case AST_Expression::EV_none:
    default:
      return 0;
    }

  AST_PredefinedType *t = 0;

  for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_decls);
       !i.is_done();
       i.next ())
    {
      as_decl = i.item ();

      if (as_decl->node_type () == AST_Decl::NT_pre_defined)
        {
          t = AST_PredefinedType::narrow_from_decl (as_decl);

          if (t == 0)
            {
              continue;
            }

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

#define OVERHAUL

AST_Decl *
UTL_Scope::lookup_by_name_local (Identifier *e,
                                 bool full_def_only)
{
  AST_Decl *d = 0;            

  // Will catch Object, TypeCode, TCKind, ValueBase and
  // AbstractBase. A non-zero result of this lookup determines the
  // generation of some #includes and, whether successful or not,
  // incurs no extra overhead.
  d = this->lookup_pseudo (e);

  if (d != 0)
    {
      return d;
    }

  if (this->idl_keyword_clash (e) != 0)
    {
      return 0;
    }

  Identifier *item_name = 0;

  bool in_corba =
    ACE_OS::strcmp (e->get_string (), "CORBA") == 0;

  // We search only the decls here, the local types are done
  // below as a last resort.
  for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *tmp = i.item ();
      item_name = tmp->local_name ();

      if (item_name == 0)
        {
          continue;
        }

      // Right now we populate the global scope with all the CORBA basic
      // types, so something like 'ULong' in an IDL file will find a
      // match, unless we skip over these items. This is a workaround until
      // there's time to fix the code generation for CORBA basic types.
      if (!in_corba
          && ACE_OS::strcmp (tmp->name ()->head ()->get_string (), "CORBA") == 0)
        {
          continue;
        }

      if (e->case_compare (item_name))
        {
          d = tmp;
          AST_Decl::NodeType nt = tmp->node_type ();

          // Special case for forward declared types,
          // In this case, we want to return
          // the full definition member, whether defined yet or not.
          // NOTE: corresponding full_definition fe_add_* methods
          // depend on the behavior below!
          if (nt == AST_Decl::NT_interface_fwd
              || nt == AST_Decl::NT_valuetype_fwd
              || nt == AST_Decl::NT_component_fwd
              || nt == AST_Decl::NT_eventtype_fwd)
            {
              d = AST_InterfaceFwd::narrow_from_decl (d)->full_definition ();
            }
          else if (nt == AST_Decl::NT_struct_fwd
                   || nt == AST_Decl::NT_union_fwd)
            {
              d = AST_StructureFwd::narrow_from_decl (d)->full_definition ();
            }
        
          if (AST_Template_Module_Ref::narrow_from_decl (d) != 0)
            {
              // An IDL module has been created in this scope corresponding
              // to this node. That's the one we want to match, and it
              // occurs in the scope right after this node, so we'll match
              // what we're looking for on the next iteration.
              continue;
            }
        }
    }

  if (d == 0)
    {
      AST_Module *m = AST_Module::narrow_from_scope (this);
      
      if (m != 0)
        {
          d = m->look_in_prev_mods (e);

          if (0 != d && full_def_only && !d->is_defined ())
            {
              d = 0;
            }
        }
    }
        
  if (d == 0)
    {  
      AST_Interface *i =
        AST_Interface::narrow_from_scope (this);
      
      if (i != 0)
        {
          d = i->look_in_inherited_local (e);
        }
    }
  
  /// There are some cases where the iteration over IK_decls
  /// above will fail because what we're searching for has been
  /// added only to the local types. It's less complicated to
  /// do the iteration below only as a last resort.
  if (d == 0)
    {
      for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_localtypes);
           !i.is_done ();
           i.next ())
        {
          AST_Decl *l = i.item ();
          
          if (e->case_compare (l->local_name ()))
            {
              d = l;
              break;
            }
        }
    }
        
  return d;
}

#if defined (OVERHAUL)
AST_Decl *
UTL_Scope::lookup_by_name (UTL_ScopedName *e,
                           bool full_def_only)
{
  /// Empty name? Exit immediately.
  if (e == 0)
    {
      return 0;
    }
    
  UTL_ScopedName *work = e;
    
  /// If name starts with "::" or "" start lookup in global scope,
  /// if we're not there already, short_circuiting the
  /// scope-expanding iteration below.
  if (this->is_global_name (e->head ()))
    {
      work = static_cast<UTL_ScopedName *> (e->tail ());
      
      if (this != idl_global->root ())
        {
          return
            idl_global->root ()->lookup_by_name (work,
                                                 full_def_only);
        }
    }
   
  /// Before proceeding to normal lookup, check if the name
  /// matches a template module parameter. If so, the return
  /// value is created on the heap and is owned by the caller
  /// of this lookup. 
  if (work->length () == 1)
    {
      AST_Param_Holder *param_holder =
        UTL_Scope::match_param (e);
        
      // Since we are inside the scope of a template module, any
      // single-segment scoped name that matches a template
      // parameter name has to be a reference to that parameter,
      // so we return the created placeholder. If there's no
      // match, 0 is returned, and we proceed with the regular
      // lookup.  
      if (param_holder != 0)
        {
          return param_holder;
        }
    }

  AST_Decl *d = 0;
  UTL_Scope *s = this;
  
  d = s->lookup_by_name_local (work->head (),
                               full_def_only);
     
  UTL_Scope *outer = ScopeAsDecl (s)->defined_in ();
    
  /// If all else fails, expand the scope, otherwise
  /// try to match the rest of the scoped name.                             
  if (d == 0)
    {
      return (outer == 0
                ? 0
                : outer->lookup_by_name (work, full_def_only));
    }
  else
    {
      UTL_ScopedName *sn =
        static_cast<UTL_ScopedName *> (work->tail ());
      
      if (sn != 0)
        {
          d = this->iter_lookup_by_name_local (d,
                                               sn,
                                               full_def_only);
            
          if (d == 0)
            {
              return (outer == 0
                        ? 0
                        : outer->lookup_by_name (work,
                                                 full_def_only));
            }
        }
    }

  return d;
}
#else
// Implements lookup by name for scoped names.
AST_Decl *
UTL_Scope::lookup_by_name (UTL_ScopedName *e,
                           bool treat_as_ref,
                           bool in_parent,
                           bool full_def_only)
{
  AST_Decl *d = 0;
  UTL_Scope *s = 0;
  AST_Type *t = 0;

  // Empty name? Error.
  if (e == 0)
    {
      return 0;
    }
    
  if (e->length () == 1)
    {
      AST_Param_Holder *param_holder =
        UTL_Scope::match_param (e);
        
      // Since we are inside the scope of a template module, any
      // single-segment scoped name that matches a template
      // parameter name has to be a reference to that parameter,
      // so we return the created placeholder. If there's no
      // match, 0 is returned, and we proceed with the regular
      // lookup.  
      if (param_holder != 0)
        {
          return param_holder;
        }
    }

  // If name starts with "::" or "" start lookup in global scope.
  if (this->is_global_name (e->head ()))
    {
     // Get parent scope.
      d = ScopeAsDecl (this);

      if (0 == d)
        {
          return 0;
        }

      s = d->defined_in ();

      // If this is the global scope..
      if (0 == s)
        {
          // Look up tail of name starting here.
          d = lookup_by_name ((UTL_ScopedName *) e->tail (),
                              treat_as_ref,
                              in_parent,
                              full_def_only);

          // Now return whatever we have.
          return d;
        }

      // OK, not global scope yet, so simply iterate with parent scope.
      d = s->lookup_by_name (e,
                             treat_as_ref,
                             in_parent,
                             full_def_only);

      // If treat_as_ref is true and d is not NULL, add d to
      // set of nodes referenced here.
      if (treat_as_ref && d != 0)
        {
          add_to_referenced (d,
                             false,
                             0);
        }

      // Now return what we have.
      return d;
    }

  // The name does not start with "::"
  // Is name defined here?
  long index = 0;
  AST_Decl *first_one_found = 0;
  while (true)
    {
      d = this->lookup_by_name_local (e->head (),
                                      index,
                                      full_def_only);

      if (0 == d) // Root of identifier not found in our local scope
        {
          // A no-op unless d can inherit.
          d = look_in_inherited (e, treat_as_ref);

          if (0 == d)
            {
              // A no-op unless d can support interfaces.
              d = look_in_supported (e, treat_as_ref);
            }

          if ((0 == d) && in_parent)
            {
              if (full_def_only && (0 != first_one_found))
                {
                  // We don't need any diagnostic messages and we will
                  // not be following down the parent scope chain.

                  return 0;
                }

              // OK, not found. Go down parent scope chain.
              d = ScopeAsDecl (this);

              if (d != 0)
                {
                  s = d->defined_in ();

                  if (0 == s)
                    {
                      d = 0;
                    }
                  else
                    {
                      d = s->lookup_by_name (e,
                                             treat_as_ref,
                                             in_parent,
                                             full_def_only);
                    }
                }

              if (0 != first_one_found)
                {
                  // Since we have actually seen a match in local scope (even though that
                  // went nowhere) the full identifier is actually undefined as we are not
                  // allowed to follow down the parent scope chain. (The above is still
                  // useful to identify possible user scoping mistakes however for the
                  // following diagnostic.)

                  if (0 != d)
                    {
                      ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("%C: \"%C\", line %d: ")
                        ACE_TEXT ("Did you mean \"::%C\"\n")
                        ACE_TEXT ("   declared at "),
                        idl_global->prog_name (),
                        idl_global->filename ()->get_string (),
                        idl_global->lineno (),
                        d->full_name ()));
                        
                      const bool same_file =
                        (0 == ACE_OS::strcmp (
                                idl_global->filename ()->get_string (),
                                d->file_name ().c_str ()) );
                                
                      if (!same_file)
                        {
                          ACE_ERROR ((LM_ERROR,
                                      ACE_TEXT ("%C "),
                                      d->file_name ().c_str () ));
                        }
                        
                      ACE_ERROR ((
                        LM_ERROR,
                        ACE_TEXT ("line %d but hidden by local \""),
                        d->line () ));
                        
                      if (ScopeAsDecl (this)->full_name ()[0])
                        {
                          ACE_ERROR ((LM_ERROR,
                                      ACE_TEXT ("::%C"),
                                      ScopeAsDecl (this)->full_name () ));
                        }
                        
                      ACE_ERROR ((LM_ERROR,
                                  ACE_TEXT ("::%C\""),
                                  e->head ()->get_string () ));
                        
                      const bool same_file_again =
                        (same_file &&
                         0 == ACE_OS::strcmp (
                                idl_global->filename ()->get_string (),
                                first_one_found->file_name ().c_str ()) );
                      
                      if (!same_file_again)
                        {
                          ACE_ERROR ((
                            LM_ERROR,
                            ACE_TEXT ("\n")
                            ACE_TEXT ("   declared at %C "),
                            first_one_found->file_name ().c_str ()));
                        }
                      else
                        {
                          ACE_ERROR ((LM_ERROR, ACE_TEXT (" at ") ));
                        }
                        
                      ACE_ERROR ((LM_ERROR,
                                  ACE_TEXT ("line %d ?\n"),
                                  first_one_found->line () ));
                    }
                  return 0;
                }
            }

          // If treat_as_ref is true and d is not NULL, add d to
          // set of nodes referenced here.
          if (treat_as_ref && d != 0)
            {
              t = AST_Type::narrow_from_decl (d);

              // Are we a type, rather than an identifier?
              if (t != 0)
                {
                  // Are we defined in this scope or just referenced?
                  if (d->defined_in () == this)
                    {
                      UTL_Scope *ds = ScopeAsDecl (this)->defined_in ();

                      if (ds != 0)
                        {
                          AST_Decl *parent = ScopeAsDecl (ds);
                          AST_Decl::NodeType nt = parent->node_type ();

                          // If the scope we are defined in is itself
                          // inside a module, then we should also
                          // be exported to the enclosing scope,
                          // recursive until we get to the enclosing
                          // module (or root) scope. (CORBA 2.6 3.15.3).
                          while (nt != AST_Decl::NT_module
                                 && nt != AST_Decl::NT_root)
                            {
                              ds->add_to_referenced (d,
                                                     false,
                                                     d->local_name ());

                              ds = parent->defined_in ();
                              parent = ScopeAsDecl (ds);
                              nt = parent->node_type ();
                            }
                        }
                    }
                }
            }

          // OK, now return whatever we found.
          return d;
        }

      // We have found the root of the identifier in our local scope.
      first_one_found = d;
      s = DeclAsScope (d);

      // OK, start of name is defined. Now loop doing local lookups
      // of subsequent elements of the name, if any.
      UTL_ScopedName *sn = (UTL_ScopedName *) e->tail ();

      if (sn != 0)
        {
          d = this->iter_lookup_by_name_local (d,
                                               sn,
                                               0,
                                               full_def_only);
        }

      // If the start of the scoped name is an interface, and the
      // above lookup failed, it's possible that what we're looking
      // up was inherited into that interface. The first call to
      // look_in_inherited() is this function only checks base classes
      // of the scope (interface) we started the lookup from.
      if (d == 0 && s != 0)
        {
          d = s->look_in_inherited (sn, treat_as_ref);
        }

      // All OK, name fully resolved.
      if (d != 0)
        {
          // If treat_as_ref is true and d is not 0, add d to
          // set of nodes referenced here.
          if (treat_as_ref)
            {
              add_to_referenced (d, false, 0);
            }

          return d;
        }
      else
        {
          ++index;
        }
    }
}
#endif 
// Add a node to set of nodes referenced in this scope.
void
UTL_Scope::add_to_referenced (AST_Decl *e,
                              bool recursive,
                              Identifier *id,
                              AST_Decl *ex)
{
  UTL_Scope *s = 0;
  AST_Decl **tmp;
  AST_Interface *itf = 0;
  long oreferenced_allocated;
  long i;

  if (e == 0)
    {
      return;
    }

  AST_Decl::NodeType nt = e->node_type ();

  // Special case for forward declared interfaces in the
  // scope in which they're defined. Cannot add before full
  // definition is seen.
  if (nt == AST_Decl::NT_interface || nt == AST_Decl::NT_component)
    {
      itf = AST_Interface::narrow_from_decl (e);

      if (itf != 0
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
      oreferenced_allocated = this->pd_referenced_allocated;
      pd_referenced_allocated += INCREMENT;

      ACE_NEW (tmp,
               AST_Decl *[this->pd_referenced_allocated]);

      for (i = 0; i < oreferenced_allocated; i++)
        {
          tmp[i] = this->pd_referenced[i];
        }

      delete [] this->pd_referenced;
      this->pd_referenced = tmp;
    }

  // Insert new reference.
  if (ex == 0)
    {
      this->pd_referenced[this->pd_referenced_used++] = e;
    }
  else if (this->referenced (ex))
    {
      for (i = this->pd_referenced_used; i > 0; i--)
        {
          this->pd_referenced[i] = this->pd_referenced[i - 1];

          if (this->pd_referenced[i - 1] == ex)
            {
              this->pd_referenced[i] = e;
              break;
            }
        }

      if (this->pd_referenced_used > 0)
        {
          ++this->pd_referenced_used;
        }
    }

  // Now, if recursive is specified and "this" is not a common ancestor
  // of the referencing scope and the scope of definition of "e" then
  // add "e" to the set of referenced nodes in the parent of "this".
  if (recursive && !(e->has_ancestor (ScopeAsDecl (this))))
    {
      s = e->defined_in ();

      if (s != 0)
        {
          s->add_to_referenced (e,
                                recursive,
                                id);
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
      pd_name_referenced_allocated += INCREMENT;

      Identifier **name_tmp = 0;
      ACE_NEW (name_tmp,
               Identifier *[this->pd_name_referenced_allocated]);

      for (long i = 0; i < name_referenced_allocated; i++)
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
  for (int i = 0; i < this->pd_referenced_used; i++)
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
  for (int i = 0; i < pd_decls_used; i++)
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
  if (e == 0)
    {
      return;
    }

  AST_Decl **tmp = this->pd_decls;
  long i = this->pd_decls_used;

  Identifier *decl_name = e->local_name ();
  char *decl_string = decl_name->get_string ();

  Identifier *ref_name = 0;
  char *ref_string = 0;

  // First, make sure there's no clash between e, that was
  // just declared, and some other identifier referenced
  // in this scope.
  for (; i > 0; --i, ++tmp)
    {
      // A local declaration doesn't use a scoped name.
      ref_name = (*tmp)->local_name ();
      ref_string = ref_name->get_string ();

      // If the names compare exactly, it's a redefini8tion
      // error, unless they're both modules (which can be
      // reopened) or we have a belated definition of a
      // forward-declared interface.
      AST_Decl::NodeType new_nt = e->node_type ();
      AST_Decl::NodeType scope_elem_nt = (*tmp)->node_type ();

      if (decl_name->compare (ref_name) == true
          && this->redef_clash (new_nt, scope_elem_nt) == true)
        {
          idl_global->err ()->redef_error (decl_string,
                                           ref_string);

          // if we try to continue from here, we risk a crash.
          throw Bailout ();
        }
      // If the spellings differ only by case, it's also an error,
      // unless one, but not both of the identifiers were escaped.
      else if (decl_name->case_compare_quiet (ref_name) == true
               && !(decl_name->escaped () ^ ref_name->escaped ()))
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

  AST_Decl *d = ScopeAsDecl (this);
  AST_Decl::NodeType nt = d->node_type ();

  // The name of any scope except the unnamed scope formed by an operation
  // may not be redefined immediately within (and the root scope has no name).
  // As well as OBV factory construct.
  if (nt != AST_Decl::NT_root
      && nt != AST_Decl::NT_op
      && nt != AST_Decl::NT_factory)
    {
      Identifier *parent_name = d->local_name ();

      if (decl_name->compare (parent_name) == true)
        {
          idl_global->err ()->redef_error (
                                  decl_name->get_string (),
                                  parent_name->get_string ()
                                );

          // if we try to continue from here, we risk a crash.
          throw Bailout ();
        }
      else if (decl_name->case_compare_quiet (parent_name) == true)
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

      ACE_NEW (tmp,
               AST_Decl *[pd_decls_allocated]);

      for (i = 0; i < odecls_allocated; i++)
        {
          tmp[i] = this->pd_decls[i];
        }

      delete [] this->pd_decls;

      this->pd_decls = tmp;
    }


  // Insert new decl.
  if (ex == 0)
    {
      this->pd_decls[this->pd_decls_used++] = e;
    }
  else
    {
      for (i = this->pd_decls_used; i > 0; i--)
        {
          this->pd_decls[i] = this->pd_decls[i - 1];

          if (this->pd_decls[i - 1] == ex)
            {
              this->pd_decls[i] = e;
              break;
            }
        }

      ++this->pd_decls_used;
    }
}

// Add a node to set of nodes representing manifest
// types defined in this scope.
void
UTL_Scope::add_to_local_types (AST_Decl *e)
{
  if (e == 0)
    {
      return;
    }

  // Make sure there's space for one more.
  if (this->pd_locals_allocated == this->pd_locals_used)
    {
      long olocals_allocated = pd_locals_allocated;
      pd_locals_allocated += INCREMENT;

      AST_Decl **tmp = 0;
      ACE_NEW (tmp,
               AST_Decl *[this->pd_locals_allocated]);

      for (long i = 0; i < olocals_allocated; i++)
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
  long i = pd_referenced_used;
  AST_Decl **tmp = pd_referenced;
  Identifier *member = 0;
  Identifier *test = 0;

  for (; i > 0; i--, tmp++)
    {
      // Same node?
      if (*tmp == e)
        {
          return true;
        }

      // Are we definging a forward declared struct, union, or interface,
      // or reopening a module?
      bool forward_redef = this->redef_clash (e->node_type (),
                                              (*tmp)->node_type ());

      if (forward_redef == false)
        {
          member = (*tmp)->local_name ();
          test = e->local_name ();

          if (member->compare (test) == true)
            {
              return false;
            }
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
      long j = pd_name_referenced_used;
      Identifier **name_tmp = pd_name_referenced;

      for (; j > 0; j--, name_tmp++)
        {
          // If we are a module, there is no clash, if we
          // are an interface, this is not the right place to
          // catch a clash, and if it wasn't defined in this
          // scope, then it's a type name for something else
          // that was, and it can appear any number of times
          // in this scope without a clash.
          AST_Decl::NodeType nt = e->node_type ();
          
          if (id->compare (*name_tmp) == true
              && nt != AST_Decl::NT_module
              && nt != AST_Decl::NT_param_holder
              && e->defined_in () == this)
            {
              idl_global->err ()->redef_error (id->get_string (),
                                               (*name_tmp)->get_string ());

              return true;
            }
          // No clash if one or the other of the identifiers was escaped.
          else if (id->case_compare_quiet (*name_tmp) == true
                   && !(id->escaped () ^ (*name_tmp)->escaped ()))
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
  AST_Decl *d = 0;

  if (idl_global->indent () == 0)
    {
      UTL_Indenter *idnt = 0;
      ACE_NEW (idnt,
               UTL_Indenter);

      idl_global->set_indent (idnt);
    }

  idl_global->indent ()->increase ();

  if (pd_locals_used > 0)
    {
      o << ("\n/* Locally defined types: */\n");

      for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_localtypes);
           !i.is_done ();
           i.next ())
        {
          d = i.item ();

          if (!d->imported ())
            {
              idl_global->indent ()->skip_to (o);
              d->dump (o);
              o << "\n";
            }
        }
    }

  if (pd_decls_used > 0)
    {
      o << ACE_TEXT ("\n/* Declarations: */\n");

      for (UTL_ScopeActiveIterator j (this, UTL_Scope::IK_decls);
           !j.is_done ();
           j.next ())
        {
          d = j.item ();

          if (!d->imported ())
            {
              idl_global->indent ()->skip_to (o);
              d->dump (o);
              o << ";\n";
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
  AST_Param_Holder *retval = 0;
  
  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*params);
       !i.done ();
       i.advance ())
    {
      FE_Utils::T_Param_Info *param = 0;
      i.next (param);
      
      if (param->name_ == name)
        {
          retval =
            idl_global->gen ()->create_param_holder (e, param);
            
          break;
        }
    }
    
  return retval;
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
      d = 0;
    }

  delete [] this->pd_decls;
  this->pd_decls = 0;
  this->pd_decls_used = 0;
  this->pd_locals_used = 0;

  for (long i = this->pd_name_referenced_used; i > 0; --i)
    {
      Identifier *id = this->pd_name_referenced[i - 1];
      id->destroy ();
      delete id;
      id = 0;
    }

  delete [] this->pd_name_referenced;
  this->pd_name_referenced = 0;
  this->pd_name_referenced_allocated = 0;
  this->pd_name_referenced_used = 0;

  delete [] this->pd_local_types;
  this->pd_local_types = 0;
  this->pd_locals_allocated = 0;
  this->pd_locals_used = 0;

  delete [] this->pd_referenced;
  this->pd_referenced = 0;
  this->pd_referenced_allocated = 0;
  this->pd_referenced_used = 0;
}

bool
UTL_Scope::inherited_op_attr_clash (AST_Decl *t)
{
  AST_Interface *i = AST_Interface::narrow_from_scope (this);
  
  if (i == 0)
    {
      return false;
    }
    
  AST_Decl *d = i->look_in_inherited (t->name (), false);
  
  if (d != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();
      
      if (nt == AST_Decl::NT_attr || nt == AST_Decl::NT_op)
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
  
  if (op == 0)
    {
      return false;
    }
    
  AST_Argument *arg = AST_Argument::narrow_from_decl (t);
  AST_Argument::Direction d = arg->direction ();
  AST_Operation::Flags flag = op->flags ();
  
  /// Cannot add OUT or INOUT argument to oneway operation.
  if ((d == AST_Argument::dir_OUT || d == AST_Argument::dir_INOUT)
      && flag == AST_Operation::OP_oneway)
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_ONEWAY_CONFLICT,
                                  t,
                                  op);
      return true;
    }

  AST_Type *arg_type = arg->field_type ();

  /// This error is not caught in y.tab.cpp so we check for it here.
  if (arg_type->node_type () == AST_Decl::NT_array
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
  /// Catches struct, union * exception
  AST_Structure *s = AST_Structure::narrow_from_scope (this);
  
  /// Catches AST_Field and AST_UnionBranch.
  AST_Field *f = AST_Field::narrow_from_decl (t);
  
  /// Decls inside a struct/union/exception are also referenced by
  /// fields, and so must be handled differently.
  if (s != 0 && f == 0)
    {
      this->add_to_local_types (t);
    }
  else
    {
      this->add_to_scope (t);
    }
    
  AST_Union *u = AST_Union::narrow_from_scope (this);
  AST_UnionBranch *ub = AST_UnionBranch::narrow_from_decl (t);
  
  // If we have an enum discriminator, add the label names to
  // the name_referenced list before we add the union branch,
  // so a branch name clash with a label name will be caught.
  if (u != 0 && ub != 0)
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

/// Determines if a name is global.
bool
UTL_Scope::is_global_name (Identifier *i)
{
  if (i == 0)
    {
      return false;
    }
    
  ACE_CString cmp (i->get_string (), 0, false);
  
  if (cmp == "" || cmp == "::")
    {
      return true;
    }

  return false;
}

AST_Decl *
UTL_Scope::iter_lookup_by_name_local (AST_Decl *d,
                                      UTL_ScopedName *e,
                                      bool full_def_only)
{
  AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
  AST_Decl *result = 0;

  // Remove all the layers of typedefs.
  while (d != 0 && d->node_type () == AST_Decl::NT_typedef)
    {
      if (td == 0)
        {
          return 0;
        }

      d = td->base_type ();
    }
    
  if (d == 0)
    {
      return 0;
    }

  // Try to convert the AST_Decl to a UTL_Scope.
  UTL_Scope *sc = DeclAsScope (d);

  if (sc == 0)
    {
      return 0;
    }
    
  AST_Interface *i = AST_Interface::narrow_from_decl (d);

  if (sc->nmembers () > 0)
    {
      // Look up the first component of the scoped name.
      result = sc->lookup_by_name_local (e->head (),
                                         full_def_only);
    }
  else if (i != 0)
    {
      result = i->look_in_inherited_local (e->head ());
    }
  else
    {
      result = sc->look_in_prev_mods (e->head (), true);
    }

  UTL_ScopedName *sn = (UTL_ScopedName *) e->tail ();

  if (result == 0)
    {
      if (sn == 0)
        {
          result = UTL_Scope::match_param (e);
        }

      return result;
    }
  else
    {
      if (sn == 0)
        {
          // We're done.
          return result;
        }
      else
        {
          // Look up the next component of the scoped name.
          result = iter_lookup_by_name_local (result,
                                              sn,
                                              full_def_only);
        }

      return result;
    }
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
