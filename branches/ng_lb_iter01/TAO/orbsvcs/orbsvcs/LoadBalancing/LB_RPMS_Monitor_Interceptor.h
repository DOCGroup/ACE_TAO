// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_RPMS_Monitor_Interceptor.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_RPMS_MONITOR_INTERCEPTOR_H
#define TAO_LB_RPMS_MONITOR_INTERCEPTOR_H

#include "ace/pre.h"

#include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#includ "ace/Sync.h"

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

/**
 * @class TAO_LB_RPMS_Monitor_Interceptor
 *
 * @brief ServerRequestInterceptor that provides feedback and
 *        control.
 *
 * This ServerRequestInterceptor is used to count the number of
 * requests per millisecond that are arriving at the location this
 * interceptor resides.
 */
class TAO_LB_RPMS_Monitor_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_LB_RPMS_Monitor (void);

  /**
   * @name Methods Required by the ServerRequestInterceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all server request interceptors.
   */
  //@{

  virtual char * name (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// The bulk of the work this interceptor implementations performs
  /// is in this interception point.
  virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
  //@}

  /**
   * @name Feedback and control methods.
   *
   * Methods that indirectly provide feedback and control to the load
   * balancer via the load monitor.
   */
  //@{

  /// Return the current average load (requests per second).
  CORBA::Float current_load (void) const;

  /// Force redirection of requests for targets with the given
  /// RepositoryId to the target pointed to by the given object
  /// reference.
  void register_redirect (const char *type_id,
                          CORBA::Object_ptr redirect_to,
                          CORBA::Environment &ACE_TRY_ENV);

  /// Remove the redirect for targets of the given RepositoryId.
  void remove_redirect (const char *type_id,
                        CORBA::Environment &ACE_TRY_ENV);

  //@}

public:

  /// This template is actually instantiated in
  /// `tao/Object_Ref_Table.cpp'.
  typedef ACE_Hash_Map_Manager_Ex<const char *, CORBA::Object_ptr, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex> Table;  

private:

  /// Lock that forces access request count to be modified/accessed
  /// atomically.
  TAO_SYNCH_MUTEX lock_;

  /// Flag that if set true will cause this interceptor to redirect
  /// all requests back to the load balancer.
  CORBA::Boolean redirect_requests_;

  /// The number of requests received at the location this interceptor
  /// resides in the current measurement interval.
  CORBA::ULong request_count_;

  // The start of the current measurement interval.
  ACE_Time_Value interval_start_;

  /// Reference to the Load Balancer.
  LoadBalancing::ReplicationManager_var lb_;


  
};


#if defined (__ACE_INLINE__)
# include "LB_RPMS_Monitor_Interceptor.inl"
#endif /* __ACE_INLINE__ */


#include "ace/post.h"

#endif  /* TAO_LB_RPMS_MONITOR_INTERCEPTOR_H */
