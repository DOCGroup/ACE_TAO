/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_FORK_H
#define ACE_RMCAST_FORK_H
#include "ace/pre.h"

#include "RMCast_Module.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Fork messages to multiple destinations
/**
 * In some instances the messages must be sent to multiple
 * destinations, this module is a generic component to duplicate such
 * messages.
 */
class ACE_RMCast_Export ACE_RMCast_Fork : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Fork (void);

  //! Destructor
  virtual ~ACE_RMCast_Fork (void);

  /// Set the control module, all incoming control messages go to it
  void secondary (ACE_RMCast_Module *module);

  /// Return the current control module
  ACE_RMCast_Module *secondary (void) const;

  virtual int open (void);
  virtual int close (void);
  virtual int data (ACE_RMCast::Data &);
  virtual int poll (ACE_RMCast::Poll &);
  virtual int ack_join (ACE_RMCast::Ack_Join &);
  virtual int ack_leave (ACE_RMCast::Ack_Leave &);
  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);

private:
  /// The control module
  ACE_RMCast_Module *secondary_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Fork.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_FORK_H */
