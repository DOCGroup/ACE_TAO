/* -*- C++ -*- $Id$ */

#include "Terminator.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

int
Terminator::open(void*)
{
  if(this->activate (THR_NEW_LWP | THR_JOINABLE,
                     1) == -1)
  {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("Terminator::open Error spawning thread %p\n"),
                      "err="),
                     -1);
  }
  return 0;
}

int
Terminator::svc()
{
  while (1)
    {
      ACE_Message_Block* mb = 0;
      ACE_ASSERT(this->getq(mb) != -1);
      if (mb->msg_type () == ACE_Message_Block::MB_HANGUP)
        {
          mb->release ();
          break;
        }
      int delay_secs = ACE_OS::atoi(mb->rd_ptr());
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t|%T) Terminator::svc() Sleeping %d seconds before aborting\n", delay_secs));
      ACE_OS::sleep(delay_secs);
      ACE_OS::abort();
    }
  return 0;
}
