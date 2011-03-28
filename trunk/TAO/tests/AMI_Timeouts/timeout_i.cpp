
//=============================================================================
/**
 *  @file    timeout_i.cpp
 *
 *  $Id$
 *
 *  Implements the timeout CORBA Object and its reply handler.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "timeout_i.h"
#include "ace/OS_NS_unistd.h"

Timeout_i::Timeout_i (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

Timeout_i::~Timeout_i ()
{
}

void
Timeout_i::sendTimeToWait (CORBA::Long msec)
{
  //ACE_DEBUG ((LM_DEBUG,
  //            "Timeout_i::sendTimeToWait: invoked with msec = %d\n\n",
  //            msec));

  if (msec != 0)
    {
      // ACE_DEBUG ((LM_DEBUG,
      //            "Timeout_i::sendTimeToWait: sleeping\n\n"));

      ACE_Time_Value tv (0, msec * 1000);
            ACE_OS::sleep (tv);
    }
}

void
Timeout_i::shutdown (void)
{
  orb_->shutdown ();
  //ACE_DEBUG ((LM_DEBUG,
  //            "Timeout_i::shutdown: shut down ORB\n\n"));
}

// Reply Handler implementation

TimeoutHandler_i::TimeoutHandler_i ()
: reply_counter_ (0)
, reply_excep_counter_ (0)
{
  timer_.reset ();
  timer_.start ();
  timer_.stop ();
}

TimeoutHandler_i::~TimeoutHandler_i ()
{
}

void
TimeoutHandler_i::sendTimeToWait (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "reply"));
  ++reply_counter_;
  timer_.stop ();
}

void
TimeoutHandler_i::sendTimeToWait_excep (::Messaging::ExceptionHolder *excep_holder)
{
  timer_.stop ();

  try
    {
      excep_holder->raise_exception ();
    }
  catch (const CORBA::TIMEOUT& )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "timeout"));
      ++reply_excep_counter_;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: Unexpected exception"));
    }
}

void
TimeoutHandler_i::reset_reply_counter ()
{
  reply_counter_ = 0;
}

void
TimeoutHandler_i::reset_reply_excep_counter ()
{
  reply_excep_counter_ = 0;
}

unsigned short
TimeoutHandler_i::reply_counter ()
{
  return reply_counter_;
}

unsigned short
TimeoutHandler_i::reply_excep_counter ()
{
  return reply_excep_counter_;
}

void
TimeoutHandler_i::start ()
{
  timer_.reset ();
  timer_.start ();
}

ACE_Time_Value &
TimeoutHandler_i::elapsed_time ()
{
  timer_.elapsed_time (elapsed_time_);
  return elapsed_time_;
}
