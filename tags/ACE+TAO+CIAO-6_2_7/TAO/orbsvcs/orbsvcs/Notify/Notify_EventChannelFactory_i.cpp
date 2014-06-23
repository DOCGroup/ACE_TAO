// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"

#include "orbsvcs/Notify/Service.h"
#include "ace/Dynamic_Service.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::create (PortableServer::POA_ptr default_POA,
                                          const char* factory_name)
{
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory;

  TAO_Notify_Service* notify_service = ACE_Dynamic_Service<TAO_Notify_Service>::instance (TAO_COS_NOTIFICATION_SERVICE_NAME);

  if (notify_service == 0)
  {
    ORBSVCS_DEBUG ((LM_DEBUG, "Service not found! check conf. file\n"));
    return notify_factory._retn ();
  }

  TAO_Root_POA *poa = dynamic_cast <TAO_Root_POA*> (default_POA);

  if (poa == 0)
    return notify_factory._retn ();

  CORBA::ORB_ptr orb = poa->orb_core ().orb () ;

  notify_service->init_service (orb);

  notify_factory = notify_service->create (default_POA, factory_name);

  return notify_factory._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
