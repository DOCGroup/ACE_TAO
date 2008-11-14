// -*- C++ -*-

//=============================================================================
/**
 *  @file    test_i.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "test_i.h"

#include "ace/Throughput_Stats.h"

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
		const std::string & object_id,
		StateSynchronizationAgent_ptr agent,
		int & stop)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    history_ (50),
    object_id_ (object_id),
    agent_ (StateSynchronizationAgent::_duplicate (agent)),
    state_ (1),
    stop_ (stop)
{
}

void
test_i::set_state (const ::CORBA::Any & state_value)
{
  // extract value to an intermediate long variable since it's not possible
  // to extract to a long & directly
  CORBA::Long value;

  if (state_value >>= value)
    state_ = value;
  else
    ACE_DEBUG ((LM_ERROR,
		"ReplicatedApplication_i::set_state () "
		"could not extract satet value from Any."));

  ACE_DEBUG ((LM_DEBUG, "test_i::set_state (%d) called.\n", value));
}

::CORBA::Any *
test_i::get_state (void)
{
  // create new any object
  CORBA::Any_var state (new CORBA::Any);

  // create intermediate object with the value
  CORBA::Long value = state_;

  ACE_DEBUG ((LM_DEBUG, "test_i::get_state returns %d.\n", value));

  // insert value into the any object
  *state <<= value;

  return state._retn ();
}

void
test_i::method (CORBA::ULong test_start, CORBA::ULong test_end,
                CORBA::ULong work,
                CORBA::ULong prime_number,
                CORBA::ULong kill)
{
  static int i = 0;
  ACE_DEBUG ((LM_DEBUG, "%d\n",i++));

  if (kill && stop_)
    ACE_OS::exit (1);
  if (test_start == 1)
    {
      this->start_ = ACE_OS::gethrtime ();
    }
  ACE_hrtime_t start = ACE_OS::gethrtime ();
  
  for (; work != 0; work--)
    ACE::is_prime (prime_number,
                   2,
                   prime_number / 2);
  ACE_hrtime_t end = ACE_OS::gethrtime ();

  ++state_;

  if (!CORBA::is_nil (agent_.in ()))
    agent_->state_changed (object_id_.c_str ());

  // ACE_DEBUG ((LM_DEBUG, "Time taken = %d\n", end - start));
  this->history_.sample (end - start);
  if (test_end == 1)
    {
      this->end_ = ACE_OS::gethrtime ();
    }
}

void
test_i::dump (void)
{
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_Basic_Stats stats;
  this->history_.collect_basic_stats (stats);
  stats.dump_results ("Total", gsf);
  ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                         this->end_ - this->start_,
                                         stats.samples_count ());
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
