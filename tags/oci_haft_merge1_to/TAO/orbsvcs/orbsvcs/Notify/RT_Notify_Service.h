/* -*- C++ -*- */
/**
 *  @file RT_Notify_Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_RT_NOTIFY_SERVICE_H
#define TAO_Notify_RT_NOTIFY_SERVICE_H

#include "ace/pre.h"
#include "rt_notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CosNotify_Service.h"

/**
 * @class TAO_RT_Notify_Service
 *
 * @brief Implemetation of the TAO_Notify_Service interface for RT Notification.
 *
 */
class TAO_RT_Notify_Export TAO_RT_Notify_Service : public TAO_CosNotify_Service
{
public:
  /// Constuctor
  TAO_RT_Notify_Service (void);

  /// Destructor
  ~TAO_RT_Notify_Service ();

  /// Overload base virtual method to silence HPUX11 build warning.
  virtual int init (int argc, char *argv[]);

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

ACE_FACTORY_DECLARE (TAO_RT_Notify,TAO_RT_Notify_Service)

#if defined (__ACE_INLINE__)
#include "RT_Notify_Service.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_RT_NOTIFY_SERVICE_H */
