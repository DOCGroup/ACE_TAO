/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SPIPE.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_SPIPE_H
#define ACE_SPIPE_H
#include "ace/pre.h"

#include "ace/Sockets/IPC_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/IPC/SPIPE_Addr.h"

/**
 * @class ACE_SPIPE
 *
 * @brief Defines the member functions for the base class of the
 * ACE_SPIPE abstraction.
 *
 * ACE_SPIPE was designed as a wrapper facade for STREAM pipes. On
 * Windows, however, ACE_SPIPE is a wrapper for Named Pipes. Therefore,
 * on Windows, it is better to use the ACE_Proactor or blocking operations
 * in lieu of using the ACE_Reactor.
 *
 * @sa ACE_SPIPE_Acceptor for additional notes on use with ACE_Reactor
 * on Windows.
 *
 * @warning Windows: Works only on Windows NT 4 and higher.
 * @warning Works on non-Windows platforms only when @c ACE_HAS_STREAM_PIPES
 * is defined.
 */
class ACE_Export ACE_SPIPE : public ACE_IPC_SAP
{
public:
  /// Close down the SPIPE without removing the rendezvous point.
  /**
   * Closes the underlying pipe handle and sets this object's handle value
   * to @c ACE_INVALID_HANDLE.
   * @return 0 for success; -1 for failure. If a failure occurs, further
   * error information is available from ACE_OS::last_error().
   */
  int close (void);

  /// Close down the SPIPE and remove the rendezvous point from
  /// the file system.
  /**
   * @return 0 for success; -1 for failure. If a failure occurs, further
   * error information is available from ACE_OS::last_error().
   */
  int remove (void);

  /// Return the local address of this endpoint.
  /**
   * Returns the local address of the SPIPE's endpoint in @arg addr.
   * @return 0 for success; -1 for failure. If a failure occurs, further
   * error information is available from ACE_OS::last_error().
   */
  int get_local_addr (ACE_SPIPE_Addr &) const;

  /**
   * Disable signal @arg signum.
   * @note This reimplements the inherited method from
   * ACE_IPC_SAP. Since the signal actions do not make any sense on
   * Windows, this method intercepts the call and NOPs it. On non-Windows,
   * it is passed through to ACE_IPC_SAP::disable().
   */
  int disable (int signum) const ;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Ensure that this class is an abstract base class
  ACE_SPIPE (void);

  /// Our local address.
  ACE_SPIPE_Addr local_addr_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC/SPIPE.i"
#endif

#include "ace/post.h"
#endif /* ACE_SPIPE_H */
