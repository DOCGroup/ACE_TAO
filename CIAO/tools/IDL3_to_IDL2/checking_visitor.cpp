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
#include "nr_extern.h"

checking_visitor::checking_visitor (void)
  : is_idl3_ (false),
    is_local_idl3_ (false)
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
checking_visitor::visit_interface (AST_Interface *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->is_local_idl3_ = true;
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

  if (node->imported ())
    {
      return 0;
    }

  this->is_local_idl3_ = true;
  return 0;
}

int
checking_visitor::visit_component_fwd (AST_ComponentFwd *node)
{
  this->is_idl3_ = true;

  if (node->imported ())
    {
      return 0;
    }

  this->is_local_idl3_ = true;
  return 0;
}

int
checking_visitor::visit_eventtype (AST_EventType *node)
{
  this->is_idl3_ = true;

  if (node->imported ())
    {
      return 0;
    }

  this->is_local_idl3_ = true;
  return 0;
}

int
checking_visitor::visit_eventtype_fwd (AST_EventTypeFwd *node)
{
  this->is_idl3_ = true;
  
  if (node->imported ())
    {
      return 0;
    }
  
  this->is_local_idl3_ = true;
  return 0;
}

int
checking_visitor::visit_home (AST_Home *node)
{
  this->is_idl3_ = true;

  if (node->imported ())
    {
      return 0;
    }
  
  this->is_local_idl3_ = true;
  return 0;
}


int
checking_visitor::visit_factory (AST_Factory *)
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
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "checking_visitor::visit_root - "
                         "codegen for scope failed\n"),
                        -1);
    }
    
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

bool
checking_visitor::is_idl3 (void) const
{
  return this->is_idl3_;
}

bool
checking_visitor::is_local_idl3 (void) const
{
  return this->is_local_idl3_;
}

