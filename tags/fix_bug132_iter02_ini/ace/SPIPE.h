/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    SPIPE.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_SPIPE_H
#define ACE_SPIPE_H
#include "ace/pre.h"

#include "ace/IPC_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SPIPE_Addr.h"

/**
 * @class ACE_SPIPE
 *
 * @brief Defines the member functions for the base class of the
 * ACE_SPIPE abstraction.
 */
class ACE_Export ACE_SPIPE : public ACE_IPC_SAP
{
public:
  /// Close down the STREAM pipe without removing the rendezvous point.
  int close (void);

  /// Close down the STREAM pipe and remove the rendezvous point from
  /// the file system.
  int remove (void);

  /// Return the local address of this endpoint.
  int get_local_addr (ACE_SPIPE_Addr &) const;

  /**
   * Disable signal <signum>
   * This is here to prevent Win32 from
   * disabling SPIPE using socket calls
   */
  int disable (int signum) const ;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Ensure that this class is an abstract base class
  ACE_SPIPE (void);

  /// Our local address.
  ACE_SPIPE_Addr local_addr_;
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SPIPE.i"
#endif

#include "ace/post.h"
#endif /* ACE_SPIPE_H */
