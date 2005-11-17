// $Id$

#include "PSDL_Scope_Visitor.h"
#include "PSDL_y.h"

TAO_PSDL_Scope_Visitor::TAO_PSDL_Scope_Visitor (TAO_PSDL_Node *psdl_node)
  : psdl_node_ (psdl_node)
{
}

TAO_PSDL_Scope_Visitor::~TAO_PSDL_Scope_Visitor ()
{
}

ACE_CString
TAO_PSDL_Scope_Visitor::get_scoped_type (void)
{
  return this->scoped_type_;
}

int
TAO_PSDL_Scope_Visitor::visit_module (TAO_PSDL_Module *module)
{
  //  this->psdl_node_->set_identifier ("module");

  if (module->specification ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_identifier (TAO_PSDL_Identifier *identifier)
{
  this->psdl_node_->set_identifier (identifier->value ());

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_specification (TAO_PSDL_Specification *specification)
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

int TAO_PSDL_Scope_Visitor::visit_definition (TAO_PSDL_Definition *definition)
{
  if (definition->definition_type ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_abstract_storagetype (TAO_PSDL_Abstract_Storagetype *)
{
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_dcl (TAO_PSDL_Abstract_Storagetype_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_fwd_dcl (TAO_PSDL_Abstract_Storagetype_Fwd_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_header (TAO_PSDL_Abstract_Storagetype_Header *)
{
 //  NOT YET SUPPORTED
 return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_body (TAO_PSDL_Abstract_Storagetype_Body *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_member (TAO_PSDL_Abstract_Storagetype_Member *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_inh_spec (TAO_PSDL_Abstract_Storagetype_Inh_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_name (TAO_PSDL_Abstract_Storagetype_Name *)
{
  //  NOT YET SUPPORTED
  return 0; }

int TAO_PSDL_Scope_Visitor::visit_psdl_state_dcl (TAO_PSDL_Psdl_State_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_psdl_state_type_spec (TAO_PSDL_Psdl_State_Type_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagetype_ref_type (TAO_PSDL_Abstract_Storagetype_Ref_Type)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome (TAO_PSDL_Abstract_Storagehome *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_fwd_dcl (TAO_PSDL_Abstract_Storagehome_Fwd_Dcl *)
{ //  NOT YET SUPPORTED


 return 0; }

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_dcl (TAO_PSDL_Abstract_Storagehome_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_header (TAO_PSDL_Abstract_Storagehome_Header *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_body (TAO_PSDL_Abstract_Storagehome_Body  *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_member (TAO_PSDL_Abstract_Storagehome_Member *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_inh_spec (TAO_PSDL_Abstract_Storagehome_Inh_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_abstract_storagehome_name (TAO_PSDL_Abstract_Storagehome_Name *)
{ //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_local_op_dcl (TAO_PSDL_Local_Op_Dcl *local_op_dcl)
{
  if (local_op_dcl->op_type_spec ()->accept (this) == -1)
    return -1;

  if (local_op_dcl->identifier ()->accept (this) == -1)
    return -1;

  if (local_op_dcl->parameter_dcls ()->accept (this) == -1)
    return -1;

  if (local_op_dcl->raises_expr () != 0)
    {
      if (local_op_dcl->raises_expr ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_key_dcl (TAO_PSDL_Key_Dcl *key_dcl)
{
  if (key_dcl->identifier ()->accept (this) == -1)
    return -1;

  if (key_dcl->simple_declarator () != 0)
    if (key_dcl->simple_declarator ()->accept (this) == -1)
      return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_catalog (TAO_PSDL_Catalog *)
{
  //  NOT YET SUPPORTED
  return 0; }

int TAO_PSDL_Scope_Visitor::visit_catalog_inh_spec (TAO_PSDL_Catalog_Inh_Spec *)
{
  //  NOT YET SUPPORTED
  return 0; }

int TAO_PSDL_Scope_Visitor::visit_catalog_name (TAO_PSDL_Catalog_Name *)
{ //  NOT YET SUPPORTED
  return 0; }

int TAO_PSDL_Scope_Visitor::visit_catalog_body (TAO_PSDL_Catalog_Body *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_catalog_member (TAO_PSDL_Catalog_Member *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagetype_member (TAO_PSDL_Storagetype_Member *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagetype_inh_spec (TAO_PSDL_Storagetype_Inh_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagetype_name (TAO_PSDL_Storagetype_Name *storagetype_name)
{
  if (storagetype_name->scoped_name ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagetype_impl_spec (TAO_PSDL_Storagetype_Impl_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagetype_ref_type (TAO_PSDL_Storagetype_Ref_Type *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_store_directive (TAO_PSDL_Store_Directive *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_psdl_concrete_state_type (TAO_PSDL_Psdl_Concrete_State_Type *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_ref_rep_directive (TAO_PSDL_Ref_Rep_Directive *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome (TAO_PSDL_Storagehome *storagehome)
{
  if (storagehome->storagehome_header ()->accept (this) == -1)
    return -1;

  if (storagehome->storagehome_body () != 0)
    if (storagehome->storagehome_body ()->accept (this) == -1)
      return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome_header (TAO_PSDL_Storagehome_Header *sh_header)
{
  if (sh_header->identifier ()->accept (this) == -1)
    return -1;

  if (sh_header->storagetype_name ()->accept (this) == -1)
    return -1;


  if (sh_header->storagehome_inh_spec () != 0)
    if (sh_header->storagehome_inh_spec ()->accept (this) == -1)
      return -1;

  if (sh_header->storagehome_impl_spec () != 0)
    if (sh_header->storagehome_impl_spec ()->accept (this) == -1)
      return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome_body  (TAO_PSDL_Storagehome_Body *sh_body)
{
  if (sh_body->storagehome_member ()->accept (this) == -1)
    return -1;

  if (sh_body->storagehome_body () != 0)
    if (sh_body->storagehome_body ()->accept (this) == -1)
      return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome_member  (TAO_PSDL_Storagehome_Member *sh_member)
{
  if (sh_member->key_dcl ()->accept (this) == -1)
    return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome_inh_spec  (TAO_PSDL_Storagehome_Inh_Spec *sh_inh_spec)
{
  if (sh_inh_spec->storagehome_name ()->accept (this) == -1)
    return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome_name  (TAO_PSDL_Storagehome_Name *sh_name)
{
  if (sh_name->scoped_name ()->accept (this) == -1)
    return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_storagehome_impl_spec (TAO_PSDL_Storagehome_Impl_Spec *sh_impl_spec)
{
  if (sh_impl_spec->abstract_storagehome_name ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_primary_key_dcl (TAO_PSDL_Primary_Key_Dcl *primary_key_dcl)
{
  if (primary_key_dcl->identifier ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_type_dcl (TAO_PSDL_Type_Dcl *type_dcl)
{
  if (type_dcl->key_word () == TAO_PSDL_NATIVE)
    {
      this->psdl_node_->set_identifier (TAO_PSDL_Scope::instance ()->convert_str (type_dcl->key_word ()));

      this->psdl_node_->set_predefined_type ();
    }

  if (type_dcl->type_of_type_dcl ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_type_declarator (TAO_PSDL_Type_Declarator *type_declarator)
{
  if (type_declarator->type_spec ()->accept (this) == -1)
    return -1;

  if (type_declarator->declarators ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_type_spec (TAO_PSDL_Type_Spec *type_spec)
{
  if (type_spec->type_of_type_spec ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_simple_type_spec  (TAO_PSDL_Simple_Type_Spec *simple_type_spec)
{
  if (simple_type_spec->type_of_simple_type_spec ()->accept (this) == -1)
    return -1;

  // @@ why this
  this->scoped_type_ = TAO_PSDL_Scope::instance ()->get_identifier ();
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_base_type_spec  (TAO_PSDL_Base_Type_Spec *base_type_spec)
{
  if (base_type_spec->type_of_base_type_spec ()->accept (this) == -1)
    return -1;
  return 0; }

int TAO_PSDL_Scope_Visitor::visit_template_type_spec  (TAO_PSDL_Template_Type_Spec *template_type_spec)
{
  if (template_type_spec->type_of_template_type_spec ()->accept (this) == -1)
    return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_constr_type_spec  (TAO_PSDL_Constr_Type_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_declarators  (TAO_PSDL_Declarators *declarators)
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

int TAO_PSDL_Scope_Visitor::visit_declarator  (TAO_PSDL_Declarator *declarator)
{
  if (declarator->type_of_declarator ()->accept (this) == -1)
    return -1;
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_simple_declarator  (TAO_PSDL_Simple_Declarator *simple_declarator)
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

int TAO_PSDL_Scope_Visitor::visit_simple_declarator_list  (TAO_PSDL_Simple_Declarator_List *simple_decl_list)
{

  if (simple_decl_list-> simple_declarator ()->accept (this) == -1)
    return -1;

  if (simple_decl_list->simple_declarator_list () != 0)
    {

      if (simple_decl_list->simple_declarator_list ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_complex_declarator  (TAO_PSDL_Complex_Declarator *complex_declarator)
{
  if (complex_declarator->array_declarator ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type)
{
  if ((predefined_type->type_one () != 0) && (predefined_type->type_one () != TAO_PSDL_SEQUENCE))
    {
      // This will happen in the case of typedefs like
      // typedef sequence<Istring> Name. We neednot save the sequence
      // @@ May be this is a wrong solution ... but atleast a
      // temporary one :)
      this->psdl_node_->set_identifier (TAO_PSDL_Scope::instance ()->convert_str (predefined_type->type_one ()));

      this->psdl_node_->set_predefined_type ();
    }
  if (predefined_type->type_two () != 0) {}

  if (predefined_type->type_three () != 0)
    {}

  if (predefined_type->type_of_variable_one () != 0)
    {
      if (predefined_type->type_of_variable_one ()->accept (this) == -1)
        return -1;
    }

  if (predefined_type->type_of_variable_two () != 0)
    {
      if (predefined_type->type_of_variable_two ()->accept (this) == -1)
        return -1;

      // @@ I am not sure now if I need this here. So, commenting it
      // out nad will get it back if need arises in future.
      // this->psdl_node_->set_identifier (TAO_PSDL_Scope::instance ()->get_identifier ());
    }

  if (predefined_type->type_of_variable_three () != 0)
    {
      if (predefined_type->type_of_variable_three ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_member_list (TAO_PSDL_Member_List *member_list)
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
TAO_PSDL_Scope_Visitor::visit_member (TAO_PSDL_Member *member)
{
  if (member->type_spec ()->accept (this) == -1)
    return -1;

  if (member->declarators ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_union_type (TAO_PSDL_Union_Type *)
{ //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_switch_type_spec (TAO_PSDL_Switch_Type_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_switch_body (TAO_PSDL_Switch_Body *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_case_case (TAO_PSDL_Case *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_case_label (TAO_PSDL_Case_Label *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_element_spec (TAO_PSDL_Element_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_const_dcl (TAO_PSDL_Const_Dcl *const_dcl)
{
  if (const_dcl->const_type () != 0)
    {
      if (const_dcl->const_type ()->accept (this) == -1)
        return -1;
    }

  if (const_dcl->identifier ()->accept (this) == -1)
    return -1;

  if (const_dcl->const_exp ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_const_type (TAO_PSDL_Const_Type *const_type)
{
  if (const_type->type ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_const_exp (TAO_PSDL_Const_Exp *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_or_expr (TAO_PSDL_Or_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_xor_expr (TAO_PSDL_Xor_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_and_expr (TAO_PSDL_And_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_shift_expr (TAO_PSDL_Shift_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_add_expr (TAO_PSDL_Add_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_mult_expr (TAO_PSDL_Mult_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_unary_expr (TAO_PSDL_Unary_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_primary_expr (TAO_PSDL_Primary_Expr *)
{
  //  NOT YET SUPPORTED
  return 0;
 }

int TAO_PSDL_Scope_Visitor::visit_except_dcl (TAO_PSDL_Except_Dcl *except_dcl)
{
  // @@ Do I really need this. Lets comment it for now and if it is
  // really needed will uncomment it later,
  /*
  if (except_dcl->identifier ()->accept (this) == -1)
    return -1;
  */

  if (except_dcl->member_list () != 0)
    {
      if (except_dcl->member_list ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_op_dcl (TAO_PSDL_Op_Dcl *op_dcl)
{
  if (op_dcl->op_attribute () != 0)
    if (op_dcl->op_attribute ()->accept (this) == -1)
      return -1;

  if (op_dcl->op_type_spec ()->accept (this) == -1)
    return -1;

  if (op_dcl->identifier ()->accept (this) == -1)
    return -1;

  if (op_dcl->parameter_dcls ()->accept (this) == -1)
    return -1;

  if (op_dcl->raises_expr () != 0)
    if (op_dcl->raises_expr ()->accept (this) == -1)
      return -1;

  if (op_dcl->context_expr () != 0)
    if (op_dcl->context_expr ()->accept (this) == -1)
      return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_storagetype (TAO_PSDL_Storagetype *storagetype)
{
  if (storagetype->declaration () != 0)
    if (storagetype->declaration ()->accept (this) == -1)
      return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_storagetype_dcl (TAO_PSDL_Storagetype_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_storagetype_fwd_dcl (TAO_PSDL_Storagetype_Fwd_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_storagetype_header (TAO_PSDL_Storagetype_Header *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_storagetype_body (TAO_PSDL_Storagetype_Body *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_storagehome_scope (TAO_PSDL_Storagehome_Scope *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_provides_dcl (TAO_PSDL_Provides_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_op_type_spec (TAO_PSDL_Op_Type_Spec *op_type_spec)
{
  if (op_type_spec->type () != 0)
    {
      this->psdl_node_->set_identifier (TAO_PSDL_Scope::instance ()->convert_str (op_type_spec->type ()));

      this->psdl_node_->set_predefined_type ();
    }

  if (op_type_spec->type_spec () != 0)
    if (op_type_spec->type_spec ()->accept (this) == -1)
      return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_parameter_dcls (TAO_PSDL_Parameter_Dcls *parameter_dcls)
{
  if (parameter_dcls->param_dcl () != 0)
    if (parameter_dcls->param_dcl ()->accept (this) == -1)
      return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_param_dcl (TAO_PSDL_Param_Dcl *param_dcl)
{
  if (param_dcl->param_attribute ()->accept (this) == -1)
    return -1;

  if (param_dcl->param_type_spec ()->accept (this) == -1)
    return -1;

  if (param_dcl->simple_declarator ()->accept (this) == -1)
    return -1;

  if (param_dcl->param_dcl () != 0)
    if (param_dcl->param_dcl ()->accept (this) == -1)
      return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_raises_expr (TAO_PSDL_Raises_Expr *raises_expr)
{
  this->psdl_node_->set_identifier ("raises_exception");

  if (raises_expr->first_scoped_name ()->accept (this) == -1)
    return -1;

  if (raises_expr->second_scoped_name () != 0)
    {
      if (raises_expr->second_scoped_name ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_context_expr (TAO_PSDL_Context_Expr *context_expr)
{
  if (context_expr->string_literal ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_param_type_spec (TAO_PSDL_Param_Type_Spec *param_type_spec)
{
  if (param_type_spec->type_of_param ()->accept (this) == -1)
    return -1;
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_fixed_pt_type (TAO_PSDL_Fixed_Pt_Type *fixed_pt_type)
{
  if (fixed_pt_type->positive_int_const_one ()->accept (this) == -1)
    return -1;

  if (fixed_pt_type->positive_int_const_two ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_constr_forward_decl (TAO_PSDL_Constr_Forward_Decl *constr_fwd_dcl)
{
  if (constr_fwd_dcl->type ()->accept (this) == -1)
    return -1;

  if (constr_fwd_dcl->identifier ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_interface (TAO_PSDL_Interface *interf)
{
  if (interf->type_of_dcl ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_interface_body (TAO_PSDL_Interface_Body *interface_body)
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

int TAO_PSDL_Scope_Visitor::visit_interface_dcl (TAO_PSDL_Interface_Dcl *interface_dcl)
{
  if (interface_dcl->interface_header ()->accept (this) == -1)
    return -1;

  if (interface_dcl->interface_body () != 0)
    {
      if (interface_dcl->interface_body ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_forward_dcl (TAO_PSDL_Forward_Dcl *forward_dcl)
{
  if (forward_dcl->identifier ()->accept (this) == -1)
    return -1;

  if (forward_dcl->type () != 0)
    {
      this->psdl_node_->set_identifier (TAO_PSDL_Scope::instance ()->convert_str (forward_dcl->type ()));
      this->psdl_node_->set_predefined_type ();
    }
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_interface_header (TAO_PSDL_Interface_Header *interface_header)
{
  // @@ Do I really need this. Lets comment it for now and if it is
  // really needed will uncomment it later,

  /*
    if (interface_header->identifier ()->accept (this) == -1)
    return -1;
  */

  if (interface_header->type () != 0)
    {
      if (interface_header->type ()->accept (this) == -1)
        return -1;
    }

  if (interface_header->interface_inheritance_spec () != 0)
    {
      if (interface_header->interface_inheritance_spec ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_export_dcl (TAO_PSDL_Export_Dcl *export_dcl)
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

int
TAO_PSDL_Scope_Visitor::visit_interface_inheritance_spec (TAO_PSDL_Interface_Inheritance_Spec *interface_inh_spec)
{
  if (interface_inh_spec->interface_name ()->accept (this) == -1)
    return -1;

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_interface_name (TAO_PSDL_Interface_Name *interface_name)
{
  if (interface_name->scoped_name ()->accept (this) == -1)
    return -1;

  if (interface_name->interface_name () != 0)
    {
      if (interface_name->interface_name ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_scoped_name (TAO_PSDL_Scoped_Name *scoped_name)
{
  if (scoped_name->identifier ()->accept (this) == -1)
    return -1;

  if (scoped_name->scoped_name () != 0)
    {
      if (scoped_name->scoped_name ()->accept (this) == -1)
        return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value (TAO_PSDL_Value *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_common_base (TAO_PSDL_Value_Common_Base *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_forward_dcl (TAO_PSDL_Value_Forward_Dcl *)
{
  //  NOT YET SUPPORTED
 return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_box_dcl (TAO_PSDL_Value_Box_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_abs_dcl (TAO_PSDL_Value_Abs_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0; }

int
TAO_PSDL_Scope_Visitor::visit_value_dcl (TAO_PSDL_Value_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_header (TAO_PSDL_Value_Header *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_inheritance_spec (TAO_PSDL_Value_Inheritance_Spec *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_name (TAO_PSDL_Value_Name *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_value_element (TAO_PSDL_Value_Element *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_array_declarator (TAO_PSDL_Array_Declarator *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_fixed_array_size (TAO_PSDL_Fixed_Array_Size *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_enumerator (TAO_PSDL_Enumerator *enumerator)
{
  if (enumerator->identifier ()->accept (this) == -1)
    return -1;

  if (enumerator->enumerator () != 0)
    {
      if (enumerator->enumerator ()->accept (this) == -1)
        return -1;
    }
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_literal (TAO_PSDL_Literal *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_state_member (TAO_PSDL_State_Member *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_init_dcl (TAO_PSDL_Init_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int
TAO_PSDL_Scope_Visitor::visit_init_param_decls (TAO_PSDL_Init_Param_Decls *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_init_param_decl (TAO_PSDL_Init_Param_Decl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_factory_parameters (TAO_PSDL_Factory_Parameters *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_attr_dcl (TAO_PSDL_Attr_Dcl *attr_dcl)
{
  if (attr_dcl->readonly () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "{ %s }\n",
                  attr_dcl->readonly ()));
    }

  if (attr_dcl->param_type_spec ()->accept (this) == -1)
    return -1;

  if (attr_dcl->simple_declarator ()->accept (this) == -1)
    return -1;

  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_factory_dcl (TAO_PSDL_Factory_Dcl *)
{
  //  NOT YET SUPPORTED
  return 0;
}

int TAO_PSDL_Scope_Visitor::visit_positive_int_const (TAO_PSDL_Positive_Int_Const *)
{
  //  NOT YET SUPPORTED
  return 0;
}
