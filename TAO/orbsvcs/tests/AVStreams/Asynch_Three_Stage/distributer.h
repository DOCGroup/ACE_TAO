/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Three_Stage
//
// = FILENAME
//    distributer.h
//
// = DESCRIPTION
//    Process to receive data from the sender and send it to the receiver
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#ifndef DISTRIBUTER_H
#define DISTRIBUTER_H

#include "orbsvcs/Naming/Naming_Utils.h"
//#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Endpoint_Strategy.h"
#include "orbsvcs/AV/Policy.h"

#include "Connection_Setup_Helper.h"

#define AV_MAX_STREAM_COUNT 2
// The distributer currently handles only two connections, one from
// the sender and one from the receiver.

class Distributer;

class Distributer_Receiver_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines a class for the distributer application callback
  //    for receiving data.
  //
  // = DESCRIPTION
  //    This class overides the methods of the TAO_AV_Callback so the 
  //    AVStreams can make upcalls to the application.
  
public:

  int receive_frame (ACE_Message_Block *frame,
                     TAO_AV_frame_info *frame_info,
                     const ACE_Addr &peer_address);
  // Method that is called when there is data to be received from the sender

  int handle_destroy (void);
  // Called when the sender has finished reading the file and wants
  // to close down the connection.
  
  void flowname (ACE_CString);
  ACE_CString flowname (void);
  // Accessor methods to set and get the flowname corresponding
  // to the callback
  

protected:
  ACE_CString flowname_;

};

class Distributer_Sender_Callback : public TAO_AV_Callback
{
  // = TITLE
  //    Defines a class for the distributer application callback
  //    for receiving data.
  //
  // = DESCRIPTION
  //    This class overides the methods of the TAO_AV_Callback so the 
  //    AVStreams can make upcalls to the application.
  
public:

  int handle_destroy (void);
  // Called when the sender has finished reading the file and wants
  // to close down the connection.
  
  void flowname (ACE_CString);
  ACE_CString flowname (void);
  // Accessor methods to set and get the flowname corresponding
  // to the callback
  

protected:
  ACE_CString flowname_;

};

class Distributer_Receiver_StreamEndPoint : public TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Defines the distributer aplication stream endpoint
  //
  // = DESCRIPTION
  //    This is the class that overrides the tao_server_endpoint to handle
  //    pre and post connect processing.
public:

  
  int set_protocol_object (const char *,
			   TAO_AV_Protocol_Object *object);
  // Store the reference to the protocol object corresponding
  // to the transport
  
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);
  // Create the distributer application sender callback.
  

private:
  Distributer_Receiver_Callback callback_;
  // Reference to the application callback.
};

class Distributer_Sender_StreamEndPoint : public TAO_Client_StreamEndPoint
{
  // = TITLE
  //    Defines the distributer aplication stream endpoint
  //
  // = DESCRIPTION
  //    This is the class that overrides the tao_server_endpoint to handle
  //    pre and post connect processing.
public:

  
  int set_protocol_object (const char *,
			   TAO_AV_Protocol_Object *object);
  // Store the reference to the protocol object corresponding
  // to the transport
  
  int get_callback (const char *flowname,
                    TAO_AV_Callback *&callback);
  // Create the distributer application sender callback.
  

private:
  Distributer_Sender_Callback callback_;
  // Reference to the application callback.
};


class Distributer
{
  // = TITLE
  //    Defines the distributer application class.
  // 
  // = DESCRIPTION
  //    The distributer progarm that acts as intermediate receiver
  //    that receives data from the sender process.
public:
  Distributer (void);
  // Constructor

  ~Distributer (void);
  // Destructor.

  int init (int argc,
            char **argv,
            CORBA::Environment &);
  // Initialize data components.

  int set_protocol_object  (const char*,
			    TAO_AV_Protocol_Object *object);
  TAO_AV_Protocol_Object* get_protocol_object (const char*);
  // Accessor methods to set/get the protocol object of the receiver/sender
  //  process

  void set_endpoint (const char* flowname, TAO_StreamEndPoint* endpoint);
  // Set the flowname and teh corresponding endpoint
  
  StreamEndpoint_Set* get_endpoint_set (void);
  // return the endpoint set
  
  StreamCtrl_Map* get_streamctrl_map (void);
  // Return teh stream control hash map

  void stream_created (void);
  // Called when stream created

  void stream_destroyed (void);
  // Called when stream destroyed

  Connection_Setup_Helper *helper (void);
  // Return the co0nnection setup helper

  int done (void);
  // Return the flag that suggests orb shutdown.

  Protocol_Object_Set* protocol_object_set (void);
  void protocol_object_set (Protocol_Object_Set *);
  // Accessor methods for the protocol object set

protected:

  int parse_args (int argc, char **argv);
  // Method to parse the command line arguments.

  TAO_AV_Endpoint_Reactive_Strategy_A 
  <Distributer_Sender_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> a_endpoint_strategy_;
  // The reactive strategy of the distributer receiver.

  TAO_AV_Endpoint_Reactive_Strategy_B 
  <Distributer_Receiver_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl> b_endpoint_strategy_;
  // The reactive strategy of the distributer receiver.

  TAO_MMDevice *distributer_receiver_mmdevice_;
  // The distributer receiver multimedia device

  TAO_MMDevice *distributer_sender_mmdevice_;
  // The distributer receiver multimedia device


  AVStreams::MMDevice_var distributer_receiver_mmdevice_obj_;

  AVStreams::MMDevice_var distributer_sender_mmdevice_obj_;
  
  AVStreams::MMDevice_var sender_mmdevice_;
  int count_;
  // Number of frames sent.
  
  int argc_;
  char **argv_;

  ACE_CString filename_;
  // File from which data is read.

  ACE_CString address_;
  // Address of the  distributer host machine or a multicast address - Default is
  // UDP multicast addess
  
  ACE_CString protocol_;
  // Selected protocol - default is UDP

  ACE_CString sender_device_name_;
  // The flow name of the stream set up between the 
  // sender and the distributer.

  ACE_CString distributer_device_name_;
  // The flow name of the stream set up between the 
  // receiver and the distributer.

  TAO_StreamCtrl sender_streamctrl_;
  // Video stream controller for the stream between sender and distributer
  
  int use_sfp_;
  // If set to 1 then use sfp as the flow carrier protocol.
  ACE_Message_Block mb;
  // Message block into which data is read from a file and then sent.

  int stream_count_;
  // Number of active streams. When a stream is disconnected this 
  // count is decremented.

  int done_;
  // Flag to indicate orb shutdown
  
  Recv_Obj_Ref_Set recv_obj_ref_set_;
  // Sequence of reciver object references obtained fro, the Naming Service
  
  StreamCtrl_Map streamctrl_map_;
  
  int default_port;
  //Default Port
  
  Protocol_Object_Set protocol_object_set_;
  // Contains a set of flownames and the corresponding protocol objects

  StreamEndpoint_Set endpoint_set_;
  // Contains a set of flownames and the corresponding stream endpoints

  Connection_Setup_Helper helper_;
  // The connection setuip helper
};

typedef ACE_Singleton<Distributer, ACE_Null_Mutex> DISTRIBUTER;

#endif /*DISTRIBUTER_H*/



