/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Typed_SV_Message.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_TYPED_SV_MESSAGE_H
#define ACE_TYPED_SV_MESSAGE_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Typed_SV_Message
 *
 * @brief Defines the header file for the C++ wrapper for System V
 * message queues.
 */
template <class T>
class ACE_Typed_SV_Message
{
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

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Type of message.
  long type_;

  /// Length of this message.
  int length_;

  /// Maximum length of any message.
  int max_;

  /// Data stored in a message.
  T data_;
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

#include "ace/post.h"
#endif /* ACE_TYPED_SV_MESSAGE_H */
