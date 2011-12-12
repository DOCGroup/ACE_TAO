//=============================================================================
/**
 *  @file    test_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the Test interface.
 *
 */
//=============================================================================

#include "test_i.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_time.h"

Test_i::Test_i (CORBA::ORB_ptr orb,
                A::AMI_TestHandler_ptr rh,
                CORBA::ULong max_count,
                A::RunMode mode)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     rh_ (A::AMI_TestHandler::_duplicate (rh)),
     max_count_ (max_count),
     mode_(mode)
{
  time_t _tm = ACE_OS::time ();
  this->seed_ = (unsigned int)_tm;
}

void
Test_i::set_opponent (A::Test_ptr opp)
{
  this->opponent_ = A::Test::_duplicate (opp);
}

void
Test_i::request (
  ::A::RequestMode reqmode,
  ::CORBA::ULong & counter,
  ::A::FollowUp_out follow_up)
{
  ++counter;

  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):Test_i::request: (%C)  %C - %d\n",
              (reqmode == A::RQM_SYNCH ? "SYNCH" : "ASYNCH"),
              (this->mode_ == A::RM_MASTER ? "master" : "slave"),
              counter));

  const char* follow_up_str;
  switch (ACE_OS::rand_r (&this->seed_) % 2)
  {
  case 0:
    follow_up = (reqmode == A::RQM_SYNCH ? A::FU_TIMER : A::FU_NOTIFICATION);
    follow_up_str = (reqmode == A::RQM_SYNCH ? "TIMER" : "NOTIFICATION");
    break;
  case 1:
  default:
    follow_up = (reqmode == A::RQM_ASYNCH ? A::FU_TIMER : A::FU_NOTIFICATION);
    follow_up_str = (reqmode == A::RQM_ASYNCH ? "TIMER" : "NOTIFICATION");
    break;
  }

  if (!CORBA::is_nil(this->opponent_.in ()))
  {
    char buf[1024];
    ACE_OS::snprintf (buf, sizeof(buf), "request #%d followup [%s]", counter, follow_up_str);

    if (reqmode == A::RQM_SYNCH)
    {
      if (!CORBA::is_nil (this->rh_.in ()))
        this->opponent_->sendc_report(this->rh_.in (), buf);
    }
    else
    {
      this->opponent_->report (buf);
    }
  }
}

void
Test_i::report (
  const char * msg)
{
  ACE_DEBUG ((LM_DEBUG,
              "%N:%l:(%P:%t):Test_i::report: %C - %C\n",
              (this->mode_ == A::RM_MASTER ? "master" : "slave"),
              msg));
}

void
Test_i::shutdown (void)
{
  if (this->mode_ == A::RM_SLAVE && !CORBA::is_nil (this->opponent_.in ()))
  {
    this->opponent_->shutdown ();
  }
  this->orb_->shutdown (0);
}

Test_Reply_i::Test_Reply_i (CORBA::ORB_ptr orb,
    CORBA::ULong max_count,
    A::RunMode mode)
  : evh_ (orb, max_count, mode)
{
}

void
Test_Reply_i::request (
  ::CORBA::ULong counter,
  ::A::FollowUp follow_up)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%N:%l:(%P:%t):Test_Reply_i::request\n"));

  this->evh_.handle_followup (follow_up, counter);
}

void Test_Reply_i::request_excep (
  ::Messaging::ExceptionHolder * )
{
  ACE_DEBUG ((LM_ERROR,
              "%N:%l:(%P:%t):Test_Reply_i::request_excep: %C - unexpected exception\n",
              (this->evh_.mode () == A::RM_MASTER ? "master" : "slave")));
  if (!CORBA::is_nil (this->evh_.opponent ()))
    this->evh_.opponent ()->shutdown ();
  if (this->evh_.mode () == A::RM_SLAVE)
    this->evh_.orb ()->shutdown (0);
}

void
Test_Reply_i::report (
  void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "%N:%l:(%P:%t):Test_Reply_i::report\n"));
}

void
Test_Reply_i::report_excep (
  ::Messaging::ExceptionHolder * )
{
  ACE_DEBUG ((LM_ERROR,
              "%N:%l:(%P:%t):Test_Reply_i::report_excep: %C - unexpected exception\n",
              (this->evh_.mode () == A::RM_MASTER ? "master" : "slave")));
  if (!CORBA::is_nil (this->evh_.opponent ()))
    this->evh_.opponent ()->shutdown ();
  if (this->evh_.mode () == A::RM_SLAVE)
    this->evh_.orb ()->shutdown (0);
}

TestHandler::TestHandler (CORBA::ORB_ptr orb,
                          CORBA::ULong max_count,
                          A::RunMode mode)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    max_count_ (max_count),
    mode_ (mode),
    counter_ (0)
{
  time_t _tm = ACE_OS::time ();
  this->seed_ = (unsigned int)_tm;
}

void
TestHandler::set_counter (CORBA::ULong counter)
{
  this->counter_ = counter;
}

void
TestHandler::set_opponent (A::Test_ptr opp)
{
  this->opponent_ = A::Test::_duplicate (opp);
}

void
TestHandler::set_reply_handler (A::AMI_TestHandler_ptr rh)
{
  this->rh_ = A::AMI_TestHandler::_duplicate (rh);
}

int
TestHandler::handle_timeout (const ACE_Time_Value &,
                            const void *)
{
  this->orb_->orb_core ()->reactor ()->cancel_timer (this);
  if ((ACE_OS::rand_r (&this->seed_) % 2) == 0)
  {
    A::FollowUp followup;
    if (!CORBA::is_nil (this->opponent_.in ()))
    {
      this->opponent_->request(A::RQM_SYNCH, this->counter_, followup);
      this->handle_followup (followup, this->counter_);
    }
  }
  else
  {
    if (!CORBA::is_nil (this->opponent_.in ()))
    {
      this->opponent_->sendc_request(this->rh_.in (), A::RQM_ASYNCH, this->counter_);
    }
  }
  return 0;
}

int
TestHandler::handle_exception (ACE_HANDLE)
{
  if ((ACE_OS::rand_r (&this->seed_) % 2) == 0)
  {
    A::FollowUp followup;
    if (!CORBA::is_nil (this->opponent_))
    {
      this->opponent_->request(A::RQM_SYNCH, this->counter_, followup);
      this->handle_followup (followup, this->counter_);
    }
  }
  else
  {
    if (!CORBA::is_nil (this->opponent_))
    {
      this->opponent_->sendc_request(this->rh_.in (), A::RQM_ASYNCH, this->counter_);
    }
  }
  return 0;
}

void TestHandler::start ()
{
  this->orb_->orb_core ()->reactor ()->notify (this);
}

void
TestHandler::handle_followup (A::FollowUp fup, CORBA::ULong counter)
{
  if (counter > this->max_count_)
  {
    if (this->mode_ == A::RM_MASTER && !CORBA::is_nil (this->opponent_))
    {
      this->opponent_->shutdown ();
    }
  }
  else
  {
    if (this->mode_ == A::RM_MASTER && !CORBA::is_nil (this->opponent_))
    {
      char buf[1024];
      ACE_OS::snprintf (buf, sizeof(buf), "request #%d followup [%s]",
                                          counter,
                                          (fup == A::FU_TIMER ? "TIMER" : "NOTIFICATION"));
      this->opponent_->report (buf);
    }

    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%N:%l:(%P:%t):TestHandler::handle_followup\n"));
    }

    this->set_counter (counter);
    if (fup == A::FU_TIMER)
    {
      this->orb_->orb_core ()->reactor ()->schedule_timer (this, 0,
                                                           ACE_Time_Value (0, 100),
                                                           ACE_Time_Value (0, 100));
    }
    else
    {
      this->orb_->orb_core ()->reactor ()->notify (this);
    }
  }
}
