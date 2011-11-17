/* -*- c++ -*- */
// $Id$

#include "checking_visitor.h"
#include "identifier_helper.h"
#include "be_extern.h"

#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_home.h"
#include "ast_root.h"
#include "global_extern.h"
#include "nr_extern.h"

checking_visitor::checking_visitor (void)
  : is_idl3_ (false)
{
}

checking_visitor::~checking_visitor (void)
{
}

int
checking_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int
checking_visitor::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_typedef || nt == AST_Decl::NT_pre_defined)
        {
          continue;
        }

      // Want to skip the uses_xxxConnection structs added by uses
      // multiple ports.
      // @@@ (JP) This will go away when the visitor is finished, since
      // those uses_xxxConnection structs will not be added to the AST.
      if (ScopeAsDecl (node)->node_type () == AST_Decl::NT_component
          && nt != AST_Decl::NT_attr)
        {
          continue;
        }

      if (d->ast_accept (this) != 0)
        {
           ACE_ERROR_RETURN ((LM_ERROR,
                              "checking_visitor::visit_scope - "
                              "codegen for scope failed\n"),
                             -1);
        }
    }

  return 0;
}

int
checking_visitor::visit_type (AST_Type *)
{
  return 0;
}

int
checking_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
checking_visitor::visit_module (AST_Module *node)
{
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "checking_visitor::visit_module - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
checking_visitor::visit_interface (AST_Interface *)
{
  return 0;
}

int
checking_visitor::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int
checking_visitor::visit_valuebox (AST_ValueBox *)
{
  return 0;
}

int
checking_visitor::visit_valuetype (AST_ValueType *)
{
  return 0;
}

int
checking_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

int
checking_visitor::visit_component (AST_Component *node)
{
  this->is_idl3_ = true;
  this->remove_idl2_only_filename (node->file_name ());
  return 0;
}

int
checking_visitor::visit_component_fwd (AST_ComponentFwd *node)
{
  this->is_idl3_ = true;
  this->remove_idl2_only_filename (node->file_name ());
  return 0;
}

int
checking_visitor::visit_template_module (AST_Template_Module *)
{
  return 0;
}

int
checking_visitor::visit_template_module_inst (AST_Template_Module_Inst *)
{
  return 0;
}

int
checking_visitor::visit_template_module_ref (AST_Template_Module_Ref *)
{
  return 0;
}

int
checking_visitor::visit_porttype (AST_PortType *)
{
  return 0;
}

int
checking_visitor::visit_provides (AST_Provides *)
{
  return 0;
}

int
checking_visitor::visit_uses (AST_Uses *)
{
  return 0;
}

int
checking_visitor::visit_publishes (AST_Publishes *)
{
  return 0;
}

int
checking_visitor::visit_emits (AST_Emits *)
{
  return 0;
}

int
checking_visitor::visit_consumes (AST_Consumes *)
{
  return 0;
}

int
checking_visitor::visit_extended_port (AST_Extended_Port *)
{
  return 0;
}

int
checking_visitor::visit_mirror_port (AST_Mirror_Port *)
{
  return 0;
}

int
checking_visitor::visit_connector (AST_Connector *)
{
  return 0;
}

int
checking_visitor::visit_eventtype (AST_EventType *node)
{
  this->is_idl3_ = true;
  this->remove_idl2_only_filename (node->file_name ());
  return 0;
}

int
checking_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
{
  this->is_idl3_ = true;
  this->remove_idl2_only_filename (node->file_name ());
  return 0;
}

int
checking_visitor::visit_home (AST_Home *node)
{
  this->is_idl3_ = true;
  this->remove_idl2_only_filename (node->file_name ());
  return 0;
}


int
checking_visitor::visit_factory (AST_Factory *)
{
  return 0;
}

int
checking_visitor::visit_finder (AST_Finder *)
{
  return 0;
}

int
checking_visitor::visit_structure (AST_Structure *)
{
  return 0;
}

int
checking_visitor::visit_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

int
checking_visitor::visit_exception (AST_Exception *)
{
  return 0;
}

int
checking_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int
checking_visitor::visit_enum (AST_Enum *)
{
  return 0;
}


int
checking_visitor::visit_union (AST_Union *)
{
  return 0;
}

int
checking_visitor::visit_union_fwd (AST_UnionFwd *)
{
  return 0;
}


int
checking_visitor::visit_constant (AST_Constant *)
{
  return 0;
}

int
checking_visitor::visit_enum_val (AST_EnumVal *)
{
   return 0;
}

int
checking_visitor::visit_root (AST_Root *node)
{
  // Populate this list with all included filenames. They will be
  // removed as IDL3 constructs are found in them.
  for (size_t i = 0; i < idl_global->n_included_idl_files (); ++i)
    {
      if (! this->idl2_only_files_.empty ())
        {
          this->idl2_only_files_ += " ";
        }

      this->idl2_only_files_ += idl_global->included_idl_files ()[i];
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "checking_visitor::visit_root - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // Append the remaining names in this list to the excluded
  // filenames list. If a filename ends up appearing twice, it
  // shouldn't matter.
  be_global->set_excluded_filenames (this->idl2_only_files_.c_str ());

  return 0;
}

int
checking_visitor::visit_native (AST_Native *)
{
  return 0;
}

int
checking_visitor::visit_operation (AST_Operation *)
{
  return 0;
}

int
checking_visitor::visit_field (AST_Field *)
{
  return 0;
}

int
checking_visitor::visit_argument (AST_Argument *)
{
  return 0;
}

int
checking_visitor::visit_attribute (AST_Attribute *)
{
  return 0;
}

int
checking_visitor::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int
checking_visitor::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
checking_visitor::visit_array (AST_Array *)
{
  return 0;
}

int
checking_visitor::visit_sequence (AST_Sequence *)
{
  return 0;
}

int
checking_visitor::visit_string (AST_String *)
{
  return 0;
}

int
checking_visitor::visit_typedef (AST_Typedef *)
{
  return 0;
}

int
checking_visitor::visit_param_holder (AST_Param_Holder *)
{
  return 0;
}

bool
checking_visitor::is_idl3 (void) const
{
  return this->is_idl3_;
}

void
checking_visitor::remove_idl2_only_filename (ACE_CString filename)
{
  if (this->idl2_only_files_.empty ())
    {
      return;
    }

  ACE_CString::size_type p = 0;
  ACE_CString raw_local_fname (filename);

  ACE_CString::size_type pos = raw_local_fname.rfind ('/');

  if (pos != ACE_CString::npos)
    {
      raw_local_fname =
        raw_local_fname.substr (pos + 1);
    }

  while (p != ACE_CString::npos)
    {
      ACE_CString::size_type cursor = p;

      if (cursor >= this->idl2_only_files_.length ())
        {
          break;
        }

      p = this->idl2_only_files_.find (' ', cursor);

      ACE_CString one_filename =
        this->idl2_only_files_.substr (cursor, p - cursor);

      // Skip the whitespace.
      if (p != ACE_CString::npos)
        {
          while (this->idl2_only_files_[p] == ' ')
            {
              p++;
            }
        }

      if (one_filename == raw_local_fname)
        {
          this->idl2_only_files_ =
            this->idl2_only_files_.substr (0, cursor)
            + this->idl2_only_files_.substr (p);
        }
    }
}

