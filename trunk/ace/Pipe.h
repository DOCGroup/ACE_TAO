/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Pipe.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_PIPE_H)
#define ACE_PIPE_H

#include "ace/ACE.h"

class ACE_Export ACE_Pipe
  // = TITLE
  //     Provides a bidirectional "pipe" abstraction that is portable
  //     to Windows NT, SVR4 UNIX, and BSD UNIX.
  //
  // = DESCRIPTION
  //     Uses "name" for lookup in the ACE service repository. Obtains
  //	 the object and returns it as the appropriate type.
{
public:
  // = Initialization and termination.
  ACE_Pipe (void);
  // Default constructor (does nothing...).

  ACE_Pipe (ACE_HANDLE handles[2]);
  // Open the pipe and initialize the handles.

  ACE_Pipe (ACE_HANDLE read, ACE_HANDLE write);
  // Initialize the <ACE_Pipe> from the <read> and <write> handles.

  int open (ACE_HANDLE handles[2]);
  // Open the pipe and initialize the handles.

  int open (void);
  // Open the pipe.

  int close (void);
  // Close down the pipe HANDLEs;

  // = Accessors.

  ACE_HANDLE read_handle (void);
  // This is the "read" side of the pipe.  Note, however, that
  // processes can also write to this handle as well since pipes are
  // bi-directional.
  
  ACE_HANDLE write_handle (void);
  // This is the "write" side of the pipe.  Note, however, that
  // processes can also read to this handle as well since pipes are
  // bi-directional.

  void dump (void) const;
  // Dump the state of the object.

private:
  ACE_HANDLE handles_[2];
};

#include "ace/Pipe.i"

#endif /* ACE_PIPE_H */
