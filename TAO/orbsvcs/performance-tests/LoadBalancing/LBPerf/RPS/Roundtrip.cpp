//
// $Id$
//
#include "Roundtrip.h"
#include "ace/OS_NS_stdio.h"
#include "ace/ACE.h"

ACE_RCSID(LoadBalancing,
          Roundtrip,
          "$Id$")

Roundtrip::Roundtrip (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    count_ (0)
{
}

Test::Timestamp
Roundtrip::test_method (Test::Timestamp send_time,
                        Test::number  cl_number ,
                        Test::number  it_number 
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
//   if (it_number < 100)
//     {
//       printf("Client number is %d\n", cl_number);
//     }

//   if ((it_number % 2) == 0)
//     {
//       (void) ACE::is_prime (20000, 2, 10000);
//     }

  //std::set<std::string> dictionary;
  //std::string word;
  //std::ofstream output_file ("words.jai");

  if (cl_number % 2 == 0)
  {
    for (int i = 1; i < 10000; i++)
    {
      (void) ACE::is_prime (290090, 2, 190090);
    //std::ifstream in ("words");
    //while (getline (in, word))
    //{
      //output_file << word;
    //}
    }
  }

  //output_file.close ();

  return send_time;
}

Test::Timestamp
Roundtrip::test_method_2 (Test::Timestamp send_time,
                        Test::number /* cl_number */,
                        Test::number /* it_number */
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return send_time;
}

void
Roundtrip::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
