
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ast_visitor.h
 *
 *  $Id$
 *
 *  The abstract AST visitor base class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IDL_FE_DLL_AST_VISITOR_H
#define TAO_IDL_FE_DLL_AST_VISITOR_H

#include "TAO_IDL_FE_Export.h"

class AST_Decl;
class UTL_Scope;
class AST_Type;
class AST_PredefinedType;
class AST_Module;
class AST_Interface;
class AST_InterfaceFwd;
class AST_ValueBox;
class AST_ValueType;
class AST_ValueTypeFwd;
class AST_Component;
class AST_ComponentFwd;
class AST_PortType;
class AST_Provides;
class AST_Uses;
class AST_Publishes;
class AST_Emits;
class AST_Consumes;
class AST_Template_Module;
class AST_Template_Module_Inst;
class AST_Template_Module_Ref;
class AST_Extended_Port;
class AST_Mirror_Port;
class AST_Connector;
class AST_EventType;
class AST_EventTypeFwd;
class AST_Home;
class AST_Factory;
class AST_Finder;
class AST_Structure;
class AST_StructureFwd;
class AST_Exception;
class AST_Expression;
class AST_Enum;
class AST_Operation;
class AST_Field;
class AST_Argument;
class AST_Attribute;
class AST_Union;
class AST_UnionFwd;
class AST_UnionBranch;
class AST_UnionLabel;
class AST_Constant;
class AST_EnumVal;
class AST_Array;
class AST_Sequence;
class AST_String;
class AST_Typedef;
class AST_Root;
class AST_Native;
class AST_Param_Holder;

/**
 * @class ast_visitor
 *
 * @brief ast_visitor.
 *
 * To implement BE processing we use the "Visitor Pattern", this
 * is the abstract "Visitor". This class is the abstract base class
 * for visitors that traverse the AST_<node> classes. Other abstract
 * classes for C++ code generation, Java code generation, or IfR
 * administration, for example, can be derived from this class and
 * be the root of their own subtree of specialized visitor classes.
 */
class TAO_IDL_FE_Export ast_visitor
{
public:
  virtual ~ast_visitor (void);

  virtual int visit_decl (AST_Decl *d) = 0;
  virtual int visit_scope (UTL_Scope *node) = 0;
  virtual int visit_type (AST_Type *node) = 0;
  virtual int visit_predefined_type (AST_PredefinedType *node) = 0;
  virtual int visit_module (AST_Module *node) = 0;
  virtual int visit_interface (AST_Interface *node) = 0;
  virtual int visit_interface_fwd (AST_InterfaceFwd *node) = 0;
  virtual int visit_valuetype (AST_ValueType *node) = 0;
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node) = 0;
  virtual int visit_component (AST_Component *node) = 0;
  virtual int visit_home (AST_Home *node) = 0;
  virtual int visit_component_fwd (AST_ComponentFwd *node) = 0;
  virtual int visit_template_module (AST_Template_Module *node) = 0;
  virtual int visit_template_module_inst (AST_Template_Module_Inst *node) = 0;
  virtual int visit_template_module_ref (AST_Template_Module_Ref *node) = 0;
  virtual int visit_param_holder (AST_Param_Holder *node) = 0;
  virtual int visit_porttype (AST_PortType *node) = 0;
  virtual int visit_provides (AST_Provides *node) = 0;
  virtual int visit_uses (AST_Uses *node) = 0;
  virtual int visit_publishes (AST_Publishes *node) = 0;
  virtual int visit_emits (AST_Emits *node) = 0;
  virtual int visit_consumes (AST_Consumes *node) = 0;
  virtual int visit_extended_port (AST_Extended_Port *node) = 0;
  virtual int visit_mirror_port (AST_Mirror_Port *node) = 0;
  virtual int visit_connector (AST_Connector *node) = 0;
  virtual int visit_eventtype (AST_EventType *node) = 0;
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node) = 0;
  virtual int visit_factory (AST_Factory *node) = 0;
  virtual int visit_finder (AST_Finder *node) = 0;
  virtual int visit_structure (AST_Structure *node) = 0;
  virtual int visit_structure_fwd (AST_StructureFwd *node) = 0;
  virtual int visit_exception (AST_Exception *node) = 0;
  virtual int visit_expression (AST_Expression *node) = 0;
  virtual int visit_enum (AST_Enum *node) = 0;
  virtual int visit_operation (AST_Operation *node) = 0;
  virtual int visit_field (AST_Field *node) = 0;
  virtual int visit_argument (AST_Argument *node) = 0;
  virtual int visit_attribute (AST_Attribute *node) = 0;
  virtual int visit_union (AST_Union *node) = 0;
  virtual int visit_union_fwd (AST_UnionFwd *node) = 0;
  virtual int visit_union_branch (AST_UnionBranch *node) = 0;
  virtual int visit_union_label (AST_UnionLabel *node) = 0;
  virtual int visit_constant (AST_Constant *node) = 0;
  virtual int visit_enum_val (AST_EnumVal *node) = 0;
  virtual int visit_array (AST_Array *node) = 0;
  virtual int visit_sequence (AST_Sequence *node) = 0;
  virtual int visit_string (AST_String *node) = 0;
  virtual int visit_typedef (AST_Typedef *node) = 0;
  virtual int visit_root (AST_Root *node) = 0;
  virtual int visit_native (AST_Native *node) = 0;
  virtual int visit_valuebox (AST_ValueBox *node) = 0;

protected:
  // For abstract class.
  ast_visitor (void);
};

#endif /* TAO_IDL_FE_DLL_AST_VISITOR_H */
