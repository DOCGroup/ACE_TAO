/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    ComponentRepository_i.h
//
// = DESCRIPTION
//    ComponentRepository servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_COMPONENTREPOSITORY_I_H
#define TAO_COMPONENTREPOSITORY_I_H

#include "Repository_i.h"

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

class TAO_ComponentRepository_i : public TAO_Repository_i
{
  // = TITLE
  //    TAO_ComponentRepository_i
  //
  // = DESCRIPTION
  //    Provides global access to the Interface Repository,
  //    including access to information related to
  //    CORBA Components.
  //
public:
  TAO_ComponentRepository_i (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa,
                             ACE_Configuration *config);
  // Constructor.

  virtual ~TAO_ComponentRepository_i (void);
  // Destructor.

  virtual IR::ComponentDef_ptr create_component (
      const char *id,
      const char *name,
      const char *version,
      IR::ComponentDef_ptr base_component,
      const CORBA_InterfaceDefSeq & supports_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::ComponentDef_ptr create_component_i (
      const char *id,
      const char *name,
      const char *version,
      IR::ComponentDef_ptr base_component,
      const CORBA_InterfaceDefSeq & supports_interfaces
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::HomeDef_ptr create_home (
      const char *id,
      const char *name,
      const char *version,
      IR::HomeDef_ptr base_home,
      IR::ComponentDef_ptr managed_component,
      CORBA_ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::HomeDef_ptr create_home_i (
      const char *id,
      const char *name,
      const char *version,
      IR::HomeDef_ptr base_home,
      IR::ComponentDef_ptr managed_component,
      CORBA_ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_COMPONENTREPOSITORY_I_H */
