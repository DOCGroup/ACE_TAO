/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    IO_Cntl_Msg.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_IO_CNTL_MSG_H)
#define ACE_IO_CNTL_MSG_H

class ACE_Export ACE_IO_Cntl_Msg
  // = TITLE
  //     Data format for IOCTL messages 
{
public:
  enum ACE_IO_Cntl_Cmds
  {
    SET_LWM    = 1, // Set the low water mark.
    GET_LWM    = 2, // Get the low water mark.
    SET_HWM    = 3, // Set the high water mark.
    GET_HWM    = 4, // Get the high water mark.
    MOD_LINK   = 5, // Link modules
    MOD_UNLINK = 6  // Unlink modules
  };

  // = Initialization method.
  ACE_IO_Cntl_Msg (ACE_IO_Cntl_Cmds c) { this->cmd_ = c; }
  // Initialize the control message.

  // = Get/set methods

  ACE_IO_Cntl_Cmds cmd (void) { return this->cmd_; }
  // Get command.

  void cmd (ACE_IO_Cntl_Cmds c) { this->cmd_ = c; }
  // Set command.

  size_t count (void) { return this->count_; }
  // Get count.

  void count (size_t c) { this->count_ = c; }
  // Set count.

  int error (void) { return this->error_; }
  // Get error.

  void error (int e) { this->error_ = e; }
  // Set error.

  int rval (void) { return this->rval_; }
  // Get return value.

  void rval (int r) { this->rval_ = r; }
  // Set return value.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_IO_Cntl_Cmds cmd_;
  // Command.

  size_t count_;
  // Count.

  int error_;
  // Error.

  int rval_;
  // Return value
};

#endif /* ACE_IO_CNTL_MSG_H */
