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
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_RTCORBA_SAFE_INCLUDE
#include "RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "tao/LocalObject.h"
#include "ace/Hash_Map_Manager_T.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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

  RTCORBA::Mutex_ptr create_mutex (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy_mutex (RTCORBA::Mutex_ptr the_mutex
                      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RTCORBA::Mutex_ptr create_named_mutex (const char *name,
                                         CORBA::Boolean_out created_flag
                                         TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RTCORBA::Mutex_ptr open_named_mutex (const char * name
                                       TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTCORBA::RTORB::MutexNotFound
                     ));

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
    public TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_RT_ORB (TAO_ORB_Core *orb_core);

  /**
   * Create a new mutex.  Mutexes returned by this method
   * are the same as those used internally by the ORB, so that
   * consistant priority inheritance/piority ceiling semantics
   * can be guaranteed.
   */
  virtual RTCORBA::Mutex_ptr create_mutex (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Destroy a mutex.  Currently this is a no-op since RTCORBA::Mutex
   * instances are destroyed as soon as their reference counts go to
   * 0.
   */
  virtual void destroy_mutex (RTCORBA::Mutex_ptr the_mutex
                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Create a mutex and assign a name to it.  If the
   * mutex already exists, it is returned and the created_flag
   * is set to 0.
   */
  virtual RTCORBA::Mutex_ptr create_named_mutex (const char *name,
                                                 CORBA::Boolean_out created_flag
                                                 TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Retrieve a previously created mutex.
  virtual RTCORBA::Mutex_ptr open_named_mutex (const char * name
                                                TAO_ENV_ARG_DECL_WITH_DEFAULTS )
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RTCORBA::RTORB::MutexNotFound
                      ));

  /**
   * Create and return a TCPProtocolProperties instance with the specified
   * parameters.
   */
  virtual RTCORBA::TCPProtocolProperties_ptr
  create_tcp_protocol_properties (
                                  CORBA::Long send_buffer_size,
                                  CORBA::Long recv_buffer_size,
                                  CORBA::Boolean keep_alive,
                                  CORBA::Boolean dont_route,
                                  CORBA::Boolean no_delay
                                  TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException ));

  /// Create a RTCORBA threadpool to manage a set of threads without lanes.
  virtual RTCORBA::ThreadpoolId
  create_threadpool (CORBA::ULong stacksize,
                     CORBA::ULong static_threads,
                     CORBA::ULong dynamic_threads,
                     RTCORBA::Priority default_priority,
                     CORBA::Boolean allow_request_buffering,
                     CORBA::ULong max_buffered_requests,
                     CORBA::ULong max_request_buffer_size
                     TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
                                CORBA::ULong max_request_buffer_size
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Free the resources associated with the specified threadpool.
  virtual void destroy_threadpool (RTCORBA::ThreadpoolId threadpool
                                   TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTCORBA::RTORB::InvalidThreadpool));

  /// Create a priority model policy for use when configuring a POA.
  virtual RTCORBA::PriorityModelPolicy_ptr
  create_priority_model_policy (
                                RTCORBA::PriorityModel priority_model,
                                RTCORBA::Priority server_priority
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Create a ThreadpoolPolicy instance for POA creation
  virtual RTCORBA::ThreadpoolPolicy_ptr
  create_threadpool_policy (RTCORBA::ThreadpoolId threadpool
                            TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Create a PriorityBandedConnectionPolicy instance
   * for use on either the client or server side
   */
  virtual RTCORBA::PriorityBandedConnectionPolicy_ptr
  create_priority_banded_connection_policy (const RTCORBA::PriorityBands &
                                            priority_bands
                                            TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Create a PrivateConnectionPolicy instance to use on client
   * to request a private (non-multiplexed) transport connection
   * to the server.
   */
  virtual RTCORBA::PrivateConnectionPolicy_ptr
  create_private_connection_policy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Create a ServerProtocolPolicy instance to select and configure
   * communication protocols on the server side.
   */
  virtual RTCORBA::ServerProtocolPolicy_ptr
  create_server_protocol_policy (const RTCORBA::ProtocolList & protocols
                                 TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Create a ClientProtocolPolicy instance to select and configure
   * communication protocols on the client side.
   */
  virtual RTCORBA::ClientProtocolPolicy_ptr
  create_client_protocol_policy (const RTCORBA::ProtocolList & protocols
                                 TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the Thread Pool Manager.
  TAO_Thread_Pool_Manager &tp_manager (void);

protected:
  /// Protected destructor to enforce proper memory management of this
  /// reference counted object.
  virtual ~TAO_RT_ORB (void);

  /// Reference to our creating ORB.
  TAO_ORB_Core *orb_core_;

  /// mutex_mgr_ manages the names associated with named mutexes.
  TAO_Named_RT_Mutex_Manager mutex_mgr_;

  /// Thread Pool Manager
  TAO_Thread_Pool_Manager *tp_manager_;
};

#if defined (__ACE_INLINE__)
#include "RT_ORB.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_RT_ORB_H */
