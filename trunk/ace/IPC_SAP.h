/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    IPC_SAP.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_IPC_SAP_H
#define ACE_IPC_SAP_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_IPC_SAP
{
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_IPC_SAP abstraction.
public:
  virtual ~ACE_IPC_SAP (void);
  // Default dtor.

  int control (int cmd, void *) const;
  // Interface for ioctl.

  // = Common I/O handle options related to sockets.

  int enable (int value) const;
  // Enable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
  // non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
  // which is passed as the <value>.

  int disable (int value) const;
  // Disable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
  // non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
  // which is passed as the <value>.

  ACE_HANDLE get_handle (void) const;
  // Get the underlying handle.

  void set_handle (ACE_HANDLE handle);
  // Set the underlying handle.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Ensure that ACE_IPC_SAP is an abstract base class.
  ACE_IPC_SAP (void);
  // Default constructor.

private:
  ACE_HANDLE handle_;
  // Underlying I/O handle.

  static pid_t pid_;
  // Cache the process ID.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC_SAP.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_IPC_SAP_H */
