//$Id$
#include "Job_i.h"

#include "tao/debug.h"

#include "Activity.h"

Job_i::Job_i ()
{
}

const ACE_CString&
Job_i::name (void)
{
  return job_name_;
}

const ACE_CString&
Job_i::poa (void)
{
  return POA_name_;
}

int
Job_i::init (ACE_Arg_Shifter& arg_shifter)
{
  // Read the name of the Job
  job_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
  arg_shifter.consume_arg ();

  // Read the name of the POA
  POA_name_ = ACE_TEXT_ALWAYS_CHAR(arg_shifter.get_current ());
  arg_shifter.consume_arg ();

  return 0;
}

void
Job_i::work (CORBA::ULong work)
{
  static CORBA::ULong prime_number = 9619;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::method: %hd units of work\n",
                work));

  for (; work != 0; work--)
    ACE::is_prime (prime_number,
                   2,
                   prime_number / 2);
}

void
Job_i::shutdown (void)
{
  ACTIVITY::instance()->job_ended (this);
}
