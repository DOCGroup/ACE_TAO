/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Typed_SV_Message.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_TYPED_SV_MESSAGE_H
#define ACE_TYPED_SV_MESSAGE_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class T>
class ACE_Typed_SV_Message
{
  // = TITLE
  //     Defines the header file for the C++ wrapper for System V
  //     message queues.
public:
  // = Initialization and termination methods.
  ACE_Typed_SV_Message (long type = 0,
                        int length = sizeof (T),
                        int max_size = sizeof (T));
  ACE_Typed_SV_Message (const T &data,
                        long type = 0,
                        int length = sizeof (T),
                        int max_size = sizeof (T));
  ~ACE_Typed_SV_Message (void);

  // = Get/set the type of the message.
  long type (void) const;
  void type (long type);

  // = Get/set the length of the message.
  int length (void) const;
  void length (int l);

  // = Get/set the maximum size of the message.
  int max_size (void) const;
  void max_size (int m);

  // = Get/set a pointer to the data in the message.
  T &data (void);
  void data (const T &data);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  long type_;
  // Type of message.

  int length_;
  // Length of this message.

  int max_;
  // Maximum length of any message.

  T data_;
  // Data stored in a message.
};

#if defined (__ACE_INLINE__)
#include "ace/Typed_SV_Message.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Typed_SV_Message.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Typed_SV_Message.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_TYPED_SV_MESSAGE_H */
