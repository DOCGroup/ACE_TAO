/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    Fill_ACE_QoS.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef FILL_ACE_QOS_H
#define FILL_ACE_QOS_H

#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/ACE.h"
#include "ace/OS_QoS.h"

class Fill_ACE_QoS
{
  //  TITLE
  //    This class helps users to add new flow specs and provides
  //    utility functions for filling up the flow specs for simplex/duplex
  //    sessions.

public:
  typedef ACE_Hash_Map_Manager <ACE_CString, ACE_Flow_Spec *, ACE_Null_Mutex> FLOW_SPEC_HASH_MAP;

  //Initialization and termination methods.
  Fill_ACE_QoS (void);
  // constructor.

  ~Fill_ACE_QoS (void);
  // destructor.

  int fill_simplex_receiver_qos (ACE_QoS &ace_qos,
                                 const ACE_CString &recv_flow_name);
  // To be used by receivers. Fills the receiver qos and sets the
  // sender qos to NO_TRAFFIC.

  int fill_simplex_sender_qos (ACE_QoS &ace_qos,
                               const ACE_CString &send_flow_name);
  // To be used by senders. Fills the sender qos and sets the receiver
  // qos to NO_TRAFFIC.

  int fill_duplex_qos (ACE_QoS &ace_qos,
                       const ACE_CString &recv_flow_name,
                       const ACE_CString &send_flow_name);
  // To be used by applications that wish to be both receivers and
  // senders.

  FLOW_SPEC_HASH_MAP& map (void);
  // Returns the hash map of flowspecs indexed by flowspec name.

private:

  // The Service Provider is currently set to NULL for all ACE_QoS.
  static const iovec iov_;

  // A NO_TRAFFIC flow spec. Senders set the receiving qos to this
  // while the receivers set the sending qos to this.
  ACE_Flow_Spec *default_traffic_;

  // A list of flowspecs indexed by the flowspec name.
  FLOW_SPEC_HASH_MAP flow_spec_map_;
};

#endif /* FILL_ACE_QOS_H */
