/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    FIFO.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_FIFO_H
#define ACE_FIFO_H

#include "ace/IPC_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_FIFO : public ACE_IPC_SAP
{
  // = TITLE
  //    Abstract base class for UNIX FIFOs
  //
  // = DESCRIPTION
  //    UNIX FIFOs are also known Named Pipes, which are totally
  //    unrelated to Win32 Named Pipes.  If you want to use a local
  //    IPC mechanism that will be portable to both UNIX and Win32,
  //    take a look at the <ACE_SPIPE_*> classes.
public:
  int open (const char *rendezvous, int flags, int perms,
            LPSECURITY_ATTRIBUTES sa = 0);
  // Open up the named pipe on the <rendezvous> in accordance with the
  // flags.

  int close (void);
  // Close down the ACE_FIFO without removing the rendezvous point.

  int remove (void);
  // Close down the ACE_FIFO and remove the rendezvous point from the
  // file system.

  int get_local_addr (const char *&rendezvous) const;
  // Return the local address of this endpoint.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Make these protected to ensure that the class is "abstract."
  ACE_FIFO (void);
  // Default constructor.

  ACE_FIFO (const char *rendezvous, int flags, int perms,
            LPSECURITY_ATTRIBUTES sa = 0);
  // Open up the named pipe on the <rendezvous> in accordance with the
  // flags.

private:
  char rendezvous_[MAXPATHLEN + 1];
  // Rendezvous point in the file system.
};

#if defined (__ACE_INLINE__)
#include "ace/FIFO.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_FIFO_H */
