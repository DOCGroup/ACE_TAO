// -*- C++ -*-

// ================================================================
/**
 *  @file   Connection_Purging_Strategy.h
 *
 *  $Id$
 *
 *  @author Chad Elliott (elliott_c@ociweb.com)
 */
// ================================================================

#ifndef TAO_PURGING_STRATEGY_H
#define TAO_PURGING_STRATEGY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;

// ****************************************************************

/**
 * @class TAO_Connection_Purging_Strategy
 *
 * @brief The base class for all purging strategies
 *
 * This class is used by the Transport_Cache_Manager to maintain
 * ordering information on each Transport that is created.
 */

class TAO_Export TAO_Connection_Purging_Strategy
{
public:
  /// The constructor
  TAO_Connection_Purging_Strategy (int cache_maximum);

  /// The destructor
  virtual ~TAO_Connection_Purging_Strategy (void);

  /// Return the maximum number of cache entries
  virtual int cache_maximum (void) const;

  /// Sub-classes must implement these methods
  virtual void update_item (TAO_Transport& transport) = 0;

private:
  /// The maximum number of cach entries
  int cache_maximum_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Connection_Purging_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_PURGING_STRATEGY_H */
