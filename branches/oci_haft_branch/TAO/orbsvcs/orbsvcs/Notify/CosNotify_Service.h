/* -*- C++ -*- */
/**
 *  @file CosNotify_Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_COSNOTIFY_SERVICE_H
#define TAO_Notify_COSNOTIFY_SERVICE_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Service.h"

class TAO_Notify_Factory;
class TAO_Notify_Builder;
class TAO_Notify_Properties;
class TAO_Notify_EventChannelFactory;

/**
 * @class TAO_CosNotify_Service
 *
 * @brief A service object for creating the  Notify Service Factory.
 *
 */
class TAO_Notify_Export TAO_CosNotify_Service : public TAO_Notify_Service
{
public:
  /// Constuctor
  TAO_CosNotify_Service (void);

  /// Destructor
  virtual ~TAO_CosNotify_Service ();

  /// = Service_Object virtual method overloads.
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

  /// Init
  virtual void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  /// Create the Channel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr create (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL);

  /// Called by the factory when it is destroyed.
  virtual void remove (TAO_Notify_EventChannelFactory* ecf ACE_ENV_ARG_DECL);

protected:
  /// Init the data members
  virtual void init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  /// Create the Factory for Notify objects.
  virtual void init_factory (ACE_ENV_SINGLE_ARG_DECL);

  /// Create the Builder for Notify objects.
  virtual void init_builder (ACE_ENV_SINGLE_ARG_DECL);

  /// Set thread options on <qos>.
  void set_threads (CosNotification::QoSProperties &qos, int threads);

  /// Service component for object factory operations.
  TAO_Notify_Factory* factory_;

  /// Service component for building NS participants.
  TAO_Notify_Builder* builder_;
};

ACE_STATIC_SVC_DECLARE (TAO_CosNotify_Service)
ACE_FACTORY_DECLARE (TAO_Notify, TAO_CosNotify_Service)

ACE_STATIC_SVC_DECLARE (TAO_Notify_Default_EMO_Factory_OLD)

#if defined (__ACE_INLINE__)
#include "CosNotify_Service.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_COSNOTIFY_SERVICE_H */
