
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ast_visitor_reifying.h
 *
 *  This visitor visits an AST_Param_Holder, finds the corresponding
 *  template argument, looks up or creates the type, and stores
 *  it for retrieval and use in the template module
 *  instantiation visitor
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_AST_VISITOR_REIFYING_H
#define TAO_AST_VISITOR_REIFYING_H

#include "ast_visitor.h"

#include "utl_scoped_name.h"

class AST_Decl;
class AST_Param_Holder;
class ast_visitor_context;

/**
 * @class ast_visitor_reifying
 *
 * @brief ast_visitor_reifying
 *
 * Reifies a template argument that has been substituted for
 * a template parameter reference. Complete reification may
 * include further reification, for example of array
 * dimensions, sequence bound or (w)string bound
 * for addition to the scope of the template module
 * instantiation.
 */
class ast_visitor_reifying : public ast_visitor
{
public:
  ast_visitor_reifying (ast_visitor_context *ctx);

  virtual ~ast_visitor_reifying ();

  AST_Decl *reified_node () const;

  int visit_decl (AST_Decl *d) override;
  int visit_scope (UTL_Scope *node) override;
  int visit_type (AST_Type *node) override;
  int visit_module (AST_Module *node) override;
  int visit_interface_fwd (AST_InterfaceFwd *node) override;
  int visit_valuetype_fwd (AST_ValueTypeFwd *node) override;
  int visit_home (AST_Home *node) override;
  int visit_component_fwd (AST_ComponentFwd *node) override;
  int visit_template_module (AST_Template_Module *node) override;
  int visit_template_module_inst (AST_Template_Module_Inst *node) override;
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
  int visit_factory (AST_Factory *node) override;
  int visit_finder (AST_Finder *node) override;
  int visit_structure (AST_Structure *node) override;
  int visit_structure_fwd (AST_StructureFwd *node) override;
  int visit_expression (AST_Expression *node) override;
  int visit_enum (AST_Enum *node) override;
  int visit_operation (AST_Operation *node) override;
  int visit_field (AST_Field *node) override;
  int visit_argument (AST_Argument *node) override;
  int visit_attribute (AST_Attribute *node) override;
  int visit_union (AST_Union *node) override;
  int visit_union_fwd (AST_UnionFwd *node) override;
  int visit_union_branch (AST_UnionBranch *node) override;
  int visit_union_label (AST_UnionLabel *node) override;
  int visit_enum_val (AST_EnumVal *node) override;
  int visit_root (AST_Root *node) override;
  int visit_native (AST_Native *node) override;
  int visit_valuebox (AST_ValueBox *node) override;
  int visit_interface (AST_Interface *node) override;
  int visit_valuetype (AST_ValueType *node) override;
  int visit_eventtype (AST_EventType *node) override;
  int visit_component (AST_Component *node) override;
  int visit_exception (AST_Exception *node) override;
  int visit_typedef (AST_Typedef *node) override;
  int visit_array (AST_Array *node) override;
  int visit_sequence (AST_Sequence *node) override;
  int visit_predefined_type (AST_PredefinedType *node) override;
  int visit_string (AST_String *node) override;
  int visit_constant (AST_Constant *node) override;
  int visit_param_holder (AST_Param_Holder *node) override;

private:
  // Check if arg is declared at some level in the scope of a
  // template module. If so, look up the corresponding node in
  // the corresponding instantiation, and store that instead.
  void check_and_store (AST_Decl *node);

  // Returns a relative scoped name allocated on the heap if the
  // arg is declared inside a template module, otherwise 0.
  UTL_ScopedName *template_module_rel_name (AST_Decl *d);

private:
  ast_visitor_context *ctx_;
  AST_Decl *reified_node_;
};

#endif // TAO_AST_VISITOR_REIFYING_H
