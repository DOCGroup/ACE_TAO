/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SV_Message.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SV_MESSAGE_H
#define ACE_SV_MESSAGE_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_SV_Message
{
  // = TITLE
  //     Defines the header file for the C++ wrapper for message queues. */
public:
  // = Initialization and termination methods.
  ACE_SV_Message (long type = 0);
  ~ACE_SV_Message (void);

  // = Get/set the message type.
  long type (void) const;
  void type (long);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  long type_;
  // Type of the message.
};

#if defined (__ACE_INLINE__)
#include "ace/SV_Message.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SV_MESSAGE_H */
