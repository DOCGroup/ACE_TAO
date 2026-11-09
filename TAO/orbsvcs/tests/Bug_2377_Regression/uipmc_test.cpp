
#include "ace/Task.h"
#include "ace/Mutex.h"
#include "Hello_Impl.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "ace/Get_Opt.h"
#include "ace/Manual_Event.h"
#include "tao/Policy_Manager.h"

constexpr int client_sleep_time = 1000;
constexpr int client_thread_number = 32;
constexpr int hello_call_number = 100;

struct Client_Options
{
  ACE_TString uipmc_url;
  ACE_TString client_uipmc_url;
};

void
test_sleep (int microsec)
{
  ACE_Time_Value const tv (microsec / 1000000, microsec % 1000000);

  ACE_OS::sleep (tv);
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("u:c:"));
  constexpr unsigned char full_success = 0x03;
  unsigned char success {};

  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'u':
          options.uipmc_url = get_opts.opt_arg ();
          success |= 0x01;
          break;
        case 'c':
          options.client_uipmc_url = get_opts.opt_arg ();
          success |= 0x02;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage:  %s -u <url> -c <client_url>\n"),
                             argv [0]),
                            -1);
        }
    }

  if (success != full_success)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage:  %s -u <url> -c <client_url>\n"),
                       argv [0]),
                      -1);

  // Indicates successful parsing of the command line
  return 0;
}

class ORBThread
  : public ACE_Task_Base
{
public:
  ORBThread (CORBA::ORB_ptr orb, ACE_Manual_Event &me, ACE_Thread_Manager *thr_mgr)
    : ACE_Task_Base (thr_mgr)
    , orb_ (CORBA::ORB::_duplicate (orb))
    , me_ (me)
  {
  }

  ORBThread (ORBThread const &) = delete;
  ORBThread (ORBThread &&) = delete;
  ORBThread &operator= (ORBThread const &) = delete;
  ORBThread &operator= (ORBThread &&) = delete;

  int svc () override
  {
    try
      {
        this->me_.signal ();
        this->orb_->run ();
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught in ORBThread:");
        return -1;
      }

    return 0;
  }

private:
  CORBA::ORB_var orb_;
  ACE_Manual_Event &me_;
};

class ClientThread
  : public ACE_Task_Base
{
public:
  ClientThread (Test::Hello_ptr hello, MessageLog &log, int calls)
    : hello_ (Test::Hello::_duplicate (hello))
    , logger_ (log)
    , call_count_ (calls)
    , count_ {}
  {
  }

  ClientThread (ClientThread const &) = delete;
  ClientThread (ClientThread &&) = delete;
  ClientThread &operator= (ClientThread const &) = delete;
  ClientThread &operator= (ClientThread &&) = delete;

  int svc () override
  {
    try
      {
        while (true)
          {
            int c {};
            {
              ACE_GUARD_RETURN (ACE_Mutex, ace_mon, this->mutex_, 0);
              c = this->count_++;
            }

            if (c >= this->call_count_)
              break;

            this->hello_->say_hello (c);

            this->logger_.register_message_send (c);

            test_sleep (client_sleep_time);
          }
      }
    catch (CORBA::Exception const &ex)
      {
        ex._tao_print_exception ("Exception caught in ClientThread:");
        return -1;
      }

    return 0;
  }

private:
  ACE_Mutex mutex_;
  Test::Hello_var hello_;
  MessageLog &logger_;
  int call_count_;
  int count_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  MessageLog logger (hello_call_number);

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableGroup::GOA_var root_poa =
        PortableGroup::GOA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                           1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Hello_Impl *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello_Impl (orb.in (), logger),
                      1);
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 2;

      // create UIPMC reference
      CORBA::Object_var miop_ref =
        orb->string_to_object (options.uipmc_url.c_str ());

      // create UIPMC reference for client access
      CORBA::Object_var client_miop_ref =
        orb->string_to_object (options.client_uipmc_url.c_str ());

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
        ClientThread cln_thr (hello.in (), logger, hello_call_number);
        cln_thr.activate (THR_NEW_LWP | THR_JOINABLE, client_thread_number);
        cln_thr.wait ();

        // shutdown the server, after 10 invocations of shutdown() we can be
        // more or less sure that server actually received that call
        for (int i = 0; i < 10; ++i)
          hello->shutdown ();
        orb_thr.wait ();
      }

      if (logger.report_statistics () == hello_call_number)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("\n (%P|%t) No single call got through to the server\n")),
                          3);

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main ():");
      return 4;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n (%P|%t) uipmc_test is successful..\n")));
  return 0;
}
