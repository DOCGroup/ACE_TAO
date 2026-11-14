
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

#ifndef TAO_AV_ASYNCH_THREE_STAGE_CONNECTION_MANAGER_H
#define TAO_AV_ASYNCH_THREE_STAGE_CONNECTION_MANAGER_H

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/Protocol_Factory.h"
#include "tao/PortableServer/PortableServer.h"

class Endpoint_Addresses
{
public:
  /// Sender transport address.
  ACE_CString sender_addr {};
  /// Receiver transport address.
  ACE_CString receiver_addr {};
};

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

  /// Method that binds the sender to the Naming Service and retrieves
  /// the references of any registered receivers.
  void bind_to_receivers (ACE_CString const &sender_name,
                          AVStreams::MMDevice_ptr sender);

  /// Connect to the receivers that we found.
  void connect_to_receivers (AVStreams::MMDevice_ptr sender);

  /// Bind receiver to the sender.
  void bind_to_sender (ACE_CString const &sender_name,
                       ACE_CString const &receiver_name,
                       AVStreams::MMDevice_ptr receiver);

  /// Connect to the sender that we found.
  void connect_to_sender ();

  /// Add new streamctrl.
  void add_streamctrl (ACE_CString const &flowname,
                       TAO_StreamEndPoint *endpoint);

  /// Map of named receiver devices.
  using Receivers = ACE_Hash_Map_Manager<ACE_CString,
                                        AVStreams::MMDevice_var,
                                        ACE_Null_Mutex>;

  /// Map of borrowed protocol objects from AVStreams endpoints.
  using Protocol_Objects = ACE_Hash_Map_Manager<ACE_CString,
                                                TAO_AV_Protocol_Object *,
                                                ACE_Null_Mutex>;

  /// Map of retained stream controllers.
  using StreamCtrls = ACE_Hash_Map_Manager<ACE_CString,
                                           AVStreams::StreamCtrl_var,
                                           ACE_Null_Mutex>;

  /// Map of flow names to transport endpoint addresses.
  using EP_Addr = ACE_Hash_Map_Manager<ACE_CString,
                               Endpoint_Addresses,
                               ACE_Null_Mutex>;

  /// Access the retained receiver devices.
  Receivers &receivers ();
  /// Access borrowed endpoint protocol objects.
  Protocol_Objects &protocol_objects ();
  /// Access retained stream controllers.
  StreamCtrls &streamctrls ();

  /// Load sender and receiver endpoint addresses from a file.
  void load_ep_addr (char const *file_name);

protected:
  void find_receivers ();

  void add_to_receivers (CosNaming::BindingList &binding_list);

  /// The Naming Service client.
  TAO_Naming_Client naming_client_;

  /// Receiver device references indexed by flow name.
  Receivers receivers_;
  /// Borrowed protocol objects indexed by flow name.
  Protocol_Objects protocol_objects_;
  /// Stream controller references indexed by flow name.
  StreamCtrls streamctrls_;
  /// Endpoint addresses indexed by flow name.
  EP_Addr ep_addr_;

  /// Name of the sender in the naming service.
  ACE_CString sender_name_;

  /// Sender device reference.
  AVStreams::MMDevice_var sender_;

  /// Sender naming context.
  CosNaming::NamingContext_var sender_context_;

  /// Name of the receiver in the naming service.
  ACE_CString receiver_name_;

  /// Receiver device reference.
  AVStreams::MMDevice_var receiver_;

  /// Receiver naming context.
  CosNaming::NamingContext_var receiver_context_;
};

#endif /* TAO_AV_ASYNCH_THREE_STAGE_CONNECTION_MANAGER_H */
