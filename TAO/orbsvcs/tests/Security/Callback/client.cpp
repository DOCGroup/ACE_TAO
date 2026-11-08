#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdlib.h"
#include "client_i.h"

#include <string>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://server.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s -k <ior>\n"),
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
      Client_Options options;
      std::string env ("SSL_CERT_FILE=cacert.pem");
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const server_object = orb->string_to_object (options.ior);
      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P) orb->string_to_object failed.\n")),
                          -1);

      server_var const server_obj = server::_narrow (server_object.in ());
      if (CORBA::is_nil (server_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P) server::_narrow(obj) failed.\n")),
                          -1);

      client_i *servant {};
      ACE_NEW_RETURN (servant, client_i (server_obj.in ()), -1);
      // ServantBase_var owns the initial reference; the POA retains its activated reference.
      PortableServer::ServantBase_var const client_owner (servant);
      client_var const client_ref = servant->_this ();

      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      server_obj->set_client (client_ref.in ());
      server_obj->test_request ("first secure callback to client");

      server_obj->set_client (client_ref.in ());
      server_obj->test_request ("second secure callback to client");

      server_obj->shutdown ();

      poa->destroy (true, true);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught exception\n"));
      return -1;
    }

  return 0;
}
