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

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/RTCORBAS.h"
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

/**
 * @class TAO_Named_RT_Mutex_Manager
 *
 * @brief Manages the names of RT Mutexes
 *
 */

class TAO_Export TAO_Named_RT_Mutex_Manager
{

public:
  /// Constructor.
  TAO_Named_RT_Mutex_Manager (void);

  /// Destructor.
  ~TAO_Named_RT_Mutex_Manager (void);

  /// @@ Frank, please make these APIs "Unicode friendly".  Irfan or Nanbor can help with this.

  /// Looks up a mutex based on its name.
  TAO_RT_Mutex *find_mutex (const char *name);

  /// Registers a mutex based on its name.
  int register_mutex (const char *name,
                      TAO_RT_Mutex *mutex);

  /// Unregisters a mutex based on its name
  void unregister_mutex (const char *name);

protected:

  /// Hash map for named RT Mutexes
  ACE_Hash_Map_Manager_Ex<const char *,
                          TAO_RT_Mutex *,
                          ACE_Hash<const char *>,
                          ACE_Equal_To<const char *>,
                          TAO_SYNCH_MUTEX> mutex_map_;
};

/**
 * @class TAO_RT_ORB
 *
 * @brief RTCORBA::RTORB implementation
 *
 * Creates and destroys RT CORBA objects, i.e., policies,
 * threadpools, mutexes.
 */

class TAO_Export TAO_RT_ORB : public RTCORBA::RTORB, public CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_RT_ORB (void);

  /// Destructor.
  virtual ~TAO_RT_ORB (void);

  /// @@@ Frank, please add a comment here explaining what this doe.
  virtual RTCORBA::Mutex_ptr create_mutex (CORBA::Environment
                                           &ACE_TRY_ENV =
                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// @@@ Frank, please add a comment here explaining what this doe.
  virtual void destroy_mutex (RTCORBA::Mutex_ptr the_mutex,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// @@@ Frank, please add a comment here explaining what this does.  Also, please make this operation "Unicode friendly". 
  virtual RTCORBA::Mutex_ptr create_named_mutex (const char *name,
                                                 CORBA::Boolean_out created_flag,
                                                 CORBA::Environment
                                                 &ACE_TRY_ENV =
                                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// @@@ Frank, please add a comment here explaining what this does.  Also, please make this operation "Unicode friendly". 
   virtual RTCORBA::Mutex_ptr open_named_mutex (const char * name,
                                                CORBA::Environment &ACE_TRY_ENV =
                                                TAO_default_environment () )
     ACE_THROW_SPEC ((CORBA::SystemException,
                      RTCORBA::RTORB::MutexNotFound
                      ));

  /// @@@ Frank, please add a comment here explaining what this does.
  virtual RTCORBA::ThreadpoolId
  create_threadpool (CORBA::ULong stacksize,
                     CORBA::ULong static_threads,
                     CORBA::ULong dynamic_threads,
                     RTCORBA::Priority default_priority,
                     CORBA::Boolean allow_request_buffering,
                     CORBA::ULong max_buffered_requests,
                     CORBA::ULong max_request_buffer_size,
                     CORBA::Environment
                     &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// @@@ Frank, please add a comment here explaining what this does.
  virtual RTCORBA::ThreadpoolId
  create_threadpool_with_lanes (CORBA::ULong stacksize,
                                const RTCORBA::ThreadpoolLanes & lanes,
                                CORBA::Boolean allow_borrowing,
                                CORBA::Boolean allow_request_buffering,
                                CORBA::ULong max_buffered_requests,
                                CORBA::ULong max_request_buffer_size,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// @@@ Frank, please add a comment here explaining what this does.  Yadda/yadda
  virtual void destroy_threadpool (RTCORBA::ThreadpoolId threadpool,
                                   CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTCORBA::RTORB::InvalidThreadpool));

  virtual RTCORBA::PriorityModelPolicy_ptr
  create_priority_model_policy (
                                RTCORBA::PriorityModel priority_model,
                                RTCORBA::Priority server_priority,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTCORBA::ThreadpoolPolicy_ptr
  create_threadpool_policy (RTCORBA::ThreadpoolId threadpool,
                            CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTCORBA::PriorityBandedConnectionPolicy_ptr
  create_priority_banded_connection_policy (const RTCORBA::PriorityBands &
                                            priority_bands,
                                            CORBA::Environment &ACE_TRY_ENV =
                                            TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTCORBA::PrivateConnectionPolicy_ptr
  create_private_connection_policy (CORBA::Environment &ACE_TRY_ENV =
                                    TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTCORBA::ServerProtocolPolicy_ptr
  create_server_protocol_policy (const RTCORBA::ProtocolList & protocols,
                                 CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTCORBA::ClientProtocolPolicy_ptr
  create_client_protocol_policy (const RTCORBA::ProtocolList & protocols,
                                 CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  /// @@@ Frank, please add a comment here explaining what this does.
  TAO_Named_RT_Mutex_Manager mutex_mgr_;
};

#if defined (__ACE_INLINE__)
#include "tao/RT_ORB.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_RT_CORBA == 1 */

#include "ace/post.h"
#endif /* TAO_RT_ORB_H */
