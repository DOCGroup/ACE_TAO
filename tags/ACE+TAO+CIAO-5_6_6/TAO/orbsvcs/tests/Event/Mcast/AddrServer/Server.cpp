// $Id$

// AddrServerMain.cpp
// Main program for a common Address server instance to be shared by all
// event channels

#include "TestAddrServer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
#if defined ACE_HAS_IPV6
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      const ACE_TCHAR *ior_file_name = ACE_TEXT ("as.ior");
      const char *address = "224.6.6.1";
      const char *address6 = "FF01::ABCD";
      u_short port = 12345;

      for (int i = 0; argv[i] != 0; i++)
        {
          if (ACE_OS::strcasecmp (argv[i], ACE_TEXT ("-o")) == 0)
            {
              if (argv[i+1] != 0)
                ior_file_name = argv[++i];
            }

        }

      // Get the POA
      CORBA::Object_var tmpobj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (tmpobj.in ());
      PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      // Initialize the address server with the desired address.
      // This will be used by the sender object and the multicast
      // receiver.
      ACE_INET_Addr addr (port, address);
      ACE_INET_Addr addr6 (port, address6);

      TestAddrServer_i impl (addr,addr6);

      PortableServer::ObjectId_var oid = poa->activate_object(&impl);
      tmpobj = poa->id_to_reference(oid.in());

      CORBA::String_var ior = orb->object_to_string (tmpobj.in());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_file_name, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file %s for writing IOR: %s\n",
                           ior_file_name,
                           ACE_TEXT_CHAR_TO_TCHAR (ior.in ())),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // run the ORB
      orb->run();
      orb->destroy();
      return 0;
    }
  catch (CORBA::Exception& exc)
    {
      ACE_ERROR ((LM_ERROR,
                  "Caught CORBA::Exception\n%s (%s)\n",
                  ACE_TEXT_CHAR_TO_TCHAR (exc._name ()),
                  ACE_TEXT_CHAR_TO_TCHAR (exc._rep_id ()) ));
    }
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("This test requires ACE to be built ")
              ACE_TEXT ("with support for IPv6\n")));
#endif /* ACE_HAS_IPV6 */

  return 1;
}
