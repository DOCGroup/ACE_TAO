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

#include "ace/RMCast/RMCast.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Message_Block;
class ACE_Time_Value;

//! Reliable Multicast Module
/*!
  The reliable multicast protocol is implemented as a stack of
  "Modules" each one performing one specific task.  In short, this is
  an instance of the pipes-and-filters architectural pattern.
*/
class ACE_RMCast_Export ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Module (void);

  //! Destructor
  virtual ~ACE_RMCast_Module (void);

  //! Modifier for the next element in the stack
  virtual int next (ACE_RMCast_Module *next);

  //! Accesor for the next element in the stack
  virtual ACE_RMCast_Module* next (void) const;

  //! Initialize the module, setting up the next module
  virtual int open (void);

  //! Close the module.
  virtual int close (void);

  //! Push data through the stack
  virtual int data (ACE_RMCast::Data &);

  //! Push a polling request through the stack
  virtual int poll (ACE_RMCast::Poll &);

  //! Push a message to ack a join request through the stack
  virtual int ack_join (ACE_RMCast::Ack_Join &);

  //! Push a message to ack a leave request through the stack
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);

  //! Push an ack mesage through the stack
  virtual int ack (ACE_RMCast::Ack &);

  //! Push a join message through the stack
  virtual int join (ACE_RMCast::Join &);

  //! Push a leave message through the stack
  virtual int leave (ACE_RMCast::Leave &);

private:
  //! The next element in the stack
  ACE_RMCast_Module *next_;
};

#if defined (__ACE_INLINE__)
#include "ace/RMCast/RMCast_Module.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_MODULE_H */
