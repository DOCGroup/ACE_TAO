// $Id$

// ============================================================================
//
// = DESCRIPTION
//   Helper class used in the reassembly layer of the realiable
//   multicast library.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_PARTIAL_MESSAGE_H
#define ACE_RMCAST_PARTIAL_MESSAGE_H
#include "ace/pre.h"

#include "RMCast_Export.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_RMCAST_DEFAULT_HOLE_COUNT
#define ACE_RMCAST_DEFAULT_HOLE_COUNT 16
#endif /* ACE_RMCAST_DEFAULT_HOLE_COUNT */

class ACE_RMCast_Export ACE_RMCast_Partial_Message
{
public:
  ACE_RMCast_Partial_Message (ACE_UINT32 message_size);
  ~ACE_RMCast_Partial_Message (void);

  int fragment_received (ACE_UINT32 message_size,
                         ACE_UINT32 offset,
                         ACE_Message_Block *mb);
  int is_complete (void) const;

  ACE_Message_Block *message_body (void);
  // Return the body of the message, the memory is owned by the
  // class.

private:
  int insert_hole (size_t i,
                   ACE_UINT32 start,
                   ACE_UINT32 end);
  // Insert a new hole into the list

  int remove_hole (size_t i);
  // Remove a hole from the list

private:
  ACE_Message_Block message_body_;
  // Used to rebuild the body of the message

  struct Hole
  {
    ACE_UINT32 start;
    ACE_UINT32 end;
  };

  Hole *hole_list_;
  size_t max_hole_count_;
  size_t hole_count_;
  // The current list of holes in the message_body.
};

#if defined (__ACE_INLINE__)
#include "RMCast_Partial_Message.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_PARTIAL_MESSAGE_H */
