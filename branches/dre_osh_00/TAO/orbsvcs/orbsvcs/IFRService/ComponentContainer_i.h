/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    ComponentContainer_i.h
//
// = DESCRIPTION
//    ComponentContainer_i servant class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_COMPONENTCONTAINER_I_H
#define TAO_COMPONENTCONTAINER_I_H

#include "IRObject_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IFR_ComponentsS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_ComponentContainer_i 
  : public virtual TAO_IRObject_i
{
  // = TITLE
  //    TAO_ComponentContainer_i
  //
  // = DESCRIPTION
  //    Base class used to form a containment hierarchy
  //    for component-related types in the Interface Repository.
  //    Defines the relevant create_* methods.
  //
public:
  TAO_ComponentContainer_i (TAO_Repository_i *repo);
  // Constructor.

  virtual ~TAO_ComponentContainer_i (void);
  // Destructor.

  virtual CORBA::ComponentIR::ComponentDef_ptr create_component (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::ComponentDef_ptr base_component,
      const CORBA::InterfaceDefSeq &supports_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  CORBA::ComponentIR::ComponentDef_ptr create_component_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::ComponentDef_ptr base_component,
      const CORBA::InterfaceDefSeq &supports_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::ComponentIR::HomeDef_ptr create_home (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::HomeDef_ptr base_home,
      CORBA::ComponentIR::ComponentDef_ptr managed_component,
      const CORBA::InterfaceDefSeq &supports_interfaces,
      CORBA::ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  CORBA::ComponentIR::HomeDef_ptr create_home_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA::ComponentIR::HomeDef_ptr base_home,
      CORBA::ComponentIR::ComponentDef_ptr managed_component,
      const CORBA::InterfaceDefSeq &supports_interfaces,
      CORBA::ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
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
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  
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
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
    
  static int same_as_tmp_name (const char *name);
  // Called from TAO_IFR_Service_Utils::name_exisits.
    
protected:
  static const char *tmp_name_holder_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_COMPONENTCONTAINER_I_H */
