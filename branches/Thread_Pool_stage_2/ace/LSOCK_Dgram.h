/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    LSOCK_Dgram.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_LOCAL_SOCK_DGRAM_H
#define ACE_LOCAL_SOCK_DGRAM_H
#include "ace/pre.h"

#include "ace/SOCK_Dgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/LSOCK.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

/**
 * @class ACE_LSOCK_Dgram
 *
 * @brief Create a Local ACE_SOCK datagram.
 */
class ACE_Export ACE_LSOCK_Dgram : public ACE_SOCK_Dgram, public ACE_LSOCK
{
public:
  // = Initialization methods.
  /// Default constructor.
  ACE_LSOCK_Dgram (void);

  /// Initiate a local dgram.
  ACE_LSOCK_Dgram (const ACE_Addr &local,
                   int protocol_family = PF_UNIX,
                   int protocol = 0);

  /// Initiate a local dgram.
  int open (const ACE_Addr &local,
            int protocol_family = PF_UNIX,
            int protocol = 0);

  /// Get handle.
  ACE_HANDLE get_handle (void) const;

  /// Set handle.
  void set_handle (ACE_HANDLE);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/LSOCK_Dgram.i"
#endif

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#include "ace/post.h"
#endif /* ACE_LOCAL_SOCK_DGRAM_H */
