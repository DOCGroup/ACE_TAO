#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"
#include "tao/ORB_Constants.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&proxy_ior,
            ACE_TCHAR const *&control_ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:c:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'p':
        proxy_ior = get_opts.opt_arg ();
        break;
      case 'c':
        control_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-c <control>"
                           "-p <proxy>"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  ACE_TCHAR const *proxy_ior {};
  ACE_TCHAR const *control_ior {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, proxy_ior, control_ior) == -1)
        return -1;

      CORBA::Object_var const proxy_object = orb->string_to_object (proxy_ior);

      if (CORBA::is_nil (proxy_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "The received object is nil\n"),
                            -1);
        }

      Simple_Server_var const proxy = Simple_Server::_narrow (proxy_object.in ());

      if (CORBA::is_nil (proxy.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference is nil\n"),
                            -1);
        }

      try
        {
          while (true)
            {
              // Make a remote call
              proxy->remote_call ();
              ACE_OS::sleep (2);
            }
        }
      catch (CORBA::TRANSIENT const &ex)
        {
          CORBA::ULong const minor = ex.minor () & 0x00000F80u;
          if (minor == TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE &&
              ex.completed () == CORBA::COMPLETED_NO)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "TRANSIENT caught in client as it was expected.\n"));
            }
          else
            {
              ex._tao_print_exception ("Unexpected TRANSIENT caught in client:");
              return 2;
            }
        }

      CORBA::Object_var const control_object = orb->string_to_object (control_ior);

      if (CORBA::is_nil (control_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "The received objref is NULL\n"),
                            -1);
        }

      Simple_Server_var const control_server = Simple_Server::_narrow (control_object.in ());

      control_server->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client:");
      return 1;
    }

  return 0;
}
