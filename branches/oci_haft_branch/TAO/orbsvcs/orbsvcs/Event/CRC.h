/* -*- C++ -*- */
//=============================================================================
/**
 *  @file CRC.h
 *
 *  $Id$
 *
 *  @author RTING
 *  @author Integrated by Balachandran Natarajan <bala@dre.vanderbilt.edu>
 *
 * The Event Channel uses some of this classes to simplify its event
 * manipulation.
 * For instance it keeps a reference counted RtecEventComm::EventSet
 * and classes to automatically manage this reference count.
 *
 *
 */
//=============================================================================
#ifndef TAO_EC_CRC_H
#define TAO_EC_CRC_H
#include "ace/pre.h"
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



/**
 * @namespace TAO_Event_CRC
 *
 * @brief  Temp CRC routines - need to be able to compute CRC's
 * across blocks of memory.
 *
 * @NOTE: That this ONLY works with 32-bit machines.
 *
 */
namespace TAO_Event_CRC
{

  /// Initialize the CRC
  void initialize (void);

  /// Compute CRC
  u_int compute_crc (iovec iov[],
                     int iovcnt);
 }

#include "ace/post.h"
#endif /*TAO_EC_CRC*/
