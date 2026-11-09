#include "test_i.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/Thread_Pool.h"
#include "ace/Countdown_Time.h"

test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                RTCORBA::Priority low_priority,
                RTCORBA::Priority high_priority)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    low_priority_ (low_priority),
    high_priority_ (high_priority)
{
  CORBA::Object_var const object =
    this->orb_->resolve_initial_references ("RTCurrent");

  this->rt_current_ =
    RTCORBA::Current::_narrow (object.in ());

  this->work_iterations_in_one_sec_ =
    this->estimate_iterations ();
}

void
test_i::method (CORBA::ULong work,
                char const *iteration)
{
  // Get the ORB_Core's TSS resources.
  TAO_ORB_Core_TSS_Resources * const tss =
    this->orb_->orb_core ()->get_tss_resources ();

  /// Get the lane attribute in TSS.
  TAO_Thread_Lane * const lane =
    static_cast<TAO_Thread_Lane *> (tss->lane_);

  RTCORBA::Priority const current_priority =
    this->rt_current_->the_priority ();

  char const *priority_string {"unknown"};
  if (current_priority == this->low_priority_)
    priority_string = "low";
  else if (current_priority == this->high_priority_)
    priority_string = "high";
  else
    {
      ACE_ERROR ((LM_ERROR, "ERROR: %d != %d and %d != %d\n",
                            current_priority, this->low_priority_,
                            current_priority, this->high_priority_));
      ACE_ASSERT (false);
    }

  ACE_DEBUG ((LM_DEBUG,
              "test_i::method - %s started at %T (%P|%t|%d|%d) for %d secs at priority %s\n",
              iteration,
              lane->pool ().id (),
              lane->id (),
              work,
              priority_string));

  this->work (work);

  ACE_DEBUG ((LM_DEBUG,
              "test_i::method - %s ended   at %T (%P|%t|%d|%d)\n",
              iteration,
              lane->pool ().id (),
              lane->id ()));

  ++this->current_iteration_;

  if (this->current_iteration_ == this->total_iterations_)
    {
      this->orb_->shutdown (false);
    }
}

PortableServer::POA_ptr
test_i::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::initialize (CORBA::ULong total_iterations)
{
  this->total_iterations_ = total_iterations;
}

int
test_i::estimate_iterations ()
{
  // Estimate for 2 seconds.
  ACE_Time_Value work_time (2);
  ACE_Countdown_Time countdown (&work_time);

  int i {};
  for (; work_time > ACE_Time_Value::zero; ++i)
    {
      ACE::is_prime (this->prime_number_,
                     2,
                     this->prime_number_ / 2);

      countdown.update ();
    }

  return i / 2;
}

void
test_i::work (CORBA::ULong work)
{
  int i = this->work_iterations_in_one_sec_ * work;

  for (; i > 0; --i)
    {
      ACE::is_prime (this->prime_number_,
                     2,
                     this->prime_number_ / 2);
    }
}
