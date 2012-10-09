// $Id$

#include "sink_i.h"
#include "ace/OS_NS_unistd.h"

// Constructor
Sink_i::Sink_i (CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate(orb))
, tickCount_(0)
, tockCount_(0)
{
  // no-op
}

// Destructor

Sink_i::~Sink_i ()
{
  // no-op
}

bool
Sink_i::parse_args(int &, ACE_TCHAR *[])
{
  return true;
}

// Implement the tick method

void
Sink_i::tick ()
{
  ACE_OS::sleep(1);
  tickCount_ += 1;
  ACE_DEBUG((LM_DEBUG, "Sink (%P|%t) tick: %d\n", tickCount_));
}

void
Sink_i::tock ()
{
  ACE_OS::sleep(1);
  tockCount_ += 1;
  ACE_DEBUG((LM_DEBUG, "Sink (%P|%t) tock: %d\n", tockCount_));
}

::CORBA::ULong
Sink_i::tickCount ()
{
  ACE_DEBUG((LM_DEBUG, "Sink (%P|%t) tickCount: %d\n", tickCount_));
  return tickCount_;
}

::CORBA::ULong
Sink_i::tockCount ()
{
  ACE_DEBUG((LM_DEBUG, "Sink (%P|%t) tockCount: %d\n", tockCount_));
  return tockCount_;
}

// Shutdown.

void
Sink_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Sink (%P|%t) shutting down.  Ticks: %d Tocks %d\n",
              (int)tickCount_,
              (int)tockCount_));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
