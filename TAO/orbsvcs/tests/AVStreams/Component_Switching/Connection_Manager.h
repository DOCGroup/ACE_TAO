//$Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams/Component_Switching
//
// = FILENAME
//    Connection_Manager.h
//
// = DESCRIPTION
//    This is a helper class that allows the senders bind to multiple
//    receivers and receivers to bind to senders. It also lets the
//    senders and receivers disconnect streams.
//
// = AUTHOR
//    Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
//
// ============================================================================

#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "tao/PortableServer/PortableServer.h"

class Connection_Manager
{
  // = TITLE
  //    Defines the Connection Manager.
  //
  // = DESCRIPTION
  //    This is a helper class that allows the senders bind to
  //    multiple receivers and receivers to bind to senders. It also
  //    lets the senders and receivers disconnect streams.
public:

  Connection_Manager (void);
  // Constructor

  ~Connection_Manager (void);
  // Destructor

  int init (CORBA::ORB_ptr orb);
  // Initialize this class.

  void bind_to_receivers (const ACE_CString &sender_name,
                          AVStreams::MMDevice_ptr sender
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Method that binds the sender to the Naming Service and retreives
  // the references of any registered receivers.

  void connect_to_receivers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Connect to the receivers that we found.

  void bind_to_sender (const ACE_CString &sender_name,
                       const ACE_CString &receiver_name,
                       AVStreams::MMDevice_ptr receiver
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Bind receiver to the sender.

  void connect_to_sender (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Connect to the sender that we found.

  void destroy (const ACE_CString &flowname
                ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Destroy streams associated with <flowname>.

  void unbind_sender (const ACE_CString &sender_name,
                      AVStreams::MMDevice_ptr sender_mmdevice
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS) ;
  // Unbind the sender from the Naming Service

  void unbind_receiver (const ACE_CString &sender_name,
                        const ACE_CString &receiver_name,
                        AVStreams::MMDevice_ptr receiver_mmdevice);
  // Unbind the Receiver from the Naming Service

  void add_streamctrl (const ACE_CString &flowname,
                       TAO_StreamEndPoint *endpoint
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  // Add new streamctrl.

  // Map of receivers.
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               AVStreams::MMDevice_var,
                               ACE_Null_Mutex>
          Receivers;

  // Map of protocol objects.
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               TAO_AV_Protocol_Object *,
                               ACE_Null_Mutex>
          Protocol_Objects;

  // Map of streamctrl.
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               AVStreams::StreamCtrl_var,
                               ACE_Null_Mutex>
          StreamCtrls;

  // Map accessors.
  Receivers &receivers (void);
  Protocol_Objects &protocol_objects (void);
  StreamCtrls &streamctrls (void);

protected:

  void find_receivers (ACE_ENV_SINGLE_ARG_DECL);

  void add_to_receivers (CosNaming::BindingList &binding_list
                         ACE_ENV_ARG_DECL);

  TAO_Naming_Client naming_client_;
  // The Naming Service client.

  // The maps.
  Receivers receivers_;
  Protocol_Objects protocol_objects_;
  StreamCtrls streamctrls_;

  // Sender name.
  ACE_CString sender_name_;

  // Sender device.
  AVStreams::MMDevice_var sender_;

  // Sender context.
  CosNaming::NamingContext_var sender_context_;

  // Receiver name.
  ACE_CString receiver_name_;

  // Receiver device.
  AVStreams::MMDevice_var receiver_;

  // Receiver context.
  CosNaming::NamingContext_var receiver_context_;
};

#endif /*CONNECTION_MANAGER_H*/
