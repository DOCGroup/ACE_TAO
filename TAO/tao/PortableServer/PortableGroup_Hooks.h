/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   PortableGroup_Hooks.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLEGROUP_HOOKS_H
#define TAO_PORTABLEGROUP_HOOKS_H
#include "ace/pre.h"

#include "PortableServerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "POA.h"


/**
 * @class TAO_POA_PortableGroup_Hooks
 *
 * @brief TAO_POA_PortableGroup_Hooks
 *
 * This class is an abstract base class that contains hooks for the
 * PortableGroup library.
 */
class TAO_PortableServer_Export TAO_POA_PortableGroup_Hooks
{
public:
  /// Destructor.
  virtual ~TAO_POA_PortableGroup_Hooks (void);

  virtual PortableServer::ObjectId * create_id_for_reference (
      TAO_POA &the_poa,
      CORBA::Object_ptr the_ref
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    )) = 0;

  virtual PortableServer::IDs * reference_to_ids (
      TAO_POA &the_poa,
      CORBA::Object_ptr the_ref
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    )) = 0;

  virtual void associate_reference_with_id (
      TAO_POA &the_poa,
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    )) = 0;

  virtual void disassociate_reference_with_id (
      TAO_POA &the_poa,
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    )) = 0;
};


#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_HOOKS_H */
