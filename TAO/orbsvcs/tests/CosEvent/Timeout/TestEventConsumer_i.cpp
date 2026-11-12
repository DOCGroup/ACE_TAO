#include "TestEventConsumer_i.h"

#include "ace/Log_Msg.h"

TestEventConsumer_i::TestEventConsumer_i (CORBA::ORB_ptr orb, bool hang)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    hang_ (hang),
    orbtask_ (orb),
    cond_ (this->mtx_)
{
}

TestEventConsumer_i::ORB_task::ORB_task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
TestEventConsumer_i::activate ()
{
  this->orbtask_.activate ();
}

int
TestEventConsumer_i::ORB_task::svc ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i ORB_task starting\n")));
  try
    {
      this->orb_->run ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i ORB_task exiting\n")));
    }
  catch (CORBA::SystemException const &e)
    {
      e._tao_print_exception (
        ACE_TEXT (
          "TestEventConsumer_i ORB_task: Caught CORBA::Exception:"));
    }
  return 0;
}

void
TestEventConsumer_i::push (CORBA::Any const &data)
{
  // Extract event data from the any.
  char const *eventData {};
  if (data >>= eventData)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i::push(): Received ")
                            ACE_TEXT ("event containing %B bytes.\n"),
                            std::strlen (eventData)));
      if (this->hang_)
        {
          bool wait_for_shutdown {};
          {
            ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mtx_);
            wait_for_shutdown = ++this->count_ == 10;
            if (wait_for_shutdown)
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i::push(): ")
                                    ACE_TEXT ("Simulating hung consumer\n")));
            while (wait_for_shutdown && !this->shutdown_)
              this->cond_.wait ();
          }
          if (wait_for_shutdown)
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i::push Done\n")));
        }
    }
}

void
TestEventConsumer_i::disconnect_push_consumer ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TestEventConsumer_i::disconnect_push_consumer()\n")));
  this->orb_->shutdown ();
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mtx_);
    this->shutdown_ = true;
    this->cond_.signal ();
  }
}
