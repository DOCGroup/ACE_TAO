// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSL_SOCK.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@ece.uci.edu>
 */
//=============================================================================


#ifndef ACE_SSL_SOCK_H
#define ACE_SSL_SOCK_H

#include "ace/pre.h"

#include "ace/SSL/SSL_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Sockets/SOCK.h"

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
# define ACE_SSL_SOCK_ACCEPTOR ACE_SSL_SOCK_Acceptor
# define ACE_SSL_SOCK_CONNECTOR ACE_SSL_SOCK_Connector
# define ACE_SSL_SOCK_STREAM ACE_SSL_SOCK_Stream
#else
# define ACE_SSL_SOCK_ACCEPTOR ACE_SSL_SOCK_Acceptor, ACE_INET_Addr
# define ACE_SSL_SOCK_CONNECTOR ACE_SSL_SOCK_Connector, ACE_INET_Addr
# define ACE_SSL_SOCK_STREAM ACE_SSL_SOCK_Stream, ACE_INET_Addr
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

/**
 * @class ACE_SSL_SOCK
 *
 * @brief An abstract class that forms the basis for more specific
 * classes, such as "ACE_SSL_SOCK_Acceptor" and
 * "ACE_SSL_SOCK_Stream".  Do not instantiate this class.
 *
 * This class provides functions that are common to all of the
 * ACE_SSL_SOCK_* classes.  ACE_SSL_SOCK provides the ability
 * to get and set socket options, get the local and remote
 * addresses, and close the socket.
 */
class ACE_SSL_Export ACE_SSL_SOCK : public ACE_SOCK
{
public:

  /// Default destructor.
  ~ACE_SSL_SOCK (void);

  /**
   * Override ACE_SOCK base class implementations with these SSL
   * specific ones.
   */
  //@{
  int set_option (int level,
                  int option,
                  void *optval,
                  int optlen) const;
  int get_option (int level,
                  int option,
                  void *optval,
                  int *optlen) const;
  int enable (int value) const;
  int disable (int value) const;
  void set_handle (ACE_HANDLE);
  ACE_HANDLE get_handle (void) const;
  int control (int cmd, void *arg) const;
  //@}

protected:

  /// Default constructor is private to prevent instances of this class
  /// from being defined.
  ACE_SSL_SOCK (void);

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SSL/SSL_SOCK.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"

#endif /* ACE_SSL_SOCK_H */




