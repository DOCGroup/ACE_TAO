// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"

test_i::test_i (CORBA::ORB_ptr orb, const ACE_TCHAR *file_name)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    client_done_ (false),
    client_done_file_(file_name)
{
}

void
test_i::method (CORBA::ULong request_number,
                const test::data &)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("server: Iteration %d @ %T\n"),
              request_number));

  if (!client_done_)
    {
      FILE *input_file = ACE_OS::fopen (client_done_file_, "r");
      if (input_file == 0)
        {
          // Time required to process this request.
          ACE_Time_Value work_time (0,
                                    3000 * 1000);

          ACE_OS::sleep (work_time);
        }
      else
        {
          // The client is long gone because the run_test.pl has created
          // the file to let us know. There's no need to keep on with
          // these pointless sleeps
          client_done_ = true;
          ACE_OS::fclose (input_file);
        }
    }
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) About to invoke shudown... "));
  this->orb_->shutdown (0);
}
