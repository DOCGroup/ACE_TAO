// $Id$

#include "Multiple_Impl.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (tests, server, "$Id$")

int main (int argc, char *argv[])
{


  try
    {
      // Orb Initialization
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TAO");

      CORBA::Object_var object;
      object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa = PortableServer::POA::_narrow(object.in());


      // Get the POAManager
      PortableServer::POAManager_var poa_manager = poa->the_POAManager();

      // Create the servant.
      Bottom_Impl servant (orb.in ());

      // Create the delegated servant and intialize it
      // with the "real" servant.
      PortableServer::ObjectId_var id = poa->activate_object (&servant);
      CORBA::Object_var object_act = poa->id_to_reference (id.in ());
      Delegated_Bottom_Impl delegated_servant(
                              Multiple::Bottom::_narrow (object_act.in ()),
                              orb.in ());

      // Create the CORBA Object that is incarnated by the
      // delegated servant.
      id = poa->activate_object (&delegated_servant);
      object_act = poa->id_to_reference (id.in ());

      Multiple::Bottom_var bottom =
        Multiple::Bottom::_narrow (object_act.in ());

      // Now we stringfy the object reference.
      CORBA::String_var ior =
        orb->object_to_string (bottom.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it

      FILE *output_file= ACE_OS::fopen ("s.ior", "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s"),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      // Activate the POAManager
      poa_manager->activate();

      orb->run();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Multiple Execution Interrupted Exception!\n");
      return 1;
    }
  return 0;
}
