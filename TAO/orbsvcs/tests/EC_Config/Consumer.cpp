// $Id$

//#include "ace/Thread.h"
#include "Consumer.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

Consumer::Consumer (void)
  : _ordinal(-1)
{
}

Consumer::Consumer(int ord)
  : _ordinal(ord)
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

  int prio = -1;
  ACE_hthread_t handle;
  ACE_Thread::self(handle);
  ACE_Thread::getprio(handle,prio);
  //ACE_thread_t tid = ACE_Thread::self();
  ACE_DEBUG ((LM_DEBUG, "Consumer #%d @%d (%P|%t) we received event type %d\n",
              _ordinal,prio,events[0].header.type));
}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
