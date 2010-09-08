// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"
#include "testC.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char *cert_file = "cacert.pem";

CORBA::Boolean call_shutdown = 0;
int wait_time = 10;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("sk:w:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 's':
        call_shutdown = 1;
        break;
      case 'w':
        wait_time = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "CLIENT (%P): usage:  %s "
                           "-k <ior> [-s] -w <wait-time>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      ACE_CString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CLIENT (%P): Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      for (CORBA::ULong i = 0; i < 2; i++)
        {
          try
            {
              ACE_DEBUG ((LM_DEBUG,
                          "CLIENT (%P): Connecting to the server...\n"));

              // If we are retrying then make just one request.
              do
                {
                  server->send_line ("some data");
                }
              while (i == 0);
            }
          catch (const CORBA::COMM_FAILURE&)
            {
              // If this happens second time then we are done.
              if (i != 0)
                throw;

              // Waiting for server to come back
              ACE_DEBUG ((LM_DEBUG,
                          "CLIENT (%P): Caught CORBA::COMM_FAILURE. "
                          "Assuming server crashed and will come up soon.\n"
                          "CLIENT (%P): Waiting for %d seconds...\n",
                          wait_time));
              ACE_OS::sleep (wait_time);
            }
          catch (const CORBA::TRANSIENT&)
            {
              // If this happens second time then we are done.
              if (i != 0)
                throw;

              // Waiting for server to come back
              ACE_DEBUG ((LM_DEBUG,
                          "CLIENT (%P): Caught CORBA::TRANSIENT. "
                          "Assuming server crashed and will come up soon.\n"
                          "CLIENT (%P): Waiting for %d seconds...\n",
                          wait_time));
              ACE_OS::sleep (wait_time);
            }
        }

      if (call_shutdown)
        {

          // Let other clients to finish their task if any
          ACE_OS::sleep (wait_time);

          ACE_DEBUG ((LM_DEBUG,
                      "CLIENT (%P): Calling shutdown...\n"));

          try
            {
              server->shutdown ();
            }
          catch (const CORBA::COMM_FAILURE&)
            {
              // Ignored
            }
          catch (const CORBA::TRANSIENT&)
            {
              // Ignored
            }
        }

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG,
                  "CLIENT (%P): Done.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CLIENT (%P): Caught exception:");
      return 1;
    }

  return 0;
}
