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

ACE_RCSID(TAO_RTEC_Perf, Loopback, "$Id$")

Loopback::Loopback (CORBA::Long experiment_id,
                    CORBA::Long base_event_type,
                    PortableServer::POA_ptr supplier_poa,
                    PortableServer::POA_ptr consumer_poa,
                    RtecEventChannelAdmin::EventChannel_ptr ec
                    ACE_ENV_ARG_DECL)
{
  this->loopback_pair_.init (experiment_id,
                             base_event_type,
                             supplier_poa,
                             consumer_poa);

  this->loopback_pair_.connect (ec ACE_ENV_ARG_PARAMETER);
}

void
Loopback::disconnect (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Auto_Disconnect<Loopback_Pair> disconnect (&this->loopback_pair_);

  Implicit_Deactivator deactivator (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
