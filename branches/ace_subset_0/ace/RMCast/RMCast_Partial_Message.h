// $Id$

// ============================================================================
//
// = DESCRIPTION
//   Helper class used in the reassembly layer of the realiable
//   multicast library.
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_PARTIAL_MESSAGE_H
#define ACE_RMCAST_PARTIAL_MESSAGE_H
#include "ace/pre.h"

#include "ace/RMCast/RMCast_Export.h"
#include "ace/Streams/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_RMCAST_DEFAULT_HOLE_COUNT
#define ACE_RMCAST_DEFAULT_HOLE_COUNT 16
#endif /* ACE_RMCAST_DEFAULT_HOLE_COUNT */

//! Represent a partially received message in the
//! ACE_RMCast_Reassembly module
/*!
 * This class provides temporary storage for the fragments as they are
 * received in the ACE_RMCast_Reassembly module.  It also keeps track
 * of what portions of the message are still missing.
 */
class ACE_RMCast_Export ACE_RMCast_Partial_Message
{
public:
  //! Constructor, reserve enough memory for the complete message
  ACE_RMCast_Partial_Message (ACE_UINT32 message_size);

  //! Destructor
  ~ACE_RMCast_Partial_Message (void);

  //! Process a fragment
  /*!
   * A fragment starting at <offset> has been received, copy the
   * fragment contents and update the list of holes.
   */
  int fragment_received (ACE_UINT32 message_size,
                         ACE_UINT32 offset,
                         ACE_Message_Block *mb);

  //! Return 1 if the message is complete
  int is_complete (void) const;

  //! Return the body of the message, the memory is *not* owned by the
  //! caller
  ACE_Message_Block *message_body (void);

private:
  //! Insert a new hole into the list
  /*!
   * The class keeps an array to represent the missing portions of the
   * message.  This method inserts a new hole, i.e. a new element in
   * the array at index <i>. The <start> and <end> arguments represent
   * the offsets of the missing portion of the message.
   */
  int insert_hole (size_t i,
                   ACE_UINT32 start,
                   ACE_UINT32 end);

  //! Remove a hole from the list
  int remove_hole (size_t i);

private:
  //! Maintain the message storage
  ACE_Message_Block message_body_;

  //! Represent a missing portion of a message
  struct Hole
  {
    //! Offset where the missing portion of the message starts
    ACE_UINT32 start;
    //! Offset where the missing portion of the message ends
    ACE_UINT32 end;
  };

  //! Implement a growing array of Hole structures
  //@{
  Hole *hole_list_;
  size_t max_hole_count_;
  size_t hole_count_;
  //@}
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Partial_Message.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_PARTIAL_MESSAGE_H */
