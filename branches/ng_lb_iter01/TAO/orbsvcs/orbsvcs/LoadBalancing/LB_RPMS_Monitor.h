// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_RPMS_Monitor.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_RPMS_MONITOR_H
#define TAO_LB_RPMS_MONITOR_H

#include "ace/pre.h"

#include "LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/LoadBalancingS.h"

#include "LB_RPMS_Monitor_Interceptor.h"


/**
 * @class TAO_LB_RPMS_Monitor
 *
 * @brief LoadMonitor implementation that returns monitor the number
 *        of requests per second a given location receives.
 *
 * Since interceptors are locality-constrained, the Load Balancer
 * actually makes feedback and control invocations methods through
 * this LoadMonitor exposed by this LoadMonitor.  This LoadMonitor
 * basically delegates all of its work on to its underlying
 * ServerRequestInterceptor.
 */
class TAO_LoadBalancing_Export TAO_LB_RPMS_Monitor
  : public virtual POA_LoadBalancing::LoadMonitor
{
public:

  /// Constructor
  TAO_LB_RPMS_Monitor (void);

  /**
   * @name PortableGroup::LoadMonitor Methods
   */
  //@{

  /// Return the number of requests per second arriving at the
  /// location this load monitor resides.
  virtual LoadBalancing::LoadList *current_load (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Force redirection of requests for targets with the given
  /// RepositoryId to the target pointed to by the given object
  /// reference.
  virtual void register_redirect (
      const char *type_id,
      CORBA::Object_ptr redirect_to,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove the redirect for targets of the given RepositoryId.
  virtual void remove_redirect (
      const char *type_id,
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

  /// Initialize the RPMS_Monitor.
  void init (CORBA::Environment &ACE_TRY_ENV);

private:

  /// Interceptor responsible for keeping track of the number of
  /// requests arriving per second.
  TAO_LB_RPMS_Monitor_Interceptor *interceptor_;

  /// Manage the above interceptor's memory with the following "_var."
  /// The above "narrowed" pointer is cached so that downcasting need
  /// not be performed more than once.
  PortableInterceptor::ServerRequestInterceptor_var safe_interceptor_;

};

#include "ace/post.h"

#endif  /* TAO_LB_RPMS_MONITOR_H */
