// $Id$

// ============================================================================
//
// = DESCRIPTION
//   The reassembly task for the reliable multicast library
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_REASSEMBLY_H
#define ACE_RMCAST_REASSEMBLY_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Module.h"
#include "ace/Utils/Hash_Map_Manager.h"
#include "ace/Threads/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Partial_Message;

//! Reassemble multiple data fragments into a single data message
/*!
  Data messages may not fit in a single MTU in the transport layer, in
  that case the application configure a RMCast_Fragment module on the
  sender side.  On the receiver side this layer reassemble the
  messages sent from a <EM>single</EM> source, and passes the messages
  up the stream.
*/
class ACE_RMCast_Export ACE_RMCast_Reassembly : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Reassembly (void);

  //! Destructor
  virtual ~ACE_RMCast_Reassembly (void);

  // = The ACE_RMCast_Module methods
  virtual int close (void);
  virtual int data (ACE_RMCast::Data &data);

private:
  /// Cleanup resources, but do not close the other modules in the
  /// stack
  void close_i (void);

private:
  //! A mutex used to synchronize all the internal operations.
  ACE_SYNCH_MUTEX mutex_;
  typedef
      ACE_Hash_Map_Manager<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
      Message_Map;
  typedef
      ACE_Hash_Map_Iterator<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
      Message_Map_Iterator;

  //! A map, indexed by sequence number, of the partially received
  //! messages.
  Message_Map messages_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Reassembly.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_REASSEMBLY_H */
