// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "Foo_i.h"
#include "ace/SString.h"

const ACE_TCHAR *ior_output_file = 0;
const char *cert_file = "cacert.pem";

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <iorfile>"
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) ERROR: Unable to initialize the "
                           "POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      obj =
        orb->resolve_initial_references ("SecurityLevel3:SecurityCurrent");

      SecurityLevel3::SecurityCurrent_var security_current =
        SecurityLevel3::SecurityCurrent::_narrow (obj.in ());

      if (CORBA::is_nil (security_current.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) ERROR: SecurityCurrent reference "
                           "is nil.\n"),
                          1);

      Foo_i *server_impl = 0;

      ACE_NEW_RETURN (server_impl,
                      Foo_i (orb.in (),
                             security_current.in ()),
                      -1);

      PortableServer::ServantBase_var owner_transfer (server_impl);

      Foo::Bar_var server =
        server_impl->_this ();

      // Sanity check on SSLIOP profile equivalence.
      // Since the POA is reference counting the servants, this
      // implementation must still exist when the POA is destroyed.
      Foo_i server_impl2 (orb.in (), security_current.in ());
      Foo::Bar_var server2 =
        server_impl2._this ();

      const CORBA::Boolean equivalent =
        server->_is_equivalent (server2.in ());

      if (equivalent)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: SSLIOP profile equivalence "
                      "check failed.\n"));
        }

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG,
                  "\n"
                  "Event loop finished.\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
