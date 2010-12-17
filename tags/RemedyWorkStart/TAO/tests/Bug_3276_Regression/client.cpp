// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "testC.h"
#include "tao/ORB_Constants.h"

const ACE_TCHAR *proxy_ior = 0;
const ACE_TCHAR *control_ior = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var obj =
        orb->string_to_object (proxy_ior);

      if (obj.in () == 0)
        {
          ACE_ERROR_RETURN  ((LM_ERROR,
                              "The received object is nil\n"),
                              -1);
        }

      Simple_Server_var server =
        Simple_Server::_narrow (obj.in ());

      if (CORBA::is_nil (server.in ()))
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
            server->remote_call ();
            ACE_OS::sleep (2);
          }
        }
      catch (CORBA::TRANSIENT& ex)
        {
          CORBA::ULong m = ex.minor () & 0x00000F80u;
          if (m == TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE &&
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

      obj =
        orb->string_to_object (control_ior);

      if (obj.in () == 0)
        {
          ACE_ERROR_RETURN  ((LM_ERROR,
                              "The received objref is NULL\n"),
                              -1);
        }

      server =
        Simple_Server::_narrow (obj.in ());

      server->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception & ex)
    {
      ex._tao_print_exception ("Exception caught in client:");
      return 1;
    }

  return 0;
}
