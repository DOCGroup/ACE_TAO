/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
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

class TAO_Container_i : public virtual TAO_IRObject_i
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
  TAO_Container_i (TAO_Repository_i *repo,
                   ACE_Configuration_Section_Key section_key);
  // Constructor.

  virtual ~TAO_Container_i (void);
  // Destructor.

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove our contents.

  virtual void destroy_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Contained_ptr lookup (
      const char *search_name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_Contained_ptr lookup_i (
      const char *search_name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ContainedSeq *contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ContainedSeq *contents_i (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ContainedSeq *lookup_name (
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ContainedSeq *lookup_name_i (
      const char *search_name,
      CORBA::Long levels_to_search,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Container::DescriptionSeq *describe_contents (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_Container::DescriptionSeq *describe_contents_i (
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ModuleDef_ptr create_module (
      const char *id,
      const char *name,
      const char *version
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ModuleDef_ptr create_module_i (
      const char *id,
      const char *name,
      const char *version
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ConstantDef_ptr create_constant (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr type,
      const CORBA::Any & value
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ConstantDef_ptr create_constant_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr type,
      const CORBA::Any & value
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_StructDef_ptr create_struct (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_StructMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_StructDef_ptr create_struct_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_StructMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_UnionDef_ptr create_union (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr discriminator_type,
      const CORBA_UnionMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_UnionDef_ptr create_union_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr discriminator_type,
      const CORBA_UnionMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_EnumDef_ptr create_enum (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_EnumMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_EnumDef_ptr create_enum_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_EnumMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_AliasDef_ptr create_alias (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr original_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_AliasDef_ptr create_alias_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr original_type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_InterfaceDef_ptr create_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_InterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_InterfaceDef_ptr create_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_InterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ValueDef_ptr create_value (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA_ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA_ValueDefSeq &abstract_base_values,
      const CORBA_InterfaceDefSeq & supported_interfaces,
      const CORBA_InitializerSeq &initializers
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ValueDef_ptr create_value_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      CORBA_ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const CORBA_ValueDefSeq &abstract_base_values,
      const CORBA_InterfaceDefSeq & supported_interfaces,
      const CORBA_InitializerSeq &initializers
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ValueBoxDef_ptr create_value_box (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr original_type_def
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ValueBoxDef_ptr create_value_box_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr original_type_def
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_ExceptionDef_ptr create_exception (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_StructMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_ExceptionDef_ptr create_exception_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_StructMemberSeq &members
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_NativeDef_ptr create_native (
      const char *id,
      const char *name,
      const char *version
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_NativeDef_ptr create_native_i (
      const char *id,
      const char *name,
      const char *version
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_AbstractInterfaceDef_ptr create_abstract_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_AbstractInterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_AbstractInterfaceDef_ptr create_abstract_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_AbstractInterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_LocalInterfaceDef_ptr create_local_interface (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_InterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_LocalInterfaceDef_ptr create_local_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_InterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Boolean name_exists (
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Checks for local existence of <name>.

protected:
   void store_label (
      ACE_Configuration_Section_Key key,
      const CORBA::Any &value
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Store a union member's label value.

  CORBA::Boolean pre_exist (
      const char *id,
      const char *name
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Bundles id_exists and name_exists together.

  ACE_TString create_common (
      ACE_Configuration_Section_Key sub_key,
      ACE_Configuration_Section_Key &new_key,
      const char *id,
      const char *name,
      const char *version,
      const char *sub_section,
      CORBA::DefinitionKind def_kind
    );
  // Code common to all the create_* methods.

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
      TAO_ENV_ARG_DECL_WITH_DEFAULTS);


  CORBA::Boolean id_exists (
      const char *id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Checks for global existence of the repo id.

  CORBA::Boolean valid_container (
      const CORBA::DefinitionKind op_kind
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Is this operation valid for this container type?

  void update_refs (
      const char *path,
      const char *name
    );
  // Used with structs, unions and exceptions.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CONTAINER_I_H */
