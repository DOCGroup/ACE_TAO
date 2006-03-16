// -*- C++ -*-

//=============================================================================
/**
 *  @file    GIOP_Fragmentation_Strategy.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_GIOP_FRAGMENTATION_STRATEGY_H
#define TAO_GIOP_FRAGMENTATION_STRATEGY_H

#include /**/ "ace/pre.h"

#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;

/**
 * @class TAO_GIOP_Fragmenation_Strategy
 *
 * @brief Abstract base class that defines TAO fragmentation strategy
 *        interface.
 *
 * GIOP message fragmentation is deferred to a fragmentation strategy
 */
class TAO_GIOP_Fragmentation_Strategy
{
public:

  /// Destructor.
  virtual ~TAO_GIOP_Fragmentation_Strategy (void);

  /// Fragment the (potentially partially) encoded GIOP message.
  /**
   * Fragmentation the contents of the CDR output stream @a cdr into
   * smaller chunks of data of size that fits within the configured
   * ORB fragmentation threshold, and send each fragment "over the
   * wire."
   *
   * @note Fragmentation will only occur if the CDR stream length will
   *       surpass the configured fragmentation threshold when
   *       marshaling the pending set of data.
   *
   * @arg cdr               Output CDR stream.
   * @arg pending_alignment Size of alignment boundary for next data
   *                        to be marshaled (e.g. 4 for a
   *                        CORBA::ULong).
   * @arg pending_length    Size of next data to be marshaled (e.g. 2
   *                        for a CORBA::UShort).
   */
  virtual void fragment (TAO_OutputCDR & cdr,
                         ACE_CDR::ULong pending_alignment,
                         ACE_CDR::ULong pending_length) = 0;

private:

  // Disallow copying and assignment.
  TAO_GIOP_Fragmentation_Strategy (TAO_GIOP_Fragmentation_Strategy const &);
  void operator= (TAO_GIOP_Fragmentation_Strategy const &);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_GIOP_FRAGMENTATION_STRATEGY_H */
