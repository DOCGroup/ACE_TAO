/* -*- C++ -*- */
/**
 *  @file RTCORBA_Notify_Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_RTCORBA_SERVICE_H
#define TAO_NS_RTCORBA_SERVICE_H
#include "ace/pre.h"

#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_Service.h"

/**
 * @class TAO_NS_RTCORBA_Notify_Service
 *
 * @brief Implemetation of the TAO_NS_Service interface for RT Notification.
 *
 */
class TAO_RT_Notify_Export TAO_NS_RTCORBA_Notify_Service : public TAO_NS_Notify_Service
{
public:
  /// Constuctor
  TAO_NS_RTCORBA_Notify_Service (void);

  /// Destructor
  ~TAO_NS_RTCORBA_Notify_Service ();

  /// Init the service.
  virtual void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

protected:
  /// Init the data members
  virtual void init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  /// Create the Factory for RT Notify objects.
  virtual void init_factory (ACE_ENV_SINGLE_ARG_DECL);

  /// Creates the Builder for RT Notify objects.
  virtual void init_builder (ACE_ENV_SINGLE_ARG_DECL);
};

ACE_FACTORY_DECLARE (TAO_RT_Notify,TAO_NS_RTCORBA_Notify_Service)

#if defined (__ACE_INLINE__)
#include "RTCORBA_Notify_Service.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_RTCORBA_SERVICE_H */
