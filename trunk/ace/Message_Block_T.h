/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Message_Block_T.h
//
// = AUTHOR
//    Doug Schmidt & Carlos O'Ryan
//
// ============================================================================

#ifndef ACE_MESSAGE_BLOCK_T_H
#define ACE_MESSAGE_BLOCK_T_H

#include "ace/Message_Block.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class ACE_LOCK>
class ACE_Locked_Data_Block : public ACE_Data_Block
{
  // = TITLE
  //   A Data_Block with a concrete locking strategy.
  //
  // = DESCRIPTION
  //   Data_Blocks can be parametric on the kind of lock they use; in
  //   many cases the lifetime of the lock is tied to the lifetime of
  //   the Data_Block itself. But since Data_Blocks are reference
  //   counted it is hard for users to control the lock lifetime.
  //   This class is parametric over the kind of lock used.
  //
public:
  // = Initialization and termination methods.
  ACE_Locked_Data_Block (void);
  // Default "do-nothing" constructor.

  ACE_Locked_Data_Block (size_t size,
                         ACE_Message_Block::ACE_Message_Type msg_type,
                         const char *msg_data,
                         ACE_Allocator *allocator_strategy,
                         ACE_Message_Block::Message_Flags flags,
                         ACE_Allocator *data_block_allocator);
  // Initialize.

  virtual ~ACE_Locked_Data_Block (void);
  // Delete all the resources held in the message.

  virtual ACE_Data_Block *clone (ACE_Message_Block::Message_Flags mask = 0) const;
  // Return an exact "deep copy" of the message, the dynamic type is
  // ACE_Locked_Data_Block<>

private:
  ACE_LOCK lock_;
  // The lock

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (ACE_Locked_Data_Block &operator= (const ACE_Locked_Data_Block<ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Locked_Data_Block (const ACE_Locked_Data_Block<ACE_LOCK> &))
};

#if defined (__ACE_INLINE__)
#include "ace/Message_Block_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Message_Block_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Message_Block_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MESSAGE_BLOCK_T_H */
