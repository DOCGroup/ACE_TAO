/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Reliable_Factory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_RELIABLE_FACTORY_H
#define ACE_RMCAST_RELIABLE_FACTORY_H
#include /**/ "ace/pre.h"

#include "RMCast_Module_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement an ACE_RMCast_Module_Factory that "creates" a single
/// object.
/**
 * Many applications (and even some internal components), will use a
 * single ACE_RMCast_Module to process all the events, for example, a
 * receiver may decide to use the same ACE_RMCast_Module to process
 * all incoming events, instead of using one per remote sender.
 */
class ACE_RMCast_Export ACE_RMCast_Reliable_Factory : public ACE_RMCast_Module_Factory
{
public:
  /// Constructor
  ACE_RMCast_Reliable_Factory (ACE_RMCast_Module_Factory *factory);

  //! Destructor
  virtual ~ACE_RMCast_Reliable_Factory (void);

  /**
   * The create() method will return always @c reliable.
   */
  virtual ACE_RMCast_Module *create (void);
  virtual void destroy (ACE_RMCast_Module *);

private:
  /// Delegate on another factory to create the user module
  ACE_RMCast_Module_Factory *factory_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Reliable_Factory.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_RELIABLE_FACTORY_H */
