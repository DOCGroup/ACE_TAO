// -*- C++ -*-

//=============================================================================
/**
 * @file   LB_LoadAlert.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_LOAD_ALERT_H
#define TAO_LB_LOAD_ALERT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosLoadBalancingS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"


class TAO_LoadBalancing_Export TAO_LB_LoadAlert
  : public virtual POA_CosLoadBalancing::LoadAlert
{
public:

  /// Constructor.
  TAO_LB_LoadAlert (void);

  /**
   * @name CosLoadBalancing::LoadAlert Methods
   *
   * Methods required by the CosLoadBalancing::LoadAlert interface.
   */
  //@{

  /// Forward requests back to the load manager via the object group
  /// reference.
  virtual void enable_alert (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Stop forwarding requests, and begin accepting them again.
  virtual void disable_alert (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

  /// Has this LoadAlert servant been alerted of a high load condition
  /// by the LoadManager.
  CORBA::Boolean alerted (void) const;

protected:


  /// Destructor.
  /**
   * Protected destructor to enforce correct memory management via
   * reference counting.
   */
  //~TAO_LB_LoadAlert (void);

private:

  /// Has this LoadAlert servant been alerted of a high load condition
  /// by the LoadManager?
  CORBA::Boolean alerted_;

  /// Lock used to synchronized access to the LoadAlert state.
  mutable TAO_SYNCH_MUTEX lock_;

};


#if defined (__ACE_INLINE__)
# include "LB_LoadAlert.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_LB_LOAD_ALERT_H */
