// $Id$

#include "Consumer.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

Consumer::Consumer (void)
{
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "Consumer (%P|%t) we received event type %d\n",
              events[0].header.type));
}

void
Consumer::disconnect_push_consumer (void)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

