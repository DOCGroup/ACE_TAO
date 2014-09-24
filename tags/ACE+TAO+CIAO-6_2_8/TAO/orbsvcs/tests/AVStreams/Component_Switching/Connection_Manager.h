
//=============================================================================
/**
 *  @file    Connection_Manager.h
 *
 *  $Id$
 *
 *  This is a helper class that allows the senders bind to multiple
 *  receivers and receivers to bind to senders. It also lets the
 *  senders and receivers disconnect streams.
 *
 *
 *  @author Yamuna Krishnamurthy <yamuna@cs.wustl.edu>
 */
//=============================================================================


#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "tao/PortableServer/PortableServer.h"

/**
 * @class Connection_Manager
 *
 * @brief Defines the Connection Manager.
 *
 * This is a helper class that allows the senders bind to
 * multiple receivers and receivers to bind to senders. It also
 * lets the senders and receivers disconnect streams.
 */
class Connection_Manager
{
public:

  /// Constructor
  Connection_Manager (void);

  /// Destructor
  ~Connection_Manager (void);

  /// Initialize this class.
  int init (CORBA::ORB_ptr orb);

  /// Method that binds the sender to the Naming Service and retrieves
  /// the references of any registered receivers.
  void bind_to_receivers (const ACE_CString &sender_name,
                          AVStreams::MMDevice_ptr sender);

  /// Connect to the receivers that we found.
  void connect_to_receivers (void);

  /// Bind receiver to the sender.
  void bind_to_sender (const ACE_CString &sender_name,
                       const ACE_CString &receiver_name,
                       AVStreams::MMDevice_ptr receiver);

  /// Connect to the sender that we found.
  void connect_to_sender (void);

  /// Destroy streams associated with <flowname>.
  void destroy (const ACE_CString &flowname);

  /// Unbind the sender from the Naming Service
  void unbind_sender (const ACE_CString &sender_name,
                      AVStreams::MMDevice_ptr sender_mmdevice) ;

  /// Unbind the Receiver from the Naming Service
  void unbind_receiver (const ACE_CString &sender_name,
                        const ACE_CString &receiver_name,
                        AVStreams::MMDevice_ptr receiver_mmdevice);

  /// Add new streamctrl.
  void add_streamctrl (const ACE_CString &flowname,
                       TAO_StreamEndPoint *endpoint);

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

  void find_receivers (void);

  void add_to_receivers (CosNaming::BindingList &binding_list);

  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

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
