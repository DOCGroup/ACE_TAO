/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Three_Stage
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
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "Connection_Setup_Helper.h"

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

class Sender_StreamEndPoint  : public TAO_Client_StreamEndPoint
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

typedef TAO_AV_Endpoint_Reactive_Strategy_A <Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> ENDPOINT_STRATEGY;


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
  
  void set_protocol_object (const char* flowname, TAO_AV_Protocol_Object *protocol_object);
  // Set the protocol object corresponding to the transport protocol chosen.

  void set_endpoint (const char* flowname, TAO_StreamEndPoint* endpoint);
  // Set the flowname and the corresponding endpoint

  int pace_data (CORBA::Environment&);
  // Method to pace and send data from a file.

  FILE *file (void);
  // File handle from which data is read to be sent.

  int frame_rate (void);
  // The requested frame rate for sending each frame of data read from the file.
  
private:
  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.
  

  ENDPOINT_STRATEGY endpoint_strategy_;
  // The reactive strategy of the client.

  AVStreams::MMDevice_var sender_mmdevice_obj_;
  // The receiver MMDevice that the sender connects to

  TAO_MMDevice* sender_mmdevice_;
  // The sender MMDevice.
  
  TAO_StreamCtrl streamctrl_;
  // Video stream controller

  int count_;
  // Number of frames sent.
  
  int argc_;
  char **argv_;

  ACE_CString filename_;
  // File from which data is read.

  ACE_CString address_;
  // Address of the sender host machine or a multicast address - Default is
  // UDP multicast addess


  FILE *fp_;
  // File handle of the file read from.

  ACE_CString protocol_;
  // Selected protocol - default is UDP

  ACE_CString sender_device_name_;
  // Teh flowname_ of the stream set up between the sender and receiver.

  int frame_rate_;
  // The sepcified data frame rate
  
  ACE_Message_Block mb;
  // Message block into which data is read from a file and then sent.

  Recv_Obj_Ref_Set recv_obj_ref__set_;
  // Sequence of reciver object references obtained fro, the Naming Service

  StreamCtrl_Map streamctrl_map_;
  
  int default_port;
  // Default port

  Recv_Obj_Ref_Set recv_obj_ref_set_;
  // Set of the Receiver object references

  Protocol_Object_Set protocol_object_set_;

  StreamEndpoint_Set endpoint_set_;

  Connection_Setup_Helper helper_;
};

typedef ACE_Singleton<Sender,ACE_Null_Mutex> SENDER;
// Create a singleton instance of the Sender.

#endif /* TAO_AV_FTP_H */


