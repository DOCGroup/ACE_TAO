/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Retransmission.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_RETRANSMISSION_H
#define ACE_RMCAST_RETRANSMISSION_H
#include "ace/pre.h"

#include "RMCast_Module.h"
#include "ace/RB_Tree.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Export ACE_RMCast_Retransmission : public ACE_RMCast_Module
{
  // = TITLE
  //     Reliable Multicast Retransmission
  //
  // = DESCRIPTION
  //     Define the interface for all reliable multicast retransmission
public:
  // = Initialization and termination methods.
  ACE_RMCast_Retransmission (void);
  // Constructor

  virtual ~ACE_RMCast_Retransmission (void);
  // Destructor

  // = The RMCast_Module methods
  virtual int close (void);
  virtual int data (ACE_RMCast::Data &data);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);

protected:
  typedef ACE_RB_Tree<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>
    Messages;
  typedef ACE_RB_Tree_Iterator<ACE_UINT32,ACE_RMCast::Data,ACE_Less_Than<ACE_UINT32>,ACE_Null_Mutex>
    Messages_Iterator;

  Messages messages_;
  // The retransmission buffer

  ACE_SYNCH_MUTEX mutex_;
  // Synchronization
};

#if defined (__ACE_INLINE__)
#include "RMCast_Retransmission.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_RETRANSMISSION_H */
