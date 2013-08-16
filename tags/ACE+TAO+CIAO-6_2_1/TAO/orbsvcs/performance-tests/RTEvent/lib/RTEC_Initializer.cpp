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

#include "ace/Dynamic_Service.h"

TAO_EC_Event_Channel *
RTEC_Initializer::create (PortableServer::POA_ptr consumer_poa,
                          PortableServer::POA_ptr supplier_poa,
                          RTCORBA_Setup * rtcorba_setup)
{
  TAO_EC_Event_Channel_Attributes attr (consumer_poa,
                                        supplier_poa);

  if (rtcorba_setup == 0)
    {
      return new TAO_EC_Event_Channel (attr);
    }

  TAO_EC_Factory *body =
    ACE_Dynamic_Service<TAO_EC_Factory>::instance ("EC_Factory");
  auto_ptr<TAO_EC_Factory> factory (
      new TAO_EC_RTCORBA_Factory (body,
                                  rtcorba_setup->lanes ()));

  TAO_EC_Event_Channel *ec =
    new TAO_EC_Event_Channel (attr, factory.get (), 1);
  factory.release ();

  return ec;
}
