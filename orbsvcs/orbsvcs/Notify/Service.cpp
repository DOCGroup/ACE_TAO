// $Id$

#include "orbsvcs/Notify/Service.h"
#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Service::~TAO_Notify_Service (void)
{
}

TAO_Notify_Service*
TAO_Notify_Service::load_default (void)
{
  static const char* services[] = { TAO_MC_NOTIFICATION_SERVICE_NAME,
                                    TAO_NOTIFICATION_SERVICE_NAME,
                                    TAO_COS_NOTIFICATION_SERVICE_NAME,
                                    0
                                  };

  TAO_Notify_Service* notify_service = 0;
  for(size_t i = 0; services[i] != 0; i++)
    {
      notify_service =
        ACE_Dynamic_Service<TAO_Notify_Service>::instance (services[i]);
      if (notify_service != 0)
        break;
    }
  return notify_service;
}

TAO_END_VERSIONED_NAMESPACE_DECL
