#include "Hello_i.h"
#include "HelloC.h"
#include "Client_ORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/debug.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include <atomic>
#include <iostream>
#include <string>

std::basic_string<ACE_TCHAR> const server1_ior {ACE_TEXT ("file://server1.ior")};
extern std::basic_string<ACE_TCHAR> const server2_ior {ACE_TEXT ("file://server2.ior")};
std::basic_string<ACE_TCHAR> const server1_shutdown_ior {ACE_TEXT ("file://server1_shutdown.ior")};
std::basic_string<ACE_TCHAR> const server2_shutdown_ior {ACE_TEXT ("file://server2_shutdown.ior")};
CORBA::ORB_var orb {};
Demo::HelloWorld_var server1_shutdownObj {};
Demo::HelloWorld_var server2_shutdownObj {};
std::atomic_bool reconnected {};
std::atomic_bool caught_exception {};

class ClientTask : public ACE_Task_Base
{
public:
  explicit ClientTask (int test_duration)
    : test_duration_ (test_duration)
  {
  }

  ClientTask (ClientTask const &) = delete;
  ClientTask (ClientTask &&) = delete;
  ClientTask &operator= (ClientTask const &) = delete;
  ClientTask &operator= (ClientTask &&) = delete;

  int svc () override
  {
    CORBA::Object_var const hello_obj =
      orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (server1_ior.c_str ()));
    Demo::HelloWorld_var const hello = Demo::HelloWorld::_narrow (hello_obj.in ());
    if (CORBA::is_nil (hello.in ()))
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("hello reference is nil\n")));
        return 0;
      }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("ClientTask start.\n")));
    int i {};
    ACE_Time_Value const due = ACE_OS::gettimeofday () + ACE_Time_Value (this->test_duration_);

    while (ACE_OS::gettimeofday () < due)
      {
        try
          {
            ++i;
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t)======client calling server1 sayHello %d\n"),
                        i));
            char const *const message = " server1 say Hello";
            hello->sayHello (message);
            ACE_OS::sleep (2);

            if (caught_exception.load ())
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) client reconnected to server 1\n")));
                reconnected = true;
              }
          }
        catch (CORBA::TRANSIENT const &)
          {
            caught_exception = true;
            if (TAO_debug_level > 0)
              ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("sayHello() expected TRANSIENT exceptions.\n")));
          }
        catch (...)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("sayHello() caught unknown exception\n")));
          }
      }
    return 0;
  }

private:
  int const test_duration_;
};

int
parse_args (int argc, ACE_TCHAR *argv[], int &test_duration)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("t:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 't':
        test_duration = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -t <test duration>\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer {};
      ACE_NEW_RETURN (temp_initializer, Client_ORBInitializer (), -1);
      // The var takes ownership of the ORB initializer's initial reference.
      PortableInterceptor::ORBInitializer_var const initializer (temp_initializer);
      PortableInterceptor::register_orb_initializer (initializer.in ());

      orb = CORBA::ORB_init (argc, argv);

      int test_duration {30};
      if (parse_args (argc, argv, test_duration) != 0)
        return -1;

      CORBA::Object_var const shutdown_obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (server1_shutdown_ior.c_str ()));
      server1_shutdownObj = Demo::HelloWorld::_narrow (shutdown_obj.in ());
      if (CORBA::is_nil (server1_shutdownObj.in ()))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("server1 shutdown object reference is nil\n")));
          return 1;
        }
      CORBA::Object_var const shutdown_obj2 =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (server2_shutdown_ior.c_str ()));
      server2_shutdownObj = Demo::HelloWorld::_narrow (shutdown_obj2.in ());
      if (CORBA::is_nil (server2_shutdownObj.in ()))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("server2 shutdown object reference is nil\n")));
          return 1;
        }

      ClientTask task (test_duration);
      if (task.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot activate client threads\n"), 1);

      task.wait ();

      server1_shutdownObj->shutdown ();
      server2_shutdownObj->shutdown ();
      server1_shutdownObj = Demo::HelloWorld::_nil ();
      server2_shutdownObj = Demo::HelloWorld::_nil ();
      orb->destroy ();
      orb = CORBA::ORB::_nil ();

      if (!reconnected.load ())
        ACE_ERROR_RETURN ((LM_ERROR, "Client was not able to reconnect to server 1.\n"), 1);
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Unexpected exception: " << ex << '\n';
      return 1;
    }

  return 0;
}
