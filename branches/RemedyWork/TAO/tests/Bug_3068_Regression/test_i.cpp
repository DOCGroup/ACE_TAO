// $Id$

#include "test_i.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "ace/OS_NS_unistd.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

void
Callback_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG, "Performing clean shutdown\n"));
  this->orb_->shutdown (0);
}

void
Callback_i::callback_method (const CORBA::OctetSeq &)
{
  static int count = 0;
  if (count++ > 0)
    ACE_OS::exit(0);
}


// ****************************************************************

CORBA::Long
Simple_Server_i::test_method (CORBA::Boolean do_callback)
{
  if (do_callback && this->caller_ != 0)
    {
      this->caller_->go ();
    }

  return 0;
}


void
Simple_Server_i::ow_test ()
{
  int iter = this->ow_count_++;
  ACE_DEBUG ((LM_DEBUG, "%t: Invoking ow test, iter = %d\n", iter));
}

void
Simple_Server_i::callback_object (Callback_ptr callback)
{
  this->caller_ = new Caller (callback, orb_.in ());
}

Caller::Caller (Callback_ptr callback, CORBA::ORB_ptr orb)
  : cbobj_ (Callback::_duplicate (callback))
  , orb_ (CORBA::ORB::_duplicate (orb))
  , cond_ (this->lock_)
  , going_ (false)
{
  this->payload_.length (0); //10000);
  this->activate (THR_JOINABLE | THR_NEW_LWP, 4);
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "%t returning\n"));
}

void
Caller::go ()
{
  this->lock_.acquire ();
  this->going_ = true;
  this->cond_.broadcast ();
  this->lock_.release ();
}

int
Caller::svc ()
{
  this->lock_.acquire ();
  ACE_DEBUG ((LM_DEBUG, "%t Getting ready to call\n"));
  while (!this->going_) this->cond_.wait ();
  this->lock_.release ();
  try
    {
      while (1)
        {
          ACE_DEBUG ((LM_DEBUG, "%t calling, payload len = %d\n",
                      payload_.length ()));
          this->cbobj_->callback_method (this->payload_);
          ACE_DEBUG ((LM_DEBUG, "%t callback returned\n"));
          ACE_Time_Value tv (0, 100);
          ACE_OS::sleep (tv);
        }
    }
  catch (CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "%t caught: %C\n", ex._name ()));
    }
  try
    {
      ACE_OS::sleep (1);
      this->orb_->shutdown (0);
    }
  catch (CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "%t orb shutdown raised %C\n", ex._name ()));
    }
  return 0;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (0);
}
