/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    Container_i.h
//
// = DESCRIPTION
//    Container servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONTAINER_I_H
#define TAO_CONTAINER_I_H

#include "IRObject_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Repository_i;

class TAO_IFRService_Export TAO_Container_i : public virtual TAO_IRObject_i
{
  // = TITLE
  //    TAO_Container_i
  //
  // = DESCRIPTION
  //    Abstract base class used to form a containment hierarchy
  //    in the Interface Repository, by containing objects
  //    derived from the Contained interface.
  //
public:
//  template<typename T>
//  friend class TAO_Port_Utils;

  TAO_Container_i (TAO_Repository_i *repo);
  // Constructor.

  virtual ~TAO_Container_i (void);
  // Destructor.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove our contents.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Contained_ptr lookup (
      const char *search_name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Contained_ptr lookup_i (
      const char *search_name
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ContainedSeq *contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ContainedSeq *contents_i (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ContainedSeq *lookup_name (
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ContainedSeq *lookup_name_i (
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Container::DescriptionSeq *describe_contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Container::DescriptionSeq *describe_contents_i (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ModuleDef_ptr create_module (
      const char *id,
      const char *name,
      const char *version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ModuleDef_ptr create_module_i (
      const char *id,
      const char *name,
      const char *version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ConstantDef_ptr create_constant (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      const CORBA::Any & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ConstantDef_ptr create_constant_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr type,
      const CORBA::Any & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::StructDef_ptr create_struct (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::StructDef_ptr create_struct_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::UnionDef_ptr create_union (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr discriminator_type,
      const CORBA::UnionMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::UnionDef_ptr create_union_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr discriminator_type,
      const CORBA::UnionMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::EnumDef_ptr create_enum (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::EnumMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::EnumDef_ptr create_enum_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::EnumMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::AliasDef_ptr create_alias (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::AliasDef_ptr create_alias_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::InterfaceDef_ptr create_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::InterfaceDef_ptr create_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

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
      const CORBA::InitializerSeq &initializers
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

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
      const CORBA::InitializerSeq &initializers
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ValueBoxDef_ptr create_value_box (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type_def
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ValueBoxDef_ptr create_value_box_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::IDLType_ptr original_type_def
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ExceptionDef_ptr create_exception (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ExceptionDef_ptr create_exception_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::StructMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::NativeDef_ptr create_native (
      const char *id,
      const char *name,
      const char *version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::NativeDef_ptr create_native_i (
      const char *id,
      const char *name,
      const char *version
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::AbstractInterfaceDef_ptr create_abstract_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::AbstractInterfaceDefSeq &base_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::AbstractInterfaceDef_ptr create_abstract_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::AbstractInterfaceDefSeq &base_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::LocalInterfaceDef_ptr create_local_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::LocalInterfaceDef_ptr create_local_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::InterfaceDefSeq &base_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

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
      const CORBA::ExtInitializerSeq &initializers
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

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
      const CORBA::ExtInitializerSeq &initializers
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
    
  static int same_as_tmp_name (const char *name);
  // Called from TAO_IFR_Service_Utils::name_exisits.

  static void tmp_name_holder (const char *name);
  // Accesses the static member.

protected:
   void store_label (
      ACE_Configuration_Section_Key key,
      const CORBA::Any &value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Store a union member's label value.

  void lookup_attr (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited
    );

  void lookup_op (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited
    );

private:
  void lookup_name_recursive (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  void update_refs (
      const char *path,
      const char *name
    );
  // Used with structs, unions and exceptions.

  ACE_TString create_value_common (
      CORBA::DefinitionKind container_kind,
      ACE_Configuration_Section_Key container_key,
      ACE_Configuration_Section_Key new_key,
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA::ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA::ValueDefSeq &abstract_base_values,
      const CORBA::InterfaceDefSeq &supported_interfaces
      ACE_ENV_ARG_DECL
    );
  // Common code for create_value_i and create_ext_value_i.
  
protected:
  static const char *tmp_name_holder_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONTAINER_I_H */
