/**
 * @file Loopback.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Loopback.h"
#include "Auto_Disconnect.h"
#include "Implicit_Deactivator.h"

Loopback::Loopback (CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa,
                    RtecEventChannelAdmin::EventChannel_ptr ec)
{
  this->loopback_pair_.init (experiment_id,
                             base_event_type,
                             supplier_poa,
                             consumer_poa);

  this->loopback_pair_.connect (ec);
}

void
Loopback::disconnect (void)
{
  Auto_Disconnect<Loopback_Pair> disconnect (&this->loopback_pair_);

  Implicit_Deactivator deactivator (this);
}
