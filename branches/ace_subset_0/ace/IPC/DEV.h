/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    DEV.h
 *
 *  $Id$
 *
 *  @author Gerhard Lenzer
 */
//=============================================================================


#ifndef ACE_DEV_H
#define ACE_DEV_H
#include "ace/pre.h"

#include "ace/IPC/IO_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/IPC/DEV_Addr.h"

// The following is necessary since many C++ compilers don't support
// typedef'd types inside of classes used as formal template
// arguments... ;-(.  Luckily, using the C++ preprocessor I can hide
// most of this nastiness!

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
#define ACE_DEV_CONNECTOR ACE_DEV_Connector
#define ACE_DEV_STREAM ACE_DEV_Stream
#else /* TEMPLATES are broken (must be a cfront-based compiler...) */
#define ACE_DEV_CONNECTOR ACE_DEV_Connector, ACE_DEV_Addr
#define ACE_DEV_STREAM ACE_DEV_Stream, ACE_DEV_Addr
#endif /* ACE_TEMPLATE_TYPEDEFS */

/**
 * @class ACE_DEV
 *
 * @brief Defines the member functions for the base class of the
 * ACE_DEV abstraction.
 */
class ACE_Export ACE_DEV : public ACE_IO_SAP
{
public:
  /// Close down the DEVICE
  int close (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /**
   * Disable signal <signum>
   * This is here to prevent Win32 from
   * disabling SPIPE using socket calls
   */
  int disable (int signum) const ;

protected:
  /// Ensure that this class is an abstract base class
  ACE_DEV (void);
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/IPC/DEV.i"
#endif

#include "ace/post.h"
#endif /* ACE_DEV_H */
