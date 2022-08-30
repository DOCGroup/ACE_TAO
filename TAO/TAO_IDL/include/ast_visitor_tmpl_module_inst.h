
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ast_visitor_tmpl_module_inst.h
 *
 *  This visitor creates nodes resulting
 *  from the instantiation of a template module, or from a
 *  reference to one inside another, and adds these nodes
 *  to the AST.
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

  virtual ~ast_visitor_tmpl_module_inst ();

  int visit_decl (AST_Decl *d) override;
  int visit_scope (UTL_Scope *node) override;
  int visit_type (AST_Type *node) override;
  int visit_predefined_type (AST_PredefinedType *node) override;
  int visit_interface_fwd (AST_InterfaceFwd *node) override;
  int visit_valuetype_fwd (AST_ValueTypeFwd *node) override;
  int visit_component (AST_Component *node) override;
  int visit_home (AST_Home *node) override;
  int visit_component_fwd (AST_ComponentFwd *node) override;
  int visit_template_module_ref (AST_Template_Module_Ref *node) override;
  int visit_porttype (AST_PortType *node) override;
  int visit_provides (AST_Provides *node) override;
  int visit_uses (AST_Uses *node) override;
  int visit_publishes (AST_Publishes *node) override;
  int visit_emits (AST_Emits *node) override;
  int visit_consumes (AST_Consumes *node) override;
  int visit_extended_port (AST_Extended_Port *node) override;
  int visit_mirror_port (AST_Mirror_Port *node) override;
  int visit_connector (AST_Connector *node) override;
  int visit_eventtype_fwd (AST_EventTypeFwd *node) override;
  int visit_structure_fwd (AST_StructureFwd *node) override;
  int visit_exception (AST_Exception *node) override;
  int visit_expression (AST_Expression *node) override;
  int visit_enum (AST_Enum *node) override;
  int visit_union (AST_Union *node) override;
  int visit_union_fwd (AST_UnionFwd *node) override;
  int visit_union_branch (AST_UnionBranch *node) override;
  int visit_union_label (AST_UnionLabel *node) override;
  int visit_enum_val (AST_EnumVal *node) override;
  int visit_array (AST_Array *node) override;
  int visit_sequence (AST_Sequence *node) override;
  int visit_string (AST_String *node) override;
  int visit_native (AST_Native *node) override;
  int visit_valuebox (AST_ValueBox *node) override;
  int visit_param_holder (AST_Param_Holder *node) override;
  int visit_root (AST_Root *node) override;
  int visit_module (AST_Module *node) override;
  int visit_template_module (AST_Template_Module *node) override;
  int visit_template_module_inst (AST_Template_Module_Inst *node) override;
  int visit_eventtype (AST_EventType *node) override;
  int visit_valuetype (AST_ValueType  *node) override;
  int visit_interface (AST_Interface *node) override;
  int visit_attribute (AST_Attribute *node) override;
  int visit_operation (AST_Operation *node) override;
  int visit_argument (AST_Argument *node) override;
  int visit_typedef (AST_Typedef *node) override;
  int visit_constant (AST_Constant *node) override;
  int visit_structure (AST_Structure *node) override;
  int visit_field (AST_Field *node) override;
  int visit_factory (AST_Factory *node) override;
  int visit_finder (AST_Finder *node) override;

protected:
  // Store for passing on to subsequent create modules.
  AST_Template_Module_Inst *tmi_;

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

