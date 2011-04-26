// -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCodeFactory_Adapter.h
 *
 *  $Id$
 *
 *  @author  Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODEFACTORY_ADAPTER_H
#define TAO_TYPECODEFACTORY_ADAPTER_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_methods.h"
#include "tao/Typecode_typesC.h"
#include "tao/Pseudo_VarOut_T.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
template<class T> class ACE_Array_Base;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class EnumMemberSeq;
  class StructMemberSeq;
  class UnionMemberSeq;
  class ValueMemberSeq;

  typedef TAO_Pseudo_Var_T<TypeCode> TypeCode_var;
  typedef TAO_Pseudo_Out_T<TypeCode> TypeCode_out;

  typedef CORBA::Short ValueModifier;
}

namespace TAO
{
  namespace TypeCode
  {
    template<typename StringType, typename TypeCodeType> class Case;
    template<typename StringType, typename TypeCodeType> struct Struct_Field;
    template<typename StringType, typename TypeCodeType> struct Value_Field;
  }
}


/**
 * @class TAO_TypeCodeFactory_Adapter
 *
 * @brief TAO_TypeCodeFactory_Adapter.
 *
 * Class that adapts the CORBA::ORB create_*_tc functions
 * to use the TypeCodeFactory. This is a base class for
 * the actual implementation in the TypeCodeFactory_DLL library.
 */
class TAO_Export TAO_TypeCodeFactory_Adapter : public ACE_Service_Object
{
public:
  virtual ~TAO_TypeCodeFactory_Adapter (void);

  virtual CORBA::TypeCode_ptr create_struct_tc (
      const char *id,
      const char *name,
      const CORBA::StructMemberSeq &members) = 0;

  virtual CORBA::TypeCode_ptr create_union_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr discriminator_type,
      const CORBA::UnionMemberSeq &members) = 0;

  virtual CORBA::TypeCode_ptr create_enum_tc (
      const char *id,
      const char *name,
      const CORBA::EnumMemberSeq &members) = 0;

  virtual CORBA::TypeCode_ptr create_alias_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr original_type) = 0;

  virtual CORBA::TypeCode_ptr create_exception_tc (
      const char *id,
      const char *name,
      const CORBA::StructMemberSeq &members) = 0;

  virtual CORBA::TypeCode_ptr create_interface_tc (
      const char *id,
      const char *name) = 0;

  virtual CORBA::TypeCode_ptr create_string_tc (
      CORBA::ULong bound) = 0;

  virtual CORBA::TypeCode_ptr create_wstring_tc (
      CORBA::ULong bound) = 0;

  virtual CORBA::TypeCode_ptr create_fixed_tc (
      CORBA::UShort digits,
      CORBA::UShort scale) = 0;

  virtual CORBA::TypeCode_ptr create_sequence_tc (
      CORBA::ULong bound,
      CORBA::TypeCode_ptr element_type) = 0;

  virtual CORBA::TypeCode_ptr create_array_tc (
      CORBA::ULong length,
      CORBA::TypeCode_ptr element_type) = 0;

  virtual CORBA::TypeCode_ptr create_value_tc (
      const char *id,
      const char *name,
      CORBA::ValueModifier type_modifier,
      CORBA::TypeCode_ptr concrete_base,
      const CORBA::ValueMemberSeq &members) = 0;

  virtual CORBA::TypeCode_ptr create_value_box_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr boxed_type) = 0;

  virtual CORBA::TypeCode_ptr create_native_tc (
      const char *id,
      const char *name) = 0;

  virtual CORBA::TypeCode_ptr create_recursive_tc (const char *id) = 0;

  virtual CORBA::TypeCode_ptr create_abstract_interface_tc (
      const char *id,
      const char *name) = 0;

  virtual CORBA::TypeCode_ptr create_local_interface_tc (
      const char *id,
      const char *name) = 0;

  virtual CORBA::TypeCode_ptr create_component_tc (
      const char *id,
      const char *name) = 0;

  virtual CORBA::TypeCode_ptr create_home_tc (
      const char *id,
      const char *name) = 0;

  virtual CORBA::TypeCode_ptr create_event_tc (
      const char *id,
      const char *name,
      CORBA::ValueModifier type_modifier,
      CORBA::TypeCode_ptr concrete_base,
      const CORBA::ValueMemberSeq &members) = 0;

  /**
   * @name TAO-specific TypeCode factory methods.
   *
   * Factory methods that has no corresponding TypeCodeFactory IDL,
   * i.e. they are specific to TAO.
   */
  //@{
  /// Create an enumeration TypeCode.
  virtual CORBA::TypeCode_ptr create_enum_tc (
    char const * id,
    char const * name,
    ACE_Array_Base<CORBA::String_var> const & enumerators,
    CORBA::ULong ncases) = 0;

  /// Create a structure or exception TypeCode.
  virtual CORBA::TypeCode_ptr create_struct_except_tc (
    CORBA::TCKind,
    char const * id,
    char const * name,
    ACE_Array_Base<
      TAO::TypeCode::Struct_Field<CORBA::String_var,
                                  CORBA::TypeCode_var> > const & fields,
    CORBA::ULong nfields) = 0;

  /// Create a union TypeCode.
  virtual CORBA::TypeCode_ptr create_union_tc (
    char const * id,
    char const * name,
    CORBA::TypeCode_ptr discriminant_type,
    ACE_Array_Base<TAO::TypeCode::Case<CORBA::String_var,
                                       CORBA::TypeCode_var> > const & cases,
    CORBA::ULong ncases,
    CORBA::Long default_index,
    char const * default_case_name,
    CORBA::TypeCode_ptr default_case_type) = 0;

  /// Create a valuetype or eventtype TypeCode.
  virtual CORBA::TypeCode_ptr create_value_event_tc (
    CORBA::TCKind,
    char const * id,
    char const * name,
    CORBA::ValueModifier modifier,
    CORBA::TypeCode_ptr concrete_base,
    ACE_Array_Base<
      TAO::TypeCode::Value_Field<CORBA::String_var,
                                 CORBA::TypeCode_var> > const & fields,
    CORBA::ULong nfields) = 0;
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TYPECODEFACTORY_ADAPTER_H */
