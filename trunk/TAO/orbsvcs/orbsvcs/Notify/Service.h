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
 *        Note that a Service creates only 1 Factory object.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Service : public ACE_Service_Object
{
public:
  /// Destructor.
  virtual ~TAO_Notify_Service (void);

  /// Init the service from configurator
  virtual int init (int argc, ACE_TCHAR *argv[]) = 0;

  /// Init the service.
  virtual void init_service (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL) = 0;

  /// Create the Channel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr create (
      PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL
    ) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_NOTIFY_DEF_EMO_FACTORY_NAME "Notify_Default_Event_Manager_Objects_Factory"

#define TAO_NOTIFICATION_SERVICE_NAME "TAO_Notify_Service"

#define TAO_COS_NOTIFICATION_SERVICE_NAME "TAO_CosNotify_Service"

#define TAO_RT_NOTIFICATION_SERVICE_NAME "TAO_RT_Notify_Service"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SERVICE_H */
