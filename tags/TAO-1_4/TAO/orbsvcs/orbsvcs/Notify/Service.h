/* -*- C++ -*- */
/**
 *  @file Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SERVICE_H
#define TAO_Notify_SERVICE_H
#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "tao/PortableServer/PortableServer.h"

/**
 * @class TAO_Notify_Service
 *
 * @brief Abstract ACE_Service_Object interface that is a
 * factory for creating a Notify EventChannelFactory.
 *        Note that a Service creates only 1 Factory object.
 *
 */
class TAO_Notify_Export TAO_Notify_Service : public ACE_Service_Object
{
public:
  /// Define Service_Object method otherwise SunCC compiler complains.
  virtual int init (int argc, char *argv[]) = 0;

  /// Init the service.
  virtual void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL) = 0;

  /// Create the Channel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr create (
      PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL
    ) = 0;
};

#define TAO_NOTIFY_DEF_EMO_FACTORY_NAME "Notify_Default_Event_Manager_Objects_Factory"

#define TAO_NOTIFICATION_SERVICE_NAME "TAO_Notify_Service"

#define TAO_COS_NOTIFICATION_SERVICE_NAME "TAO_CosNotify_Service"

#define TAO_RT_NOTIFICATION_SERVICE_NAME "TAO_RT_Notify_Service"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SERVICE_H */
