/* -*- C++ -*- */

/* $Id$ */

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
#include "orbsvcs/AV/AVStreams_i.h"
#include "ace/Get_Opt.h"
#include "tao/tao_util.h"

class Client
// = TITLE
//    Client for A/V Streams demo
// = DESCRIPTION
//    Encapsulates client functionality
{
public:
  // Constructor
  Client (void);
  
  // Initialize the ORB etc, and bind the MMDevices
  int init (int argc, 
            char **argv,
            CORBA::Environment &env);

  // Run the client. This will call StreamCtrl::bind_devs
  int run (CORBA::Environment &env);

protected:
  // Bind to the remote MMDevice
  int bind_to_remote_mmdevice (int argc,
                               char **argv,
                               CORBA::Environment &env);

  // The ORB manager, handles ORB initialization etc.
  TAO_ORB_Manager manager_;
  
  // Our MMDevice
  AVStreams::MMDevice_var local_mmdevice_;

  // Servers MMDevice
  AVStreams::MMDevice_var remote_mmdevice_;

  // Stream Controller
  AVStreams::StreamCtrl_var stream_ctrl_;
};

class Video_Client_StreamEndPoint
  : public virtual TAO_Client_StreamEndPoint
// = TITLE
//    Stream Endpoint for the video client
// = DESCRIPTION
//    Implements the callbacks that are made by the AVStreams library
{
public:
  // Constructor
  Video_Client_StreamEndPoint ();

  // Handles stream stoppage
  virtual void handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env);
  
  // Handles stream start. Starts the flow of data
  virtual void handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env);
  
  
  // Handles the destruction of the stream, tears down the transport
  virtual void handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env);

  // Handles stream bind, establishes the transport
  virtual CORBA::Boolean handle_connection_established (AVStreams::StreamEndPoint_ptr responder, 
                                                        AVStreams::streamQoS &qos_spec, 
                                                        const AVStreams::flowSpec &the_spec,  
                                                        CORBA::Environment &env);


};



#endif /* AVSTREAMS_CLIENT_H */
