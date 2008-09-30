// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "test_i.h"

ACE_RCSID (Redirection,
           server,
           "$Id$")

const ACE_TCHAR *ior_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:c:n:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file = get_opts.opt_arg ();
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <IOR> ",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (::parse_args (argc, argv) != 0)
        return -1;

      test_i *test_impl = 0;
      ACE_NEW_RETURN (test_impl, test_i (1, orb.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (test_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      RedirectionTest::test_var test =
        RedirectionTest::test::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      root_poa->destroy (1, 1 );

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Server event loop finished.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return -1;
    }

  return 0;
}
