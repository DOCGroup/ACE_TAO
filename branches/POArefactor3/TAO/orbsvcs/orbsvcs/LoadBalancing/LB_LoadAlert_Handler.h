// -*- C++ -*-

// ============================================================================
/**
 * @file   LB_LoadAlert_Handler.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
// ============================================================================

#ifndef TAO_LB_LOAD_ALERT_REPLY_HANDLER_H
#define TAO_LB_LOAD_ALERT_REPLY_HANDLER_H

#include "orbsvcs/CosLoadBalancingS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_LB_LoadAlert_Handler
 *
 * @brief The AMI reply handler for replies from all LoadAlert
 *        objects.
 *
 * This class handles all asynchronously received replies from all
 * registered LoadAlert objects.  It only exists to receive
 * asynchronously sent exceptions.
 */
class TAO_LB_LoadAlert_Handler
  : public virtual POA_CosLoadBalancing::AMI_LoadAlertHandler,
    public virtual PortableServer::RefCountServantBase
{
public:

  virtual void enable_alert (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void enable_alert_excep (
      CosLoadBalancing::AMI_LoadAlertExceptionHolder *
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void disable_alert (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void disable_alert_excep (
      CosLoadBalancing::AMI_LoadAlertExceptionHolder *
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Protected destructor to enforce propery memory management
  /// through reference counting.
  ~TAO_LB_LoadAlert_Handler (void);

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_LB_LOAD_ALERT_REPLY_HANDLER_H */
