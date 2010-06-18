/* -*- c++ -*- */
// $Id$

#include "idl3p_checking_visitor.h"

#include "ast_module.h"

#include "utl_scope.h"

idl3p_checking_visitor::idl3p_checking_visitor (void)
  : needs_codegen_ (false)
{
}

idl3p_checking_visitor::~idl3p_checking_visitor (void)
{
}

int
idl3p_checking_visitor::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      /// No need to check the return value, there is no
      /// case where it returns anything but 0.
      (void) si.item ()->ast_accept (this);
          
      if (this->needs_codegen_)
        {
          /// No need to go any further.
          break;
        }
    }

  return 0;
}

int
idl3p_checking_visitor::visit_module (AST_Module *node)
{
  return this->visit_scope (node);
}

int
idl3p_checking_visitor::visit_interface (AST_Interface *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_interface_fwd (AST_InterfaceFwd *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_valuebox (AST_ValueBox *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_valuetype (AST_ValueType *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_component (AST_Component *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_component_fwd (AST_ComponentFwd *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_template_module_inst (AST_Template_Module_Inst *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_extended_port (AST_Extended_Port *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_mirror_port (AST_Mirror_Port *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_connector (AST_Connector *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_eventtype (AST_EventType *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_eventtype_fwd (AST_EventTypeFwd *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_home (AST_Home *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_factory  (AST_Factory *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_finder (AST_Finder *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_structure (AST_Structure *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_structure_fwd (AST_StructureFwd *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_exception (AST_Exception *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_enum (AST_Enum *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_operation (AST_Operation *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_field (AST_Field *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_argument (AST_Argument *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_attribute (AST_Attribute *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_union (AST_Union *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_union_fwd (AST_UnionFwd *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_constant (AST_Constant *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_array (AST_Array *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_sequence (AST_Sequence *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_string (AST_String *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_typedef (AST_Typedef *)
{
  this->needs_codegen_ = true;
  return 0;
}

int
idl3p_checking_visitor::visit_root (AST_Root *)
{
  return 0;
}

int
idl3p_checking_visitor::visit_native (AST_Native *)
{
  this->needs_codegen_ = true;
  return 0;
}

bool
idl3p_checking_visitor::needs_codegen (void) const
{
  return this->needs_codegen_;
}

