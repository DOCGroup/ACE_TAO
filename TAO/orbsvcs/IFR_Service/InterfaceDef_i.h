/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    InterfaceDef_i.h
//
// = DESCRIPTION
//    InterfaceDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_INTERFACEDEF_I_H
#define TAO_INTERFACEDEF_I_H

#include "Container_i.h"
#include "Contained_i.h"
#include "IDLType_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_InterfaceDef_i : public virtual TAO_Container_i,
                           public virtual TAO_Contained_i,
                           public virtual TAO_IDLType_i
{
  // = TITLE
  //    TAO_InterfaceDef_i
  //
  // = DESCRIPTION
  //    Represents an interface definition.
  //
public:
  TAO_InterfaceDef_i (TAO_Repository_i *repo,
                      ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_InterfaceDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Contained::Description *describe (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA_Contained::Description *describe_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA_InterfaceDefSeq *base_interfaces (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_InterfaceDefSeq *base_interfaces_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void base_interfaces (
      const CORBA_InterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void base_interfaces_i (
      const CORBA_InterfaceDefSeq &base_interfaces
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean is_a (
      const char *interface_id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Boolean is_a_i (
      const char *interface_id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_InterfaceDef::FullInterfaceDescription *describe_interface (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_InterfaceDef::FullInterfaceDescription *describe_interface_i (
      TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_AttributeDef_ptr create_attribute (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr type,
      CORBA::AttributeMode mode
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_AttributeDef_ptr create_attribute_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr type,
      CORBA::AttributeMode mode
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_OperationDef_ptr create_operation (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr result,
      CORBA::OperationMode mode,
      const CORBA_ParDescriptionSeq &params,
      const CORBA_ExceptionDefSeq &exceptions,
      const CORBA_ContextIdSeq &contexts
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA_OperationDef_ptr create_operation_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_IDLType_ptr result,
      CORBA::OperationMode mode,
      const CORBA_ParDescriptionSeq &params,
      const CORBA_ExceptionDefSeq &exceptions,
      const CORBA_ContextIdSeq &contexts
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void interface_contents (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Gathers the attributes and operations of all the ancestors.

private:
  void destroy_special (
      const char *sub_section
        TAO_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Removed the repo ids of our attributes and operations from
  // the flat repo ids section of the repository.

  void create_attr_ops (const char *id,
                        const char *name,
                        const char *version,
                        CORBA_IDLType_ptr type,
                        CORBA::AttributeMode mode
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Create set and/or get operations for an attribute.

  void base_interfaces_recursive (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue
    );
  // Depth-first traversal of the inheritance tree to get all
  // the base interfaces.

  void inherited_attributes (
      ACE_Unbounded_Queue<ACE_Configuration_Section_Key> &key_queue
    );
  // Depth-first traversal of the inheritance tree to get all the
  // attributes.

  void inherited_operations (
      ACE_Unbounded_Queue<ACE_Configuration_Section_Key> &key_queue
    );
  // Depth-first traversal of the inheritance tree to get all the
  // operations.

  CORBA::Boolean check_inherited_attrs (const char *name
                                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // When creating a new attribute, check for a clash with an inherited
  // attribute name.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERFACEDEF_I_H */

