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
  // = TITLE
  //     Defines the member functions for the base class of the ACE_IO_SAP
  //     abstraction.  
{
public:
  enum
  {
    INVALID_HANDLE = -1 // Be consistent with Winsock 
  };

  int control (int cmd, void *) const;
  // Interface for ioctl. 

  // = Methods for manipulating common I/O descriptor options related to
  //   files. 
  int enable (int signum) const;
  // Enable signal <signum>.
  int disable (int signum) const;
  // Disable signal <signum>.

  ACE_HANDLE get_handle (void) const;
  // Get the underlying descriptor. 

  void set_handle (ACE_HANDLE handle);
  // Set the underlying descriptor. 

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_IO_SAP (void);
  // Ensure that ACE_IO_SAP is an abstract base class. 

private:
  ACE_HANDLE handle_;
  // Underlying I/O descriptor. 

  static pid_t pid_;
  // Cache the process ID. 
};

#include "ace/IO_SAP.i"

#endif /* ACE_IO_SAP_H */
