// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL Backend
//
// = FILENAME
//    be_interpretive_visitor.cpp
//
// = DESCRIPTION
//    Defines a factory that returns a specialized visitor object based on the
//    code generation state. The visitors returned by this factory generate
//    stubs and skeletons that use interpretive form of marshaling
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// individual visitors included only here
#include "be_interpretive.h"

#include "be_visitor_argument.h"
#include "be_visitor_array.h"
#include "be_visitor_attribute.h"
#include "be_visitor_constant.h"
#include "be_visitor_enum.h"
#include "be_visitor_exception.h"
#include "be_visitor_field.h"
#include "be_visitor_interface.h"
#include "be_visitor_interface_fwd.h"
#include "be_visitor_module.h"
#include "be_visitor_operation.h"
#include "be_visitor_root.h"
#include "be_visitor_sequence.h"
#include "be_visitor_structure.h"
#include "be_visitor_typecode.h"
#include "be_visitor_typedef.h"
#include "be_visitor_union.h"
#include "be_visitor_union_branch.h"

TAO_Interpretive_Visitor_Factory::TAO_Interpretive_Visitor_Factory (void)
{
}

// destructor
TAO_Interpretive_Visitor_Factory::~TAO_Interpretive_Visitor_Factory (void)
{
}

be_visitor *
TAO_Interpretive_Visitor_Factory::make_visitor (be_visitor_context *ctx)
{
  TAO_CodeGen::CG_STATE st = ctx->state ();
  // create a new context so that ownership issues are not confused. This newly
  // created context is a copy of what was sent by the caller. The newly
  // created visitor will own this new copy.
  be_visitor_context *new_ctx = new be_visitor_context (*ctx);

  switch (st)
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      return new be_visitor_root_ch (new_ctx);
    case TAO_CodeGen::TAO_ROOT_CI:
      return new be_visitor_root_ci (new_ctx);
    case TAO_CodeGen::TAO_ROOT_CS:
      return new be_visitor_root_cs (new_ctx);
    case TAO_CodeGen::TAO_ROOT_SH:
      return new be_visitor_root_sh (new_ctx);
    case TAO_CodeGen::TAO_ROOT_SI:
      return new be_visitor_root_si (new_ctx);
    case TAO_CodeGen::TAO_ROOT_SS:
      return new be_visitor_root_ss (new_ctx);
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      return new be_visitor_root_any_op (new_ctx);

    case TAO_CodeGen::TAO_MODULE_CH:
      return new be_visitor_module_ch (new_ctx);
    case TAO_CodeGen::TAO_MODULE_SH:
      return new be_visitor_module_sh (new_ctx);
    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
      return new be_visitor_module (new_ctx);
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      return new be_visitor_module_any_op (new_ctx);


    case TAO_CodeGen::TAO_INTERFACE_CH:
      return new be_visitor_interface_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_CI:
      return new be_visitor_interface_ci (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_CS:
      return new be_visitor_interface_cs (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_SH:
      return new be_visitor_interface_sh (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_SI:
      return new be_visitor_interface_si (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_SS:
      return new be_visitor_interface_ss (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH:
      return new be_visitor_interface_collocated_sh (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SS:
      return new be_visitor_interface_collocated_ss (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      return new be_visitor_interface_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      return new be_visitor_interface_any_op_cs (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
      return new be_visitor_interface_tie_sh (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
      return new be_visitor_interface_tie_si (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_FWD_CH:
      return new be_visitor_interface_fwd_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_FWD_CI:
      return new be_visitor_interface_fwd_ci (new_ctx);

    case TAO_CodeGen::TAO_STRUCT_CH:
      return new be_visitor_structure_ch (new_ctx);
    case TAO_CodeGen::TAO_STRUCT_CS:
      return new be_visitor_structure_cs (new_ctx);
    case TAO_CodeGen::TAO_STRUCT_CI:
      return new be_visitor_structure_ci (new_ctx);
    case TAO_CodeGen::TAO_STRUCT_ANY_OP_CH:
      return new be_visitor_structure_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_STRUCT_ANY_OP_CS:
      return new be_visitor_structure_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_CONSTANT_CH:
      return new be_visitor_constant_ch (new_ctx);
    case TAO_CodeGen::TAO_CONSTANT_CS:
      return new be_visitor_constant_cs (new_ctx);

    case TAO_CodeGen::TAO_ENUM_CH:
      return new be_visitor_enum_ch (new_ctx);
    case TAO_CodeGen::TAO_ENUM_CS:
      return new be_visitor_enum_cs (new_ctx);
    case TAO_CodeGen::TAO_ENUM_ANY_OP_CH:
      return new be_visitor_enum_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_ENUM_ANY_OP_CS:
      return new be_visitor_enum_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_FIELD_CH:
      return new be_visitor_field_ch (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CS:
      return new be_visitor_field_cs (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CI:
      return new be_visitor_field_ci (new_ctx);

    case TAO_CodeGen::TAO_UNION_CH:
      return new be_visitor_union_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_CI:
      return new be_visitor_union_ci (new_ctx);
    case TAO_CodeGen::TAO_UNION_CS:
      return new be_visitor_union_cs (new_ctx);
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
      return new be_visitor_union_branch_public_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_PUBLIC_CI:
      return new be_visitor_union_branch_public_ci (new_ctx);
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
      return new be_visitor_union_branch_public_cs (new_ctx);
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
      return new be_visitor_union_branch_public_assign_cs (new_ctx);
    case TAO_CodeGen::TAO_UNION_PUBLIC_RESET_CS:
      return new be_visitor_union_branch_public_reset_cs (new_ctx);
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
      return new be_visitor_union_branch_private_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CH:
      return new be_visitor_union_discriminant_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CI:
      return new be_visitor_union_discriminant_ci (new_ctx);
    case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CS:
      return new be_visitor_union_discriminant_cs (new_ctx);
    case TAO_CodeGen::TAO_UNION_ANY_OP_CH:
      return new be_visitor_union_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_ANY_OP_CS:
      return new be_visitor_union_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_SEQUENCE_CH:
      return new be_visitor_sequence_ch (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_CI:
      return new be_visitor_sequence_ci (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_CS:
      return new be_visitor_sequence_cs (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
      return new be_visitor_sequence_base (new_ctx);
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH:
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
      return new be_visitor_sequence_elemtype (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH:
    case TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CI:
    case TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CS:
      return new be_visitor_sequence_buffer_type (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_ANY_OP_CH:
      return new be_visitor_sequence_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_ANY_OP_CS:
      return new be_visitor_sequence_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_TYPEDEF_CH:
      return new be_visitor_typedef_ch (new_ctx);
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      return new be_visitor_typedef_cs (new_ctx);
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      return new be_visitor_typedef_ci (new_ctx);
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      return new be_visitor_typedef_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      return new be_visitor_typedef_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_TYPECODE_DECL:
      return new be_visitor_typecode_decl (new_ctx);
    case TAO_CodeGen::TAO_TYPECODE_DEFN:
      return new be_visitor_typecode_defn (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_CH:
      return new be_visitor_operation_ch (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_CS:
      return new be_visitor_operation_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_SH:
      return new be_visitor_operation_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_SS:
      return new be_visitor_operation_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_SH:
      return new be_visitor_operation_collocated_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_SS:
      return new be_visitor_operation_collocated_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETTYPE_CH:
    case TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS:
      return new be_visitor_operation_rettype (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
      return new be_visitor_operation_rettype_vardecl_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_PRE_DOCALL_CS:
      return new be_visitor_operation_rettype_pre_docall_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DOCALL_CS:
      return new be_visitor_operation_rettype_docall_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_POST_DOCALL_CS:
      return new be_visitor_operation_rettype_post_docall_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
      return new be_visitor_operation_rettype_return_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_EXCEPTLIST_CS:
      return new be_visitor_operation_exceptlist_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
      return new be_visitor_operation_rettype_vardecl_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_MARSHAL_SS:
      return new be_visitor_operation_rettype_marshal_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
      return new be_visitor_operation_rettype_assign_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_RETVAL_POST_UPCALL_SS:
      return new be_visitor_operation_rettype_post_upcall_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS:
      return new be_visitor_operation_arglist (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_DOCALL_CS:
    case TAO_CodeGen::TAO_OPERATION_ARG_DOCALL_CS:
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_DOCALL_CS:
    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_MARSHAL_SS:
      return new be_visitor_operation_argument (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_TIE_SH:
      return new be_visitor_operation_tie_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_TIE_SI:
      return new be_visitor_operation_tie_si (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH:
    case TAO_CodeGen::TAO_ARGUMENT_ARGLIST_OTHERS:
      return new be_visitor_args_arglist (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
      return new be_visitor_args_pre_docall_cs (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
      return new be_visitor_args_docall_cs (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
      return new be_visitor_args_post_docall_cs (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
      return new be_visitor_args_vardecl_ss (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
      return new be_visitor_args_pre_upcall_ss (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS:
      return new be_visitor_args_upcall_ss (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
      return new be_visitor_args_post_upcall_ss (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_MARSHAL_SS:
      return new be_visitor_args_marshal_ss (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_POST_MARSHAL_SS:
      return new be_visitor_args_post_marshal_ss (new_ctx);

    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
    case TAO_CodeGen::TAO_ATTRIBUTE_COLLOCATED_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_COLLOCATED_SS:
      return new be_visitor_attribute (new_ctx);

    case TAO_CodeGen::TAO_EXCEPTION_CH:
      return new be_visitor_exception_ch (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_CI:
      return new be_visitor_exception_ci (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_CS:
      return new be_visitor_exception_cs (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_CTOR_CH:
    case TAO_CodeGen::TAO_EXCEPTION_CTOR_CS:
      return new be_visitor_exception_ctor (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS:
      return new be_visitor_exception_ctor_assign (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CH:
      return new be_visitor_exception_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_ANY_OP_CS:
      return new be_visitor_exception_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_ARRAY_CH:
      return new be_visitor_array_ch (new_ctx);
    case TAO_CodeGen::TAO_ARRAY_CI:
      return new be_visitor_array_ci (new_ctx);
    case TAO_CodeGen::TAO_ARRAY_CS:
      return new be_visitor_array_cs (new_ctx);
    case TAO_CodeGen::TAO_ARRAY_ANY_OP_CH:
      return new be_visitor_array_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_ARRAY_ANY_OP_CS:
      return new be_visitor_array_any_op_cs (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
    case TAO_CodeGen::TAO_ARGUMENT_CH:
    case TAO_CodeGen::TAO_ARGUMENT_CS:
    case TAO_CodeGen::TAO_ARGUMENT_SH:
    case TAO_CodeGen::TAO_ARGUMENT_SS:
    case TAO_CodeGen::TAO_ARRAY_DEFN_CH:
    case TAO_CodeGen::TAO_ARRAY_OTHER_CH:
    case TAO_CodeGen::TAO_ARRAY_DEFN_CI:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
    default:
      return new be_visitor_decl (new_ctx);
    }
}
