// -*- C++ -*-

//=============================================================================
/**
 *  @file    Fill_ACE_QoS.h
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef FILL_ACE_QOS_H
#define FILL_ACE_QOS_H

#include "ace/config-all.h"

#if defined (ACE_HAS_RAPI) || defined (ACE_HAS_WINSOCK2_GQOS)

#include "tao/Versioned_Namespace.h"

#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Synch.h"
#include "ace/ACE.h"
#include "ace/OS_QoS.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class Fill_ACE_QoS
 *
 * This class helps users to add new flow specs and provides
 * utility functions for filling up the flow specs for simplex/duplex
 * sessions.
 */
class Fill_ACE_QoS
{

public:
  typedef ACE_Hash_Map_Manager <ACE_CString, ACE_Flow_Spec *, ACE_Null_Mutex> FLOW_SPEC_HASH_MAP;

  //Initialization and termination methods.
  /// constructor.
  Fill_ACE_QoS (void);

  /// destructor.
  ~Fill_ACE_QoS (void);

  /// To be used by receivers. Fills the receiver qos and sets the
  /// sender qos to NO_TRAFFIC.
  int fill_simplex_receiver_qos (ACE_QoS &ace_qos,
                                 ACE_Flow_Spec *recv_flow_spec);

  /// To be used by senders. Fills the sender qos and sets the receiver
  /// qos to NO_TRAFFIC.
  int fill_simplex_sender_qos (ACE_QoS &ace_qos,
                               ACE_Flow_Spec *recv_flow_spec);

  /// To be used by applications that wish to be both receivers and
  /// senders.
  int fill_duplex_qos (ACE_QoS &ace_qos,
                       ACE_Flow_Spec *recv_flow_spec,
                       ACE_Flow_Spec *sender_flow_spec);

  /// Returns the hash map of flowspecs indexed by flowspec name.
  FLOW_SPEC_HASH_MAP& map (void);

private:

  // The Service Provider is currently set to NULL for all ACE_QoS.
  static const iovec iov_;

  // A NO_TRAFFIC flow spec. Senders set the receiving qos to this
  // while the receivers set the sending qos to this.
  ACE_Flow_Spec *default_traffic_;

  // A list of flowspecs indexed by the flowspec name.
  FLOW_SPEC_HASH_MAP flow_spec_map_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_RAPI || ACE_HAS_WINSOCK2_GQOS */
#endif /* FILL_ACE_QOS_H */
