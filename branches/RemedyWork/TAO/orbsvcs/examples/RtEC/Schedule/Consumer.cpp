// $Id$

#include "Consumer.h"

Consumer::Consumer (void)
{
}

void
Consumer::push (const RtecEventComm::EventSet& events)
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
{
}

