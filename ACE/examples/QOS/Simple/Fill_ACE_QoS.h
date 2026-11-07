/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Fill_ACE_QoS.h
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef FILL_ACE_QOS_H
#define FILL_ACE_QOS_H

#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/ACE.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_QoS.h"

#include <memory>

/**
 * @class Fill_ACE_QoS
 TITLE
 * This class helps users to add new flow specs and provides
 * utility functions for filling up the flow specs for simplex/duplex
 * sessions.
 */
class Fill_ACE_QoS
{
public:
  using FLOW_SPEC_HASH_MAP = ACE_Hash_Map_Manager<ACE_CString, ACE_Flow_Spec *, ACE_Null_Mutex>;

  //Initialization and termination methods.
  /// constructor.
  Fill_ACE_QoS ();
  Fill_ACE_QoS (Fill_ACE_QoS const &) = delete;
  Fill_ACE_QoS (Fill_ACE_QoS &&) = delete;
  Fill_ACE_QoS &operator= (Fill_ACE_QoS const &) = delete;
  Fill_ACE_QoS &operator= (Fill_ACE_QoS &&) = delete;

  /// destructor.
  ~Fill_ACE_QoS () = default;

  /// To be used by receivers. Fills the receiver qos and sets the
  /// sender qos to NO_TRAFFIC.
  int fill_simplex_receiver_qos (ACE_QoS &ace_qos,
                                 ACE_CString const &recv_flow_name);

  /// To be used by senders. Fills the sender qos and sets the receiver
  /// qos to NO_TRAFFIC.
  int fill_simplex_sender_qos (ACE_QoS &ace_qos,
                               ACE_CString const &send_flow_name);

  /// To be used by applications that wish to be both receivers and
  /// senders.
  int fill_duplex_qos (ACE_QoS &ace_qos,
                       ACE_CString const &recv_flow_name,
                       ACE_CString const &send_flow_name);

  /// Returns the hash map of flowspecs indexed by flowspec name.
  FLOW_SPEC_HASH_MAP& map ();

private:
  // The Service Provider is currently set to NULL for all ACE_QoS.
  static const iovec iov_;

  // A NO_TRAFFIC flow spec. Senders set the receiving qos to this
  // while the receivers set the sending qos to this.
  std::unique_ptr<ACE_Flow_Spec> default_traffic_;

  // A list of flowspecs indexed by the flowspec name.
  FLOW_SPEC_HASH_MAP flow_spec_map_;
};

#endif /* FILL_ACE_QOS_H */
