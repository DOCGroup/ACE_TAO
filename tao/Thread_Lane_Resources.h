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

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread_Mutex.h"
#include /**/ "tao/TAO_Export.h"
#include "tao/params.h"
#include "tao/Transport_Cache_Manager.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Allocator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Acceptor_Registry;
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
  TAO_Thread_Lane_Resources (
      TAO_ORB_Core &orb_core,
      TAO_New_Leader_Generator *new_leader_generator = 0);

  /// Destructor.
  ~TAO_Thread_Lane_Resources (void);

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Open the acceptor registry.
  int open_acceptor_registry (const TAO_EndpointSet &endpoint_set,
                              bool ignore_address);

  /// Finalize resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// Certain ORB policies such as dropping replies on shutdown
  /// would need cleanup of transports to wake threads up.
  void close_all_transports (void);

  /// @name Accessors
  //@{

  TAO_Acceptor_Registry &acceptor_registry (void);

  /**
   * @note Returning a pointer helps to return 0 in case of
   * exceptions.
   */
  TAO_Connector_Registry *connector_registry (void);

  /// Get the transport cache
  TAO::Transport_Cache_Manager &transport_cache (void);

  TAO_Leader_Follower &leader_follower (void);

  /**
   * Allocator is intended for allocating the ACE_Data_Blocks used in
   * incoming CDR streams.  This allocator has locks.
   */
  ACE_Allocator *input_cdr_dblock_allocator (void);

  /**
   * Allocator is intended for allocating the buffers in the incoming
   * CDR streams.  This allocator has locks.
   */
  ACE_Allocator *input_cdr_buffer_allocator (void);

  /**
   * Allocator is intended for allocating the ACE_Message_Blocks used
   * in incoming CDR streams.  This allocator is global, and has locks.
   */
  ACE_Allocator *input_cdr_msgblock_allocator (void);

  /**
   * Allocator is intended for allocating the buffers used in the
   * Transport object. This allocator has locks.
   */
  ACE_Allocator *transport_message_buffer_allocator (void);

  /**
   * Allocator is intended for allocating the ACE_Data_Blocks used in
   * outgoing CDR streams.  This allocator has locks.
   */
  ACE_Allocator *output_cdr_dblock_allocator (void);

  /**
   * Allocator is intended for allocating the buffers in the outgoing
   * CDR streams.  This allocator has locks.
   */
  ACE_Allocator *output_cdr_buffer_allocator (void);

  /**
   * Allocator is intended for allocating the ACE_Message_Blocks used
   * in the outgoing CDR streams.  This allocator is global, and has
   * locks.
   */
  ACE_Allocator *output_cdr_msgblock_allocator (void);

  /**
   * Allocator is intended for allocating the AMH response handlers
   * This allocator is global.
   */
  ACE_Allocator *amh_response_handler_allocator (void);

  /**
   * Allocator is intended for allocating the AMI response handlers
   * This allocator is global.
   */
  ACE_Allocator *ami_response_handler_allocator (void);
  //@}

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
  TAO::Transport_Cache_Manager *transport_cache_;

  /// The leader/followers management class for this lane.
  TAO_Leader_Follower *leader_follower_;

  /// Synchronization.
  TAO_SYNCH_MUTEX lock_;

  /// Generator of new leader threads.
  TAO_New_Leader_Generator *new_leader_generator_;

  /// @name The allocators for the input CDR streams.
  //@{
  ACE_Allocator *input_cdr_dblock_allocator_;
  ACE_Allocator *input_cdr_buffer_allocator_;
  ACE_Allocator *input_cdr_msgblock_allocator_;
  //@}

  /// @name The allocators for the buffering messages in the transport.
  //@{
  ACE_Allocator *transport_message_buffer_allocator_;
  //@}

  /// @name The allocators for the output CDR streams.
  //@{
  ACE_Allocator *output_cdr_dblock_allocator_;
  ACE_Allocator *output_cdr_buffer_allocator_;
  ACE_Allocator *output_cdr_msgblock_allocator_;
  //@}

  /// @name The allocators for AMH.
  //@{
  ACE_Allocator *amh_response_handler_allocator_;
  //@}

  /// @name The allocators for AMI.
  //@{
  ACE_Allocator *ami_response_handler_allocator_;
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_THREAD_LANE_RESOURCES_H */
