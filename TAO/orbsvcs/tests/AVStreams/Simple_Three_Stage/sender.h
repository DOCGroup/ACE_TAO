/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Simple_Three_Stage
//
// = FILENAME
//    sender.h
//
// = DESCRIPTION
//    Sender that reads data from a file and sends it
//    at a requested data rate.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================


#ifndef TAO_AV_SENDER_H
#define TAO_AV_SENDER_H

#include "ace/Get_Opt.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"

class Sender_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines a class for the sender application callback.
  //
  // = DESCRIPTION
  //    This class overides the methods of the TAO_AV_Callback so the 
  //    AVStreams can make upcalls to the application.

public:
  int handle_start (void);
  // Method called when the device is ready to start sending data
};

class Sender_StreamEndPoint  : public TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Defines the sender stream endpoint.
  // 
  // = DESCRIPTION
  //    This class overrides the methods of TAO_ClientStreamendpoint 
  //    so the application can perform its processing during post and pre
  //    connection set up.
  
public:
  Sender_StreamEndPoint (void);
  //Contructor

  virtual int get_callback (const char *flowname,
                            TAO_AV_Callback *&callback);
  // Create the application client callback and return its handle to the 
  // AVStreams for further application callbacks

  virtual int set_protocol_object (const char *flowname,
                                   TAO_AV_Protocol_Object *object);
  // Set protocol object corresponding to the transport protocol chosen.
  
protected:
  Sender_Callback callback_;
  // Reference to the client application callback.
};

typedef TAO_AV_Endpoint_Reactive_Strategy_B <Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> ENDPOINT_STRATEGY;


class Sender
{
  // = TITLE
  //    Defines the Client Application
  // 
  // = DESCRIPTION
  //    The actual client program that streams data
  //    to the distributers that are waiting for data.
public:
  Sender (void);
  // Constructor

  int init (int argc, 
	    char **argv,
	    CORBA::Environment&);
  // Method to initialize the various data components.
  
  void set_protocol_object (TAO_AV_Protocol_Object *protocol_object);
  // Set the protocol object corresponding to the transport protocol chosen.

  void set_endpoint (Sender_StreamEndPoint* endpoint);
  Sender_StreamEndPoint* get_endpoint (void);
  // Accessor methods for setting/getting the sender stream endpoint

  int pace_data (CORBA::Environment&);
  // Method to pace and send data from a file.

  FILE *file (void);
  // File handle from which data is read to be sent.

private:
  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.
  
  int register_sender (CORBA::Environment& ACE_TRY_ENV);
  // Method that binds the client to the Naming Service
  
  ENDPOINT_STRATEGY endpoint_strategy_;
  // The reactive strategy of the client.
  
  AVStreams::MMDevice_var receiver_mmdevice_;
  // The receiver MMDevice that the sender connects to
  
  TAO_MMDevice* sender_mmdevice_;
  // The sender MMDevice.
  
  Sender_StreamEndPoint* endpoint_;
  // Reference to the sender streamendpoint
  
  int count_;
  // Number of frames sent.
  
  ACE_CString filename_;
  // File from which data is read.

  TAO_Naming_Client my_naming_client_;
  // The Naming Service client.

  FILE *fp_;
  // File handle of the file read from.

  int frame_rate_;
  // The sepcified data frame rate
  
  ACE_Message_Block mb_;
  // Message block into which data is read from a file and then sent.

  TAO_AV_Protocol_Object* protocol_object_;
  // Protocol object corresponding to the transport protocol selected.

};

typedef ACE_Singleton<Sender,ACE_Null_Mutex> SENDER;
// Create a singleton instance of the Sender.

#endif /* TAO_AV_FTP_H */


