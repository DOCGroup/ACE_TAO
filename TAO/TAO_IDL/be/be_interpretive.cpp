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

#include "be_visitor_root.h"
#include "be_visitor_sequence.h"

TAO_Interpretive_Visitor_Factory::TAO_Interpretive_Visitor_Factory (void)
{
}

// destructor
TAO_Interpretive_Visitor_Factory::~TAO_Interpretive_Visitor_Factory (void)
{
}

be_visitor *
TAO_Interpretive_Visitor_Factory::make_visitor (TAO_CodeGen::CG_STATE st)
{
  switch (st)
    {
    case TAO_CodeGen::TAO_ROOT_CH:
      return TAO_BE_VISITOR_ROOT_CH::instance ();
    case TAO_CodeGen::TAO_ROOT_CI:
      return TAO_BE_VISITOR_ROOT_CI::instance ();
    case TAO_CodeGen::TAO_ROOT_CS:
      return TAO_BE_VISITOR_ROOT_CS::instance ();
    case TAO_CodeGen::TAO_ROOT_SH:
      return TAO_BE_VISITOR_ROOT_SH::instance ();
    case TAO_CodeGen::TAO_ROOT_SI:
      return TAO_BE_VISITOR_ROOT_SI::instance ();
    case TAO_CodeGen::TAO_ROOT_SS:
      return TAO_BE_VISITOR_ROOT_SS::instance ();
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
      return TAO_BE_VISITOR_SEQUENCE_CH::instance ();
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH:
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
    case TAO_CodeGen::TAO_STRUCT_CH:
    case TAO_CodeGen::TAO_STRUCT_CS:
    case TAO_CodeGen::TAO_STRUCT_CI:
    case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CH:
    case TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CI:
    case TAO_CodeGen::TAO_UNION_PUBLIC_CH:
    case TAO_CodeGen::TAO_UNION_PUBLIC_CI:
    case TAO_CodeGen::TAO_UNION_PUBLIC_CS:
    case TAO_CodeGen::TAO_UNION_PUBLIC_ASSIGN_CS:
    case TAO_CodeGen::TAO_UNION_PRIVATE_CH:
    case TAO_CodeGen::TAO_OPERATION_CH:
    case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
    case TAO_CodeGen::TAO_OPERATION_SH:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
    case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
    case TAO_CodeGen::TAO_ARGUMENT_CH:
    case TAO_CodeGen::TAO_ARGUMENT_CS:
    case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
    case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
    case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
    case TAO_CodeGen::TAO_ARGUMENT_SH:
    case TAO_CodeGen::TAO_ARGUMENT_SS:
    case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
    case TAO_CodeGen::TAO_TYPEDEF_CH:
    case TAO_CodeGen::TAO_TYPEDEF_CS:
    case TAO_CodeGen::TAO_TYPEDEF_CI:
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
    case TAO_CodeGen::TAO_EXCEPTION_CH:
    case TAO_CodeGen::TAO_EXCEPTION_CTOR_CH:
    case TAO_CodeGen::TAO_EXCEPTION_CS:
    case TAO_CodeGen::TAO_EXCEPTION_CTOR_CS:
    case TAO_CodeGen::TAO_EXCEPTION_CTOR_ASSIGN_CS:
    case TAO_CodeGen::TAO_EXCEPTION_CI:
    default:
      return 0;
    }
}
