// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION 
//    Test server for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "server.h"
#include "Property_Exporter.h"
#include "Machine_Properties.h"
#include "orbsvcs/Trader/Dynamic_Property.h"

// Callbacks made by the AVStreams library into the application.

Video_Server_StreamEndPoint::Video_Server_StreamEndPoint (void)
{
}

void 
Video_Server_StreamEndPoint::handle_stop (const AVStreams::flowSpec &the_spec,
                                          CORBA::Environment &env)
{
}

void 
Video_Server_StreamEndPoint::handle_start (const AVStreams::flowSpec &the_spec,  
                                           CORBA::Environment &env)
{
}

void 
Video_Server_StreamEndPoint::handle_destroy (const AVStreams::flowSpec &the_spec,  
                                             CORBA::Environment &env)
{
}

CORBA::Boolean 
Video_Server_StreamEndPoint::handle_connection_requested (AVStreams::StreamEndPoint_ptr initiator, 
                                                          CORBA::Boolean is_mcast, 
                                                          AVStreams::streamQoS &qos, 
                                                          AVStreams::flowSpec &the_spec,  
                                                          CORBA::Environment &env)
{
  return 1;
} 

int
export_properties (CORBA::ORB_ptr orb,
		   CORBA::Object_ptr mmdevice,
		   TAO_Machine_Properties& mach_props,
		   TAO_DP_Dispatcher& dprop)
{
  // Bootstrap to the Lookup interface.
  ACE_DEBUG ((LM_ERROR, "Bootstrap to the Lookup interface.\n"));
  CORBA::Object_var trading_obj =
    orb->resolve_initial_references ("TradingService");
  
  if (CORBA::is_nil (trading_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize the POA.\n"),
		      -1);

  TAO_TRY
    {
      ACE_DEBUG ((LM_ERROR, "Narrow the Lookup interface.\n"));
      // Narrow the lookup interface.
      ACE_DEBUG ((LM_DEBUG, "Narrowing the lookup interface.\n"));
      CosTrading::Lookup_var lookup_if = 
	CosTrading::Lookup::_narrow (trading_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Narrow the mmdevice to a property set reference. 
      CORBA::Any extra_info;
      CosPropertyService::PropertySet_ptr prop_set =
	CosPropertyService::PropertySet::_narrow (mmdevice, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Instantiate the property exporter helper class. 
      TAO_Property_Exporter prop_exporter (lookup_if, prop_set);
      
      // Add properties to server description.

      
      // Add the machine properties.
      ACE_DEBUG ((LM_ERROR, "Adding machine properties.\n"));
      for (int i = (int) TAO_Machine_Properties::CPU;
	   i <= (int) TAO_Machine_Properties::LOAD; i++)
	{
	  CORBA::Any extra_info;
	  const char* name = TAO_Machine_Properties::PROP_NAMES[i];
	  const CORBA::TypeCode_ptr prop_type = TAO_Machine_Properties::PROP_TYPE;
	  
	  CosTradingDynamic::DynamicProp* dp_struct = 
	    dprop.construct_dynamic_prop (name, prop_type, extra_info);
	  
	  dprop.register_handler (name, &mach_props);
	  prop_exporter.add_dynamic_property (name, *dp_struct);
	}
      
      ACE_DEBUG ((LM_ERROR, "Exporting to the Trader.\n"));
      CosTrading::OfferId_var offer_id =
	prop_exporter.export (mmdevice, "MMDevice", TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("export_properties");
    }
  TAO_ENDTRY;
}

// Main program.

int
main (int argc, char *argv[])
{
  TAO_TRY
    {
      TAO_ORB_Manager orb_manager;

      // Initialize the ORB.
      orb_manager.init (argc, 
                        argv,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create server-side MMDevice.
      TAO_Server_MMDevice <Video_Server_StreamEndPoint> *mmdevice_impl;
      ACE_NEW_RETURN (mmdevice_impl,
                      TAO_Server_MMDevice <Video_Server_StreamEndPoint>,
                      -1);

      // Activate the MMDevice, i.e., register with POA.
      CORBA::Object_var obj_ptr = mmdevice_impl->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Add properties
      TAO_DP_Dispatcher dprop_dispatcher;
      TAO_Machine_Properties mach_props;
      ::export_properties (orb_manager.orb (),
			   obj_ptr,
			   mach_props,
			   dprop_dispatcher);
      
      // Activate Dynamic Properties
      CORBA::String_var dp_ior =
	orb_manager.activate (&dprop_dispatcher, TAO_TRY_ENV);
      
      // Print the IOR.
      ACE_DEBUG ((LM_DEBUG,
		  "\nThe IOR is: <%s>\n", 
		  orb_manager.orb ()-> object_to_string (obj_ptr, TAO_TRY_ENV)));      
      TAO_CHECK_ENV;
	 
      // Run the ORB Event loop.
      orb_manager.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG,
              "\nServer is terminating"));
  return 0;
}

