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

Video_Server_StreamEndPoint::Video_Server_StreamEndPoint ()
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
main (int argc, char ** argv)
{
  TAO_TRY
    {
      TAO_ORB_Manager m;

      // Initialize the ORB
      m.init (argc, 
              argv,
              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create server-side MMDevice
      TAO_Server_MMDevice <Video_Server_StreamEndPoint> *mmdevice_impl 
        = new TAO_Server_MMDevice <Video_Server_StreamEndPoint>;
      TAO_CHECK_ENV;

      // Activate the MMDevice, i.e. register with POA
      CORBA::String_var s;
      s = m.activate (mmdevice_impl,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Print the IOR
      ACE_DEBUG ((LM_DEBUG,
                  "\nThe IOR is: <%s>\n", 
                  s.in ()));

      // Run the ORB Event loop
      m.run (TAO_TRY_ENV);
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
  ACE_DEBUG ((LM_DEBUG, "\nServer is terminating"));
  return 0;
}

