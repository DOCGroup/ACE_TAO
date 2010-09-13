
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ast_visitor_tmpl_module_inst.h
 *
 *  $Id$
 *
 *  This visitor creates nodes resulting
 *  from the instantiation of a template module, or from a
 *  reference to one inside another, and adds these nodes
 *  to the AST.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_AST_VISITOR_TMPL_MODULE_INST_H
#define TAO_AST_VISITOR_TMPL_MODULE_INST_H

#include "ast_visitor.h"
#include "fe_utils.h"

class UTL_ExceptList;
class UTL_NameList;
class ast_visitor_context;

/**
 * @class ast_visitor_tmpl_module_inst
 *
 * @brief ast_visitor_tmpl_module_inst
 *
 * Adds implied IDL code to the AST related to the
 * instantiation or alias of a template module.
 */
class ast_visitor_tmpl_module_inst : public ast_visitor
{
public:
  ast_visitor_tmpl_module_inst (ast_visitor_context *ctx,
                                bool ref_only = false);

  virtual ~ast_visitor_tmpl_module_inst (void);

  virtual int visit_decl (AST_Decl *d);
  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_type (AST_Type *node);
  virtual int visit_predefined_type (AST_PredefinedType *node);
  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_home (AST_Home *node);
  virtual int visit_component_fwd (AST_ComponentFwd *node);
  virtual int visit_template_module_ref (AST_Template_Module_Ref *node);
  virtual int visit_porttype (AST_PortType *node);
  virtual int visit_provides (AST_Provides *node);
  virtual int visit_uses (AST_Uses *node);
  virtual int visit_publishes (AST_Publishes *node);
  virtual int visit_emits (AST_Emits *node);
  virtual int visit_consumes (AST_Consumes *node);
  virtual int visit_extended_port (AST_Extended_Port *node);
  virtual int visit_mirror_port (AST_Mirror_Port *node);
  virtual int visit_connector (AST_Connector *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_structure_fwd (AST_StructureFwd *node);
  virtual int visit_exception (AST_Exception *node);
  virtual int visit_expression (AST_Expression *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_union (AST_Union *node);
  virtual int visit_union_fwd (AST_UnionFwd *node);
  virtual int visit_union_branch (AST_UnionBranch *node);
  virtual int visit_union_label (AST_UnionLabel *node);
  virtual int visit_enum_val (AST_EnumVal *node);
  virtual int visit_array (AST_Array *node);
  virtual int visit_sequence (AST_Sequence *node);
  virtual int visit_string (AST_String *node);
  virtual int visit_native (AST_Native *node);
  virtual int visit_valuebox (AST_ValueBox *node);
  virtual int visit_param_holder (AST_Param_Holder *node);
  virtual int visit_root (AST_Root *node);
  virtual int visit_module (AST_Module *node);
  virtual int visit_template_module (AST_Template_Module *node);
  virtual int visit_template_module_inst (AST_Template_Module_Inst *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_valuetype (AST_ValueType  *node);
  virtual int visit_interface (AST_Interface *node);
  virtual int visit_attribute (AST_Attribute *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_argument (AST_Argument *node);
  virtual int visit_typedef (AST_Typedef *node);
  virtual int visit_constant (AST_Constant *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_field (AST_Field *node);
  virtual int visit_factory (AST_Factory *node);
  virtual int visit_finder (AST_Finder *node);

private:
  // If arg is a param holder, return the matching template arg,
  // otherwise return arg unchanged. If the arg is declared in
  // the scope of a template module, the returned node will be a
  // copy created in the scope of the template module instantiation.
  AST_Decl *reify_type (AST_Decl *d);

  // Utility method used for attributes and operations.
  UTL_ExceptList *reify_exception_list (UTL_ExceptList *orig);

  // Utility method to create a NameList from a list of inherited
  // or supported types that may contain a template param ref.
  UTL_NameList *create_name_list (AST_Type **list, long length);

private:
  ast_visitor_context *ctx_;

  // Flag to branch in visit_valuetype() where we create an
  // eventtype or a valuetype.
  bool for_eventtype_;

  // Flag to branch in visit_factory() where we create a
  // finder or a factory.
  bool for_finder_;

  // Flag to short_circuit reifying if we are the base class of
  // ast_visitor_tmpl_module_ref.
  bool ref_only_;
};

#endif // TAO_AST_VISITOR_TMPL_MODULE_INST_H

