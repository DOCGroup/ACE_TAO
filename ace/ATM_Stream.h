/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    ATM_Stream.h
//
// = AUTHOR
//    Joe Hoffert
//
// ============================================================================

#ifndef ACE_ATM_STREAM_H
#include "ace/pre.h"
#define ACE_ATM_STREAM_H

#include "ace/ATM_Addr.h"
#include "ace/ATM_Params.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ATM)

#if defined (ACE_WIN32)
#include "SOCK_Stream.h"
typedef ACE_SOCK_Stream ATM_Stream;
#else
#include "TLI_Stream.h"
typedef ACE_TLI_Stream ATM_Stream;
#endif

class ACE_Export ACE_ATM_Stream
{
  // = TITLE
  //     Defines the member functions for ACE_ATM_Stream abstraction.
public:
  // = Initialization and termination methods.
  ACE_ATM_Stream (void);
  // Default constructor.

  // = ATM-specific open and shutdown operations.
  int open (ACE_ATM_Params params = ACE_ATM_Params());
  // open the stream.
  int close (void);
  // Close down and release resources.

  ACE_HANDLE get_handle (void) const;
  // Get the underlying handle.

  ATM_Stream& get_stream (void);
  // Get the underlying stream.

  char* get_peer_name (void) const;
  // Get the name of the connected host.

  int get_vpi_vci (ACE_UINT16 &vpi,
                   ACE_UINT16 &vci) const;
  // Get the VPI and VCI of the stream.

  ssize_t recv (void *buf,
                size_t n,
                int *flags = 0) const;
  // Recv an n byte buffer from the connected transport mechanism.

  ssize_t send_n (const void *buf,
                  size_t n,
                  int flags) const;
  // Send exactly n bytes to the connected transport mechanism.

  // = Meta-type info
  typedef ACE_ATM_Addr PEER_ADDR;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ATM_Stream stream_;
  // Typedef'd to the appropriate stream mechanism above.
};

#if defined (__ACE_INLINE__)
#include "ace/ATM_Stream.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ATM */
#include "ace/post.h"
#endif /* ACE_ATM_STREAM_H */

