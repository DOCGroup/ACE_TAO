/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    FILE.h
//
// = AUTHOR
//    Gerhard Lenzer
//
// ============================================================================

#ifndef ACE_FILE_H
#define ACE_FILE_H

#include "ace/IO_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/FILE_Addr.h"

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_FILE_CONNECTOR ACE_FILE_Connector
#define ACE_FILE_STREAM ACE_FILE_IO
#else /* TEMPLATES are broken (must be a cfront-based compiler...) */
#define ACE_FILE_CONNECTOR ACE_FILE_Connector, ACE_FILE_Addr
#define ACE_FILE_STREAM ACE_FILE_IO, ACE_FILE_Addr
#endif /* ACE_TEMPLATE_TYPEDEFS */

class ACE_Export ACE_FILE_Info
{
  // = TITLE
  //     Abstracts basic OS FILE information.
public:
  mode_t mode_;
  // mode of file

  nlink_t nlink_;
  // no of links

  off_t size_;
  // size of file
};

class ACE_Export ACE_FILE : public ACE_IO_SAP
{
  // = TITLE
  //     Defines the core methods of the <ACE_FILE> abstraction.
public:
  int close (void);
  // Close the <ACE_FILE> handle without removing the <ACE_FILE> from
  // the file system.
  
  int remove (void);
  // Close and remove the <ACE_FILE> from the file system.

  int unlink (void);
  // Remove the <ACE_FILE> from the file system without closing the
  // <ACE_FILE> handle.

  int get_info (ACE_FILE_Info *finfo);
  // Get information on this <ACE_FILE>.

  int get_info (ACE_FILE_Info &finfo);
  // Get information on this <ACE_FILE>.

  int truncate (off_t length);
  // Set filesize to length byte.

  off_t seek (off_t offset, int whence = SEEK_CUR);
  // Sets the file pointer as follows:
  //    o If <whence> is <SEEK_SET>, the pointer is set to <offset>
  //      bytes.
  // 
  //    o  If <whence> is <SEEK_CUR>, the pointer is set to its
  //       current location plus <offset>.
  //
  //    o  If <whence> is <SEEK_END>, the pointer is set to the size
  //       of the file plus offset.
  off_t position (long offset, int startpos);
  // Same as <seek>, but <position> is deprecated.

  off_t tell (void);
  // Return an offset for the file handle.

  off_t position (void);
  // Same as <tell>, but <position> is deprecated.

  int disable (int signum) const ;
  // Disable signal <signum>
  // This is here to prevent Win32 from
  // disabling SPIPE using socket calls

  int get_local_addr (ACE_Addr &) const;
  // Return the local endpoint address in the referenced <ACE_Addr>.
  // Returns 0 if successful, else -1.

  int get_remote_addr (ACE_Addr &) const;
  // Return the same thing as <get_local_addr>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_FILE (void);
  // Ensure that this class is only created by the
  // <ACE_FILE_Connector>.

  ACE_FILE_Addr addr_;
  // File we are "connected" with...
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/FILE.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_FILE_H */
