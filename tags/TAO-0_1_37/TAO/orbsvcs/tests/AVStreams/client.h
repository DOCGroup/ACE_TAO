/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    client.h
//
// = DESCRIPTION 
//    Test client for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#if !defined (AVSTREAMS_CLIENT_H)
#define AVSTREAMS_CLIENT_H

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "orbsvcs/Property/CosPropertyService_i.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/Get_Opt.h"
#include "tao/TAO.h"

class Client
{
  // = TITLE
  //    Client for A/V Streams demo.
  //
  // = DESCRIPTION
  //    Encapsulates client functionality.
public:
  Client (void);
  // Constructor.
  
  int init (int argc, 
            char **argv,
            CORBA::Environment &env);
  // Initialize the ORB etc, and bind the MMDevices.

  int run (CORBA::Environment &env);
  // Run the client.  This will call StreamCtrl::bind_devs.
  
  int property_tester (CORBA::Environment &env);
  // Testing  the property service methods.

protected:
  int bind_to_remote_mmdevice (int argc,
                               char *argv[],
                               CORBA::Environment &env);
  // Bind to the remote MMDevice.
  
  // = Property Testing.
  
  int test_define_property (CORBA::Environment &env);
  // Testing the define property method.
  
  int test_get_number_of_properties (CORBA::Environment &env);
  // Testing the number of properties, currently in the Property Set. 

  int test_get_all_property_names (CORBA::Environment &env);
  //Testing get_all_property_names.
  
  int test_get_properties (CORBA::Environment &env);
  //Testing get_properties.  Give the names and get their properties.

  int test_get_all_properties (CORBA::Environment &env);
  // Testing get_all_properties.
  
  int test_delete_property (CORBA::String property_name,
                            CORBA::Environment &env);
  // Testing delete_property.
  
  int test_delete_properties (CORBA::Environment &env);
  // Testing delete_properties.

  int test_define_properties (CORBA::Environment &env);
  // Defining a sequence of properties.

  TAO_ORB_Manager manager_;
  // The ORB manager, handles ORB initialization etc.
  
  AVStreams::MMDevice_var local_mmdevice_;
  // Our MMDevice.
  
  AVStreams::MMDevice_var remote_mmdevice_;
  // Servers MMDevice.
  
  AVStreams::StreamCtrl_var stream_ctrl_;
  // Stream Controller.
};

class Video_Client_StreamEndPoint
  : public virtual TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Stream Endpoint for the video client.
  //
  // = DESCRIPTION
  //    Implements the callbacks that are made by the AVStreams library.
public:
  Video_Client_StreamEndPoint (void);
  // Constructor.

  virtual void handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env);
  // Handles stream stoppage.
  
  virtual void handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env);
  // Handles stream start. Starts the flow of data.
  
  virtual void handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env);
  // Handles the destruction of the stream, tears down the transport.

  virtual CORBA::Boolean handle_connection_established (AVStreams::StreamEndPoint_ptr responder, 
                                                        AVStreams::streamQoS &qos_spec, 
                                                        const AVStreams::flowSpec &the_spec,  
                                                        CORBA::Environment &env);
  // Handles stream bind, establishes the transport.
};

#endif /* AVSTREAMS_CLIENT_H */
