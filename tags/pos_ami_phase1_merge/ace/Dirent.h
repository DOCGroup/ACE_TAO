/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Dirent.h
//
// = DESCRIPTION
//    Define a portable directory-entry manipulation interface.
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_DIRENT_H
#define ACE_DIRENT_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Dirent
{
  // = TITLE
  //   Define a portable UNIX directory-entry iterator.
public:
  // = Initialization and termination methods.
  ACE_Dirent (void);
  // Default constructor.

  ACE_Dirent (const char *dirname);
  // Constructor calls <opendir>

  int open (const char *filename);
  // Opens the directory named by filename and associates a directory
  // stream with it.

  ~ACE_Dirent (void);
  // Destructor calls <closedir>.

  void close (void);
  // Closes the directory stream and frees the DIR structure.

  // = Iterator methods.
  dirent *read (void);
  // Returns a pointer to a structure representing the directory entry
  // at the current position in the directory stream to which dirp
  // refers, and positions the directory stream at the next entry,
  // except on read-only filesystems.  It returns a NULL pointer upon
  // reaching the end of the directory stream, or upon detecting an
  // invalid location in the directory.  <readdir> shall not return
  // directory entries containing empty names.  It is unspecified
  // whether entries are returned for dot or dot-dot.  The pointer
  // returned by <readdir> points to data that may be overwritten by
  // another call to <readdir> on the same directory stream.  This
  // data shall not be overwritten by another call to readdir() on a
  // different directory stream.  <readdir> may buffer several
  // directory entries per actual read operation; <readdir> marks for
  // update the st_atime field of the directory each time the
  // directory is actually read.

  int read (struct dirent *entry,
            struct dirent **result);
  // Has the equivalent functionality as <readdir> except that an
  // <entry> and <result> buffer must be supplied by the caller to
  // store the result.

  // = Manipulators.
  long tell (void);
  // Returns the current location associated with the directory
  // stream.

  void seek (long loc);
  // Sets the position of the next <readdir> operation on the
  // directory stream.  The new position reverts to the position
  // associated with the directory stream at the time the <telldir>
  // operation that provides loc was performed.  Values returned by
  // <telldir> are good only for the lifetime of the DIR pointer from
  // which they are derived.  If the directory is closed and then
  // reopened, the <telldir> value may be invalidated due to
  // undetected directory compaction.  It is safe to use a previous
  // <telldir> value immediately after a call to <opendir> and before
  // any calls to readdir.

  void rewind (void);
  // Resets the position of the directory stream to the beginning of
  // the directory.  It also causes the directory stream to refer to
  // the current state of the corresponding directory, as a call to
  // <opendir> would.

private:
  DIR *dirp_;
  // Pointer to the directory stream.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Dirent.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_DIRENT_H */
