#include "ami_ccm_i.h"
#include "tao/debug.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(AMI, ami_test_i, "$Id$")

AMI_CCM_i::AMI_CCM_i (CORBA::ORB_ptr orb)
    :  orb_ (CORBA::ORB::_duplicate (orb)),
       answer_count_ (0)
{
}

CORBA::Long
AMI_CCM_i::asynch_foo (const char* string, ::CORBA::Long client_id,
          ::CORBA::String_out answer)
{
  try
    {
      ACE_OS::sleep (ACE_OS::rand () % 2);
      ++answer_count_;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%N:%l:(%P:%t):AMI_CCM_i::foo:  %d %C\n"),
                  answer_count_,
                  string));
      char tmp[255] = {0};
      ACE_OS::sprintf(tmp, "Hello World %d", answer_count_);
      answer = CORBA::string_dup (tmp);
      return client_id;
    }
  catch (const CORBA::Exception& e)
    {
      throw A::InternalError (123, ACE_TEXT ("CORBA exception"));
    }
  catch (...)
    {
      throw A::InternalError (456, ACE_TEXT ("Unknow exception"));
    }
  //answer = answer_count_;
}

void
AMI_CCM_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
