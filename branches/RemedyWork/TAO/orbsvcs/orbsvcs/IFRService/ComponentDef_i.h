// -*- C++ -*-


//=============================================================================
/**
 *  @file    ComponentDef_i.h
 *
 *  $Id$
 *
 *  ComponentDef servant class.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_COMPONENTDEF_I_H
#define TAO_COMPONENTDEF_I_H

#include "orbsvcs/IFRService/ExtInterfaceDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IFRService/IFR_ComponentsS.h"
#include "orbsvcs/IFRService/IFR_Service_Utils_T.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * Represents a component definition.
 */
class TAO_IFRService_Export TAO_ComponentDef_i
  : public virtual TAO_ExtInterfaceDef_i
{
public:
  /// Constructor
  TAO_ComponentDef_i (TAO_Repository_i *repo);

  /// Destructor
  virtual ~TAO_ComponentDef_i (void);

  /// Return our definition kind.
  virtual CORBA::DefinitionKind def_kind (
    );

  /// Remove the repository entry.
  virtual void destroy (
    );

  /// Remove the repository entry.
  void destroy_i (
    );

  /// From Contained_i's pure virtual function.
  virtual CORBA::Contained::Description *describe (
    );

  /// From Contained_i's pure virtual function.
  CORBA::Contained::Description *describe_i (
    );

  /// From IDLType_i's pure virtual function.
  virtual CORBA::TypeCode_ptr type (
    );

  /// From IDLType_i's pure virtual function.
  CORBA::TypeCode_ptr type_i (
    );

  virtual CORBA::InterfaceDefSeq *supported_interfaces (
    );

  CORBA::InterfaceDefSeq *supported_interfaces_i (
    );

  virtual void supported_interfaces (
      const CORBA::InterfaceDefSeq &supported_interfaces
    );

  void supported_interfaces_i (
      const CORBA::InterfaceDefSeq &supported_interfaces
    );

  virtual CORBA::ComponentIR::ComponentDef_ptr base_component (
    );

  CORBA::ComponentIR::ComponentDef_ptr base_component_i (
    );

  virtual void base_component (
      CORBA::ComponentIR::ComponentDef_ptr base_component
    );

  void base_component_i (
      CORBA::ComponentIR::ComponentDef_ptr base_component
    );

  virtual CORBA::ComponentIR::ProvidesDef_ptr create_provides (
      const char *id,
      const char *name,
      const char *version,
      CORBA::InterfaceDef_ptr interface_type
    );

  CORBA::ComponentIR::ProvidesDef_ptr create_provides_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::InterfaceDef_ptr interface_type
    );

  virtual CORBA::ComponentIR::UsesDef_ptr create_uses (
      const char *id,
      const char *name,
      const char *version,
      CORBA::InterfaceDef_ptr interface_type,
      CORBA::Boolean is_multiple
    );

  CORBA::ComponentIR::UsesDef_ptr create_uses_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::InterfaceDef_ptr interface_type,
      CORBA::Boolean is_multiple
    );

  virtual CORBA::ComponentIR::EmitsDef_ptr create_emits (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ValueDef_ptr value
    );

  CORBA::ComponentIR::EmitsDef_ptr create_emits_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ValueDef_ptr value
    );

  virtual CORBA::ComponentIR::PublishesDef_ptr create_publishes (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ValueDef_ptr value
    );

  CORBA::ComponentIR::PublishesDef_ptr create_publishes_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ValueDef_ptr value
    );

  virtual CORBA::ComponentIR::ConsumesDef_ptr create_consumes (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ValueDef_ptr value
    );

  CORBA::ComponentIR::ConsumesDef_ptr create_consumes_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ValueDef_ptr value
    );

  /// Called from TAO_IFR_Service_Utils::name_exists() when we
  /// are a base component.
  static int name_clash (const char *name);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_COMPONENTDEF_I_H */



