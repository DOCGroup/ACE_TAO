/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Singleton_Factory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_SINGLETON_FACTORY_H
#define ACE_RMCAST_SINGLETON_FACTORY_H
#include "ace/pre.h"

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
class ACE_RMCast_Export ACE_RMCast_Singleton_Factory : public ACE_RMCast_Module_Factory
{
public:
  /// Constructor
  /**
   * @param singleton This object is returned by any call to
   * create().
   */
  ACE_RMCast_Singleton_Factory (ACE_RMCast_Module *singleton);

  //! Destructor
  virtual ~ACE_RMCast_Singleton_Factory (void);

  virtual ACE_RMCast_Module *create (void);
  virtual void destroy (ACE_RMCast_Module *);

private:
  /// The singleton object
  ACE_RMCast_Module *singleton_;
};

#if defined (__ACE_INLINE__)
#include "RMCast_Singleton_Factory.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_SINGLETON_FACTORY_H */
