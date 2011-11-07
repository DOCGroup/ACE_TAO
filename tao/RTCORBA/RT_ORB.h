/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RT_ORB.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RT_ORB_H
#define TAO_RT_ORB_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTCORBA/RTCORBA_includeC.h"
#include "tao/RTCORBA/RT_ORBInitializer.h"
#include "tao/LocalObject.h"
#include "ace/Hash_Map_Manager_T.h"

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
# include "ace/Null_Mutex.h"
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward reference for TAO_Named_RT_Mutex_Manager
class TAO_RT_Mutex;
class TAO_Thread_Pool_Manager;

/**
 * @class TAO_Named_RT_Mutex_Manager
 *
 * @brief Manages the names of named and unnamed RT Mutexes
 *
 */
class TAO_RTCORBA_Export TAO_Named_RT_Mutex_Manager
{

public:
  /// Constructor.
  TAO_Named_RT_Mutex_Manager (void);

  /// Destructor.
  ~TAO_Named_RT_Mutex_Manager (void);

  RTCORBA::Mutex_ptr create_mutex (void);

  void destroy_mutex (RTCORBA::Mutex_ptr the_mutex);

  RTCORBA::Mutex_ptr create_named_mutex (const char *name,
                                         CORBA::Boolean_out created_flag);

  RTCORBA::Mutex_ptr open_named_mutex (const char * name);

private:

#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
  /// Hash map for named RT Mutexes
  ACE_Hash_Map_Manager_Ex<
    ACE_CString,
    RTCORBA::Mutex_var,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> map_;

  TAO_SYNCH_MUTEX lock_;
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */
};


/**
 * @class TAO_RT_ORB
 *
 * @brief RTCORBA::RTORB implementation
 *
 * Creates and destroys RT CORBA objects, i.e., policies,
 * threadpools, mutexes.
 */
class TAO_RTCORBA_Export TAO_RT_ORB
  : public RTCORBA::RTORB,
    public ::CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_RT_ORB (TAO_ORB_Core *orb_core,
              TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan,
              ACE_Time_Value const &dynamic_thread_time);

  /**
   * Create a new mutex.  Mutexes returned by this method
   * are the same as those used internally by the ORB, so that
   * consistant priority inheritance/piority ceiling semantics
   * can be guaranteed.
   */
  virtual RTCORBA::Mutex_ptr create_mutex (void);

  /**
   * Destroy a mutex.  Currently this is a no-op since RTCORBA::Mutex
   * instances are destroyed as soon as their reference counts go to
   * 0.
   */
  virtual void destroy_mutex (RTCORBA::Mutex_ptr the_mutex);

  /**
   * Create a mutex and assign a name to it.  If the
   * mutex already exists, it is returned and the created_flag
   * is set to 0.
   */
  virtual RTCORBA::Mutex_ptr create_named_mutex (const char *name,
                                                 CORBA::Boolean_out created_flag);

  /// Retrieve a previously created mutex.
  virtual RTCORBA::Mutex_ptr open_named_mutex (const char * name);

  /**
   * Create and return a TCPProtocolProperties instance with the specified
   * parameters.
   */
  RTCORBA::TCPProtocolProperties_ptr
  create_tcp_protocol_properties (CORBA::Long send_buffer_size,
                                  CORBA::Long recv_buffer_size,
                                  CORBA::Boolean keep_alive,
                                  CORBA::Boolean dont_route,
                                  CORBA::Boolean no_delay,
                                  CORBA::Boolean enable_network_priority);

  RTCORBA::UnixDomainProtocolProperties_ptr
  create_unix_domain_protocol_properties (CORBA::Long send_buffer_size,
                                          CORBA::Long recv_buffer_size);

  RTCORBA::SharedMemoryProtocolProperties_ptr
  create_shared_memory_protocol_properties (CORBA::Long send_buffer_size,
                                            CORBA::Long recv_buffer_size,
                                            CORBA::Boolean keep_alive,
                                            CORBA::Boolean dont_route,
                                            CORBA::Boolean no_delay,
                                            CORBA::Long preallocate_buffer_size,
                                            const char *mmap_filename,
                                            const char *mmap_lockname);

  RTCORBA::UserDatagramProtocolProperties_ptr
  create_user_datagram_protocol_properties (CORBA::Long send_buffer_size,
                                            CORBA::Long recv_buffer_size,
                                            CORBA::Boolean enable_network_priority);

  RTCORBA::StreamControlProtocolProperties_ptr
  create_stream_control_protocol_properties (CORBA::Long send_buffer_size,
                                             CORBA::Long recv_buffer_size,
                                             CORBA::Boolean keep_alive,
                                             CORBA::Boolean dont_route,
                                             CORBA::Boolean no_delay,
                                             CORBA::Boolean enable_network_priority);

  /// Create a RTCORBA threadpool to manage a set of threads without lanes.
  virtual RTCORBA::ThreadpoolId
  create_threadpool (CORBA::ULong stacksize,
                     CORBA::ULong static_threads,
                     CORBA::ULong dynamic_threads,
                     RTCORBA::Priority default_priority,
                     CORBA::Boolean allow_request_buffering,
                     CORBA::ULong max_buffered_requests,
                     CORBA::ULong max_request_buffer_size);

  /**
   * Create a threadpool and separate it into subsets based on
   * priorities.
   */
  virtual RTCORBA::ThreadpoolId
  create_threadpool_with_lanes (CORBA::ULong stacksize,
                                const RTCORBA::ThreadpoolLanes & lanes,
                                CORBA::Boolean allow_borrowing,
                                CORBA::Boolean allow_request_buffering,
                                CORBA::ULong max_buffered_requests,
                                CORBA::ULong max_request_buffer_size);

  /// Free the resources associated with the specified threadpool.
  virtual void destroy_threadpool (RTCORBA::ThreadpoolId threadpool);

  /// Create a priority model policy for use when configuring a POA.
  virtual RTCORBA::PriorityModelPolicy_ptr
  create_priority_model_policy (RTCORBA::PriorityModel priority_model,
                                RTCORBA::Priority server_priority);

  /// Create a ThreadpoolPolicy instance for POA creation
  virtual RTCORBA::ThreadpoolPolicy_ptr
  create_threadpool_policy (RTCORBA::ThreadpoolId threadpool);

  /**
   * Create a PriorityBandedConnectionPolicy instance
   * for use on either the client or server side
   */
  virtual RTCORBA::PriorityBandedConnectionPolicy_ptr
  create_priority_banded_connection_policy (const RTCORBA::PriorityBands &
                                            priority_bands);

  /**
   * Create a PrivateConnectionPolicy instance to use on client
   * to request a private (non-multiplexed) transport connection
   * to the server.
   */
  virtual RTCORBA::PrivateConnectionPolicy_ptr
  create_private_connection_policy (void);

  /**
   * Create a ServerProtocolPolicy instance to select and configure
   * communication protocols on the server side.
   */
  virtual RTCORBA::ServerProtocolPolicy_ptr
  create_server_protocol_policy (const RTCORBA::ProtocolList & protocols);

  /**
   * Create a ClientProtocolPolicy instance to select and configure
   * communication protocols on the client side.
   */
  virtual RTCORBA::ClientProtocolPolicy_ptr
  create_client_protocol_policy (const RTCORBA::ProtocolList & protocols);

  /// Reference to our creating ORB Core.
  TAO_ORB_Core *orb_core (void) const;

  /// Get the Thread Pool Manager.
  TAO_Thread_Pool_Manager &tp_manager (void) const;

  /**
   * This method changes the scheduling policy of the calling thread
   * to match the scheduling policy specified in the svc.conf file.
   * The priority of the calling thread will be set to the minimum
   * priority supported by that scheduling policy.
   *
   * This method make sense on those platform (e.g., Linux) where
   * PTHREAD_SCOPE_SYSTEM is the only scheduling scope supported.  On
   * other platforms, this method is a no-op since the only way to get
   * the real-time threading behavior is to setup the
   * PTHREAD_SCOPE_SYSTEM scheduling scope when a thread is being
   * created.  On such platforms, one can set the correct scheduling
   * scope and policy when creating the thread, thus not needing to
   * use this method.
   */
  static int modify_thread_scheduling_policy (CORBA::ORB_ptr orb);

protected:

  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_RT_ORB (void);

protected:

  /// Reference to our creating ORB Core.
  TAO_ORB_Core * const orb_core_;

  /// mutex_mgr_ manages the names associated with named mutexes.
  TAO_Named_RT_Mutex_Manager mutex_mgr_;

  /// Thread Pool Manager
  TAO_Thread_Pool_Manager *tp_manager_;

  /// Dynamic thread lifespan policy
  TAO_RT_ORBInitializer::TAO_RTCORBA_DT_LifeSpan lifespan_;

  /// Dynamic thread run time
  ACE_Time_Value const dynamic_thread_time_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"
#endif /* TAO_RT_ORB_H */
