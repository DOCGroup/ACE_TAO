// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL Backend
//
// = FILENAME
//    be_visitor_factory.cpp
//
// = DESCRIPTION
//    Defines a factory that returns a specialized visitor object based on the
//    code generation state. This factory creates all the commonly used
//    visitors that are not affected by decisions such as whether we use
//    compiled marshaling or interpretive marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

// individual visitors included only here
#include "be_visitor_factory.h"

#include "be_visitor_argument.h"
#include "be_visitor_array.h"
#include "be_visitor_attribute.h"
#include "be_visitor_constant.h"
#include "be_visitor_enum.h"
#include "be_visitor_exception.h"
#include "be_visitor_field.h"
#include "be_visitor_interface.h"
#include "be_visitor_interface_fwd.h"
#include "be_visitor_valuetype.h"
#include "be_visitor_valuetype_fwd.h"
#include "be_visitor_module.h"
#include "be_visitor_operation.h"
#include "be_visitor_root.h"
#include "be_visitor_sequence.h"
#include "be_visitor_structure.h"
#include "be_visitor_typecode.h"
#include "be_visitor_typedef.h"
#include "be_visitor_union.h"
#include "be_visitor_union_branch.h"

ACE_RCSID(be, be_visitor_factory, "$Id$")


  // The BASE abstract visitor factory
  TAO_Visitor_Factory::TAO_Visitor_Factory (void)
{
}

// destructor
TAO_Visitor_Factory::~TAO_Visitor_Factory (void)
{
}

// The concrete visitor factory for visitors irrespective of the the marshaling
// approaches used.
//
// This common visitor factory also deals with a 2nd level of code-generation
// choices that the user has requested. This involves enabling/disabling
// support for specific type of kind, e.g., generation of TypeCodes or Any
// operators or Real C++ exception suport.
TAO_Common_Visitor_Factory::TAO_Common_Visitor_Factory (void)
{
}

// destructor
TAO_Common_Visitor_Factory::~TAO_Common_Visitor_Factory (void)
{
}

be_visitor *
TAO_Common_Visitor_Factory::make_visitor (be_visitor_context *ctx)
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
    case TAO_CodeGen::TAO_ROOT_IH:
      return new be_visitor_root_ih (new_ctx);
    case TAO_CodeGen::TAO_ROOT_IS:
      return new be_visitor_root_is (new_ctx);
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
    case TAO_CodeGen::TAO_ROOT_ANY_OP_CS:
      // this is the starting point for generation of all Any operators. If we
      // have the any support enabled, then go ahead
      if (be_global->any_support ())
        return new be_visitor_root_any_op (new_ctx);
      else
        // return a NO_OP visitor
        return new be_visitor_decl (new_ctx);

    case TAO_CodeGen::TAO_ROOT_CDR_OP_CH:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CI:
    case TAO_CodeGen::TAO_ROOT_CDR_OP_CS:
      return new be_visitor_root_cdr_op (new_ctx);

    case TAO_CodeGen::TAO_MODULE_CH:
      return new be_visitor_module_ch (new_ctx);

    case TAO_CodeGen::TAO_MODULE_SH:
      return new be_visitor_module_sh (new_ctx);

    case TAO_CodeGen::TAO_MODULE_IH:
      return new be_visitor_module_ih (new_ctx);

    case TAO_CodeGen::TAO_MODULE_CI:
    case TAO_CodeGen::TAO_MODULE_CS:
    case TAO_CodeGen::TAO_MODULE_SI:
    case TAO_CodeGen::TAO_MODULE_SS:
    case TAO_CodeGen::TAO_MODULE_IS:
      return new be_visitor_module (new_ctx);

    case TAO_CodeGen::TAO_MODULE_ANY_OP_CH:
    case TAO_CodeGen::TAO_MODULE_ANY_OP_CS:
      return new be_visitor_module_any_op (new_ctx);

    case TAO_CodeGen::TAO_MODULE_CDR_OP_CH:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CI:
    case TAO_CodeGen::TAO_MODULE_CDR_OP_CS:
      return new be_visitor_module_cdr_op (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_CH:
      return new be_visitor_interface_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_CI:
      return new be_visitor_interface_ci (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_CS:
      return new be_visitor_interface_cs (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_SH:
      return new be_visitor_interface_sh (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_IH:
      return new be_visitor_interface_ih (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_SI:
      return new be_visitor_interface_si (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_SS:
      return new be_visitor_interface_ss (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_IS:
      return new be_visitor_interface_is (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_PROXY_BROKERS_CH:
      return new be_visitor_interface_proxy_brokers_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_BROKER_CH:
      return new be_visitor_interface_base_proxy_broker_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CH:
      return new be_visitor_interface_remote_proxy_broker_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_BROKER_CS:
      return new be_visitor_interface_remote_proxy_broker_cs (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SH:
      return new be_visitor_interface_strategized_proxy_broker_sh (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_STRATEGIZED_PROXY_BROKER_SS:
      return new be_visitor_interface_strategized_proxy_broker_ss (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_PROXY_IMPLS_CH:
      return new be_visitor_interface_proxy_impls_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_BASE_PROXY_IMPL_CH:
      return new be_visitor_interface_base_proxy_impl_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CH:
      return new be_visitor_interface_remote_proxy_impl_ch (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_REMOTE_PROXY_IMPL_CS:
      return new be_visitor_interface_remote_proxy_impl_cs (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH:
      return new be_visitor_interface_thru_poa_proxy_impl_sh (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS:
      return new be_visitor_interface_thru_poa_proxy_impl_ss (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      return new be_visitor_interface_direct_proxy_impl_sh (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SS:
      return new be_visitor_interface_direct_proxy_impl_ss (new_ctx);

      // Old Collocation Stuff.

    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SH:
      return new be_visitor_interface_thru_poa_collocated_sh (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_THRU_POA_COLLOCATED_SS:
      return new be_visitor_interface_thru_poa_collocated_ss (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SH:
      return new be_visitor_interface_direct_collocated_sh (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_DIRECT_COLLOCATED_SS:
      return new be_visitor_interface_direct_collocated_ss (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CH:
      return new be_visitor_interface_any_op_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_ANY_OP_CS:
      return new be_visitor_interface_any_op_cs (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_TIE_SH:
      return new be_visitor_interface_tie_sh (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_TIE_SI:
      return new be_visitor_interface_tie_si (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CH:
      return new be_visitor_interface_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CI:
      return new be_visitor_interface_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_CDR_OP_CS:
      return new be_visitor_interface_cdr_op_cs (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      return new be_visitor_interface_smart_proxy_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CS:
      return new be_visitor_interface_smart_proxy_cs (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CH:
      return new be_visitor_interface_interceptors_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_CS:
      return new be_visitor_interface_interceptors_cs (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SH:
      return new be_visitor_interface_interceptors_sh (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_INTERCEPTORS_SS:
      return new be_visitor_interface_interceptors_ss (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_FWD_CH:
      return new be_visitor_interface_fwd_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_FWD_CI:
      return new be_visitor_interface_fwd_ci (new_ctx);

    case TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CH:
      return new be_visitor_interface_fwd_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CI:
      return new be_visitor_interface_fwd_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_FWD_CDR_OP_CS:
      return new be_visitor_decl (new_ctx); // noop
    case TAO_CodeGen::TAO_INTERFACE_FWD_ANY_OP_CH:
      return new be_visitor_interface_fwd_any_op_ch (new_ctx);

      // Valuetype-related visitors

    case TAO_CodeGen::TAO_VALUETYPE_CH:
      return new be_visitor_valuetype_ch (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_CS:
      return new be_visitor_valuetype_cs (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_CI:
      return new be_visitor_valuetype_ci (new_ctx);

    case TAO_CodeGen::TAO_VALUETYPE_FWD_CH:
      return new be_visitor_valuetype_fwd_ch (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_FWD_CI:
      return new be_visitor_valuetype_fwd_ci (new_ctx);

    case TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CH:
      return new be_visitor_valuetype_fwd_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_FWD_CDR_OP_CI:
      return new be_visitor_valuetype_fwd_cdr_op_ci (new_ctx);

    case TAO_CodeGen::TAO_VALUETYPE_OBV_CH:
      return new be_visitor_valuetype_obv_ch (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CI:
      return new be_visitor_valuetype_obv_ci (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_OBV_CS:
      return new be_visitor_valuetype_obv_cs (new_ctx);

    case TAO_CodeGen::TAO_FIELD_OBV_CH:
      return new be_visitor_valuetype_field_ch (new_ctx);

    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IMPL_CH:
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_IMPL_CS:
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OBV_OPERATION_ARGLIST_OBV_CH:
      return new be_visitor_obv_operation_arglist (new_ctx);

    case TAO_CodeGen::TAO_MODULE_OBV_CH:
    case TAO_CodeGen::TAO_MODULE_OBV_CI:
    case TAO_CodeGen::TAO_MODULE_OBV_CS:
      return new be_visitor_obv_module (new_ctx);

    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CH:
      return new be_visitor_valuetype_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CI:
      return new be_visitor_valuetype_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_CDR_OP_CS:
      return new be_visitor_valuetype_cdr_op_cs (new_ctx);

    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CH:
    case TAO_CodeGen::TAO_VALUETYPE_ANY_OP_CS:
      return new be_visitor_decl (new_ctx);  // @@ TODO

    case TAO_CodeGen::TAO_VALUETYPE_INIT_CH:
      return new be_visitor_valuetype_init_ch (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_INIT_CI:
      return new be_visitor_valuetype_init_ci (new_ctx);
    case TAO_CodeGen::TAO_VALUETYPE_INIT_CS:
      return new be_visitor_valuetype_init_cs (new_ctx);

    case TAO_CodeGen::TAO_VALUETYPE_INIT_ARGLIST_CH:
      return new be_visitor_valuetype_init_arglist_ch (new_ctx);

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
    case TAO_CodeGen::TAO_STRUCT_CDR_OP_CH:
      return new be_visitor_structure_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_STRUCT_CDR_OP_CI:
      return new be_visitor_structure_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_STRUCT_CDR_OP_CS:
      return new be_visitor_structure_cdr_op_cs (new_ctx);

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
    case TAO_CodeGen::TAO_ENUM_CDR_OP_CH:
      return new be_visitor_enum_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_ENUM_CDR_OP_CI:
      return new be_visitor_enum_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_ENUM_CDR_OP_CS:
      return new be_visitor_decl (new_ctx);

    case TAO_CodeGen::TAO_FIELD_CH:
      return new be_visitor_field_ch (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CS:
      return new be_visitor_field_cs (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CI:
      return new be_visitor_field_ci (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CDR_OP_CH:
      return new be_visitor_field_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CDR_OP_CI:
      return new be_visitor_field_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_FIELD_CDR_OP_CS:
      return new be_visitor_field_cdr_op_cs (new_ctx);

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
    case TAO_CodeGen::TAO_UNION_PUBLIC_ACCESS_CS:
      return new be_visitor_union_branch_public_access_cs (new_ctx);
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
      break;
    case TAO_CodeGen::TAO_UNION_CDR_OP_CH:
      return new be_visitor_union_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_CDR_OP_CI:
      return new be_visitor_union_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_UNION_CDR_OP_CS:
      return new be_visitor_union_cdr_op_cs (new_ctx);
    case TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CH:
      return new be_visitor_union_branch_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CI:
      return new be_visitor_union_branch_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_UNION_BRANCH_CDR_OP_CS:
      return new be_visitor_union_branch_cdr_op_cs (new_ctx);

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
    case TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CH:
      return new be_visitor_sequence_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CI:
      return new be_visitor_sequence_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CS:
      return new be_visitor_sequence_cdr_op_cs (new_ctx);

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
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
      return new be_visitor_typedef_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
      return new be_visitor_typedef_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
      return new be_visitor_typedef_cdr_op_cs (new_ctx);

    case TAO_CodeGen::TAO_TYPECODE_DECL:
      // Do we need TypeCode Support?
      if (be_global->tc_support ())
        return new be_visitor_typecode_decl (new_ctx);
      else
        // return a NO_OP visitor
        return new be_visitor_decl (new_ctx);

    case TAO_CodeGen::TAO_TYPECODE_DEFN:
      // Do we need TypeCode Support?
      if (be_global->tc_support ())
        return new be_visitor_typecode_defn (new_ctx);
      else
        // return a NO_OP visitor
        return new be_visitor_decl (new_ctx);

    case TAO_CodeGen::TAO_ATTRIBUTE_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_IH:
    case TAO_CodeGen::TAO_ATTRIBUTE_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_IS:
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_TIE_SI:
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_COLLOCATED_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_COLLOCATED_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_SMART_PROXY_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_INTERCEPTORS_SS:

    case TAO_CodeGen::TAO_ATTRIBUTE_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_PROXY_IMPL_XH:
    case TAO_CodeGen::TAO_ATTRIBUTE_REMOTE_PROXY_IMPL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_THRU_POA_PROXY_IMPL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_DIRECT_PROXY_IMPL_SS:
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
    case TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CH:
      return new be_visitor_exception_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CI:
      return new be_visitor_exception_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_EXCEPTION_CDR_OP_CS:
      return new be_visitor_exception_cdr_op_cs (new_ctx);

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
    case TAO_CodeGen::TAO_ARRAY_CDR_OP_CH:
      return new be_visitor_array_cdr_op_ch (new_ctx);
    case TAO_CodeGen::TAO_ARRAY_CDR_OP_CI:
      return new be_visitor_array_cdr_op_ci (new_ctx);
    case TAO_CodeGen::TAO_ARRAY_CDR_OP_CS:
      return new be_visitor_array_cdr_op_cs (new_ctx);
    default:
      // an error
      delete new_ctx;
      return 0;
    }
  return 0;
}

// The concrete visitor factory for operation visitors  generating compiled
// stubs and skeletons

TAO_Compiled_Visitor_Factory::TAO_Compiled_Visitor_Factory (void)
{
}

// destructor
TAO_Compiled_Visitor_Factory::~TAO_Compiled_Visitor_Factory (void)
{
}

be_visitor *
TAO_Compiled_Visitor_Factory::make_visitor (be_visitor_context *ctx)
{
  TAO_CodeGen::CG_STATE st = ctx->state ();
  // create a new context so that ownership issues are not confused. This newly
  // created context is a copy of what was sent by the caller. The newly
  // created visitor will own this new copy.
  be_visitor_context *new_ctx = new be_visitor_context (*ctx);

  switch (st)
    {
    case TAO_CodeGen::TAO_UNION_ANY_OP_CS:
      return new be_visitor_union_any_op_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_CH:
      return new be_visitor_operation_ch (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_CS:
      return new be_visitor_operation_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_SH:
      return new be_visitor_operation_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_SS:
      return new be_visitor_operation_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_IS:
      return new be_visitor_operation_is (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_IH:
      return new be_visitor_operation_ih (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SH:
      return new be_visitor_operation_thru_poa_collocated_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_THRU_POA_COLLOCATED_SS:
      return new be_visitor_operation_thru_poa_collocated_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SH:
      return new be_visitor_operation_direct_collocated_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_DIRECT_COLLOCATED_SS:
      return new be_visitor_operation_direct_collocated_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CH:
      return new be_visitor_operation_smart_proxy_ch (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_SMART_PROXY_CS:
      return new be_visitor_operation_smart_proxy_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CH:
      return new be_visitor_operation_interceptors_ch (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS:
      return new be_visitor_operation_interceptors_cs (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SH:
      return new be_visitor_operation_interceptors_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_SS:
      return new be_visitor_operation_interceptors_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_BASE_PROXY_IMPL_CH:
      return new be_visitor_operation_base_proxy_impl_ch (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_PROXY_IMPL_XH:
      return new be_visitor_operation_proxy_impl_xh (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_REMOTE_PROXY_IMPL_CS:
      return new be_visitor_operation_remote_proxy_impl_cs (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_THRU_POA_PROXY_IMPL_SS:
      return new be_visitor_operation_thru_poa_proxy_impl_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_DIRECT_PROXY_IMPL_SS:
      return new be_visitor_operation_direct_proxy_impl_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETTYPE_CH:
    case TAO_CodeGen::TAO_OPERATION_RETTYPE_SH:
    case TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS:
      return new be_visitor_operation_rettype (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_RETTYPE_CH:
      return new be_visitor_operation_interceptors_info_rettype (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
      return new be_visitor_operation_rettype_vardecl_cs (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_PRE_INVOKE_CS:
      return new be_visitor_operation_rettype_pre_invoke_cs (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_INVOKE_CS:
      return new be_visitor_operation_rettype_marshal_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
      return new be_visitor_operation_rettype_return_cs (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_EXCEPTLIST_CS:
      return new be_visitor_operation_exceptlist_cs (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST:
      return new be_visitor_operation_interceptors_exceptlist (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
      return new be_visitor_operation_rettype_vardecl_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_MARSHAL_SS:
      return new be_visitor_operation_rettype_marshal_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
      return new be_visitor_operation_rettype_assign_ss (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_INVOKE_ARG_LIST:
      return new be_visitor_operation_inv_arglist (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_IS:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_COLLOCATED_SH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS:

    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH:
    case TAO_CodeGen::TAO_OPERATION_ARGLIST_PROXY_IMPL_XS:
      return new be_visitor_operation_arglist (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SS:
    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS:
      return new be_visitor_operation_interceptors_arglist (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_RESULT:
      return new be_visitor_operation_interceptors_result (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_ARG_PRE_INVOKE_CS:
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_INVOKE_CS:
    case TAO_CodeGen::TAO_OPERATION_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_POST_UPCALL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_DECL_SS:
      return new be_visitor_operation_argument (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_ARG_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_OPERATION_ARG_MARSHAL_SS:
      return new be_compiled_visitor_operation_argument_marshal (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_TIE_SH:
      return new be_visitor_operation_tie_sh (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_TIE_SI:
      return new be_visitor_operation_tie_si (new_ctx);

    case TAO_CodeGen::TAO_OPERATION_ARG_INVOKE_CS:
      return new be_compiled_visitor_operation_argument_invoke (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH:
    case TAO_CodeGen::TAO_ARGUMENT_ARGLIST_SH:
    case TAO_CodeGen::TAO_ARGUMENT_ARGLIST_OTHERS:
      return new be_visitor_args_arglist (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_CH:
      return new be_visitor_args_request_info_ch (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CH:
      return new be_visitor_args_request_info_arglist (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_CS:
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CS:
      return new be_visitor_args_request_info_cs (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_PARAMLIST:
      return new be_visitor_args_paramlist (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_RESULT:
      return new be_visitor_args_request_info_result (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_SH:
      return new be_visitor_args_request_info_sh (new_ctx);
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_ARGLIST_SS:
    case TAO_CodeGen::TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_SS:
      return new be_visitor_args_request_info_ss (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_PRE_INVOKE_CS:
      return new be_visitor_args_pre_invoke_cs (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_INVOKE_CS:
      return new be_visitor_args_invoke_cs (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_POST_INVOKE_CS:
      return new be_visitor_args_post_invoke_cs (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
      return new be_visitor_args_vardecl_ss (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_UPCALL_SS:
      return new be_visitor_args_upcall_ss (new_ctx);

    case TAO_CodeGen::TAO_ARGUMENT_DEMARSHAL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_MARSHAL_SS:
      return new be_visitor_args_marshal_ss (new_ctx);

      // AMI next generation visitors.
    case TAO_CodeGen::TAO_AMI_INTERFACE_CH:
      return new be_visitor_ami_interface_ch (new_ctx);

    case TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CH:
      return new be_visitor_operation_ami_handler_reply_stub_operation_ch (new_ctx);

    case TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CS:
      return new be_compiled_visitor_operation_ami_handler_reply_stub_operation_cs (new_ctx);

    case TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CH:
      return new be_visitor_operation_ami_ch (new_ctx);

    case TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CS:
      return new be_compiled_visitor_operation_ami_cs (new_ctx);

    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS:
      return new be_visitor_operation_ami_exception_holder_operation_cs (new_ctx);

    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CH:
      return new be_visitor_valuetype_ami_exception_holder_ch (new_ctx);

    case TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CS:
      return new be_visitor_valuetype_ami_exception_holder_cs (new_ctx);

      /*      // AMH ResponseHandler class visitors
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
      return new be_visitor_amh_rh_interface_sh (new_ctx);
    case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SS:
      return new be_visitor_amh_rh_interface_ss (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_AMH_RH_SH:
      return new be_visitor_amh_rh_operation_sh (new_ctx);
    case TAO_CodeGen::TAO_OPERATION_AMH_RH_SS:
      return new be_visitor_amh_rh_operation_ss (new_ctx);
      */

    default:
      // cannot handle it; delegate it to the common visitor factory
      delete new_ctx;
      return TAO_COMMON_VISITOR_FACTORY::instance ()->make_visitor (ctx);
    }
}

