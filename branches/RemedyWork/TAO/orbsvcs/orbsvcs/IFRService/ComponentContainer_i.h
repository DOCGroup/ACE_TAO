// -*- C++ -*-


//=============================================================================
/**
 *  @file    ComponentContainer_i.h
 *
 *  $Id$
 *
 *  ComponentContainer_i servant class.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_COMPONENTCONTAINER_I_H
#define TAO_COMPONENTCONTAINER_I_H

#include "orbsvcs/IFRService/Container_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IFRService/IFR_ComponentsS.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * Base class used to form a containment hierarchy
 * for component-related types in the Interface Repository.
 * Defines the relevant create_* methods.
 */
class TAO_IFRService_Export TAO_ComponentContainer_i
  : public virtual TAO_Container_i
{
public:
  TAO_ComponentContainer_i (TAO_Repository_i *repo);

  virtual ~TAO_ComponentContainer_i (void);

  virtual CORBA::ComponentIR::ComponentDef_ptr create_component (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::ComponentDef_ptr base_component,
      const CORBA::InterfaceDefSeq &supports_interfaces
    );

  CORBA::ComponentIR::ComponentDef_ptr create_component_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::ComponentDef_ptr base_component,
      const CORBA::InterfaceDefSeq &supports_interfaces
    );

  virtual CORBA::ComponentIR::HomeDef_ptr create_home (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::HomeDef_ptr base_home,
      CORBA::ComponentIR::ComponentDef_ptr managed_component,
      const CORBA::InterfaceDefSeq &supports_interfaces,
      CORBA::ValueDef_ptr primary_key
    );

  CORBA::ComponentIR::HomeDef_ptr create_home_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::HomeDef_ptr base_home,
      CORBA::ComponentIR::ComponentDef_ptr managed_component,
      const CORBA::InterfaceDefSeq &supports_interfaces,
      CORBA::ValueDef_ptr primary_key
    );

  virtual CORBA::ComponentIR::EventDef_ptr create_event (
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
    );

  CORBA::ComponentIR::EventDef_ptr create_event_i (
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
    );

  /// Called from TAO_IFR_Service_Utils::name_exisits.
  static int same_as_tmp_name (const char *name);

protected:
  static const char *tmp_name_holder_;
};

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_COMPONENTCONTAINER_I_H */
