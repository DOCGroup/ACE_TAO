// -*- C++ -*-

//==========================================================================
/**
 *  @file    IO_Cntl_Msg.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//==========================================================================


#ifndef ACE_IO_CNTL_MSG_H
#define ACE_IO_CNTL_MSG_H

#include "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS.h"

/**
 * @class ACE_IO_Cntl_Msg
 *
 * @brief Data format for IOCTL messages
 */
class ACE_Export ACE_IO_Cntl_Msg
{
public:
  enum
  {
    /// Set the low water mark.
    SET_LWM    = 1,
    /// Get the low water mark.
    GET_LWM    = 2,
    /// Set the high water mark.
    SET_HWM    = 3,
    /// Get the high water mark.
    GET_HWM    = 4,
    /// Link modules
    MOD_LINK   = 5,
    /// Unlink modules
    MOD_UNLINK = 6 
  };

  typedef u_short ACE_IO_Cntl_Cmds;

  // = Initialization method.
  /// Initialize the control message.
  ACE_IO_Cntl_Msg (ACE_IO_Cntl_Cmds c) { this->cmd_ = c; }

  // = Get/set methods

  /// Get command.
  ACE_IO_Cntl_Cmds cmd (void) { return this->cmd_; }

  /// Set command.
  void cmd (ACE_IO_Cntl_Cmds c) { this->cmd_ = c; }

  /// Get count.
  size_t count (void) { return this->count_; }

  /// Set count.
  void count (size_t c) { this->count_ = c; }

  /// Get error.
  int error (void) { return this->error_; }

  /// Set error.
  void error (int e) { this->error_ = e; }

  /// Get return value.
  int rval (void) { return this->rval_; }

  /// Set return value.
  void rval (int r) { this->rval_ = r; }

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Command.
  ACE_IO_Cntl_Cmds cmd_;

  /// Count.
  size_t count_;

  /// Error.
  int error_;

  /// Return value
  int rval_;
};

#include "ace/post.h"

#endif /* ACE_IO_CNTL_MSG_H */
