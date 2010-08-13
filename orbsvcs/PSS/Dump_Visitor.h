// $Id$

#include "PSDL_Node_Visitor.h"

class Dump_Visitor : public TAO_PSDL_Node_Visitor
{
public:

  Dump_Visitor (int depth);

  ~Dump_Visitor ();

  // The following methods help print the parse tree. Each method
  // will invoke the 'accept' methods of the private members (accessed
  // via the public accessor methods) of the class. Again, we are
  // making using of the visitor pattern here.
  int visit_specification (TAO_PSDL_Specification *);
  int visit_definition (TAO_PSDL_Definition *);
  int visit_module (TAO_PSDL_Module *);
  int visit_abstract_storagetype (TAO_PSDL_Abstract_Storagetype *);
  int visit_abstract_storagetype_dcl (TAO_PSDL_Abstract_Storagetype_Dcl *);
  int visit_abstract_storagetype_fwd_dcl (TAO_PSDL_Abstract_Storagetype_Fwd_Dcl *);
  int visit_abstract_storagetype_header (TAO_PSDL_Abstract_Storagetype_Header *);
  int visit_abstract_storagetype_body (TAO_PSDL_Abstract_Storagetype_Body *);
  int visit_abstract_storagetype_member (TAO_PSDL_Abstract_Storagetype_Member *);
  int visit_abstract_storagetype_inh_spec (TAO_PSDL_Abstract_Storagetype_Inh_Spec *);
  int visit_abstract_storagetype_name (TAO_PSDL_Abstract_Storagetype_Name *);
  int visit_psdl_state_dcl (TAO_PSDL_Psdl_State_Dcl *);
  int visit_psdl_state_type_spec (TAO_PSDL_Psdl_State_Type_Spec *);

  int visit_abstract_storagetype_ref_type (TAO_PSDL_Abstract_Storagetype_Ref_Type);

  int visit_abstract_storagehome (TAO_PSDL_Abstract_Storagehome *);
  int visit_abstract_storagehome_fwd_dcl (TAO_PSDL_Abstract_Storagehome_Fwd_Dcl *);
  int visit_abstract_storagehome_dcl (TAO_PSDL_Abstract_Storagehome_Dcl *);

  int visit_abstract_storagehome_header (TAO_PSDL_Abstract_Storagehome_Header *);
  int visit_abstract_storagehome_body (TAO_PSDL_Abstract_Storagehome_Body  *);
  int visit_abstract_storagehome_member (TAO_PSDL_Abstract_Storagehome_Member *);
  int visit_abstract_storagehome_inh_spec (TAO_PSDL_Abstract_Storagehome_Inh_Spec *);
  int visit_abstract_storagehome_name (TAO_PSDL_Abstract_Storagehome_Name *);
  int visit_local_op_dcl (TAO_PSDL_Local_Op_Dcl *);
  int visit_key_dcl (TAO_PSDL_Key_Dcl *);
  int visit_catalog (TAO_PSDL_Catalog *);
  int visit_catalog_inh_spec (TAO_PSDL_Catalog_Inh_Spec *);
  int visit_catalog_name (TAO_PSDL_Catalog_Name *);
  int visit_catalog_body (TAO_PSDL_Catalog_Body *);
  int visit_catalog_member (TAO_PSDL_Catalog_Member *);
  int visit_storagetype_member (TAO_PSDL_Storagetype_Member *);
  int visit_storagetype_inh_spec (TAO_PSDL_Storagetype_Inh_Spec *);
  int visit_storagetype_name (TAO_PSDL_Storagetype_Name *);
  int visit_storagetype_impl_spec (TAO_PSDL_Storagetype_Impl_Spec *);
  int visit_storagetype_ref_type (TAO_PSDL_Storagetype_Ref_Type *);
  int visit_store_directive (TAO_PSDL_Store_Directive *);
  int visit_psdl_concrete_state_type (TAO_PSDL_Psdl_Concrete_State_Type *) ;
  int visit_ref_rep_directive (TAO_PSDL_Ref_Rep_Directive *) ;
  int visit_storagehome (TAO_PSDL_Storagehome *) ;
  int visit_storagehome_header (TAO_PSDL_Storagehome_Header *) ;
  int visit_storagehome_body  (TAO_PSDL_Storagehome_Body *) ;
  int visit_storagehome_member  (TAO_PSDL_Storagehome_Member *) ;
  int visit_storagehome_inh_spec  (TAO_PSDL_Storagehome_Inh_Spec *) ;
  int visit_storagehome_name  (TAO_PSDL_Storagehome_Name *) ;

  int visit_storagehome_impl_spec (TAO_PSDL_Storagehome_Impl_Spec *) ;
  int visit_primary_key_dcl (TAO_PSDL_Primary_Key_Dcl *);
  int visit_identifier (TAO_PSDL_Identifier *);

  int visit_type_dcl (TAO_PSDL_Type_Dcl *) ;
  int visit_type_declarator (TAO_PSDL_Type_Declarator *) ;
  int visit_type_spec (TAO_PSDL_Type_Spec *) ;
  int visit_simple_type_spec  (TAO_PSDL_Simple_Type_Spec *) ;
  int visit_base_type_spec  (TAO_PSDL_Base_Type_Spec *) ;
  int visit_template_type_spec  (TAO_PSDL_Template_Type_Spec *) ;
  int visit_constr_type_spec  (TAO_PSDL_Constr_Type_Spec *) ;
  int visit_declarators  (TAO_PSDL_Declarators *) ;
  int visit_declarator  (TAO_PSDL_Declarator *) ;
  int visit_simple_declarator  (TAO_PSDL_Simple_Declarator *) ;
  int visit_simple_declarator_list  (TAO_PSDL_Simple_Declarator_List *) ;
  int visit_complex_declarator  (TAO_PSDL_Complex_Declarator *) ;
  int visit_predefined_type  (TAO_PSDL_Predefined_Type *) ;
  int visit_member_list (TAO_PSDL_Member_List *) ;
  int visit_member (TAO_PSDL_Member *) ;
  int visit_union_type (TAO_PSDL_Union_Type *) ;
  int visit_switch_type_spec (TAO_PSDL_Switch_Type_Spec *) ;
  int visit_switch_body (TAO_PSDL_Switch_Body *) ;
  int visit_case_case (TAO_PSDL_Case *) ;
  int visit_case_label (TAO_PSDL_Case_Label *) ;
  int visit_element_spec (TAO_PSDL_Element_Spec *) ;
  int visit_const_dcl (TAO_PSDL_Const_Dcl *) ;
  int visit_const_type (TAO_PSDL_Const_Type *) ;
  int visit_const_exp (TAO_PSDL_Const_Exp *) ;
  int visit_or_expr (TAO_PSDL_Or_Expr *) ;
  int visit_xor_expr (TAO_PSDL_Xor_Expr *) ;
  int visit_and_expr (TAO_PSDL_And_Expr *) ;
  int visit_shift_expr (TAO_PSDL_Shift_Expr *) ;
  int visit_add_expr (TAO_PSDL_Add_Expr *) ;
  int visit_mult_expr (TAO_PSDL_Mult_Expr *) ;
  int visit_unary_expr (TAO_PSDL_Unary_Expr *) ;
  int visit_primary_expr (TAO_PSDL_Primary_Expr *) ;
  int visit_except_dcl (TAO_PSDL_Except_Dcl *) ;
  int visit_op_dcl (TAO_PSDL_Op_Dcl *) ;
  int visit_storagetype (TAO_PSDL_Storagetype *) ;
  int visit_storagetype_dcl (TAO_PSDL_Storagetype_Dcl *) ;
  int visit_storagetype_fwd_dcl (TAO_PSDL_Storagetype_Fwd_Dcl *);
  int visit_storagetype_header (TAO_PSDL_Storagetype_Header *);
  int visit_storagetype_body (TAO_PSDL_Storagetype_Body *);
  int visit_storagehome_scope (TAO_PSDL_Storagehome_Scope *);
  int visit_provides_dcl (TAO_PSDL_Provides_Dcl *);
  int visit_op_type_spec (TAO_PSDL_Op_Type_Spec *);
  int visit_parameter_dcls (TAO_PSDL_Parameter_Dcls *);
  int visit_param_dcl (TAO_PSDL_Param_Dcl *);
  int visit_raises_expr (TAO_PSDL_Raises_Expr *);
  int visit_context_expr (TAO_PSDL_Context_Expr *);
  int visit_param_type_spec (TAO_PSDL_Param_Type_Spec *);
  int visit_fixed_pt_type (TAO_PSDL_Fixed_Pt_Type *);
  int visit_constr_forward_decl (TAO_PSDL_Constr_Forward_Decl *);
  int visit_interface (TAO_PSDL_Interface *);
  int visit_interface_body (TAO_PSDL_Interface_Body *);
  int visit_interface_dcl (TAO_PSDL_Interface_Dcl *);
  int visit_forward_dcl (TAO_PSDL_Forward_Dcl *);
  int visit_interface_header (TAO_PSDL_Interface_Header *);
  int visit_export_dcl (TAO_PSDL_Export_Dcl *);
  int visit_interface_inheritance_spec (TAO_PSDL_Interface_Inheritance_Spec *);
  int visit_interface_name (TAO_PSDL_Interface_Name *);
  int visit_scoped_name (TAO_PSDL_Scoped_Name *);
  int visit_value (TAO_PSDL_Value *);
  int visit_value_common_base (TAO_PSDL_Value_Common_Base *);
  int visit_value_forward_dcl (TAO_PSDL_Value_Forward_Dcl *);
  int visit_value_box_dcl (TAO_PSDL_Value_Box_Dcl *);
  int visit_value_abs_dcl (TAO_PSDL_Value_Abs_Dcl *);
  int visit_value_dcl (TAO_PSDL_Value_Dcl *);
  int visit_value_header (TAO_PSDL_Value_Header *);
  int visit_value_inheritance_spec (TAO_PSDL_Value_Inheritance_Spec *);
  int visit_value_name (TAO_PSDL_Value_Name *);
  int visit_value_element (TAO_PSDL_Value_Element *);
  int visit_array_declarator (TAO_PSDL_Array_Declarator *);
  int visit_fixed_array_size (TAO_PSDL_Fixed_Array_Size *);
  int visit_enumerator (TAO_PSDL_Enumerator *);
  int visit_literal (TAO_PSDL_Literal *);
  int visit_state_member (TAO_PSDL_State_Member *);
  int visit_init_dcl (TAO_PSDL_Init_Dcl *);
  int visit_init_param_decls (TAO_PSDL_Init_Param_Decls *);
  int visit_init_param_decl (TAO_PSDL_Init_Param_Decl *);
  int visit_factory_parameters (TAO_PSDL_Factory_Parameters *);
  int visit_attr_dcl (TAO_PSDL_Attr_Dcl *);
  int visit_factory_dcl (TAO_PSDL_Factory_Dcl *);
  int visit_positive_int_const (TAO_PSDL_Positive_Int_Const *);

 private:

  void print_depth (int depth);

private:
  int depth_;
};
