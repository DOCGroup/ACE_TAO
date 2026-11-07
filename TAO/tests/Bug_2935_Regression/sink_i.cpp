#include "sink_i.h"
#include "ace/OS_NS_unistd.h"

// Constructor
Sink_i::Sink_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

// Implement the tick method

void
Sink_i::tick ()
{
  ACE_OS::sleep (1);
  ++tickCount_;
  ACE_DEBUG ((LM_DEBUG, "Sink (%P|%t) tick: %u\n",
              static_cast<unsigned int> (tickCount_)));
}

void
Sink_i::tock ()
{
  ACE_OS::sleep (1);
  ++tockCount_;
  ACE_DEBUG ((LM_DEBUG, "Sink (%P|%t) tock: %u\n",
              static_cast<unsigned int> (tockCount_)));
}

::CORBA::ULong
Sink_i::tickCount ()
{
  ACE_DEBUG ((LM_DEBUG, "Sink (%P|%t) tickCount: %u\n",
              static_cast<unsigned int> (tickCount_)));
  return tickCount_;
}

::CORBA::ULong
Sink_i::tockCount ()
{
  ACE_DEBUG ((LM_DEBUG, "Sink (%P|%t) tockCount: %u\n",
              static_cast<unsigned int> (tockCount_)));
  return tockCount_;
}

// Shutdown.

void
Sink_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Sink (%P|%t) shutting down.  Ticks: %u Tocks %u\n",
              static_cast<unsigned int> (tickCount_),
              static_cast<unsigned int> (tockCount_)));

  // Instruct the ORB to shutdown.
  this->orb_->shutdown ();
}
