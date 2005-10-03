// -*- C++ -*-

//=============================================================================
/**
 *  @file    LSOCK.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_LOCAL_SOCK_H
#define ACE_LOCAL_SOCK_H

#include /**/ "ace/pre.h"

#ifdef ACE_SOCKETS_BUILD_DLL
# include "ace/ACE_Sockets_export.h"
#else
# include "ace/ACE_export.h"
# define ACE_Sockets_Export ACE_Export
#endif  /* ACE_SOCKETS_BUILD_DLL */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "ace/SOCK.h"

/**
 * @class ACE_LSOCK
 *
 * @brief Create a Local ACE_SOCK, which is used for passing file
 * descriptors.
 */
class ACE_Sockets_Export ACE_LSOCK
{
public:
#if defined (ACE_HAS_MSG)
  /// Send an open FD to another process.
  ssize_t send_handle (const ACE_HANDLE handle) const;

  /// Recv an open FD from another process.
  ssize_t recv_handle (ACE_HANDLE &handles,
                       char *pbuf = 0,
                       ssize_t *len = 0) const;
#endif /* ACE_HAS_MSG */

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Ensure that ACE_LSOCK is an abstract base class

  /// Default constructor.
  ACE_LSOCK (void);

  /// Initialize based on @a handle.
  ACE_LSOCK (ACE_HANDLE handle);

  /// Get handle.
  ACE_HANDLE get_handle (void) const;

  /// Set handle.
  void set_handle (ACE_HANDLE handle);

private:
  /// An auxiliary handle used to avoid virtual base classes...
  ACE_HANDLE aux_handle_;
};

#if defined (__ACE_INLINE__)
#include "ace/LSOCK.inl"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#include /**/ "ace/post.h"
#endif /* ACE_LOCAL_SOCK_H */
