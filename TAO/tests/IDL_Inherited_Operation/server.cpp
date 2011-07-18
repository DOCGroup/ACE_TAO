// $Id$

#include "InheritedOp_i.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Derived_i *derived_impl = 0;
      ACE_NEW_RETURN (derived_impl,
                      Derived_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer (derived_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (derived_impl);

      CORBA::Object_var object =
        root_poa->id_to_reference (id.in ());

      Outer::Derived_var derived =
        Outer::Derived::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (derived.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      
      if (output_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output ")
                             ACE_TEXT ("file for writing IOR: %s\n"),
                             ior_output_file),
                             1);
        }
        
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (true, true);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server - Exception caught:");
      return 1;
    }

  return 0;
}
