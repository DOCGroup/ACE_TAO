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
//    IR_Container servant class.
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
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove our contents.

  virtual void destroy_i (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_Contained_ptr lookup (
      const char *search_name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_Contained_ptr lookup_i (
      const char *search_name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ContainedSeq *contents (
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ContainedSeq *contents_i (
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ContainedSeq *lookup_name (
      const char *search_name,
      CORBA::Long levels_to_search,
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ContainedSeq *lookup_name_i (
      const char *search_name,
      CORBA::Long levels_to_search,
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_Container::DescriptionSeq *describe_contents (
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_Container::DescriptionSeq *describe_contents_i (
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Long max_returned_objs,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ModuleDef_ptr create_module (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ModuleDef_ptr create_module_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ConstantDef_ptr create_constant (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr type,
      const CORBA::Any & value,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ConstantDef_ptr create_constant_i (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr type,
      const CORBA::Any & value,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_StructDef_ptr create_struct (
      const char *id,
      const char *name,
      const char *version,
      const IR_StructMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_StructDef_ptr create_struct_i (
      const char *id,
      const char *name,
      const char *version,
      const IR_StructMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_UnionDef_ptr create_union (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr discriminator_type,
      const IR_UnionMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_UnionDef_ptr create_union_i (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr discriminator_type,
      const IR_UnionMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_EnumDef_ptr create_enum (
      const char *id,
      const char *name,
      const char *version,
      const IR_EnumMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_EnumDef_ptr create_enum_i (
      const char *id,
      const char *name,
      const char *version,
      const IR_EnumMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_AliasDef_ptr create_alias (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr original_type,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_AliasDef_ptr create_alias_i (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr original_type,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_InterfaceDef_ptr create_interface (
      const char *id,
      const char *name,
      const char *version,
      const IR_InterfaceDefSeq &base_interfaces,
      CORBA::Boolean is_abstract,
      CORBA::Boolean is_local,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_InterfaceDef_ptr create_interface_i (
      const char *id,
      const char *name,
      const char *version,
      const IR_InterfaceDefSeq &base_interfaces,
      CORBA::Boolean is_abstract,
      CORBA::Boolean is_local,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ValueDef_ptr create_value (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      IR_ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const IR_ValueDefSeq &abstract_base_values,
      const IR_InterfaceDefSeq & supported_interfaces,
      const IR_InitializerSeq &initializers,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ValueDef_ptr create_value_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Boolean is_custom,
      CORBA::Boolean is_abstract,
      IR_ValueDef_ptr base_value,
      CORBA::Boolean is_truncatable,
      const IR_ValueDefSeq &abstract_base_values,
      const IR_InterfaceDefSeq & supported_interfaces,
      const IR_InitializerSeq &initializers,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ValueBoxDef_ptr create_value_box (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr original_type_def,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ValueBoxDef_ptr create_value_box_i (
      const char *id,
      const char *name,
      const char *version,
      IR_IDLType_ptr original_type_def,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_ExceptionDef_ptr create_exception (
      const char *id,
      const char *name,
      const char *version,
      const IR_StructMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_ExceptionDef_ptr create_exception_i (
      const char *id,
      const char *name,
      const char *version,
      const IR_StructMemberSeq &members,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR_NativeDef_ptr create_native (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  IR_NativeDef_ptr create_native_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Boolean name_exists (
      const char *name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Checks for local existence of <name>.
                  
protected:
   void store_label (
      ACE_Configuration_Section_Key key,
      const CORBA::Any &value,                           
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Store a union member's label value.

  CORBA::Boolean pre_exist (
      const char *id,
      const char *name,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Bundles id_exists and name_exists together.

  ACE_TString create_common (
      ACE_Configuration_Section_Key sub_key,
      ACE_Configuration_Section_Key &new_key,
      const char *id,
      const char *name,
      const char *version,
      const char *sub_section,
      IR_DefinitionKind def_kind
    );
  // Code common to all the create_* methods.

protected:
  void lookup_attr (
      ACE_Unbounded_Queue<IR_DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited
    );

  void lookup_op (
      ACE_Unbounded_Queue<IR_DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Boolean exclude_inherited
    );

private:
  void lookup_name_recursive (
      ACE_Unbounded_Queue<IR_DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      const char *search_name,
      CORBA::Long levels_to_search,
      IR_DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    );

  CORBA::Boolean id_exists (
      const char *id,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Checks for global existence of the repo id.

  CORBA::Boolean valid_container (
      const IR_DefinitionKind op_kind,
      CORBA::Environment &ACE_TRY_ENV = 
        TAO_default_environment ()
    )
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
