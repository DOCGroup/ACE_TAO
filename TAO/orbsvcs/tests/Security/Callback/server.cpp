#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Get_Opt.h"
#include "server_i.h"

#include <memory>
#include <string>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("server.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s -o <iorfile>\n",
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
      Server_Options options;
      std::string env ("SSL_CERT_FILE=cacert.pem");
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (poa_object.in ());

      server_i *servant {};
      ACE_NEW_RETURN (servant, server_i (orb.in ()), -1);
      PortableServer::ServantBase_var const server_owner (servant);

      PortableServer::ObjectId_var const oid = poa->activate_object (servant);
      CORBA::Object_var const server_object = poa->id_to_reference (oid.in ());
      CORBA::String_var const server_ior = orb->object_to_string (server_object.in ());

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR\n")),
                            -1);
        ACE_OS::fprintf (output_file.get (), "%s", server_ior.in ());
      }

      PortableServer::POAManager_var const manager = poa->the_POAManager ();
      manager->activate ();

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P) calling orb->run () ...\n")));
      orb->run ();

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
