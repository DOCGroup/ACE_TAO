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
    : public virtual CORBA::TypeCodeFactory,
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
  /// ctor
  TAO_TypeCodeFactory_i (void);

  /// dtor
  ~TAO_TypeCodeFactory_i (void);

  /// = LocalObject methods
  static TAO_TypeCodeFactory_i *_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual CORBA::TypeCode_ptr create_struct_tc (
      const char *id,
      const char *name,
      const CORBA::StructMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_union_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr discriminator_type,
      const CORBA::UnionMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_enum_tc (
      const char *id,
      const char *name,
      const CORBA::EnumMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_alias_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr original_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_exception_tc (
      const char *id,
      const char *name,
      const CORBA::StructMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_interface_tc (
      const char *id,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_string_tc (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_wstring_tc (
      CORBA::ULong bound
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_fixed_tc (
      CORBA::UShort digits,
      CORBA::UShort scale
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_sequence_tc (
      CORBA::ULong bound,
      CORBA::TypeCode_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_array_tc (
      CORBA::ULong length,
      CORBA::TypeCode_ptr element_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_value_tc (
      const char *id,
      const char *name,
      CORBA::ValueModifier type_modifier,
      CORBA::TypeCode_ptr concrete_base,
      const CORBA::ValueMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_value_box_tc (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr boxed_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_native_tc (
      const char *id,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_recursive_tc (
      const char *id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_abstract_interface_tc (
      const char *id,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_local_interface_tc (
      const char *id,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_component_tc (
      const char *id,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_home_tc (
      const char *id,
      const char *name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::TypeCode_ptr create_event_tc (
      const char *id,
      const char *name,
      CORBA::ValueModifier type_modifier,
      CORBA::TypeCode_ptr concrete_base,
      const CORBA::ValueMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Finds a legal default label value.
  void compute_default_label (CORBA::TCKind kind,
                              CORBA::ULong skip_slot,
                              const CORBA::UnionMemberSeq &members,
                              TAO_OutputCDR &cdr);

  /// Insert a non-default label value into the typecode being created.
  CORBA::Boolean insert_label_value (CORBA::TCKind kind,
                                     const CORBA::Any &any,
                                     TAO_OutputCDR &cdr
                                     ACE_ENV_ARG_DECL);

  /// Called for all types that take just an id and a name.
  CORBA::TypeCode_ptr create_tc_common (
      const char *id,
      const char *name,
      CORBA::TCKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Code for strings and wstrings is identical except for TCKind.
  CORBA::TypeCode_ptr string_wstring_tc_common (
      CORBA::ULong bound,
      CORBA::TCKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Code for arrays and sequences is identical except for TCKind.
  CORBA::TypeCode_ptr sequence_array_tc_common (
      CORBA::ULong bound,
      CORBA::TypeCode_ptr element_type,
      CORBA::TCKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Code for structs and unions is identical except for TCKind.
  CORBA::TypeCode_ptr struct_except_tc_common (
      const char *id,
      const char *name,
      const CORBA::StructMemberSeq &members,
      CORBA::TCKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Code for aliases and boxed valuetypes is identical except for TCKind.
  CORBA::TypeCode_ptr alias_value_box_tc_common (
      const char *id,
      const char *name,
      CORBA::TypeCode_ptr underlying_type,
      CORBA::TCKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::TypeCode_ptr value_event_tc_common (
      const char *id,
      const char *name,
      CORBA::ValueModifier type_modifier,
      CORBA::TypeCode_ptr concrete_base,
      const CORBA::ValueMemberSeq &members,
      CORBA::TCKind kind
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// If any of these fail, we raise a spec-defined minor code
  /// of BAD_PARAM or BAD_TYPECODE.
  CORBA::Boolean valid_name (const char *name);
  CORBA::Boolean valid_id (const char *id);
  CORBA::Boolean valid_content_type (CORBA::TypeCode_ptr tc
                                     ACE_ENV_ARG_DECL);
  CORBA::Boolean unique_label_values (const CORBA::UnionMemberSeq &members,
                                      CORBA::TypeCode_ptr disc_tc,
                                      CORBA::ULong default_index_slot
                                      ACE_ENV_ARG_DECL);
  CORBA::Boolean valid_disc_type (CORBA::TypeCode_ptr tc
                                  ACE_ENV_ARG_DECL);

  /// Common code for allocating and doing the final assembly of a typecode.
  CORBA::TypeCode_ptr assemble_tc (TAO_OutputCDR &cdr,
                                   CORBA::TCKind kind,
                                   CORBA::TypeCode::OFFSET_MAP *map
                                   ACE_ENV_ARG_DECL);

  /// Update the offset map we will pass to the typecode we are constructing.
  void update_map (CORBA::TypeCode::OFFSET_MAP *&offset_map,
                   CORBA::TypeCode_ptr member_tc,
                   const char *id,
                   TAO_OutputCDR &cdr);

  /// Prohibited
  TAO_TypeCodeFactory_i (const TAO_TypeCodeFactory_i &src);
  TAO_TypeCodeFactory_i &operator= (const TAO_TypeCodeFactory_i &src);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_TYPECODEFACTORY_I_H */
