/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    PortableGroup
//
// = FILENAME
//    POA_Hooks.h
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_POA_HOOKS_H
#define TAO_POA_HOOKS_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "portablegroup_export.h"
#include "tao/PortableServer/PortableGroup_Hooks.h"
#include "PortableGroupC.h"

// Forward references
class TAO_PortableGroup_Loader;
class TAO_PortableGroup_Acceptor_Registry;
class TAO_Profile;

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
  /// Constructor
  TAO_POA_Hooks (TAO_PortableGroup_Loader *portable_group_adapter);

  virtual ~TAO_POA_Hooks (void);

  virtual PortableServer::ObjectId *create_id_for_reference (
    TAO_POA &the_poa,
    CORBA::Object_ptr the_ref,
    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::POA::WrongAdapter
    ));

  virtual PortableServer::IDs *reference_to_ids (
    TAO_POA &the_poa,
    CORBA::Object_ptr the_ref,
    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableServer::POA::WrongAdapter
    ));

protected:

  int find_group_component (const CORBA::Object_ptr the_ref,
                            PortableGroup::TagGroupTaggedComponent &group);

  int find_group_component_in_profile (const TAO_Profile* profile,
                                       PortableGroup::TagGroupTaggedComponent &group);

  int create_group_acceptors (CORBA::Object_ptr the_ref,
                              TAO_PortableGroup_Acceptor_Registry &acceptor_registry,
                              TAO_ORB_Core &orb_core,
                              CORBA::Environment &ACE_TRY_ENV);


protected:
  /// Back pointer to the PortableGroup.
  TAO_PortableGroup_Loader *portable_group_adapter_;
};


#include "ace/post.h"
#endif /* TAO_POA_HOOKS_H */
