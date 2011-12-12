/* -*- c++ -*- */
// $Id$

#include "ifr_visitor.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

ifr_visitor::ifr_visitor (void)
  : lock_ (0)
{
  // Create the appropriate lock.
  if (be_global->enable_locking ())
    {
      ACE_NEW (this->lock_,
               ACE_Lock_Adapter<TAO_SYNCH_MUTEX> ());
    }
  else
    {
      ACE_NEW (this->lock_,
               ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> ());
    }
}

ifr_visitor::~ifr_visitor (void)
{
  delete this->lock_;
}

// All the visit methods. Each concrete ifr visitors will
// override some subset of these.

int
ifr_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int
ifr_visitor::visit_scope (UTL_Scope *)
{
  return 0;
}

int
ifr_visitor::visit_type (AST_Type *)
{
  return 0;
}

int
ifr_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int
ifr_visitor::visit_module (AST_Module *)
{
  return 0;
}

int
ifr_visitor::visit_interface (AST_Interface *)
{
  return 0;
}

int
ifr_visitor::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int
ifr_visitor::visit_valuebox (AST_ValueBox *)
{
  return 0;
}

int
ifr_visitor::visit_valuetype (AST_ValueType *)
{
  return 0;
}

int
ifr_visitor::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

int
ifr_visitor::visit_component (AST_Component *)
{
  return 0;
}

int
ifr_visitor::visit_component_fwd (AST_ComponentFwd *)
{
  return 0;
}

int
ifr_visitor::visit_template_module (AST_Template_Module *)
{
  return 0;
}

int
ifr_visitor::visit_template_module_inst (AST_Template_Module_Inst *)
{
  return 0;
}

int
ifr_visitor::visit_template_module_ref (AST_Template_Module_Ref *)
{
  return 0;
}

int
ifr_visitor::visit_param_holder(AST_Param_Holder *)
{
  return 0;
}


int ifr_visitor::visit_porttype (AST_PortType *)
{
  return 0;
}

int
ifr_visitor::visit_provides (AST_Provides *)
{
  return 0;
}

int
ifr_visitor::visit_uses (AST_Uses *)
{
  return 0;
}

int
ifr_visitor::visit_publishes (AST_Publishes *)
{
  return 0;
}

int
ifr_visitor::visit_emits (AST_Emits *)
{
  return 0;
}

int
ifr_visitor::visit_consumes (AST_Consumes *)
{
  return 0;
}

int
ifr_visitor::visit_extended_port (AST_Extended_Port *)
{
  return 0;
}

int
ifr_visitor::visit_mirror_port (AST_Mirror_Port *)
{
  return 0;
}

int
ifr_visitor::visit_connector (AST_Connector *)
{
  return 0;
}

int
ifr_visitor::visit_eventtype (AST_EventType *)
{
  return 0;
}

int
ifr_visitor::visit_eventtype_fwd (AST_EventTypeFwd *)
{
  return 0;
}

int
ifr_visitor::visit_home (AST_Home *)
{
  return 0;
}

int
ifr_visitor::visit_factory (AST_Factory *)
{
  return 0;
}

int
ifr_visitor::visit_finder (AST_Finder *)
{
  return 0;
}

int
ifr_visitor::visit_structure (AST_Structure *)
{
  return 0;
}

int
ifr_visitor::visit_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

int
ifr_visitor::visit_exception (AST_Exception *)
{
  return 0;
}

int
ifr_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int
ifr_visitor::visit_enum (AST_Enum *)
{
  return 0;
}

int
ifr_visitor::visit_operation (AST_Operation *)
{
  return 0;
}

int
ifr_visitor::visit_field (AST_Field *)
{
  return 0;
}

int
ifr_visitor::visit_argument (AST_Argument *)
{
  return 0;
}

int
ifr_visitor::visit_attribute (AST_Attribute *)
{
  return 0;
}

int
ifr_visitor::visit_union (AST_Union *)
{
  return 0;
}

int
ifr_visitor::visit_union_fwd (AST_UnionFwd *)
{
  return 0;
}

int
ifr_visitor::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int
ifr_visitor::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
ifr_visitor::visit_constant (AST_Constant *)
{
  return 0;
}

int
ifr_visitor::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int
ifr_visitor::visit_array (AST_Array *)
{
  return 0;
}

int
ifr_visitor::visit_sequence (AST_Sequence *)
{
  return 0;
}

int
ifr_visitor::visit_string (AST_String *)
{
  return 0;
}

int
ifr_visitor::visit_typedef (AST_Typedef *)
{
  return 0;
}

int
ifr_visitor::visit_root (AST_Root *)
{
  return 0;
}

int
ifr_visitor::visit_native (AST_Native *)
{
  return 0;
}

ACE_Lock &
ifr_visitor::lock (void) const
{
  return *this->lock_;
}

