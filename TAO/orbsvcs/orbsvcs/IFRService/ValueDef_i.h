// -*- C++ -*-


//=============================================================================
/**
 *  @file    ValueDef_i.h
 *
 *  $Id$
 *
 *  ValueDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_VALUEDEF_I_H
#define TAO_VALUEDEF_I_H

#include "orbsvcs/IFRService/Contained_i.h"
#include "orbsvcs/IFRService/IDLType_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_ValueDef_i : public virtual TAO_Container_i,
                                             public virtual TAO_Contained_i,
                                             public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_ValueDef_i
  //
  // = DESCRIPTION
  //    Represents a valuetype definition.
  //
public:
  /// Constructor
  TAO_ValueDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ValueDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );

  /// Remove the repository entry.
  virtual void destroy (
    );

  virtual void destroy_i (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe_i (
    );

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type (
    );

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type_i (
    );

  virtual CORBA::InterfaceDefSeq *supported_interfaces (
    );

  CORBA::InterfaceDefSeq *supported_interfaces_i (
    );

  virtual void supported_interfaces (
      const CORBA::InterfaceDefSeq & supported_interfaces
    );

  void supported_interfaces_i (
      const CORBA::InterfaceDefSeq & supported_interfaces
    );

  virtual CORBA::InitializerSeq *initializers (
    );

  CORBA::InitializerSeq *initializers_i (
    );

  virtual void initializers (
      const CORBA::InitializerSeq & initializers
    );

  void initializers_i (
      const CORBA::InitializerSeq & initializers
    );

  virtual CORBA::ValueDef_ptr base_value (
    );

  CORBA::ValueDef_ptr base_value_i (
    );

  virtual void base_value (
      CORBA::ValueDef_ptr base_value
    );

  void base_value_i (
      CORBA::ValueDef_ptr base_value
    );

  virtual CORBA::ValueDefSeq *abstract_base_values (
    );

  CORBA::ValueDefSeq *abstract_base_values_i (
    );

  virtual void abstract_base_values (
      const CORBA::ValueDefSeq & abstract_base_values
    );

  void abstract_base_values_i (
      const CORBA::ValueDefSeq & abstract_base_values
    );

  virtual CORBA::Boolean is_abstract (
    );

  CORBA::Boolean is_abstract_i (
    );

  virtual void is_abstract (
      CORBA::Boolean is_abstract
    );

  void is_abstract_i (
      CORBA::Boolean is_abstract
    );

  virtual CORBA::Boolean is_custom (
    );

  CORBA::Boolean is_custom_i (
    );

  virtual void is_custom (
      CORBA::Boolean is_custom
    );

  void is_custom_i (
      CORBA::Boolean is_custom
    );

  virtual CORBA::Boolean is_truncatable (
    );

  CORBA::Boolean is_truncatable_i (
    );

  virtual void is_truncatable (
      CORBA::Boolean is_truncatable
    );

  void is_truncatable_i (
      CORBA::Boolean is_truncatable
    );

  virtual CORBA::Boolean is_a (
      const char *id
    );

  CORBA::Boolean is_a_i (
      const char *id
    );

  virtual CORBA::ValueDef::FullValueDescription *describe_value (
    );

  CORBA::ValueDef::FullValueDescription *describe_value_i (
    );

  virtual CORBA::ValueMemberDef_ptr create_value_member (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::Visibility access
    );

  CORBA::ValueMemberDef_ptr create_value_member_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::Visibility access
    );

  virtual CORBA::AttributeDef_ptr create_attribute (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode
    );

  CORBA::AttributeDef_ptr create_attribute_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode
    );

  virtual CORBA::OperationDef_ptr create_operation (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr result,
      CORBA::OperationMode mode,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions,
      const CORBA::ContextIdSeq &contexts
    );

  CORBA::OperationDef_ptr create_operation_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr result,
      CORBA::OperationMode mode,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions,
      const CORBA::ContextIdSeq &contexts
    );

  /// Called from TAO_IFR_Service_Utils::name_exists() when we
  /// are a base valuetype.
  static int name_clash (const char *name);

  void fill_value_description (CORBA::ValueDescription &desc);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_VALUEDEF_I_H */
