// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    TypeCodeFactory_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_TYPECODEFACTORY_I_H
#define TAO_TYPECODEFACTORY_I_H

#include "ace/pre.h"
#include "TypeCodeFactory_Loader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_TypeCodeFactory_Export TAO_TypeCodeFactory_i
    : public virtual CORBA_TypeCodeFactory,
      public virtual TAO_Local_RefCounted_Object
{
/**
 * @class TAO_TypeCodeFactory_i
 *
 * @brief TAO_TypeCodeFactory_i.
 *
 * Implementation of the CORBA::TypeCodeFactory interface
 */
public:
  TAO_TypeCodeFactory_i (void);
  /// ctor

  ~TAO_TypeCodeFactory_i (void);
  /// dtor

  /// = LocalObject methods
  static TAO_TypeCodeFactory_i *_narrow (
      CORBA::Object_ptr obj
      TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual CORBA::TypeCode_ptr create_struct_tc (
      const char *id,
      const char *name,
      const CORBA_StructMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_union_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr discriminator_type,
      const CORBA_UnionMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_enum_tc (
      const char *id,
      const char *name,
      const CORBA_EnumMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_alias_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr original_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_exception_tc (
      const char *id,
      const char *name,
      const CORBA_StructMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_interface_tc (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_string_tc (
      CORBA::ULong bound
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_wstring_tc (
      CORBA::ULong bound
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_fixed_tc (
      CORBA::UShort digits,
      CORBA::UShort scale
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_sequence_tc (
      CORBA::ULong bound,
      CORBA::TypeCode_ptr element_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_array_tc (
      CORBA::ULong length,
      CORBA::TypeCode_ptr element_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_value_tc (
      const char *id,
      const char *name,
      CORBA::ValueModifier type_modifier,
      CORBA::TypeCode_ptr concrete_base,
      const CORBA_ValueMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_value_box_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr boxed_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_native_tc (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_recursive_tc (
      const char *id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_abstract_interface_tc (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_local_interface_tc (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_component_tc (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_home_tc (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Finds a legal default label value.
  void compute_default_label (CORBA::TCKind kind,
                              CORBA::ULong skip_slot,
                              const CORBA_UnionMemberSeq &members,
                              TAO_OutputCDR &cdr);

  /// Called for all types that take just an id and a name.
  CORBA::TypeCode_ptr create_tc_common (
      const char *id,
      const char *name,
      CORBA::TCKind kind
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Code for structs and unions is identical except for TCKind.
  CORBA::TypeCode_ptr struct_except_tc_common (
      const char *id,
      const char *name,
      const CORBA_StructMemberSeq &members,
      CORBA::TCKind kind
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  /// If any of these fail, we raise a spec-defined minor code
  /// of BAD_PARAM or BAD_TYPECODE.
  CORBA::Boolean valid_name (const char *name);
  CORBA::Boolean valid_id (const char *id);
  CORBA::Boolean valid_content_type (CORBA::TypeCode_ptr tc
                                     TAO_ENV_ARG_DECL);
  CORBA::Boolean unique_label_values (const CORBA_UnionMemberSeq &members,
                                      CORBA::TypeCode_ptr disc_tc,
                                      CORBA::ULong default_index_slot
                                      TAO_ENV_ARG_DECL);
  CORBA::Boolean valid_disc_type (CORBA::TypeCode_ptr tc
                                  TAO_ENV_ARG_DECL);

  /// A string in a TAO typecode contains padding to give it a
  /// total size which is a multiple of 4 bytes.
  void string_pad (TAO_OutputCDR &cdr,
                   CORBA::ULong slen);

  /// Prohibited
  TAO_TypeCodeFactory_i (const TAO_TypeCodeFactory_i &src);
  TAO_TypeCodeFactory_i &operator= (const TAO_TypeCodeFactory_i &src);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_TYPECODEFACTORY_I_H */
