/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PortableGroup_Loader.h
//
// = AUTHOR
//    Frank Hunleth <fhunleth@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_PORTABLEGROUP_LOADER_H
#define TAO_PORTABLEGROUP_LOADER_H
#include "ace/pre.h"

#include "portablegroup_export.h"
#include "tao/PortableGroup_Adapter.h"
#include "ace/Service_Config.h"

#include "POA_Hooks.h"
#include "Portable_Group_Map.h"
#include "PortableGroup_Acceptor_Registry.h"
#include "PortableGroup_Request_Dispatcher.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_PortableGroup_Adapter
 *
 * @brief TAO_PortableGroup_Adapter.
 *
 * This class acts as a facade for the PortableGroup library to the
 * ORB.
 */
class TAO_PortableGroup_Export TAO_PortableGroup_Loader : public TAO_PortableGroup_Adapter
{
  friend class TAO_POA_Hooks;

public:
  /// Constructor.
  TAO_PortableGroup_Loader (void);

  /// Activate the PortableGroup library
  virtual void activate (CORBA::ORB_ptr,
                         int argc,
                         char *argv [],
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Creates a Miop object and returns it.
  virtual CORBA::Object_ptr miop (CORBA::Environment &)
    ACE_THROW_SPEC (());

  /// Retrieve the hooks for interfacing with the POA.
  virtual TAO_POA_PortableGroup_Hooks *poa_hooks (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);

  /// Get a reference to the Group Id map.
  TAO_Portable_Group_Map &group_map (void);

protected:
  /// The MIOP object factory instance.
  CORBA::Object_var miop_;

  /// Hash map containing the GroupId->ObjectKey mappings.
  TAO_Portable_Group_Map group_map_;

  /// Registry for all of the group (multicast) acceptors.
  TAO_PortableGroup_Acceptor_Registry acceptor_registry_;

  /// Hooks for the POA.
  TAO_POA_Hooks *poa_hooks_;

  /// Strategy for dispatching group requests.
  PortableGroup_Request_Dispatcher request_dispatcher_;
};

ACE_STATIC_SVC_DECLARE (TAO_PortableGroup_Loader)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_PortableGroup_Loader)

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_PortableGroup_Initializer = &TAO_PortableGroup_Loader::Initializer;

#else

static int
TAO_Requires_PortableGroup_Initializer = TAO_PortableGroup_Loader::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

//#define TAO_PORTABLEGROUP_SAFE_INCLUDE
//#include "IORC.h"
//#undef TAO_IORMANIP_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_PORTABLEGROUP_LOADER_H */
