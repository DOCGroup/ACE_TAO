// -*- C++ -*-

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

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "ace/Synch_T.h"
#include "ace/CORBA_macros.h"

class ACE_Allocator;

class TAO_ORB_Core;
class TAO_Acceptor_Registry;
class TAO_Transport_Cache_Manager;
class TAO_Leader_Follower;
class TAO_MProfile;
class TAO_New_Leader_Generator;
class TAO_Connector_Registry;
class TAO_Resource_Factory;

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
  TAO_Thread_Lane_Resources (TAO_ORB_Core &orb_core,
                             TAO_New_Leader_Generator *new_leader_generator = 0);

  /// Destructor.
  ~TAO_Thread_Lane_Resources (void);

  // Does <mprofile> belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Open the acceptor registry.
  int open_acceptor_registry (int ignore_address
                              ACE_ENV_ARG_DECL);

  /// Finalize resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// @name Accessors
  // @{

  TAO_Acceptor_Registry &acceptor_registry (void);

  /*
   * @@ NOTE: Returning a pointer helps to return 0 in case of
   * exceptions.
   */
  TAO_Connector_Registry *connector_registry (ACE_ENV_SINGLE_ARG_DECL);

  TAO_Transport_Cache_Manager &transport_cache (void);

  TAO_Leader_Follower &leader_follower (void);

  /* Allocator is intended for allocating the ACE_Data_Blocks used in
   * incoming CDR streams.  This allocator has locks.
   */
  ACE_Allocator *input_cdr_dblock_allocator (void);

  /* Allocator is intended for allocating the buffers in the incoming
   * CDR streams.  This allocator has locks.
   */
  ACE_Allocator *input_cdr_buffer_allocator (void);

  /* Allocator is intended for allocating the ACE_Message_Blocks used
   * in incoming CDR streams.  This allocator is global, and has locks.
   */
  ACE_Allocator *input_cdr_msgblock_allocator (void);

  /* Allocator is intended for allocating the buffers used in the
   * Transport object. This allocator has locks.
   */
  ACE_Allocator *transport_message_buffer_allocator (void);
  // @}

private:

  /// Checks if the acceptor registry has been created.
  int has_acceptor_registry_been_created (void) const;

  /// Helper to get the resource factory in the ORB_Core
  TAO_Resource_Factory *resource_factory (void);

private:
  /// ORB_Core related to this thread lane.
  TAO_ORB_Core &orb_core_;

  /// The registry which maintains a list of acceptor factories for
  /// each loaded protocol.
  TAO_Acceptor_Registry *acceptor_registry_;

  /// The connector registry which all active connectors must register
  /// themselves with.
  TAO_Connector_Registry *connector_registry_;

  /// Transport cache.
  TAO_Transport_Cache_Manager *transport_cache_;

  /// The leader/followers management class for this lane.
  TAO_Leader_Follower *leader_follower_;

  /// Synchronization.
  TAO_SYNCH_MUTEX lock_;

  /// Generator of new leader threads.
  TAO_New_Leader_Generator *new_leader_generator_;

  /// The allocators for the input CDR streams.
  //@{
  ACE_Allocator *input_cdr_dblock_allocator_;
  ACE_Allocator *input_cdr_buffer_allocator_;
  ACE_Allocator *input_cdr_msgblock_allocator_;
  //@}

  /// The allocators for the buffering messages in the transport.
  //@{
  ACE_Allocator *transport_message_buffer_allocator_;
  //@}
};

#if defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_THREAD_LANE_RESOURCES_H */
