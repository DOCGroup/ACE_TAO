/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Module.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_MODULE_H
#define ACE_RMCAST_MODULE_H
#include "ace/pre.h"

#include "RMCast.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Message_Block;
class ACE_Time_Value;

class ACE_RMCast_Export ACE_RMCast_Module
{
  // = TITLE
  //     Reliable Multicast Module
  //
  // = DESCRIPTION
  //     The reliable multicast protocol is implemented as a stack of
  //     "Modules" each one performing one specific task.
  //     In short, this is an instance of the pipes-and-filters
  //     architectural pattern.
  //
public:
  // = Initialization and termination methods.
  ACE_RMCast_Module (void);
  // Constructor

  virtual ~ACE_RMCast_Module (void);
  // Destructor

  virtual int next (ACE_RMCast_Module *next);
  virtual ACE_RMCast_Module* next (void) const;
  virtual int prev (ACE_RMCast_Module *prev);
  virtual ACE_RMCast_Module* prev (void) const;
  // Modifiers and accessors for the previous and next module in the
  // stack

  virtual int open (void);
  // Initialize the module, setting up the next module

  virtual int close (void);
  // Close the module.

  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);
  // Push data down the stack

private:
  ACE_RMCast_Module *next_;
  ACE_RMCast_Module *prev_;
  // The next and previous modules in the stack
};

#if defined (__ACE_INLINE__)
#include "RMCast_Module.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_MODULE_H */
