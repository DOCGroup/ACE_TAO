/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Handle_Set.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_HANDLE_SET_H)
#define ACE_HANDLE_SET_H

#include "ace/ACE.h"

// This wrapper design is not very portable to DEC OSF/1 I had to
// redefine NFDBITS to 32. On OSF/1 NFDBITS is a macro that expands to
// (sizeof(fd_mask)*8) which is 4096 by default. This was an
// inappropriate value for defining the MSB_MASK default value. Any
// ideas? The workaround is a pretty severe restriction for OSF/1.
// DJT
// #if defined (__osf__)						
// #define NFDBITS 32						
//  #endif							

class ACE_Export ACE_Handle_Set 
{
  // = TITLE
  //     C++ wrapper for the socket <FD_SET> abstraction.
friend class ACE_Handle_Set_Iterator;
public:
  // = Initialization and termination. 

  enum 
  {
    MAXSIZE = ACE_DEFAULT_REACTOR_SIZE
  };

  // = Initialization methods.
  ACE_Handle_Set (void);
  // Constructor, initializes the bitmask to all 0s.

  ACE_Handle_Set (const ACE_FD_SET_TYPE &mask);

  // = Methods for manipulating bitsets.
  void reset (void);
  // Initialize the bitmask to all 0s and reset the associated fields.

  int is_set (ACE_HANDLE) const;
  // Checks whether handle is enabled.

  void set_bit (ACE_HANDLE);
  // Enables the handle.

  void clr_bit (ACE_HANDLE);
  // Disables the handle.

  int num_set (void) const;
  // Returns a count of the number of enabled bits.

  ACE_HANDLE max_set (void) const;
  // Returns the number of the large bit.

  void sync (ACE_HANDLE max);
  // Synchronize the underlying FD_SET with the MAX_FD and the SIZE.

  operator fd_set *();
  // Returns a pointer to the underlying fd_set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int size_;
  // Size of the set.

  ACE_HANDLE max_handle_;
  // Current max handle.

  fd_set mask_;
  // Bitmask.

  enum
  {
    WORDSIZE = NFDBITS,
#if !defined(ACE_WIN32)
    NUM_WORDS = howmany (MAXSIZE, NFDBITS),
#endif
    NBITS = 256
  };

  int count_bits (unsigned long n) const;
  // Counts the number of bits enabled in N.  Uses a table lookup to
  // speed up the count.

  void set_max (ACE_HANDLE max);
  // Resets the MAX_FD after a clear of the original MAX_FD.

  static const char nbits_[ACE_Handle_Set::NBITS];
  // Table that maps bytes to counts of the enabled bits.
};

class ACE_Export ACE_Handle_Set_Iterator
  // = TITLE
  //  Iterator for the ACE_Handle_Set abstraction. 
{
public:
  ACE_Handle_Set_Iterator (const ACE_Handle_Set &);
  // Constructor.

  ACE_HANDLE operator ()(void);
  // "Next" operator.

  void operator++ (void);
  // Advance by "one."

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Handle_Set &handles_;
  // The <Handle_Set> we are iterating through.

  int num_; 
  // Number of the word we're iterating on.

  size_t index_;
  // Index of the current <num_> word.

#if !defined (ACE_WIN32)
  fd_mask val_;
#endif /* ACE_WIN32 */
  // Value of the bits in the word we're iterating on.
};

#if defined (__ACE_INLINE__)
#include "ace/Handle_Set.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HANDLE_SET */
