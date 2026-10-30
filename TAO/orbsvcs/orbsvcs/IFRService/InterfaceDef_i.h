// -*- C++ -*-


//=============================================================================
/**
 *  @file    InterfaceDef_i.h
 *
 *  InterfaceDef servant class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


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
  /// Constructor
  TAO_InterfaceDef_i (TAO_Repository_i *repo);

  /// Destructor
  ~TAO_InterfaceDef_i () override;

  /// Return our definition kind.
  CORBA::DefinitionKind def_kind () override;

  void destroy () override;

  void destroy_i () override;

  /// From Contained_i's pure virtual function.
  CORBA::Contained::Description *describe () override;

  /// From Contained_i's pure virtual function.
  CORBA::Contained::Description *describe_i () override;

  /// From IDLType_i's pure virtual function.
  CORBA::TypeCode_ptr type () override;

  /// From IDLType_i's pure virtual function.
  CORBA::TypeCode_ptr type_i () override;

  virtual CORBA::InterfaceDefSeq *base_interfaces ();

  CORBA::InterfaceDefSeq *base_interfaces_i ();

  virtual void base_interfaces (CORBA::InterfaceDefSeq const &base_interfaces);

  void base_interfaces_i (CORBA::InterfaceDefSeq const &base_interfaces);

  virtual CORBA::Boolean is_a (char const *interface_id);

  CORBA::Boolean is_a_i (char const *interface_id);

  virtual CORBA::InterfaceDef::FullInterfaceDescription *describe_interface ();

  CORBA::InterfaceDef::FullInterfaceDescription *describe_interface_i ();

  virtual CORBA::AttributeDef_ptr create_attribute (
      char const *id,
      char const *name,
      char const *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode);

  CORBA::AttributeDef_ptr create_attribute_i (
      char const *id,
      char const *name,
      char const *version,
      CORBA::IDLType_ptr type,
      CORBA::AttributeMode mode);

  virtual CORBA::OperationDef_ptr create_operation (
      char const *id,
      char const *name,
      char const *version,
      CORBA::IDLType_ptr result,
      CORBA::OperationMode mode,
      CORBA::ParDescriptionSeq const &params,
      CORBA::ExceptionDefSeq const &exceptions,
      CORBA::ContextIdSeq const &contexts);

  CORBA::OperationDef_ptr create_operation_i (
      char const *id,
      char const *name,
      char const *version,
      CORBA::IDLType_ptr result,
      CORBA::OperationMode mode,
      CORBA::ParDescriptionSeq const &params,
      CORBA::ExceptionDefSeq const &exceptions,
      CORBA::ContextIdSeq const &contexts);

  /// Gathers the attributes and operations of all the ancestors.
  void interface_contents (
      std::deque<CORBA::DefinitionKind> &kind_queue,
      std::deque<ACE_TString> &path_queue,
      CORBA::DefinitionKind limit_type,
      CORBA::Boolean exclude_inherited);

  /// Called from TAO_IFR_Service_Utils::name_exists() when we
  /// are in a list of supported interfaces.
  static int name_clash (char const *name);

  /// Depth-first traversal of the inheritance tree to get all the
  /// attributes.
  void inherited_attributes (
      std::deque<ACE_Configuration_Section_Key> &key_queue);

  /// Depth-first traversal of the inheritance tree to get all the
  /// operations.
  void inherited_operations (
      std::deque<ACE_Configuration_Section_Key> &key_queue);

private:
  /// Depth-first traversal of the inheritance tree to get all
  /// the base interfaces.
  void base_interfaces_recursive (
      std::deque<CORBA::DefinitionKind> &kind_queue,
      std::deque<ACE_TString> &path_queue);

  /// When creating a new attribute, check for a clash with an inherited
  /// attribute name.
  void check_inherited (char const *name, CORBA::DefinitionKind kind);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERFACEDEF_I_H */
