/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    LSOCK.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_LOCAL_SOCK_H
#define ACE_LOCAL_SOCK_H

#include "ace/SOCK.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Export ACE_LSOCK
{
  // = TITLE
  //     Create a Local ACE_SOCK, which is used for passing file
  //     descriptors.
public:
#if defined (ACE_HAS_MSG)
  int send_handle (const ACE_HANDLE handle) const;
  // Send an open FD to another process.

  int recv_handle (ACE_HANDLE &handles,
                   char *pbuf = 0,
                   int *len = 0) const;
  // Recv an open FD from another process.
#endif /* ACE_HAS_MSG */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Ensure that ACE_LSOCK is an abstract base class

  ACE_LSOCK (void);
  // Default constructor.

  ACE_LSOCK (ACE_HANDLE handle);
  // Initialize based on <handle>

  ACE_HANDLE get_handle (void) const;
  // Get handle.

  void set_handle (ACE_HANDLE handle);
  // Set handle.

private:
  ACE_HANDLE aux_handle_;
  // An auxiliary handle used to avoid virtual base classes...
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/LSOCK.i"
#endif

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* ACE_LOCAL_SOCK_H */
