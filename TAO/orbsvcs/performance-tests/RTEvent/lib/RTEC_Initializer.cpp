/**
 * @file RTEC_Initializer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "RTEC_Initializer.h"
#include "RTCORBA_Setup.h"

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/Event/EC_RTCORBA_Factory.h"

ACE_RCSID(TAO_PERF_RTEC, RTEC_Initializer, "$Id$")

TAO_EC_Event_Channel *
RTEC_Initializer::create (PortableServer::POA_ptr consumer_poa,
                          PortableServer::POA_ptr supplier_poa,
                          RTCORBA_Setup * rtcorba_setup
                          ACE_ENV_ARG_DECL_NOT_USED)
{
#if 1
  auto_ptr<TAO_EC_Factory> factory (new TAO_EC_Default_Factory);
#else
  auto_ptr<TAO_EC_Factory> factory;
  if (rtcorba_setup == 0)
    {
      factory = auto_ptr<TAO_EC_Factory> (new TAO_EC_Default_Factory);
    }
  else
    {
      factory = auto_ptr<TAO_EC_Factory> (new TAO_EC_RTCORBA_Factory (new TAO_EC_Default_Factory,
                                                                      rtcorba_setup->lanes ()));
    }
#endif /* 0 */

  TAO_EC_Event_Channel_Attributes attr (consumer_poa,
                                        supplier_poa);

  TAO_EC_Event_Channel *ec =
    new TAO_EC_Event_Channel (attr, factory.get (), 1);
  factory.release ();

  return ec;
}
