#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include <memory>

test_i::test_i (CORBA::ORB_ptr orb, ACE_TString const& file_name)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    client_done_file_ (file_name)
{
}

void
test_i::method (CORBA::ULong request_number,
                test::data const&)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("server: Iteration %u @ %T\n"),
              request_number));

  if (!client_done_)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> const input_file (
        ACE_OS::fopen (client_done_file_.c_str (), ACE_TEXT ("r")), ACE_OS::fclose);
      if (!input_file)
        {
          // Time required to process this request.
          ACE_Time_Value const work_time (0, 3000 * 1000);

          ACE_OS::sleep (work_time);
        }
      else
        {
          // The client is long gone because the run_test.pl has created
          // the file to let us know. There's no need to keep on with
          // these pointless sleeps
          client_done_ = true;
        }
    }
}

void
test_i::shutdown ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) About to invoke shutdown... ")));
  this->orb_->shutdown (false);
}
