//
// $Id$
//

#include "ace/Task.h"
#include "ace/Mutex.h"
#include "Hello_Impl.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "ace/Get_Opt.h"
#include "ace/Manual_Event.h"
#include "tao/Policy_Manager.h"

#define CLIENT_SLEEP_TIME       1000     /* in microseconds */
#define CLIENT_THREAD_NUMBER    32
#define HELLO_CALL_NUMBER       100

const ACE_TCHAR *uipmc_url = 0;
const ACE_TCHAR *client_uipmc_url = 0;

void
test_sleep (int microsec)
{
  ACE_Time_Value tv (microsec / 1000000, microsec % 1000000);

  ACE_OS::sleep ((const ACE_Time_Value &) tv);
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("u:c:"));
  const unsigned char full_success = 0x03;
  unsigned char success = 0;

  do
    {
      int c = get_opts ();
      if (success == full_success && c == -1)
        break;

      if (c == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -u <url>"
                           " -c <client_url>"
                           "\n",
                           argv [0]),
                           -1);

      switch (c)
        {
        case 'u':
          uipmc_url = get_opts.opt_arg ();
          success |= 0x01;
          break;
        case 'c':
          client_uipmc_url = get_opts.opt_arg ();
          success |= 0x02;
          break;
        }
    }
  while (true);

  // Indicates sucessful parsing of the command line
  return 0;
}


class ORBThread
  : public ACE_Task_Base
{
public:
  ORBThread (CORBA::ORB_ptr orb, ACE_Manual_Event& me, ACE_Thread_Manager *thr_mgr)
    : ACE_Task_Base (thr_mgr)
    , orb_ (CORBA::ORB::_duplicate (orb))
    , me_ (me)
  {
  }

  virtual int
  svc ()
  {
    try
      {
        this->me_.signal ();
        this->orb_->run ();
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught in ORBThread:");
        return -1;
      }

    return 0;
  }

private:
  CORBA::ORB_var orb_;
  ACE_Manual_Event& me_;
};


class ClientThread
  : public ACE_Task_Base
{
public:
  ClientThread (Test::Hello_ptr hello, MessageLog *log, int calls)
    : hello_ (Test::Hello::_duplicate (hello))
    , logger_ (log)
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
            int c = 0;
            {
              ACE_GUARD (ACE_Mutex, ace_mon, this->mutex_);
              c = this->count_++;
            }

            if (c >= this->call_count_)
              break;

            this->hello_->say_hello (c);

            this->logger_->register_message_send (c);

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
  MessageLog *logger_;
  int call_count_;
  int count_;
};


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  MessageLog logger (HELLO_CALL_NUMBER);

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableGroup::GOA_var root_poa =
        PortableGroup::GOA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                           1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Hello_Impl* hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello_Impl (orb.in (), &logger),
                      1);
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      if (parse_args (argc, argv) != 0)
        return 2;

      // create UIPMC reference
      CORBA::String_var multicast_url =
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(uipmc_url));
      CORBA::Object_var miop_ref =
        orb->string_to_object (multicast_url.in ());

      // create UIPMC reference for client access
      CORBA::String_var client_multicast_url =
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(client_uipmc_url));
      CORBA::Object_var client_miop_ref =
        orb->string_to_object (client_multicast_url.in ());

      // create ids
      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (miop_ref.in ());
      PortableServer::ObjectId_var client_id =
        root_poa->create_id_for_reference (client_miop_ref.in ());

      // activate Hello Object
      root_poa->activate_object_with_id (id.in (),
                                         hello_impl);

      // create Hello reference
      Test::Hello_var hello =
        Test::Hello::_unchecked_narrow (client_miop_ref.in ());

      poa_manager->activate ();

      {
        // start orb thread
        ACE_Manual_Event me;
        ORBThread orb_thr (orb.in (), me, ACE_Thread_Manager::instance ());
        orb_thr.activate (THR_NEW_LWP | THR_JOINABLE, 1, 1);
        me.wait ();

        // start clients
        ClientThread cln_thr (hello.in (), &logger, HELLO_CALL_NUMBER);
        cln_thr.activate (THR_NEW_LWP | THR_JOINABLE, CLIENT_THREAD_NUMBER);
        cln_thr.wait ();

        // shutdown the server, after 10 invocations of shutdown() we can be
        // more or less sure that server actually received that call
        for (int i = 0; i < 10; i++)
          hello->shutdown ();
        orb_thr.wait ();
      }

      if (logger.report_statistics () == HELLO_CALL_NUMBER)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\n (%P|%t) No single call got through to the server\n"),
                           3);

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main ():");
      return 4;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n (%P|%t) uipmc_test is successful..\n"));
  return 0;
}
