/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   RT_ORB.h
 *
 *  $Id$
 *
 *  @author Marina Spivak (marina@cs.wustl.edu)
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

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Export TAO_RT_ORB :
  public RTCORBA::RTORB,
  public CORBA::LocalObject

{
  // = TITLE
  //   RTCORBA::RTORB implementation
  //
  // = DESCRIPTION
  //   Creates and destroys RT CORBA objects, i.e., policies,
  //   threadpools, mutexes.
  //

public:
  /// Constructor.
  TAO_RT_ORB (void);

  /// Destructor.
  virtual ~TAO_RT_ORB (void);

  virtual RTCORBA::Mutex_ptr create_mutex (CORBA::Environment
                                           &ACE_TRY_ENV =
                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy_mutex (RTCORBA::Mutex_ptr the_mutex,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

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
