//$Id$

////////////////////////////////////////////////////////////////////////
// This files tests the Client exposed policies:
//     - PriorityModelPolicy
//     - PriorityBandedConnectionPolicy
//     - ClientProtocolPolicy
//
// This policies are embedded in the object reference, by writem
// them into the IOR. 
//
//
#include "Counter_i.h"
#include <tao/RT_ORB.h>
#include <tao/RT_Policy_i.h>
#include <ace/Arg_Shifter.h>

#include "RT_Properties.h"

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
           
      ACE_Arg_Shifter arg_shifter (argc, argv);
      
      RT_Properties *rt_object_properties = 0;
      RT_Properties *rt_poa_properties = 0;

      while (arg_shifter.is_anything_left ())
        {
          char *arg;
          // IOR File Name Option.
          if ((arg = arg_shifter.get_the_parameter ("-POAConfigFile")))
            {
              rt_poa_properties = RT_Properties::read_from (arg, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else if ((arg = arg_shifter.get_the_parameter ("-ObjectConfigFile")))
            {
              rt_object_properties = RT_Properties::read_from (arg, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else 
            arg_shifter.consume_arg ();
          
        }  
      
      CORBA::Object_var object;
      
      // Get a reference to the RT-ORB.
      object = orb->resolve_initial_references ("RTORB", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (3);
      
      
      // Create the priority policy using the RT-ORB.
      RTCORBA::Priority priority = rt_poa_properties->priority ();
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              priority,
                                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      // Create priority Banded Connection Policy.
      RTCORBA::PriorityBands poa_priority_bands = rt_poa_properties->priority_bands ();
      
      poa_policy_list[1] = 
        rt_orb->create_priority_banded_connection_policy (poa_priority_bands, 
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

      poa_policy_list[2] =
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
                         poa_policy_list);
      
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

      FILE *output_file = ACE_OS::fopen (rt_poa_properties->ior_source (), "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           rt_poa_properties->ior_source ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
   
      // Now we create an object that overrides some of the policies
      // set at the POA level.


      // Create a Corba Object reference, using the policies
      // set at the POA level.
      
      // @@ Shortcut - The following code is not definitive, and 
      //               the cast is only used to access a RTPortableServer::POA
      //               method that isn't currently accessible otherwise.
      
      object =
        ((TAO_POA*)child_poa.ptr ())->create_reference_with_priority ("IDL:Counter:1.0",
                                                                      rt_object_properties->priority (),
                                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT ("Reference Created!\n")));

      if (!check_reference (object, 
                            "Unable to create a Counter Object!\n"))
        return 1;

      Counter_var counter_over = Counter::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      if (!check_reference (counter_over.in(), 
                            "Unable to create a Counter Object!\n"))
        return 1;

      
      CORBA::String_var o_ior = orb->object_to_string (counter_over.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT ("Activated as <%s>\n"), o_ior.in ()));
      
      output_file = ACE_OS::fopen (rt_object_properties->ior_source (), "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                           rt_object_properties->ior_source ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", o_ior.in ());
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

