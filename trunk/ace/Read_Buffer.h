/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Read_Buffer.h
//
// = AUTHOR
//    Doug Schmidt and Seth Widoff
//
// ============================================================================

#ifndef ACE_READ_BUFFER_H
#define ACE_READ_BUFFER_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Malloc.h"

class ACE_Export ACE_Read_Buffer
{
  // = TITLE
  //     Efficiently reads an artibrarily large buffer from an input
  //     stream up to and including a termination character.  Also
  //     performs search/replace on single occurrences a character in
  //     the buffer using the principles of Integrated Layer
  //     Processing.
  //
  // = DESCRIPTION
  //     This implementation is optimized to do a single dynamic
  //     allocation and make only one copy of the data.  It uses
  //     recursion and the run-time stack to accomplish this
  //     efficiently.
public:
  // = Initialization and termination methods.
  ACE_Read_Buffer (FILE *fp,
                   int close_on_delete = 0,
                   ACE_Allocator * = 0);
  // Read from a FILE *.

  ACE_Read_Buffer (ACE_HANDLE handle,
                   int close_on_delete = 0,
                   ACE_Allocator * = 0);
  // Read from an open HANDLE.

  ~ACE_Read_Buffer (void);
  // Closes the FILE *.

  char *read (int terminator = EOF,
              int search = '\n',
              int replace = '\0');
  // Returns a pointer dynamically allocated with
  // <ACE_Allocator::malloc> to data from the input stream up to (and
  // including) the <terminator>.  If <search> is >= 0 then all
  // occurrences of the <search> value are substituted with the
  // <replace> value.  The last of the byte of data is a 0, so that
  // <strlen> can be used on it.  The caller is responsible for
  // freeing the pointer returned from this method using the
  // <ACE_Allocator::free>.

  size_t replaced (void) const;
  // Returns the number of characters replaced during a <read>.

  size_t size (void) const;
  // Returns the size of the allocated buffer obtained during a
  // <read>, not including the null terminator.

  ACE_Allocator *alloc (void) const;
  // Returns a pointer to its allocator.

  void dump (void) const;
  // Dump the state of the object.

private:
  char *rec_read (int term, int search, int replace);
  // Recursive helper method that does the work...

  size_t size_;
  // The total number of characters in the buffer.

  size_t occurrences_;
  // The total number of characters replaced.

  FILE *stream_;
  // The stream we are reading from.

  int close_on_delete_;
  // Keeps track of whether we should close the FILE in the
  // destructor.

  ACE_Allocator *allocator_;
  // Pointer to the allocator.

  // = Disallow copying and assignment...
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Read_Buffer &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Read_Buffer (const ACE_Read_Buffer &))
};

#if defined (__ACE_INLINE__)
# include "ace/Read_Buffer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_READ_BUFFER_H */
