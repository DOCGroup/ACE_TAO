// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
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

#include "orbsvcs/IFRService/Container_i.h"
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

class TAO_IFRService_Export TAO_InterfaceDef_i
  : public virtual TAO_Container_i,
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
  TAO_InterfaceDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_InterfaceDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
    );
  // Return our definition kind.

  virtual void destroy (
    );

  virtual void destroy_i (
    );

  virtual CORBA::Contained::Description *describe (
    );
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i (
    );
  // From Contained_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type (
    );
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
    );
  // From IDLType_i's pure virtual function.

  virtual CORBA::InterfaceDefSeq *base_interfaces (
    );

  CORBA::InterfaceDefSeq *base_interfaces_i (
    );

  virtual void base_interfaces (
      const CORBA::InterfaceDefSeq &base_interfaces
    );

  void base_interfaces_i (
      const CORBA::InterfaceDefSeq &base_interfaces
    );

  virtual CORBA::Boolean is_a (
      const char *interface_id
    );

  CORBA::Boolean is_a_i (
      const char *interface_id
    );

  virtual CORBA::InterfaceDef::FullInterfaceDescription *describe_interface (
    );

  CORBA::InterfaceDef::FullInterfaceDescription *describe_interface_i (
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

  void interface_contents (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited
    );
  // Gathers the attributes and operations of all the ancestors.

  static int name_clash (const char *name);
  // Called from TAO_IFR_Service_Utils::name_exists() when we
  // are in a list of supported interfaces.

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

private:
  void base_interfaces_recursive (
      ACE_Unbounded_Queue<CORBA::DefinitionKind> &kind_queue,
      ACE_Unbounded_Queue<ACE_TString> &path_queue
    );
  // Depth-first traversal of the inheritance tree to get all
  // the base interfaces.

  void check_inherited (const char *name,
                        CORBA::DefinitionKind kind);
  // When creating a new attribute, check for a clash with an inherited
  // attribute name.
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERFACEDEF_I_H */
