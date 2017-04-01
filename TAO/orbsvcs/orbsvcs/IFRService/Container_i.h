// -*- C++ -*-


//=============================================================================
/**
 *  @file    Container_i.h
 *
 *  Container servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CONTAINER_I_H
#define TAO_CONTAINER_I_H

#include "orbsvcs/IFRService/IRObject_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Repository_i;

/**
 * @class TAO_Container_i
 *
 * @brief TAO_Container_i
 *
 * Abstract base class used to form a containment hierarchy
 * in the Interface Repository, by containing objects
 * derived from the Contained interface.
 */
class TAO_IFRService_Export TAO_Container_i : public virtual TAO_IRObject_i
{
public:
//  template<typename T>
//  friend class TAO_Port_Utils;

  /// Constructor.
  TAO_Container_i (TAO_Repository_i *repo);

  /// Destructor.
  virtual ~TAO_Container_i (void);

  /// Remove our contents.
  virtual void destroy ();

  virtual void destroy_i ();

  virtual void destroy_references_i ();

  virtual void destroy_definitions_i ();

  virtual CORBA::Contained_ptr lookup (
      const char *search_name);

  CORBA::Contained_ptr lookup_i (
      const char *search_name);

  virtual CORBA::ContainedSeq *contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited);

  CORBA::ContainedSeq *contents_i (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited);

  virtual CORBA::ContainedSeq *lookup_name (
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited);

  CORBA::ContainedSeq *lookup_name_i (
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited);

  virtual CORBA::Container::DescriptionSeq *describe_contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs);

  CORBA::Container::DescriptionSeq *describe_contents_i (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs);

  virtual CORBA::ModuleDef_ptr create_module (
      const char *id,
      const char *name,
      const char *version);

  CORBA::ModuleDef_ptr create_module_i (
      const char *id,
      const char *name,
      const char *version);

  virtual CORBA::ConstantDef_ptr create_constant (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      const CORBA::Any & value);

  CORBA::ConstantDef_ptr create_constant_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      const CORBA::Any & value);

  virtual CORBA::StructDef_ptr create_struct (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members);

  CORBA::StructDef_ptr create_struct_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members);

  virtual CORBA::UnionDef_ptr create_union (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr discriminator_type,
      const CORBA::UnionMemberSeq &members);

  CORBA::UnionDef_ptr create_union_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr discriminator_type,
      const CORBA::UnionMemberSeq &members);

  virtual CORBA::EnumDef_ptr create_enum (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::EnumMemberSeq &members);

  CORBA::EnumDef_ptr create_enum_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::EnumMemberSeq &members);

  virtual CORBA::AliasDef_ptr create_alias (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type);

  CORBA::AliasDef_ptr create_alias_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type);

  virtual CORBA::InterfaceDef_ptr create_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces);

  CORBA::InterfaceDef_ptr create_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces);

  virtual CORBA::ValueDef_ptr create_value (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq & supported_interfaces,
      const CORBA::InitializerSeq &initializers);

  CORBA::ValueDef_ptr create_value_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq & supported_interfaces,
      const CORBA::InitializerSeq &initializers);

  virtual CORBA::ValueBoxDef_ptr create_value_box (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type_def);

  CORBA::ValueBoxDef_ptr create_value_box_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type_def);

  virtual CORBA::ExceptionDef_ptr create_exception (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members);

  CORBA::ExceptionDef_ptr create_exception_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members);

  virtual CORBA::NativeDef_ptr create_native (
      const char *id,
      const char *name,
      const char *version);

  CORBA::NativeDef_ptr create_native_i (
      const char *id,
      const char *name,
      const char *version);

  virtual CORBA::AbstractInterfaceDef_ptr create_abstract_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::AbstractInterfaceDefSeq &base_interfaces);

  CORBA::AbstractInterfaceDef_ptr create_abstract_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::AbstractInterfaceDefSeq &base_interfaces);

  virtual CORBA::LocalInterfaceDef_ptr create_local_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces);

  CORBA::LocalInterfaceDef_ptr create_local_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces);

  virtual ::CORBA::ExtValueDef_ptr create_ext_value (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq &supported_interfaces,
      const CORBA::ExtInitializerSeq &initializers);

  CORBA::ExtValueDef_ptr create_ext_value_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq &supported_interfaces,
      const CORBA::ExtInitializerSeq &initializers);

  /// Called from TAO_IFR_Service_Utils::name_exists.
  static int same_as_tmp_name (const char *name);

  /// Accesses the static member.
  static void tmp_name_holder (const char *name);

  /// Used with structs, unions and exceptions.
  void update_refs (
      const char *path,
      const char *name = 0
    );

protected:
  /// Store a union member's label value.
   void store_label (
      ACE_Configuration_Section_Key key,
      const CORBA::Any &value);

  void lookup_attr (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited);

  void lookup_op (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited);

private:
  void lookup_name_recursive (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited);

  /// Common code for create_value_i and create_ext_value_i.
  ACE_TString create_value_common (
      CORBA::DefinitionKind container_kind,
      ACE_Configuration_Section_Key &container_key,
      ACE_Configuration_Section_Key &new_key,
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq &supported_interfaces);

protected:
  static const char *tmp_name_holder_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONTAINER_I_H */
