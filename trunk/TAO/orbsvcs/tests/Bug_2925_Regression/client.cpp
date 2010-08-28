//
// $Id$
//

#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "tao/Policy_Manager.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/Client_Network_Priority_Policy.h"
#include "HelloC.h"

#define CLIENT_SLEEP_TIME       1000     /* in microseconds */
#define CLIENT_THREAD_NUMBER    32
#define HELLO_CALL_NUMBER       100

#define IPDSFIELD_DSCP_DEFAULT  0x00
#define IPDSFIELD_DSCP_CS1      0x08
#define IPDSFIELD_DSCP_CS2      0x10
#define IPDSFIELD_DSCP_CS3      0x18
#define IPDSFIELD_DSCP_CS4      0x20
#define IPDSFIELD_DSCP_CS5      0x28
#define IPDSFIELD_DSCP_CS6      0x30
#define IPDSFIELD_DSCP_CS7      0x38
#define IPDSFIELD_DSCP_AF11     0x0A
#define IPDSFIELD_DSCP_AF12     0x0C
#define IPDSFIELD_DSCP_AF13     0x0E
#define IPDSFIELD_DSCP_AF21     0x12
#define IPDSFIELD_DSCP_AF22     0x14
#define IPDSFIELD_DSCP_AF23     0x16
#define IPDSFIELD_DSCP_AF31     0x1A
#define IPDSFIELD_DSCP_AF32     0x1C
#define IPDSFIELD_DSCP_AF33     0x1E
#define IPDSFIELD_DSCP_AF41     0x22
#define IPDSFIELD_DSCP_AF42     0x24
#define IPDSFIELD_DSCP_AF43     0x26
#define IPDSFIELD_ECT_MASK      0x02
#define IPDSFIELD_CE_MASK       0x01
#define IPDSFIELD_DSCP_EF       0x2E

static int const dscp[] =
{
  IPDSFIELD_DSCP_DEFAULT ,
  IPDSFIELD_DSCP_CS1     ,
  IPDSFIELD_DSCP_CS2     ,
  IPDSFIELD_DSCP_CS3     ,
  IPDSFIELD_DSCP_CS4     ,
  IPDSFIELD_DSCP_CS5     ,
  IPDSFIELD_DSCP_CS6     ,
  IPDSFIELD_DSCP_CS7     ,
  IPDSFIELD_DSCP_AF11    ,
  IPDSFIELD_DSCP_AF12    ,
  IPDSFIELD_DSCP_AF13    ,
  IPDSFIELD_DSCP_AF21    ,
  IPDSFIELD_DSCP_AF22    ,
  IPDSFIELD_DSCP_AF23    ,
  IPDSFIELD_DSCP_AF31    ,
  IPDSFIELD_DSCP_AF32    ,
  IPDSFIELD_DSCP_AF33    ,
  IPDSFIELD_DSCP_AF41    ,
  IPDSFIELD_DSCP_AF42    ,
  IPDSFIELD_DSCP_AF43    ,
  IPDSFIELD_DSCP_EF
};

const ACE_TCHAR *ior_file = 0;

void
test_sleep (int microsec)
{
  ACE_Time_Value tv (microsec / 1000000, microsec % 1000000);

  ACE_OS::sleep ((const ACE_Time_Value &) tv);
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  const unsigned char full_success = 0x01;
  unsigned char success = 0;

  do
    {
      int c = get_opts ();
      if (success == full_success && c == -1)
        break;

      if (c == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -k <iorfile>"
                           "\n",
                           argv [0]),
                           -1);

      switch (c)
        {
        case 'k':
          ior_file = get_opts.opt_arg ();
          success |= 0x01;
          break;
        }
    }
  while (true);

  // Indicates sucessful parsing of the command line
  return 0;
}


class ClientThread
  : public ACE_Task_Base
{
public:
  ClientThread (Test::Hello_ptr hello, CORBA::Short calls)
    : hello_ (Test::Hello::_duplicate (hello))
    , call_count_ (calls)
    , count_ (0)
  {
  }

  virtual int
  svc ()
  {
    try
      {
        while (true)
          {
            CORBA::Short c = 0;
            {
              ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
              c = this->count_++;
            }

            if (c >= this->call_count_)
              break;

            this->hello_->say_hello (c);

            ACE_DEBUG ((LM_DEBUG,
                        "%02d>>; ",
                        c));

            test_sleep (CLIENT_SLEEP_TIME);
          }
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught in ClientThread:");
        return -1;
      }

    return 0;
  }

private:
  ACE_Mutex mutex_;
  Test::Hello_var hello_;
  CORBA::Short call_count_;
  CORBA::Short count_;
};


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 2;

      // create UIPMC reference
      CORBA::Object_var miop_ref =
        orb->string_to_object (ior_file);

      // create Hello reference
      Test::Hello_var hello =
        Test::Hello::_unchecked_narrow (miop_ref.in ());

      if (CORBA::is_nil (hello.in ()))
        return 5;

      TAO::DiffservCodepoint request_dscp = dscp[12];
      TAO::DiffservCodepoint reply_dscp = dscp[13];

      CORBA::PolicyList policy_list;
      policy_list.length (1);

      CORBA::Policy_var client_network_policy =
        orb->_create_policy (TAO::CLIENT_NETWORK_PRIORITY_TYPE);

      TAO::NetworkPriorityPolicy_var nw_priority =
        TAO::NetworkPriorityPolicy::_narrow (client_network_policy.in ());

      nw_priority->request_diffserv_codepoint (
           request_dscp);

      nw_priority->reply_diffserv_codepoint (
           reply_dscp);

      nw_priority->network_priority_model (
        TAO::CLIENT_PROPAGATED_NETWORK_PRIORITY);

      policy_list[0] = TAO::NetworkPriorityPolicy::_duplicate (nw_priority.in ());

      CORBA::Object_var over_ridden_object = hello->_set_policy_overrides (
         policy_list, CORBA::SET_OVERRIDE);

      policy_list[0]->destroy ();

      hello = Test::Hello::_unchecked_narrow (over_ridden_object.in ());

      {
        // start clients
        ClientThread cln_thr (hello.in (), HELLO_CALL_NUMBER);
        cln_thr.activate (THR_NEW_LWP | THR_JOINABLE, CLIENT_THREAD_NUMBER);
        cln_thr.wait ();
      }

      // shutdown the server, after 10 invocations of shutdown() we can be
      // more or less sure that server actually received that call
      for (int i = 0; i < 10; i++)
        hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client main ():");
      return 4;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n (%P|%t) client finished successfully..\n"));
  return 0;
}
