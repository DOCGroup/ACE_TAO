// $Id$

#include "TestEventConsumer_i.h"

#include "ace/Log_Msg.h"

TestEventConsumer_i::TestEventConsumer_i (CORBA::ORB_ptr orb, bool hang)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    hang_ (hang),
    count_ (0),
    orbtask_ (orb),
    cond_ (this->mtx_),
    shutdown_ (false)
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
  catch (CORBA::SystemException &e)
    {
      e._tao_print_exception (
        ACE_TEXT (
          "TestEventConsumer_i ORB_task: Caught CORBA::Exception:"));
    }
  return 0;
}

void
TestEventConsumer_i::push (const CORBA::Any & data)
{
  // Extract event data from the any.
  const char *eventData;
  if (data >>= eventData)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i::push(): Received ")
                            ACE_TEXT ("event containing %d bytes.\n"),
                                      ACE_OS::strlen (eventData)));
      if (this->hang_ && ++this->count_ == 10)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i::push(): ")
                                ACE_TEXT ("Simulating hung consumer\n")));
          {
            ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->mtx_);
            while (!this->shutdown_)
              this->cond_.wait ();
          }
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("TestEventConsumer_i::push Done\n")
                      ));
        }
    }
}

void
TestEventConsumer_i::disconnect_push_consumer ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("TestEventConsumer_i::disconnect_push_consumer()\n")));
  this->orb_->shutdown ();
  this->shutdown_ = true;
  this->cond_.signal ();
}
