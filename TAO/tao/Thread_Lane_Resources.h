//=============================================================================
/**
 *  @file    Thread_Lane_Resources.h
 *
 *  $Id$
 *
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_THREAD_LANE_RESOURCES_H
#define TAO_THREAD_LANE_RESOURCES_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

class TAO_ORB_Core;
class TAO_Acceptor_Registry;
class TAO_Transport_Cache_Manager;
class TAO_Leader_Follower;

/**
 * @class TAO_Thread_Lane_Resources
 *
 * @brief Class representing a thread lane's resources.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_Thread_Lane_Resources
{
public:
  /// Constructor.
  TAO_Thread_Lane_Resources (TAO_ORB_Core &orb_core);

  /// Destructor.
  ~TAO_Thread_Lane_Resources (void);

  /// Checks if the acceptor registry has been created.
  int has_acceptor_registry_been_created (void) const;

  /// Opens the acceptor registry.
  int open_acceptor_registry (CORBA::Environment &ACE_TRY_ENV);

  /// Finalize resources.
  void finalize (void);

  /// Get the acceptor registry.
  TAO_Acceptor_Registry &acceptor_registry (void);

  /// Get the transport cache.
  TAO_Transport_Cache_Manager &transport_cache (void);

  /// Get the leader/followers management class for this lane.
  TAO_Leader_Follower &leader_follower (void);

private:
  /// ORB_Core related to this thread lane.
  TAO_ORB_Core &orb_core_;

  /// The registry which maintains a list of acceptor factories for
  /// each loaded protocol.
  TAO_Acceptor_Registry *acceptor_registry_;

  /// Transport cache.
  TAO_Transport_Cache_Manager *transport_cache_;

  /// The leader/followers management class for this lane.
  TAO_Leader_Follower *leader_follower_;

  /// Synchronization.
  TAO_SYNCH_MUTEX lock_;
};

/**
 * @class TAO_Thread_Lane_Resources_Manager
 *
 * This class is a manager for thread resources.
 **/
class TAO_Export TAO_Thread_Lane_Resources_Manager
  : public ACE_Service_Object
{
public:
  virtual ~TAO_Thread_Lane_Resources_Manager (void);

  virtual int initialize (TAO_ORB_Core &orb_core) = 0;
  virtual void finalize (void) = 0;

  virtual int open (CORBA_Environment &ACE_TRY_ENV) = 0;

  virtual TAO_Thread_Lane_Resources &lane_resources (void) = 0;
};

/**
 * @class TAO_Default_Thread_Lane_Resources_Manager
 *
 * @brief Simple manager for thread lane resources.
 *
 * \nosubgrouping
 *
 **/
class TAO_Export TAO_Default_Thread_Lane_Resources_Manager :
  public TAO_Thread_Lane_Resources_Manager
{
public:
  TAO_Default_Thread_Lane_Resources_Manager (void);
  ~TAO_Default_Thread_Lane_Resources_Manager (void);

  int initialize (TAO_ORB_Core &orb_core);
  void finalize (void);

  int open (CORBA_Environment &ACE_TRY_ENV);

  TAO_Thread_Lane_Resources &lane_resources (void);

  /// Mutual exclusion for calling open.
  TAO_SYNCH_MUTEX open_lock_;

  /// Flag which denotes that the open method was called.
  int open_called_;

  TAO_Thread_Lane_Resources *lane_resources_;

  /// ORB_Core related to this thread lane.
  TAO_ORB_Core *orb_core_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_Default_Thread_Lane_Resources_Manager)
ACE_FACTORY_DECLARE (TAO, TAO_Default_Thread_Lane_Resources_Manager)

#if defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_THREAD_LANE_RESOURCES_H */
