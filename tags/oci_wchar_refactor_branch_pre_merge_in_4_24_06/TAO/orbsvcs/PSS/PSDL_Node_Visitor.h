/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs/PSS
//
// = FILENAME
//    PSDL_Node_Visitor
//
// = DESCRIPTION
//    This class serves as a base class for the classes that put the
//    generated code that should go into the stubs. Derived classes
//    override the necessary methods. Uses the Visitor Pattern.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_NODE_VISITOR_H
#define TAO_PSDL_NODE_VISITOR_H

#include "psdl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PSDL_Node.h"
#include "ace/Array_Base.h"

// Forward declarations.
class TAO_PSDL_Visitor_Factory;


class TAO_PSDL_Specification;
class TAO_PSDL_Definition;
class TAO_PSDL_Module;
class TAO_PSDL_Abstract_Storagetype;
class TAO_PSDL_Abstract_Storagetype_Dcl;
class TAO_PSDL_Abstract_Storagetype_Fwd_Dcl;
class TAO_PSDL_Abstract_Storagetype_Header;
class TAO_PSDL_Abstract_Storagetype_Body;
class TAO_PSDL_Abstract_Storagetype_Member;
class TAO_PSDL_Abstract_Storagetype_Inh_Spec;
class TAO_PSDL_Abstract_Storagetype_Name;
class TAO_PSDL_Psdl_State_Dcl;
class TAO_PSDL_Psdl_State_Type_Spec;
class TAO_PSDL_Abstract_Storagetype_Ref_Type;
class TAO_PSDL_Abstract_Storagehome;
class TAO_PSDL_Abstract_Storagehome_Fwd_Dcl;
class TAO_PSDL_Abstract_Storagehome_Dcl;
class TAO_PSDL_Abstract_Storagehome_Header;
class TAO_PSDL_Abstract_Storagehome_Body;
class TAO_PSDL_Abstract_Storagehome_Member;
class TAO_PSDL_Abstract_Storagehome_Inh_Spec;
class TAO_PSDL_Abstract_Storagehome_Name;
class TAO_PSDL_Local_Op_Dcl;
class TAO_PSDL_Key_Dcl;
class TAO_PSDL_Catalog;
class TAO_PSDL_Catalog_Inh_Spec;
class TAO_PSDL_Catalog_Name;
class TAO_PSDL_Catalog_Body;
class TAO_PSDL_Catalog_Member;
class TAO_PSDL_Provides_Dcl;
class TAO_PSDL_Storagetype;
class TAO_PSDL_Storagetype_Dcl;
class TAO_PSDL_Abstract_Storagetype_Fwd_Dcl;
class TAO_PSDL_Abstract_Storagetype_Header;
class TAO_PSDL_Abstract_Storagetype_Body;
class TAO_PSDL_Abstract_Storagetype_Member;
class TAO_PSDL_Abstract_Storagetype_Inh_Spec;
class TAO_PSDL_Abstract_Storagetype_Name;
class TAO_PSDL_Psdl_State_Dcl;
class TAO_PSDL_Psdl_State_Type_Spec;
class TAO_PSDL_Abstract_Storagetype_Ref_Type;
class TAO_PSDL_Abstract_Storagehome;
class TAO_PSDL_Abstract_Storagehome_Fwd_Dcl;
class TAO_PSDL_Abstract_Storagehome_Dcl;
class TAO_PSDL_Abstract_Storagehome_Header;
class TAO_PSDL_Abstract_Storagehome_Body;
class TAO_PSDL_Abstract_Storagehome_Member;
class TAO_PSDL_Abstract_Storagehome_Inh_Spec;
class TAO_PSDL_Abstract_Storagehome_Name;
class TAO_PSDL_Local_Op_Dcl;
class TAO_PSDL_Key_Dcl;
class TAO_PSDL_Catalog;
class TAO_PSDL_Catalog_Inh_Spec;
class TAO_PSDL_Catalog_Name;
class TAO_PSDL_Catalog_Body;
class TAO_PSDL_Catalog_Member;
class TAO_PSDL_Provides_Dcl;
class TAO_PSDL_Storagetype;
class TAO_PSDL_Storagetype_Dcl;
class TAO_PSDL_Storagetype_Fwd_Dcl;
class TAO_PSDL_Storagetype_Header;
class TAO_PSDL_Storagetype_Body;
class TAO_PSDL_Storagetype_Member;
class TAO_PSDL_Storagetype_Inh_Spec;
class TAO_PSDL_Storagetype_Name;
class TAO_PSDL_Storagetype_Impl_Spec;
class TAO_PSDL_Storagetype_Ref_Type;
class TAO_PSDL_Storagehome_Scope;
class TAO_PSDL_Store_Directive;
class TAO_PSDL_Psdl_Concrete_State_Type;
class TAO_PSDL_Ref_Rep_Directive;
class TAO_PSDL_Storagehome;
class TAO_PSDL_Storagehome_Header;
class TAO_PSDL_Storagehome_Body;
class TAO_PSDL_Storagehome_Member;
class TAO_PSDL_Storagehome_Inh_Spec;
class TAO_PSDL_Storagehome_Name;
class TAO_PSDL_Storagehome_Impl_Spec;
class TAO_PSDL_Primary_Key_Dcl;
class TAO_PSDL_Identifier;
class TAO_PSDL_Type_Dcl;
class TAO_PSDL_Type_Declarator;
class TAO_PSDL_Type_Spec;
class TAO_PSDL_Simple_Type_Spec;
class TAO_PSDL_Base_Type_Spec;
class TAO_PSDL_Template_Type_Spec;
class TAO_PSDL_Constr_Type_Spec;
class TAO_PSDL_Declarators;
class TAO_PSDL_Declarator;
class TAO_PSDL_Simple_Declarator;
class TAO_PSDL_Simple_Declarator_List;
class TAO_PSDL_Complex_Declarator;
class TAO_PSDL_Predefined_Type;
class TAO_PSDL_Member_List;
class TAO_PSDL_Member;
class TAO_PSDL_Union_Type;
class TAO_PSDL_Switch_Type_Spec;
class TAO_PSDL_Switch_Body;
class TAO_PSDL_Case;
class TAO_PSDL_Case_Label;
class TAO_PSDL_Element_Spec;
class TAO_PSDL_Const_Dcl;
class TAO_PSDL_Const_Type;
class TAO_PSDL_Const_Exp;
class TAO_PSDL_Or_Expr;
class TAO_PSDL_Xor_Expr;
class TAO_PSDL_And_Expr;
class TAO_PSDL_Shift_Expr;
class TAO_PSDL_Add_Expr;
class TAO_PSDL_Mult_Expr;
class TAO_PSDL_Unary_Expr;
class TAO_PSDL_Primary_Expr;
class TAO_PSDL_Except_Dcl;
class TAO_PSDL_Op_Dcl;
class TAO_PSDL_Op_Type_Spec;
class TAO_PSDL_Parameter_Dcls;
class TAO_PSDL_Param_Dcl;
class TAO_PSDL_Raises_Expr;
class TAO_PSDL_Context_Expr;
class TAO_PSDL_Param_Type_Spec;
class TAO_PSDL_Fixed_Pt_Type;
class TAO_PSDL_Constr_Forward_Decl;
class TAO_PSDL_Interface;
class TAO_PSDL_Interface_Body;
class TAO_PSDL_Interface_Dcl;
class TAO_PSDL_Forward_Dcl;
class TAO_PSDL_Interface_Header;
class TAO_PSDL_Export_Dcl;
class TAO_PSDL_Storagetype_Dcl;
class TAO_PSDL_Storagetype_Fwd_Dcl;
class TAO_PSDL_Storagetype_Header;
class TAO_PSDL_Storagetype_Body;
class TAO_PSDL_Interface_Inheritance_Spec;
class TAO_PSDL_Interface_Name;
class TAO_PSDL_Scoped_Name;
class TAO_PSDL_Value;
class TAO_PSDL_Value_Common_Base;
class TAO_PSDL_Value_Forward_Dcl;
class TAO_PSDL_Value_Box_Dcl;
class TAO_PSDL_Value_Abs_Dcl;
class TAO_PSDL_Value_Dcl;
class TAO_PSDL_Value_Header;
class TAO_PSDL_Value_Inheritance_Spec;
class TAO_PSDL_Value_Name;
class TAO_PSDL_Value_Element;
class TAO_PSDL_Enumerator;
class TAO_PSDL_Literal;
class TAO_PSDL_Array_Declarator;
class TAO_PSDL_Fixed_Array_Size;
class TAO_PSDL_State_Member;
class TAO_PSDL_Init_Dcl;
class TAO_PSDL_Init_Param_Decls;
class TAO_PSDL_Init_Param_Decl;
class TAO_PSDL_Factory_Parameters;
class TAO_PSDL_Attr_Dcl;
class TAO_PSDL_Factory_Dcl;
class TAO_PSDL_Positive_Int_Const;

class TAO_PSDL_Export TAO_PSDL_Node_Visitor
{
public:

  TAO_PSDL_Node_Visitor ();

  // Destructor.
  virtual ~TAO_PSDL_Node_Visitor (void);

  // All the nodes in the parse tree are of base type
  // TAO_PSDL_Node. Based on the visitor pattern, the following
  // methods are invoked depending on the actual type of the node.
  // The visitor pattern is being used. And, the return value is a '0'
  // when successful and '-1' on a failure.
  virtual int visit_specification (TAO_PSDL_Specification *);
  virtual int visit_definition (TAO_PSDL_Definition *);
  virtual int visit_module (TAO_PSDL_Module *);
  virtual int visit_abstract_storagetype (TAO_PSDL_Abstract_Storagetype *);
  virtual int visit_abstract_storagetype_dcl (TAO_PSDL_Abstract_Storagetype_Dcl *);
  virtual int visit_abstract_storagetype_fwd_dcl (TAO_PSDL_Abstract_Storagetype_Fwd_Dcl *);
  virtual int visit_abstract_storagetype_header (TAO_PSDL_Abstract_Storagetype_Header *);
  virtual int visit_abstract_storagetype_body (TAO_PSDL_Abstract_Storagetype_Body *);
  virtual int visit_abstract_storagetype_member (TAO_PSDL_Abstract_Storagetype_Member *);
  virtual int visit_abstract_storagetype_inh_spec (TAO_PSDL_Abstract_Storagetype_Inh_Spec *);
  virtual int visit_abstract_storagetype_name (TAO_PSDL_Abstract_Storagetype_Name *);
  virtual int visit_psdl_state_dcl (TAO_PSDL_Psdl_State_Dcl *);
  virtual int visit_psdl_state_type_spec (TAO_PSDL_Psdl_State_Type_Spec *);

  virtual int visit_abstract_storagetype_ref_type (TAO_PSDL_Abstract_Storagetype_Ref_Type);

  virtual int visit_abstract_storagehome (TAO_PSDL_Abstract_Storagehome *);
  virtual int visit_abstract_storagehome_fwd_dcl (TAO_PSDL_Abstract_Storagehome_Fwd_Dcl *);
  virtual int visit_abstract_storagehome_dcl (TAO_PSDL_Abstract_Storagehome_Dcl *);

  virtual int visit_abstract_storagehome_header (TAO_PSDL_Abstract_Storagehome_Header *);
  virtual int visit_abstract_storagehome_body (TAO_PSDL_Abstract_Storagehome_Body  *);
  virtual int visit_abstract_storagehome_member (TAO_PSDL_Abstract_Storagehome_Member *);
  virtual int visit_abstract_storagehome_inh_spec (TAO_PSDL_Abstract_Storagehome_Inh_Spec *);
  virtual int visit_abstract_storagehome_name (TAO_PSDL_Abstract_Storagehome_Name *);
  virtual int visit_local_op_dcl (TAO_PSDL_Local_Op_Dcl *);
  virtual int visit_key_dcl (TAO_PSDL_Key_Dcl *);
  virtual int visit_catalog (TAO_PSDL_Catalog *);
  virtual int visit_catalog_inh_spec (TAO_PSDL_Catalog_Inh_Spec *);
  virtual int visit_catalog_name (TAO_PSDL_Catalog_Name *);
  virtual int visit_catalog_body (TAO_PSDL_Catalog_Body *);
  virtual int visit_catalog_member (TAO_PSDL_Catalog_Member *);
  virtual int visit_storagetype_member (TAO_PSDL_Storagetype_Member *);
  virtual int visit_storagetype_inh_spec (TAO_PSDL_Storagetype_Inh_Spec *);
  virtual int visit_storagetype_name (TAO_PSDL_Storagetype_Name *);
  virtual int visit_storagetype_impl_spec (TAO_PSDL_Storagetype_Impl_Spec *);
  virtual int visit_storagetype_ref_type (TAO_PSDL_Storagetype_Ref_Type *);
  virtual int visit_store_directive (TAO_PSDL_Store_Directive *);
  virtual int visit_psdl_concrete_state_type (TAO_PSDL_Psdl_Concrete_State_Type *);
  virtual int visit_ref_rep_directive (TAO_PSDL_Ref_Rep_Directive *);
  virtual int visit_storagehome (TAO_PSDL_Storagehome *);
  virtual int visit_storagehome_header (TAO_PSDL_Storagehome_Header *);
  virtual int visit_storagehome_body  (TAO_PSDL_Storagehome_Body *);
  virtual int visit_storagehome_member  (TAO_PSDL_Storagehome_Member *);
  virtual int visit_storagehome_inh_spec  (TAO_PSDL_Storagehome_Inh_Spec *);
  virtual int visit_storagehome_name  (TAO_PSDL_Storagehome_Name *);

  virtual int visit_storagehome_impl_spec (TAO_PSDL_Storagehome_Impl_Spec *);
  virtual int visit_primary_key_dcl (TAO_PSDL_Primary_Key_Dcl *);
  virtual int visit_identifier (TAO_PSDL_Identifier *);

  virtual int visit_type_dcl (TAO_PSDL_Type_Dcl *);
  virtual int visit_type_declarator (TAO_PSDL_Type_Declarator *);
  virtual int visit_type_spec (TAO_PSDL_Type_Spec *);
  virtual int visit_simple_type_spec  (TAO_PSDL_Simple_Type_Spec *);
  virtual int visit_base_type_spec  (TAO_PSDL_Base_Type_Spec *);
  virtual int visit_template_type_spec  (TAO_PSDL_Template_Type_Spec *);
  virtual int visit_constr_type_spec  (TAO_PSDL_Constr_Type_Spec *);
  virtual int visit_declarators  (TAO_PSDL_Declarators *);
  virtual int visit_declarator  (TAO_PSDL_Declarator *);
  virtual int visit_simple_declarator  (TAO_PSDL_Simple_Declarator *);
  virtual int visit_simple_declarator_list  (TAO_PSDL_Simple_Declarator_List *);
  virtual int visit_complex_declarator  (TAO_PSDL_Complex_Declarator *);
  virtual int visit_predefined_type  (TAO_PSDL_Predefined_Type *);
  virtual int visit_member_list (TAO_PSDL_Member_List *);
  virtual int visit_member (TAO_PSDL_Member *);
  virtual int visit_union_type (TAO_PSDL_Union_Type *);
  virtual int visit_switch_type_spec (TAO_PSDL_Switch_Type_Spec *);
  virtual int visit_switch_body (TAO_PSDL_Switch_Body *);
  virtual int visit_case_case (TAO_PSDL_Case *);
  virtual int visit_case_label (TAO_PSDL_Case_Label *);
  virtual int visit_element_spec (TAO_PSDL_Element_Spec *);
  virtual int visit_const_dcl (TAO_PSDL_Const_Dcl *);
  virtual int visit_const_type (TAO_PSDL_Const_Type *);
  virtual int visit_const_exp (TAO_PSDL_Const_Exp *);
  virtual int visit_or_expr (TAO_PSDL_Or_Expr *);
  virtual int visit_xor_expr (TAO_PSDL_Xor_Expr *);
  virtual int visit_and_expr (TAO_PSDL_And_Expr *);
  virtual int visit_shift_expr (TAO_PSDL_Shift_Expr *);
  virtual int visit_add_expr (TAO_PSDL_Add_Expr *);
  virtual int visit_mult_expr (TAO_PSDL_Mult_Expr *);
  virtual int visit_unary_expr (TAO_PSDL_Unary_Expr *);
  virtual int visit_primary_expr (TAO_PSDL_Primary_Expr *);
  virtual int visit_except_dcl (TAO_PSDL_Except_Dcl *);
  virtual int visit_op_dcl (TAO_PSDL_Op_Dcl *);
  virtual int visit_storagetype (TAO_PSDL_Storagetype *);
  virtual int visit_storagetype_dcl (TAO_PSDL_Storagetype_Dcl *);
  virtual int visit_storagetype_fwd_dcl (TAO_PSDL_Storagetype_Fwd_Dcl *);
  virtual int visit_storagetype_header (TAO_PSDL_Storagetype_Header *);
  virtual int visit_storagetype_body (TAO_PSDL_Storagetype_Body *);
  virtual int visit_storagehome_scope (TAO_PSDL_Storagehome_Scope *);
  virtual int visit_provides_dcl (TAO_PSDL_Provides_Dcl *);
  virtual int visit_op_type_spec (TAO_PSDL_Op_Type_Spec *);
  virtual int visit_parameter_dcls (TAO_PSDL_Parameter_Dcls *);
  virtual int visit_param_dcl (TAO_PSDL_Param_Dcl *);
  virtual int visit_raises_expr (TAO_PSDL_Raises_Expr *);
  virtual int visit_context_expr (TAO_PSDL_Context_Expr *);
  virtual int visit_param_type_spec (TAO_PSDL_Param_Type_Spec *);
  virtual int visit_fixed_pt_type (TAO_PSDL_Fixed_Pt_Type *);
  virtual int visit_constr_forward_decl (TAO_PSDL_Constr_Forward_Decl *);
  virtual int visit_interface (TAO_PSDL_Interface *);
  virtual int visit_interface_body (TAO_PSDL_Interface_Body *);
  virtual int visit_interface_dcl (TAO_PSDL_Interface_Dcl *);
  virtual int visit_forward_dcl (TAO_PSDL_Forward_Dcl *);
  virtual int visit_interface_header (TAO_PSDL_Interface_Header *);
  virtual int visit_export_dcl (TAO_PSDL_Export_Dcl *);
  virtual int visit_interface_inheritance_spec (TAO_PSDL_Interface_Inheritance_Spec *);
  virtual int visit_interface_name (TAO_PSDL_Interface_Name *);
  virtual int visit_scoped_name (TAO_PSDL_Scoped_Name *);
  virtual int visit_value (TAO_PSDL_Value *);
  virtual int visit_value_common_base (TAO_PSDL_Value_Common_Base *);
  virtual int visit_value_forward_dcl (TAO_PSDL_Value_Forward_Dcl *);
  virtual int visit_value_box_dcl (TAO_PSDL_Value_Box_Dcl *);
  virtual int visit_value_abs_dcl (TAO_PSDL_Value_Abs_Dcl *);
  virtual int visit_value_dcl (TAO_PSDL_Value_Dcl *);
  virtual int visit_value_header (TAO_PSDL_Value_Header *);
  virtual int visit_value_inheritance_spec (TAO_PSDL_Value_Inheritance_Spec *);
  virtual int visit_value_name (TAO_PSDL_Value_Name *);
  virtual int visit_value_element (TAO_PSDL_Value_Element *);
  virtual int visit_array_declarator (TAO_PSDL_Array_Declarator *);
  virtual int visit_fixed_array_size (TAO_PSDL_Fixed_Array_Size *);
  virtual int visit_enumerator (TAO_PSDL_Enumerator *);
  virtual int visit_literal (TAO_PSDL_Literal *);
  virtual int visit_state_member (TAO_PSDL_State_Member *);
  virtual int visit_init_dcl (TAO_PSDL_Init_Dcl *);
  virtual int visit_init_param_decls (TAO_PSDL_Init_Param_Decls *);
  virtual int visit_init_param_decl (TAO_PSDL_Init_Param_Decl *);
  virtual int visit_factory_parameters (TAO_PSDL_Factory_Parameters *);
  virtual int visit_attr_dcl (TAO_PSDL_Attr_Dcl *);
  virtual int visit_factory_dcl (TAO_PSDL_Factory_Dcl *);
  virtual int visit_positive_int_const (TAO_PSDL_Positive_Int_Const *);

  /// To check if the identifier_type is a predefined type or one that
  /// is defined in the idl in case.
  virtual int base_type (const ACE_CString &identifier_name,
                         ACE_CString &identifier_type);

  virtual ACE_CString version_for_stub (ACE_CString identifier_type);

  // Generate the code that should go into the stub implementation
  // file for the type_declarators.
  virtual void gen_code_for_si (void);

  // Generate the code that should go into the stub inline
  // file for the type_declarators.
  virtual void gen_code_for_sinline (void);

protected:

  // identifiers_ and predefined_type_ is used to store the temporary
  // values while building ASTs.
  ACE_Array_Base <ACE_CString> identifiers_;
  ACE_Array_Base <CORBA::ULong> predefined_type_;
  CORBA::ULong count_;
  TAO_PSDL_Node_Visitor *node_visitor_;
  TAO_PSDL_Visitor_Factory *visitor_factory_;
};

#endif /* TAO_PSDL_NODE_VISITOR_H */
