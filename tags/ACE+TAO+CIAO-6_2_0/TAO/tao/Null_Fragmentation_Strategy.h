// -*- C++ -*-

//=============================================================================
/**
 *  @file    Null_Fragmentation_Strategy.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_NULL_FRAGMENTATION_STRATEGY_H
#define TAO_NULL_FRAGMENTATION_STRATEGY_H

#include /**/ "ace/pre.h"

#include "tao/GIOP_Fragmentation_Strategy.h"
#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_OutputCDR;


/**
 * @class TAO_Null_Fragmenation_Strategy
 *
 * @brief Null GIOP message fragmentation strategy.
 *
 * No-op GIOP message fragmentation strategy.  This strategy performs
 * no GIOP fragmentation.
 *
 * @see TAO_GIOP_Fragmentation_Strategy
 */
class TAO_Null_Fragmentation_Strategy
  : public TAO_GIOP_Fragmentation_Strategy
{
public:

  TAO_Null_Fragmentation_Strategy (void) {}
  virtual ~TAO_Null_Fragmentation_Strategy (void);
  virtual int fragment (TAO_OutputCDR &, ACE_CDR::ULong, ACE_CDR::ULong);

private:

  // Disallow copying and assignment.
  TAO_Null_Fragmentation_Strategy (TAO_Null_Fragmentation_Strategy const &);
  void operator= (TAO_Null_Fragmentation_Strategy const &);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_NULL_FRAGMENTATION_STRATEGY_H */
