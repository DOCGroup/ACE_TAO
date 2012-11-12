// $Id$

#include "ace/OS_main.h"
#include "ace/Log_Msg.h"

#include "pub_qos_test.h"
#include "sub_qos_test.h"
#include "dw_qos_test.h"
#include "dr_qos_test.h"
#include "tp_qos_test.h"
#include "dp_qos_test.h"
#include "states_test.h"
#include "string_seq.h"

int handle_result (const int & result,
                   const char * test)
{
  int ret = 0;
  if (result == -1)
    {
      ACE_ERROR ((LM_ERROR, "ERROR POLICIES : "
                            "Exception caught while testing "
                            "%C QoS\n",
                            test));
    }
  else if (result > 0)
    {
      ret = result;
      ACE_ERROR ((LM_ERROR, "ERROR POLICIES : "
                            "<%d> errors found while testing "
                            "%C QoS\n",
                            result, test));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "%C QoS test passed!\n",
                             test));
    }
  return ret;
}

bool
test_string_seq ()
{
  DDS::StringSeq foo (2);
  foo.length (2);
  foo[0] = CORBA::string_dup ("bar");
  foo[1] = CORBA::string_dup ("foo");
  DDS_StringSeq rti_foo;
  rti_foo <<= foo;
  DDS::StringSeq tao_foo;
  tao_foo <<= rti_foo;
  return StringSequence::check (rti_foo, tao_foo, "StringSeq");
}

int
ACE_TMAIN (int , ACE_TCHAR **)
{
  PublisherPolicyTest publisher;
  SubscriberPolicyTest subscriber;
  DatawriterPolicyTest datawriter;
  DatareaderPolicyTest datareader;
  TopicPolicyTest topic;
  StatesTest states;
  DomainParticipantPolicyTest domainparticipant;

  int ret = 0;
  int test = 0;
  try
    {
      test = publisher.run ();
      ret += handle_result (test, "Publisher");
      test = subscriber.run ();
      ret += handle_result (test, "Subscriber");
      test = datawriter.run ();
      ret += handle_result (test, "Datawriter");
      test = datareader.run ();
      ret += handle_result (test, "DataReader");
      test = topic.run ();
      ret += handle_result (test, "Topic");
      test = domainparticipant.run ();
      ret += handle_result (test, "DomainParticipant");
      test = states.run ();
      ret += handle_result (test, "States");
      ret += test_string_seq ();
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "ERROR: Unexpected exception caught."));
      return -1;
    }
  return 0;
}
