//$Id$
#ifndef CONNECTION_SETUP_HELPER_H
#define CONNECTION_SETUP_HELPER_H

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Three_Stage
//
// = FILENAME
//    Connection_Setup_Helper.h
//
// = DESCRIPTION
//    This is a helper class that allows the senders bind to multiple receivers
//    and receivers to bind to senders. It also lets the senders and receivers
//    disconnect the streams set up.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================



#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "tao/PortableServer/PortableServer.h"


class Recv_Obj_Ref_Item
{
  // = TITLE
  //    Defines a class which stores the receiver device name
  //    and its reference.
  // 
  // = DESCRIPTION
  //    This class defines a component to store the receiver
  //    device name and refernce.
  
 public:
  ACE_CString name;
  // Receiver MMDevice Name

  AVStreams::MMDevice_var ref;
  // Receiver MMDevice Object Reference
};

class StreamCtrl_Item
{
  // = TITLE
  //    Defines a class which stores the flow name
  //    and the corresponding stream controller
  // 
  // = DESCRIPTION
  //    This class defines a component to store  the flow name
  //    and the corresponding stream controller
  
 public:
  ACE_CString flowname;
  // The name of the stream

  TAO_StreamCtrl *streamctrl;
  // The corresponding stream controller
};

class Protocol_Object_Item
{
  // = TITLE
  //    Defines a class which stores the flow name
  //    and the corresponding protocol object
  // 
  // = DESCRIPTION
  //    This class defines a component to store  the flow name
  //    and the corresponding protocol object

 public:
  ACE_CString flowname;
  // Name of the stream

  TAO_AV_Protocol_Object* protocol_object;
  // The corresponding protocol object
};

class StreamEndpoint_Item
{
  // = TITLE
  //    Defines a class which stores the flow name
  //    and the corresponding stream endpoint
  // 
  // = DESCRIPTION
  //    This class defines a component to store  the flow name
  //    and the corresponding streamendpoint

 public:
  ACE_CString flowname;
  // The name of the stream

  TAO_StreamEndPoint* endpoint;
  // The corresponding stream enpoint
};


// Set of receiver device names and thier object refences
typedef ACE_Unbounded_Set <Recv_Obj_Ref_Item*> Recv_Obj_Ref_Set;
typedef ACE_Unbounded_Set_Iterator <Recv_Obj_Ref_Item*> Recv_Obj_Ref_Set_Itor;

// Set of flow names and the corresponding protocol objects
typedef ACE_Unbounded_Set <Protocol_Object_Item*> Protocol_Object_Set;
typedef ACE_Unbounded_Set_Iterator <Protocol_Object_Item*> Protocol_Object_Set_Itor;

// Set of flow names and the corresponding stream endpoints
typedef ACE_Unbounded_Set <StreamEndpoint_Item*> StreamEndpoint_Set;
typedef ACE_Unbounded_Set_Iterator <StreamEndpoint_Item*> StreamEndpoint_Set_Itor;

// Hash map of flow names and corresponding stream controllers.
typedef ACE_Hash_Map_Manager <TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex> StreamCtrl_Map;
typedef ACE_Hash_Map_Iterator <TAO_String_Hash_Key, StreamCtrl_Item*, ACE_Null_Mutex> StreamCtrl_Map_Iterator;
typedef ACE_Hash_Map_Entry <TAO_String_Hash_Key, StreamCtrl_Item*> StreamCtrl_Map_Entry;

class Connection_Setup_Helper
{
  // = TITLE
  //    Defines the Connection Setup Helper Class.
  // 
  // = DESCRIPTION
  //    This is a helper class that allows the senders bind to multiple receivers
  //    and receivers to bind to senders. It also lets the senders and receivers
  //    disconnect the streams set up.
  
 public:
  
  Connection_Setup_Helper (void);
  // Constructor
  
  ~Connection_Setup_Helper (void);
  // Destructor

  int init (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);
  // Initialize the helper

  int bind_to_receivers (ACE_CString *sender_device_name,
                         AVStreams::MMDevice_ptr sender_mmdevice_obj,
                         Recv_Obj_Ref_Set &recv_obj_ref_set,
                         CORBA::Environment& ACE_TRY_ENV);
  // Method that binds the client to the Naming Service and retreives
  // the references of the receivers
  
  int connect_to_receivers (ACE_CString *sender_device_name,
                            ACE_CString *flow_protocol_str,
                            ACE_CString *protocol,
                            ACE_CString *host_address,
                            AVStreams::MMDevice_ptr sender_mmdevice_obj,
                            Recv_Obj_Ref_Set *recv_obj_ref_set,
                            StreamCtrl_Map &streamctrl_map,
                            CORBA::Environment&);
  //Method that sets up connections between the sender and receivers
  
  int bind_to_sender (ACE_CString *sender_device_name,
                      ACE_CString *receiver_device_name,
                      AVStreams::MMDevice_ptr &sender_mmdevice_obj,
                      AVStreams::MMDevice_ptr receiver_mmdevice_obj, 
                      CORBA::Environment&);
  // Bind receiver to the sender
  
  int connect_to_sender (ACE_CString *sender_device_name,
                         ACE_CString *receiver_device_name,
                         ACE_CString *flow_protocol_str,
                         ACE_CString *protocol,
                         ACE_CString *host_address,
                         AVStreams::MMDevice_ptr &sender_mmdevice_obj,
                         AVStreams::MMDevice_ptr receiver_mmdevice_obj,
                         StreamCtrl_Map &streamctrl_map,
                         CORBA::Environment &ACE_TRY_ENV);
  // Connect the receiver with the sender

  int disconnect (StreamEndpoint_Set *endpoint_set,
                  StreamCtrl_Map *streamctrl_map,
                  CORBA::Environment&);
  // Disconnect all the streams
    
 protected:
  int create_recv_obj_ref_list (CosNaming::BindingList &bl,
                                ACE_CString *sender_device_name,
                                Recv_Obj_Ref_Set &recv_obj_ref_set,
                                CORBA::Environment& ACE_TRY_ENV);
  // Retrieve the receiver device names and the corresponding object
  // references and store them in the recv_obj_ref_set
  
  TAO_Naming_Client my_naming_client_;
  // The Naming Service client.
  
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  
};

#endif /*CONNECTION_SETUP_HELPER_H*/
