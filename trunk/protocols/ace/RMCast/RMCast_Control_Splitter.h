/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_CONTROL_SPLITTER_H
#define ACE_RMCAST_CONTROL_SPLITTER_H
#include "ace/pre.h"

#include "RMCast_Module.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Split control and data messages different modules in the stack
/**
 * In some instances the control messages must go a destination
 * different from the regular data flow.  This class performs this
 * separation.
 */
class ACE_RMCast_Export ACE_RMCast_Control_Splitter : public ACE_RMCast_Module
{
public:
  //! Constructor
  ACE_RMCast_Control_Splitter (void);

  //! Destructor
  virtual ~ACE_RMCast_Control_Splitter (void);

  /// Set the control module, all incoming control messages go to it
  void control_module (ACE_RMCast_Module *module);

  /// Return the current control module
  ACE_RMCast_Module *control_module (void) const;

  virtual int ack (ACE_RMCast::Ack &);
  virtual int join (ACE_RMCast::Join &);
  virtual int leave (ACE_RMCast::Leave &);

private:
  /// The control module
  ACE_RMCast_Module *control_module_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Control_Splitter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_CONTROL_SPLITTER_H */
