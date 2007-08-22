// $Id$

#include "PSDL_Node_Visitor.h"
#include "PSDL_Visitor_Factory.h"
#include "PSDL_Typecode_Defn.h"
#include "PSDL_y.h"

TAO_PSDL_Node_Visitor::TAO_PSDL_Node_Visitor ()
  : identifiers_ (0),
    predefined_type_ (0),
    count_ (0),
    node_visitor_ (0)
{
  ACE_NEW (this->visitor_factory_,
           TAO_PSDL_Visitor_Factory);
}

TAO_PSDL_Node_Visitor::~TAO_PSDL_Node_Visitor ()
{
}

int
TAO_PSDL_Node_Visitor::visit_specification (TAO_PSDL_Specification *specification)
{
  if (specification->definition ()->accept (this) == -1)
      return -1;

  if (specification->specification () != 0)
    {
      if (specification->specification ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_definition (TAO_PSDL_Definition *definition)
{
  if (definition->definition_type ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_module (TAO_PSDL_Module *module)
{
  this->node_visitor_ =
    this->visitor_factory_->make_visitor (module->type_of_node ());

  if (this->node_visitor_ != 0)
    {
      return this->node_visitor_->visit_module (module);
    }
  else
    {
      return -1;
    }
}

int TAO_PSDL_Node_Visitor::visit_abstract_storagetype (TAO_PSDL_Abstract_Storagetype *)
{ return 0; }

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_dcl (TAO_PSDL_Abstract_Storagetype_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_fwd_dcl (TAO_PSDL_Abstract_Storagetype_Fwd_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_header (TAO_PSDL_Abstract_Storagetype_Header *)
{
  return 0;
}

int TAO_PSDL_Node_Visitor::visit_abstract_storagetype_body (TAO_PSDL_Abstract_Storagetype_Body *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_member (TAO_PSDL_Abstract_Storagetype_Member *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_inh_spec (TAO_PSDL_Abstract_Storagetype_Inh_Spec *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_name (TAO_PSDL_Abstract_Storagetype_Name *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_psdl_state_dcl (TAO_PSDL_Psdl_State_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_psdl_state_type_spec (TAO_PSDL_Psdl_State_Type_Spec *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagetype_ref_type (TAO_PSDL_Abstract_Storagetype_Ref_Type)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagehome (TAO_PSDL_Abstract_Storagehome *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagehome_fwd_dcl (TAO_PSDL_Abstract_Storagehome_Fwd_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagehome_dcl (TAO_PSDL_Abstract_Storagehome_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagehome_header (TAO_PSDL_Abstract_Storagehome_Header *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagehome_body (TAO_PSDL_Abstract_Storagehome_Body  *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_abstract_storagehome_member (TAO_PSDL_Abstract_Storagehome_Member *)
{
  return 0;
}

int TAO_PSDL_Node_Visitor::visit_abstract_storagehome_inh_spec (TAO_PSDL_Abstract_Storagehome_Inh_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_abstract_storagehome_name (TAO_PSDL_Abstract_Storagehome_Name *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_local_op_dcl (TAO_PSDL_Local_Op_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_key_dcl (TAO_PSDL_Key_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_catalog (TAO_PSDL_Catalog *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_catalog_inh_spec (TAO_PSDL_Catalog_Inh_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_catalog_name (TAO_PSDL_Catalog_Name *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_catalog_body (TAO_PSDL_Catalog_Body *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_catalog_member (TAO_PSDL_Catalog_Member *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_member (TAO_PSDL_Storagetype_Member *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_inh_spec (TAO_PSDL_Storagetype_Inh_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_name (TAO_PSDL_Storagetype_Name *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_impl_spec (TAO_PSDL_Storagetype_Impl_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_ref_type (TAO_PSDL_Storagetype_Ref_Type *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_store_directive (TAO_PSDL_Store_Directive *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_psdl_concrete_state_type (TAO_PSDL_Psdl_Concrete_State_Type *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_ref_rep_directive (TAO_PSDL_Ref_Rep_Directive *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome (TAO_PSDL_Storagehome *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome_header (TAO_PSDL_Storagehome_Header *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome_body  (TAO_PSDL_Storagehome_Body *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome_member  (TAO_PSDL_Storagehome_Member *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome_inh_spec  (TAO_PSDL_Storagehome_Inh_Spec *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome_name  (TAO_PSDL_Storagehome_Name *)  { return 0; }

int TAO_PSDL_Node_Visitor::visit_storagehome_impl_spec (TAO_PSDL_Storagehome_Impl_Spec *)  { return 0; }

int
TAO_PSDL_Node_Visitor::visit_primary_key_dcl (TAO_PSDL_Primary_Key_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
  if (this->identifiers_.size () == 0)
    {
      this->identifiers_.size (1);
      this->count_ = 0;
    }

  this->identifiers_ [this->count_] = identifier->value ();
  ++this->count_;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_type_dcl (TAO_PSDL_Type_Dcl *type_dcl)
{
 if (type_dcl->type_of_type_dcl ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_type_declarator (TAO_PSDL_Type_Declarator *type_declarator)
{
  int type = TAO_PSDL_TYPEDEF;
  this->node_visitor_ =
    this->visitor_factory_->make_visitor (type);

  // Generates code into the client header
  if (this->node_visitor_ != 0)
      return this->node_visitor_->visit_type_declarator (type_declarator);
  else
    return -1;
}

int
TAO_PSDL_Node_Visitor::visit_type_spec (TAO_PSDL_Type_Spec *type_spec)
{
  if (type_spec->type_of_type_spec ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_simple_type_spec (TAO_PSDL_Simple_Type_Spec *simple_type_spec)
{
  if (simple_type_spec->type_of_simple_type_spec ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_base_type_spec (TAO_PSDL_Base_Type_Spec *base_type_spec)
{


  if (base_type_spec->type_of_base_type_spec ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_template_type_spec (TAO_PSDL_Template_Type_Spec *template_type_spec)
{
  if (template_type_spec->type_of_template_type_spec ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_constr_type_spec (TAO_PSDL_Constr_Type_Spec *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_declarators (TAO_PSDL_Declarators *declarators)
{
  if (declarators->declarator ()->accept (this) == -1)
    return -1;

  if (declarators->set_of_declarators () != 0)
    {
      if (declarators->set_of_declarators ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_declarator (TAO_PSDL_Declarator *declarator)
{
  if (declarator->type_of_declarator ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_simple_declarator (TAO_PSDL_Simple_Declarator *simple_declarator)
{
  if (simple_declarator->identifier ()->accept (this) == -1)
    return -1;

  if (simple_declarator->simple_declarator () != 0)
    {
      if (simple_declarator->simple_declarator ()->accept (this) == -1)
        return -1;
    }

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_simple_declarator_list (TAO_PSDL_Simple_Declarator_List *)  { return 0; }

int
TAO_PSDL_Node_Visitor::visit_complex_declarator  (TAO_PSDL_Complex_Declarator *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type)
{
  if (predefined_type->type_one () != 0)
    {
      // @@ mainly for case 291:struct as of now.
      this->node_visitor_ =
        this->visitor_factory_->make_visitor (predefined_type->type_one ());

      if (this->node_visitor_ != 0)
        {
          int result =
            this->node_visitor_->visit_predefined_type (predefined_type);

          if (result != 0)
            return result;

          this->node_visitor_->gen_code_for_si ();
          this->node_visitor_->gen_code_for_sinline ();
        }
    }

  if (predefined_type->type_two () != 0)
    {}

  if (predefined_type->type_three () != 0)
    {}

  // @@ Abt predefined PSDL_Nodes later.
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_member_list (TAO_PSDL_Member_List *member_list)
{
  if (member_list->member ()->accept (this) == -1)
    return -1;

  if (member_list->member_list () != 0)
    {
      if (member_list->member_list ()->accept (this) == -1)
        return -1;
    }

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_member (TAO_PSDL_Member *member)
{
  if (member->type_spec ()->accept (this) == -1)
    return -1;

  if (member->declarators ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_union_type (TAO_PSDL_Union_Type *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_switch_type_spec (TAO_PSDL_Switch_Type_Spec *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_switch_body (TAO_PSDL_Switch_Body *)
{
  return 0;
}

int TAO_PSDL_Node_Visitor::visit_case_case (TAO_PSDL_Case *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_case_label (TAO_PSDL_Case_Label *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_element_spec (TAO_PSDL_Element_Spec *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_const_dcl (TAO_PSDL_Const_Dcl *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_const_type (TAO_PSDL_Const_Type *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_const_exp (TAO_PSDL_Const_Exp *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_or_expr (TAO_PSDL_Or_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_xor_expr (TAO_PSDL_Xor_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_and_expr (TAO_PSDL_And_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_shift_expr (TAO_PSDL_Shift_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_add_expr (TAO_PSDL_Add_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_mult_expr (TAO_PSDL_Mult_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_unary_expr (TAO_PSDL_Unary_Expr *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_primary_expr (TAO_PSDL_Primary_Expr *)  { return 0; }

int
TAO_PSDL_Node_Visitor::visit_except_dcl (TAO_PSDL_Except_Dcl *except_dcl)
{
  int type = TAO_PSDL_EXCEPTION;
  this->node_visitor_ =
    this->visitor_factory_->make_visitor (type);

  if (this->node_visitor_ != 0)
    return this->node_visitor_->visit_except_dcl (except_dcl);

  /*
    if (except_dcl->identifier ()->accept (this) == -1)
    return -1;

  if (except_dcl->member () != 0)
    {
      if (except_dcl->member ()->accept (&body_visitor) == -1)
        return -1;
    }
  */
  return -1;
}

int
TAO_PSDL_Node_Visitor::visit_op_dcl (TAO_PSDL_Op_Dcl *)
{
  return 0;
}

int TAO_PSDL_Node_Visitor::visit_storagetype (TAO_PSDL_Storagetype *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_dcl (TAO_PSDL_Storagetype_Dcl *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_fwd_dcl (TAO_PSDL_Storagetype_Fwd_Dcl *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_header (TAO_PSDL_Storagetype_Header *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagetype_body (TAO_PSDL_Storagetype_Body *)  { return 0; }
int TAO_PSDL_Node_Visitor::visit_storagehome_scope (TAO_PSDL_Storagehome_Scope *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_provides_dcl (TAO_PSDL_Provides_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_op_type_spec (TAO_PSDL_Op_Type_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_parameter_dcls (TAO_PSDL_Parameter_Dcls *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_param_dcl (TAO_PSDL_Param_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_raises_expr (TAO_PSDL_Raises_Expr *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_context_expr (TAO_PSDL_Context_Expr *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_param_type_spec (TAO_PSDL_Param_Type_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_fixed_pt_type (TAO_PSDL_Fixed_Pt_Type *) { return 0; }

int
TAO_PSDL_Node_Visitor::visit_constr_forward_decl (TAO_PSDL_Constr_Forward_Decl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_interface (TAO_PSDL_Interface *interf)
{
  if (interf->type_of_dcl ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_interface_body (TAO_PSDL_Interface_Body *interface_body)
{
  if (interface_body->export_variable ()->accept (this) == -1)
    return -1;

  if (interface_body->interface_body () != 0)
    {
      if (interface_body->interface_body ()->accept (this) == -1)
        return -1;

    }
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_interface_dcl (TAO_PSDL_Interface_Dcl *interface_dcl)
{
  int type = TAO_PSDL_INTERFACE;
  this->node_visitor_ =
    this->visitor_factory_->make_visitor (type);

  if (this->node_visitor_ != 0)
    {
      this->node_visitor_->visit_interface_dcl (interface_dcl);
      this->node_visitor_->gen_code_for_si ();
    }
  else
    return -1;

  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_forward_dcl (TAO_PSDL_Forward_Dcl *forward_dcl)
{
  int type = TAO_PSDL_INTERFACE;
  this->node_visitor_ =
    this->visitor_factory_->make_visitor (type);

  if (this->node_visitor_ != 0)
    return this->node_visitor_->visit_forward_dcl (forward_dcl);

  return -1;
}

int
TAO_PSDL_Node_Visitor::visit_interface_header (TAO_PSDL_Interface_Header *interface_header)
{
  int type = 325;
  this->node_visitor_ =
    this->visitor_factory_->make_visitor (type);

  if (this->node_visitor_ != 0)
    return this->node_visitor_->visit_interface_header (interface_header);
  else
    return -1;
}

int
TAO_PSDL_Node_Visitor::visit_export_dcl (TAO_PSDL_Export_Dcl *export_dcl)
{
  if (export_dcl->type_of_export_one ()->accept (this) == -1)
      return -1;

  if (export_dcl->type_of_export_two () != 0)
    {
      if (export_dcl->type_of_export_two ()->accept (this) == -1)
        return -1;
      }

  return 0;
}

int TAO_PSDL_Node_Visitor::visit_interface_inheritance_spec (TAO_PSDL_Interface_Inheritance_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_interface_name (TAO_PSDL_Interface_Name *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_scoped_name (TAO_PSDL_Scoped_Name *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value (TAO_PSDL_Value *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_common_base (TAO_PSDL_Value_Common_Base *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_forward_dcl (TAO_PSDL_Value_Forward_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_box_dcl (TAO_PSDL_Value_Box_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_abs_dcl (TAO_PSDL_Value_Abs_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_dcl (TAO_PSDL_Value_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_header (TAO_PSDL_Value_Header *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_inheritance_spec (TAO_PSDL_Value_Inheritance_Spec *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_name (TAO_PSDL_Value_Name *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_value_element (TAO_PSDL_Value_Element *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_array_declarator (TAO_PSDL_Array_Declarator *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_fixed_array_size (TAO_PSDL_Fixed_Array_Size *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_enumerator (TAO_PSDL_Enumerator *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_literal (TAO_PSDL_Literal *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_state_member (TAO_PSDL_State_Member *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_init_dcl (TAO_PSDL_Init_Dcl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_init_param_decls (TAO_PSDL_Init_Param_Decls *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_init_param_decl (TAO_PSDL_Init_Param_Decl *) { return 0; }
int TAO_PSDL_Node_Visitor::visit_factory_parameters (TAO_PSDL_Factory_Parameters *) { return 0; }

int
TAO_PSDL_Node_Visitor::visit_attr_dcl (TAO_PSDL_Attr_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_factory_dcl (TAO_PSDL_Factory_Dcl *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::visit_positive_int_const (TAO_PSDL_Positive_Int_Const *)
{
  return 0;
}

int
TAO_PSDL_Node_Visitor::base_type (const ACE_CString &identifier,
                                  ACE_CString &identifier_type)
{
  ACE_CString identifier_name = identifier;
  TAO_PSDL_Scope::instance ()->to_lower_case (identifier_name);

  int result =
    TAO_PSDL_Scope::instance ()->find (identifier_name,
                                       identifier_type);

  if (result == 0)
    {
      // check if the base type is a predefined base type defined in
      // PSDL.yy
      if (ACE_OS::strcmp (identifier_type.c_str (), "enum") == 0)
        return 0;

      // The way I am deciding is trying to find the idn
      result = TAO_PSDL_Scope::instance ()->find (identifier_type);

      if (result == 0)
        {
          return 0;
        }
      else
        {
          return 1;
        }
    }

  // identifier_name is a predefined type like char *.
  identifier_type = identifier_name;

  return 1;
}

ACE_CString
TAO_PSDL_Node_Visitor::version_for_stub (ACE_CString identifier_type)
{
  if (ACE_OS::strcmp (identifier_type.c_str (), "char *") == 0)
    {
      return "CORBA::String";
    }
  else if (ACE_OS::strcmp (identifier_type.c_str (), "long") == 0)
    {
      return "CORBA::Long";
    }
  else
    return identifier_type;
}

void
TAO_PSDL_Node_Visitor::gen_code_for_si (void)
{
  return;
}

void
TAO_PSDL_Node_Visitor::gen_code_for_sinline (void)
{
  return;
}
