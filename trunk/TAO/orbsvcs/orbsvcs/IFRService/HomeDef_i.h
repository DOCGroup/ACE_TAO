/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    HomeDef_i.h
//
// = DESCRIPTION
//    HomeDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_HOMEDEF_I_H
#define TAO_HOMEDEF_I_H

#include "ExtInterfaceDef_i.h"
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

class TAO_IFRService_Export TAO_HomeDef_i 
  : public virtual TAO_ExtInterfaceDef_i
{
  // = TITLE
  //    HomTAO_HomeDef_ieDef_i
  //
  // = DESCRIPTION
  //    Represents a home definition, containing factory and finder.
  //
public:
  TAO_HomeDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_HomeDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::ComponentIR::HomeDef_ptr base_home (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ComponentIR::HomeDef_ptr base_home_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void base_home (
      CORBA::ComponentIR::HomeDef_ptr base_home
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  void base_home_i (
      CORBA::ComponentIR::HomeDef_ptr base_home
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  virtual CORBA::InterfaceDefSeq *supported_interfaces (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  CORBA::InterfaceDefSeq *supported_interfaces_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  virtual void supported_interfaces (
      const CORBA::InterfaceDefSeq &supported_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  void supported_interfaces_i (
      const CORBA::InterfaceDefSeq &supported_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  virtual CORBA::ComponentIR::ComponentDef_ptr managed_component (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ComponentIR::ComponentDef_ptr managed_component_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void managed_component (
      CORBA::ComponentIR::ComponentDef_ptr managed_component
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  void managed_component_i (
      CORBA::ComponentIR::ComponentDef_ptr managed_component
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  virtual CORBA::ValueDef_ptr primary_key (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ValueDef_ptr primary_key_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void primary_key (
      CORBA::ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  void primary_key_i (
      CORBA::ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
      
  virtual CORBA::ComponentIR::FactoryDef_ptr create_factory (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ComponentIR::FactoryDef_ptr create_factory_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ComponentIR::FinderDef_ptr create_finder (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ComponentIR::FinderDef_ptr create_finder_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA::ParDescriptionSeq &params,
      const CORBA::ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HOMEDEF_I_H */




