// $Id$

#include "RTCORBA_Application_Command.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_Application_Command.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_RTCORBA_Application_Command, "$id$")

#include "ace/Dynamic_Service.h"
#include "../../../orbsvcs/Notify/Service.h"
#include "LookupManager.h"

TAO_NS_RTCORBA_Application_Command::TAO_NS_RTCORBA_Application_Command (void)
{
}

TAO_NS_RTCORBA_Application_Command::~TAO_NS_RTCORBA_Application_Command ()
{
}

void
TAO_NS_RTCORBA_Application_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  // Call the Base class.
  TAO_NS_Application_Command::init (arg_shifter);
}

void
TAO_NS_RTCORBA_Application_Command::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  // Intercept init
  if (this->command_ == INIT)
    {
  /*      TAO_NS_Service* notify_service = ACE_Dynamic_Service<TAO_NS_Service>::instance ("TAO_NS_Service");

        if (notify_service == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Notification Service not found! check conf. file\n"));
            return;
          };

        CORBA::ORB_var orb;
        LOOKUP_MANAGER->resolve (orb);

        notify_service->init (orb.in () ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;*/
    }

  // Call the Base class.
  TAO_NS_Application_Command::execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}
