/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ATM_Stream.h
 *
 *  $Id$
 *
 *  @author Joe Hoffert
 */
//=============================================================================


#ifndef ACE_ATM_STREAM_H
#define ACE_ATM_STREAM_H
#include "ace/pre.h"

#include "ace/ATM/ATM_Addr.h"
#include "ace/ATM/ATM_Params.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_ATM)

#if defined (ACE_WIN32)
#include "ace/Sockets/SOCK_Stream.h"
typedef ACE_SOCK_Stream ATM_Stream;
#else
#include "ace/IPC/TLI_Stream.h"
typedef ACE_TLI_Stream ATM_Stream;
#endif

/**
 * @class ACE_ATM_Stream
 *
 * @brief Defines the member functions for ACE_ATM_Stream abstraction.
 */
class ACE_Export ACE_ATM_Stream
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  ACE_ATM_Stream (void);

  // = ATM-specific open and shutdown operations.
  /// open the stream.
  /// Close down and release resources.
  int open (ACE_ATM_Params params = ACE_ATM_Params());
  int close (void);

  /// Get the underlying handle.
  ACE_HANDLE get_handle (void) const;

  /// Get the underlying stream.
  ATM_Stream& get_stream (void);

  /// Get the name of the connected host.
  char* get_peer_name (void) const;

  /// Get the VPI and VCI of the stream.
  int get_vpi_vci (ACE_UINT16 &vpi,
                   ACE_UINT16 &vci) const;

  /// Recv an n byte buffer from the connected transport mechanism.
  ssize_t recv (void *buf,
                size_t n,
                int *flags = 0) const;

  /// Send exactly n bytes to the connected transport mechanism.
  ssize_t send_n (const void *buf,
                  size_t n,
                  int flags) const;

  // = Meta-type info
  typedef ACE_ATM_Addr PEER_ADDR;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Typedef'd to the appropriate stream mechanism above.
  ATM_Stream stream_;
};

#if defined (__ACE_INLINE__)
#include "ace/ATM/ATM_Stream.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_ATM */
#include "ace/post.h"
#endif /* ACE_ATM_STREAM_H */
