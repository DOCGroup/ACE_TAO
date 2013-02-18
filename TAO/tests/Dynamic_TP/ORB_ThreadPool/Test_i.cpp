//
// $Id$
//
#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

Sleeper_i::Sleeper_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Sleeper_i::delay(CORBA::Short sec)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Sleeper_i::delay called\n")));
  ACE_OS::sleep (sec);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Sleeper_i::delay returning\n")));
}

void
Sleeper_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

//-----------------------------------------------

Middle_i::Middle_i (CORBA::ORB_ptr orb, Test::Sleeper_ptr s)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    sleeper_ (Test::Sleeper::_duplicate (s))
{
}

void
Middle_i::call_delay(CORBA::Short sec)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Middle_i::delay called\n")));

  ACE_hthread_t thr;
  ACE_Thread::self (thr);
  bool recursive = false;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
    int result = this->threads_.insert (thr);
    ACE_ASSERT (result != -1);
    if (result == 1)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Middle_i::call_delay, ")
                    ACE_TEXT ("recursive upcall detected for thr = %d\n"),
                    thr));
        recursive = true;
      }
  }

  this->sleeper_->delay(sec);

  if (!recursive)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);
      int result = this->threads_.remove (thr);
      if (result == -1)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Middle_i::call_delay, ")
                    ACE_TEXT ("unable to remove thr = %d\n"),
                    thr));
      ACE_ASSERT (result != -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Middle_i::delay returning recursive = %d\n"),
              recursive));
}

void
Middle_i::shutdown (void)
{
  sleeper_->shutdown ();
  this->orb_->shutdown (0);
}
