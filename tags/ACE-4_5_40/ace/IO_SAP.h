/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    IO_SAP.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_IO_SAP_H)
#define ACE_IO_SAP_H

#include "ace/ACE.h"

class ACE_Export ACE_IO_SAP
{
  // = TITLE
  //     Defines the methods for the base class of the <ACE_IO_SAP>
  //     abstraction, which includes <ACE_FILE> and <ACE_DEV>.
public:
  enum
  {
    INVALID_HANDLE = -1 // Be consistent with Winsock 
  };

  ~ACE_IO_SAP (void);
  // Default dtor.

  int control (int cmd, void *) const;
  // Interface for ioctl. 

  // = Common I/O handle options related to files. 

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
  ACE_IO_SAP (void);
  // Ensure that ACE_IO_SAP is an abstract base class. 

private:
  ACE_HANDLE handle_;
  // Underlying I/O handle. 

  static pid_t pid_;
  // Cache the process ID. 
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IO_SAP.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_IO_SAP_H */
