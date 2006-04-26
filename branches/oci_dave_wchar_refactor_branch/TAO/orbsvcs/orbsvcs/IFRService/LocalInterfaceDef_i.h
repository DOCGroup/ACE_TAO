// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    LocalInterfaceDef_i.h
//
// = DESCRIPTION
//    LocalInterfaceDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_LOCALINTERFACEDEF_I_H
#define TAO_LOCALINTERFACEDEF_I_H

#include "orbsvcs/IFRService/InterfaceDef_i.h"
#include "orbsvcs/IFRService/ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IFRService_Export TAO_LocalInterfaceDef_i : public virtual TAO_InterfaceDef_i
{
  // = TITLE
  //    TAO_LocalInterfaceDef_i
  //
  // = DESCRIPTION
  //    Represents an local interface definition.
  //
public:
  TAO_LocalInterfaceDef_i (TAO_Repository_i *repo);
  // Constructor

  virtual ~TAO_LocalInterfaceDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual CORBA::Boolean is_a (
      const char *interface_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Boolean is_a_i (
      const char *interface_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((CORBA::SystemException));
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_LOCALINTERFACEDEF_I_H */
