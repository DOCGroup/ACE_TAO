// $Id$

#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");
const ACE_TCHAR *uipmc_ior = ACE_TEXT("corbaloc:miop:1.0@1.0-domain-1/127.0.0.1:23232");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
      orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();
      poa_manager->activate ();

      CORBA::Object_var obj =orb->string_to_object (uipmc_ior);

      CORBA::Object_var tmp = orb->string_to_object (ior);
      server_var server = server::_narrow (tmp.in ());

      try
        {
          server->method (obj.in());

          ACE_DEBUG ((LM_DEBUG, "Test passed !!\n"));
        }
      catch (const CORBA::SystemException& marshal)
        {
          ACE_DEBUG ((LM_ERROR, "Test Failed - Regression. "
                      "Sending UIPMC object ref to server failed with:\n"));
          marshal._tao_print_exception ("Exception :\n");
          result = 1;
        }

      server->shutdown ();
    }
  catch(const CORBA::SystemException& e)
    {
        e._tao_print_exception ("Unexpected exception - not a regression.\n");
        result = 1;
    }
    return result;
}
