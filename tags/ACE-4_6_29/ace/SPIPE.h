/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    SPIPE.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SPIPE_H
#define ACE_SPIPE_H

#include "ace/IPC_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SPIPE_Addr.h"

class ACE_Export ACE_SPIPE : public ACE_IPC_SAP
{
  // = TITLE
  //     Defines the member functions for the base class of the
  //     ACE_SPIPE abstraction.
public:
  int close (void);
  // Close down the STREAM pipe without removing the rendezvous point.

  int remove (void);
  // Close down the STREAM pipe and remove the rendezvous point from
  // the file system.

  int get_local_addr (ACE_SPIPE_Addr &) const;
  // Return the local address of this endpoint.

  int disable (int signum) const ;
  // Disable signal <signum>
  // This is here to prevent Win32 from
  // disabling SPIPE using socket calls

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_SPIPE (void);
  // Ensure that this class is an abstract base class

  ACE_SPIPE_Addr local_addr_;
  // Our local address.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SPIPE.i"
#endif

#endif /* ACE_SPIPE_H */
