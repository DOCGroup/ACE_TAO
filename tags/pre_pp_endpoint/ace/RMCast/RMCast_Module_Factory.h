/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    RMCast_Module_Factory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_RMCAST_MODULE_FACTORY_H
#define ACE_RMCAST_MODULE_FACTORY_H
#include "ace/pre.h"

#include "RMCast.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_RMCast_Module;
class ACE_RMCast_IO_UDP;

class ACE_RMCast_Export ACE_RMCast_Module_Factory
{
  // = DESCRIPTION
  //
public:
  virtual ~ACE_RMCast_Module_Factory (void);
  // Destructor

  virtual ACE_RMCast_Module *create (ACE_RMCast_IO_UDP *) = 0;
  // Create a new proxy

  virtual void destroy (ACE_RMCast_Module *) = 0;
  // Destroy a proxy
};

#if defined (__ACE_INLINE__)
#include "RMCast_Module_Factory.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_RMCAST_MODULE_FACTORY_H */
