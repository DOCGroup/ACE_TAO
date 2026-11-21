//=============================================================================
/**
 *  @file    Connection_Manager.h
 *
 *  This is a helper class that allows the senders bind to multiple
 *  receivers and receivers to bind to senders. It also lets the
 *  senders and receivers disconnect streams.
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
  Connection_Manager () = default;
  Connection_Manager (Connection_Manager const &) = delete;
  Connection_Manager &operator= (Connection_Manager const &) = delete;
  Connection_Manager (Connection_Manager &&) = delete;
  Connection_Manager &operator= (Connection_Manager &&) = delete;

  /// Initialize this class.
  int init (CORBA::ORB_ptr orb);

  /// Bind the sender in the Naming Service and retrieve registered receivers.
  void bind_to_receivers (ACE_CString const &sender_name,
                          AVStreams::MMDevice_ptr sender);

  /// Connect to the receivers that we found.
  void connect_to_receivers ();

  /// Bind receiver to the sender.
  void bind_to_sender (ACE_CString const &sender_name,
                       ACE_CString const &receiver_name,
                       AVStreams::MMDevice_ptr receiver);

  /// Connect to the sender that we found.
  void connect_to_sender ();

  /// Destroy streams associated with <flowname>.
  void destroy (ACE_CString const &flowname);

  /// Unbind the sender from the Naming Service
  void unbind_sender (ACE_CString const &sender_name,
                      AVStreams::MMDevice_ptr sender_mmdevice);

  /// Unbind the Receiver from the Naming Service
  void unbind_receiver (ACE_CString const &sender_name,
                        ACE_CString const &receiver_name,
                        AVStreams::MMDevice_ptr receiver_mmdevice);

  /// Add new streamctrl.
  void add_streamctrl (ACE_CString const &flowname,
                       TAO_StreamEndPoint *endpoint);

  /// Map of receiver devices.
  using Receivers = ACE_Hash_Map_Manager<ACE_CString,
                               AVStreams::MMDevice_var,
                               ACE_Null_Mutex>;

  /// Map of borrowed protocol objects owned by AVStreams.
  using Protocol_Objects = ACE_Hash_Map_Manager<ACE_CString,
                               TAO_AV_Protocol_Object *,
                               ACE_Null_Mutex>;

  /// Map of retained stream controllers.
  using StreamCtrls = ACE_Hash_Map_Manager<ACE_CString,
                               AVStreams::StreamCtrl_var,
                               ACE_Null_Mutex>;

  /// Access the receiver device map.
  Receivers &receivers ();
  /// Access the borrowed protocol object map.
  Protocol_Objects &protocol_objects ();
  /// Access the stream controller map.
  StreamCtrls &streamctrls ();

protected:
  void find_receivers ();

  void add_to_receivers (CosNaming::BindingList &binding_list);

  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

  /// Receiver devices indexed by flow name.
  Receivers receivers_;
  /// Borrowed protocol objects indexed by flow name.
  Protocol_Objects protocol_objects_;
  /// Retained stream controllers indexed by flow name.
  StreamCtrls streamctrls_;

  /// Sender name in the Naming Service.
  ACE_CString sender_name_;

  /// Sender device reference.
  AVStreams::MMDevice_var sender_;

  /// Sender Naming Service context.
  CosNaming::NamingContext_var sender_context_;

  /// Receiver name in the Naming Service.
  ACE_CString receiver_name_;

  /// Receiver device reference.
  AVStreams::MMDevice_var receiver_;

  /// Receiver Naming Service context.
  CosNaming::NamingContext_var receiver_context_;
};

#endif /* CONNECTION_MANAGER_H */
