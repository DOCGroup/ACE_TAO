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

#include "RMCast_Module.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Partial_Message;

class ACE_RMCast_Export ACE_RMCast_Reassembly : public ACE_RMCast_Module
{
public:
  ACE_RMCast_Reassembly (void);
  // Constructor

  virtual ~ACE_RMCast_Reassembly (void);
  // Destructor

  // = The ACE_RMCast_Module methods
  virtual int put_data (ACE_RMCast::Data &data);

private:
  ACE_SYNCH_MUTEX mutex_;
  typedef
      ACE_Hash_Map_Manager<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
      Message_Map;
  typedef
      ACE_Hash_Map_Iterator<ACE_UINT32,ACE_RMCast_Partial_Message*,ACE_Null_Mutex>
      Message_Map_Iterator;

  Message_Map messages_;
  // The array of partially received messages
};

#if defined (__ACE_INLINE__)
#include "RMCast_Reassembly.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_REASSEMBLY_H */
