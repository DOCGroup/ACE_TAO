/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    IPC_SAP.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_IPC_SAP_H
#define ACE_IPC_SAP_H
#include "ace/pre.h"

#include "ace/Utils/Flag_Manip.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_IPC_SAP
 *
 * @brief Defines the member functions for the base class of the
 * ACE_IPC_SAP abstraction.
 */
class ACE_Export ACE_IPC_SAP
{
public:
  /// Default dtor.
  ~ACE_IPC_SAP (void);

  /// Interface for <ioctl>.
  int control (int cmd, void *) const;

  // = Common I/O handle options related to sockets.

  /**
   * Enable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
   * non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
   * which is passed as the <value>.
   */
  int enable (int value) const;

  /**
   * Disable asynchronous I/O (ACE_SIGIO), urgent data (ACE_SIGURG),
   * non-blocking I/O (ACE_NONBLOCK), or close-on-exec (ACE_CLOEXEC),
   * which is passed as the <value>.
   */
  int disable (int value) const;

  /// Get the underlying handle.
  ACE_HANDLE get_handle (void) const;

  /// Set the underlying handle.
  void set_handle (ACE_HANDLE handle);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Ensure that ACE_IPC_SAP is an abstract base class.
  /// Default constructor.
  ACE_IPC_SAP (void);

private:
  /// Underlying I/O handle.
  ACE_HANDLE handle_;

  /// Cache the process ID.
  static pid_t pid_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Sockets/IPC_SAP.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_IPC_SAP_H */
