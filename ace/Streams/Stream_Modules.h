/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Stream_Modules.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


// This needs to go outside of the #if !defined() block.  Don't ask...
#include "ace/Streams/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#ifndef ACE_STREAM_MODULES
#define ACE_STREAM_MODULES
#include "ace/pre.h"

/**
 * @class ACE_Stream_Head
 *
 * @brief Standard module that acts as the head of a ustream.
 */
template <ACE_SYNCH_DECL>
class ACE_Stream_Head : public ACE_Task<ACE_SYNCH_USE>
{
public:
  /// Construction
  ACE_Stream_Head (void);

  /// Destruction
  ~ACE_Stream_Head (void);

  // = ACE_Task hooks
  virtual int open (void *a = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  virtual int svc (void);

  // = Dynamic linking hooks
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int info (ACE_TCHAR **info_string, size_t length) const;
  virtual int fini (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Performs canonical flushing at the ACE_Stream Head.
  int control (ACE_Message_Block *);
  int canonical_flush (ACE_Message_Block *);
};

/**
 * @class ACE_Stream_Tail
 *
 * @brief Standard module that acts as the head of a ustream.
 */
template <ACE_SYNCH_DECL>
class ACE_Stream_Tail : public ACE_Task<ACE_SYNCH_USE>
{
public:
  /// Construction
  ACE_Stream_Tail (void);

  /// Destruction
  ~ACE_Stream_Tail (void);

  // = ACE_Task hooks
  virtual int open (void *a = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  virtual int svc (void);

  // = Dynamic linking hooks
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int info (ACE_TCHAR **info_string, size_t length) const;
  virtual int fini (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Performs canonical flushing at the ACE_Stream tail.
  int control (ACE_Message_Block *);
  int canonical_flush (ACE_Message_Block *);
};

/**
 * @class ACE_Thru_Task
 *
 * @brief Standard module that acts as a "no op", simply passing on all
 * data to its adjacent neighbor.
 */
template <ACE_SYNCH_DECL>
class ACE_Thru_Task : public ACE_Task<ACE_SYNCH_USE>
{
public:
  /// Construction
  ACE_Thru_Task (void);

  /// Destruction
  ~ACE_Thru_Task (void);

  // = ACE_Task hooks
  virtual int open (void *a = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);
  virtual int svc (void);

  // = Dynamic linking hooks
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int info (ACE_TCHAR **info_string, size_t length) const;
  virtual int fini (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Streams/Stream_Modules.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Stream_Modules.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_STREAM_MODULES */
