//$Id$
//
// This files tests the Client exposed policies:
//     - PriorityModelPolicy
//     - PriorityBandedConnectionPolicy
//     - ClientProtocolPolicy
//
// This policies are embedded in the object reference, by writem
// them into the IOR. 
//
//
// TODO: Create another object reference and override its POA's policies
//       using the CORBA::Object::_set_policy_ovverides method.
//      
//
#include "Counter_i.h"
#include <tao/RT_ORB.h>
#include <tao/RT_Policy_i.h>
#include <ace/Arg_Shifter.h>

const char *ior_output_file = "s.ior";

ACE_RCSID(tao, server, "$Id$");


CORBA::Boolean check_reference (CORBA::Object_ptr object,
                                const char *msg = 0)
{
  if (CORBA::is_nil (object))
    {
      if (msg == 0)
        ACE_DEBUG ((LM_DEBUG, 
                    ACE_TEXT ("The Object reference is nil.\n")));
      else
        ACE_DEBUG ((LM_DEBUG, 
                    ACE_TEXT (msg)));
      return 0;
    }
  return 1;
}

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // ORB Initialization.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TAO", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Here we parse the command line paramether passed
      // to the application.
      /*
      ACE_Arg_Shifter arg_shifter (argc, argv);
      
      char *arg;
      while (arg_shifter.is_anything_left ())
        {
        }  
      */
      CORBA::Object_var object;
      
      // Get a reference to the RT-ORB.
      object = orb->resolve_initial_references ("RTORB", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      CORBA::PolicyList policy_list;
      policy_list.length (3);
      
      // Create the priority policy using the RT-ORB.
      RTCORBA::Priority priority = 10;
      policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              priority,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Create priority Banded Connection Policy.
      // In order to do this we have to create a sequence
      // of priority bands.

      RTCORBA::PriorityBands priority_bands;
      priority_bands.length (2);
      
      RTCORBA::PriorityBand p_band1, p_band2;
      
      p_band1.low = 10;
      p_band1.high = 30;
      
      p_band2.low = 7;
      p_band2.high = 21;
      
      priority_bands[0] = p_band1;
      priority_bands[1] = p_band2;
      
      policy_list[1] = 
        rt_orb->create_priority_banded_connection_policy (priority_bands, 
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Client Protocol Policy.
      RTCORBA::ProtocolList protocol_list;
      protocol_list.length (1);
      
      protocol_list[0].protocol_type = IOP::TAG_INTERNET_IOP;
      protocol_list[0].orb_protocol_properties = 
        TAO_Protocol_Properties_Factory::create_orb_protocol_property (IOP::TAG_INTERNET_IOP);

      protocol_list[0].transport_protocol_properties =
        TAO_Protocol_Properties_Factory::create_transport_protocol_property (IOP::TAG_INTERNET_IOP);

      policy_list[2] =
        rt_orb->create_client_protocol_policy (protocol_list);

      object = orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_mgr =
        PortableServer::POAManager::_nil ();
      
      PortableServer::POA_var child_poa =
        poa->create_POA ("Child_POA",
                         poa_mgr ,
                         policy_list);
      
      // Create a Corba Object reference, using the policies
      // set at the POA level.
      object =
        child_poa->create_reference ("IDL:Counter:1.0",
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT ("Reference Created!\n")));

      if (!check_reference (object, 
                            "Unable to create a Counter Object!\n"))
        return 1;

      Counter_var counter = Counter::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (!check_reference (counter.in(), 
                            "Unable to create a Counter Object!\n"))
        return 1;

      Counter_Servant servant (orb.in ());

      // TODO: Incarnate the CORBA Object with  servant.

      CORBA::String_var ior =
        orb->object_to_string (counter.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT ("Activated as <%s>\n"), ior.in ()));

      FILE *output_file = ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      PortableServer::POAManager_var poa_manager =
        child_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, 
                           ACE_TEXT ("CORBA Exception Raised."));
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

