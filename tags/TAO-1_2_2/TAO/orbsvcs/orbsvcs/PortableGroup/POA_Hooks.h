// -*- C++ -*-

//=============================================================================
/**
 *  @file   POA_Hooks.h
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_POA_HOOKS_H
#define TAO_POA_HOOKS_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "portablegroup_export.h"
#include "tao/PortableServer/PortableGroup_Hooks.h"
#include "orbsvcs/PortableGroupC.h"

// Forward references
class PortableGroup_Request_Dispatcher;
class TAO_PortableGroup_Loader;
class TAO_Profile;
class TAO_PortableGroup_Acceptor_Registry;

/**
 * @class TAO_POA_Hooks
 *
 * @brief TAO_POA_Hooks
 *
 * This class acts as a facade for all of the POA specific methods in the
 * PortableGroup library.
 */
class TAO_PortableGroup_Export TAO_POA_Hooks :
  public TAO_POA_PortableGroup_Hooks
{
public:
  TAO_POA_Hooks (PortableGroup_Request_Dispatcher &request_dispatcher);

  virtual ~TAO_POA_Hooks (void);

  virtual PortableServer::ObjectId * create_id_for_reference (
      TAO_POA &the_poa,
      CORBA::Object_ptr the_ref
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual PortableServer::IDs * reference_to_ids (
      TAO_POA &the_poa,
      CORBA::Object_ptr the_ref
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual void associate_reference_with_id (
      TAO_POA &the_poa,
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

  virtual void disassociate_reference_with_id (
      TAO_POA &the_poa,
      CORBA::Object_ptr ref,
      const PortableServer::ObjectId & oid
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::NotAGroupObject
    ));

protected:

  int find_group_component (const CORBA::Object_ptr the_ref,
                            PortableGroup::TagGroupTaggedComponent &group);

  int find_group_component_in_profile (const TAO_Profile* profile,
                                       PortableGroup::TagGroupTaggedComponent &group);

  int create_group_acceptors (CORBA::Object_ptr the_ref,
                              TAO_PortableGroup_Acceptor_Registry &acceptor_registry,
                              TAO_ORB_Core &orb_core
                              ACE_ENV_ARG_DECL);

  /// Helper function to associate group references with
  /// object references.
  void associate_group_with_ref (
        TAO_POA &the_poa,
        CORBA::Object_ptr group_ref,
        CORBA::Object_ptr obj_ref
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::NotAGroupObject));


private:
  PortableGroup_Request_Dispatcher &request_dispatcher_;
};


#include "ace/post.h"
#endif /* TAO_POA_HOOKS_H */
