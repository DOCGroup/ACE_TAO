// $Id$

#include "Notify_EventChannelFactory_i.h"

#include "Service.h"
#include "ace/Dynamic_Service.h"
#include "tao/PortableServer/POA.h"
#include "tao/ORB_Core.h"

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory_i::create (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL)
{
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory;

  TAO_Notify_Service* notify_service = ACE_Dynamic_Service<TAO_Notify_Service>::instance (TAO_NS_COS_NOTIFICATION_SERVICE_NAME);

  if (notify_service == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "Service not found! check conf. file\n"));
    return notify_factory._retn ();
  }

  TAO_POA *poa = default_POA->_tao_poa_downcast();

  if (poa == 0)
    return notify_factory._retn ();

  CORBA::ORB_ptr orb = poa->orb_core ().orb () ;

  notify_service->init (orb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (notify_factory._retn ());

  notify_factory = notify_service->create (default_POA ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (notify_factory._retn ());

  return notify_factory._retn ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Notify_Service>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Notify_Service>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
