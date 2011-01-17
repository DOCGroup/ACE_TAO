// -*- C++ -*-

/**
 *  @file Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_SERVICE_H
#define TAO_Notify_SERVICE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  class POA;
  typedef POA *POA_ptr;
}

namespace CosNotifyChannelAdmin
{
  class EventChannelFactory;
  typedef EventChannelFactory *EventChannelFactory_ptr;
}

namespace CORBA
{
  class ORB;
  typedef ORB *ORB_ptr;
}

/**
 * @class TAO_Notify_Service
 *
 * @brief Abstract ACE_Service_Object interface that is a
 * factory for creating a Notify EventChannelFactory.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Service : public ACE_Service_Object
{
public:
  /// Destructor.
  virtual ~TAO_Notify_Service (void);

  /// Load an implementation of the TAO_Notify_Service based on which
  /// implementations are loaded.
  static TAO_Notify_Service* load_default (void);

  /// Init the service from configurator
  virtual int init (int argc, ACE_TCHAR *argv[]) = 0;

  /// Init the service.
  virtual void init_service (CORBA::ORB_ptr orb) = 0;

  /// Init the service with separate dispatching orb.
  virtual void init_service2 (CORBA::ORB_ptr orb, CORBA::ORB_ptr dispatching_orb) = 0;

  /// Finalize the service.  This hook allows the service to
  /// perform actions when the user wants it to shut down.
  virtual void finalize_service (
      CosNotifyChannelAdmin::EventChannelFactory_ptr factory) = 0;

  /// Create the Channel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr create (
      PortableServer::POA_ptr default_POA,
      const char* factory_name = "EventChannelFactory") = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

/// @deprecated Use TAO_COS_NOTIFICATION_SERVICE_NAME instead.
#define TAO_NOTIFY_DEF_EMO_FACTORY_NAME "Notify_Default_Event_Manager_Objects_Factory"

#define TAO_NOTIFICATION_SERVICE_NAME "TAO_Notify_Service"

#define TAO_COS_NOTIFICATION_SERVICE_NAME "TAO_CosNotify_Service"

#define TAO_RT_NOTIFICATION_SERVICE_NAME "TAO_RT_Notify_Service"

#define TAO_MC_NOTIFICATION_SERVICE_NAME "TAO_MC_Notify_Service"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SERVICE_H */
