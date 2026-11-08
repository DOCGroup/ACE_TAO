#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"
#include "testC.h"
#include <string>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  bool call_shutdown {};
  int wait_time {10};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("sk:w:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 's':
        options.call_shutdown = true;
        break;
      case 'w':
        options.wait_time = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("CLIENT (%P): usage:  %s -k <ior> [-s] -w <wait-time>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::string env ("SSL_CERT_FILE=cacert.pem");
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Client_Options options;

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (options.ior.c_str ());

      Simple_Server_var server = Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("CLIENT (%P): Object reference <%s> is nil\n"),
                             options.ior.c_str ()),
                            1);
        }

      constexpr CORBA::ULong attempt_count {2};
      for (CORBA::ULong i {}; i < attempt_count; ++i)
        {
          try
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("CLIENT (%P): Connecting to the server...\n")));

              // If we are retrying then make just one request.
              do
                {
                  server->send_line ("some data");
                }
              while (i == 0);
            }
          catch (CORBA::COMM_FAILURE const &)
            {
              // If this happens second time then we are done.
              if (i != 0)
                throw;

              // Waiting for server to come back
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("CLIENT (%P): Caught CORBA::COMM_FAILURE. "
                                    "Assuming server crashed and will come up soon.\n"
                                    "CLIENT (%P): Waiting for %d seconds...\n"),
                          options.wait_time));
              ACE_OS::sleep (options.wait_time);
            }
          catch (CORBA::TRANSIENT const &)
            {
              // If this happens second time then we are done.
              if (i != 0)
                throw;

              // Waiting for server to come back
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("CLIENT (%P): Caught CORBA::TRANSIENT. "
                                    "Assuming server crashed and will come up soon.\n"
                                    "CLIENT (%P): Waiting for %d seconds...\n"),
                          options.wait_time));
              ACE_OS::sleep (options.wait_time);
            }
        }

      if (options.call_shutdown)
        {
          // Let other clients to finish their task if any
          ACE_OS::sleep (options.wait_time);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("CLIENT (%P): Calling shutdown...\n")));

          try
            {
              server->shutdown ();
            }
          catch (CORBA::COMM_FAILURE const &)
            {
              // Ignored
            }
          catch (CORBA::TRANSIENT const &)
            {
              // Ignored
            }
        }

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CLIENT (%P): Done.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CLIENT (%P): Caught exception:");
      return 1;
    }

  return 0;
}
