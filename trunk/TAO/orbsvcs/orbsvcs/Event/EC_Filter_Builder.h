/* -*- C++ -*- */
/**
 *  @file   EC_Filter_Builder.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_FILTER_BUILDER_H
#define TAO_EC_FILTER_BUILDER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Filter;
class TAO_EC_ProxyPushSupplier;

/**
 * @class TAO_EC_Filter_Builder
 *
 * @brief Abstract base class for the filter builders.
 *
 * The creation of a filter hierarchy is controlled by a
 * Filter_Builder. The relationship between TAO_EC_Filter and
 * TAO_EC_Filter_Builder follows the "Builder Pattern" (Gamma et
 * al.)
 */
class TAO_RTEvent_Export TAO_EC_Filter_Builder
{
public:
  /// destructor...
  virtual ~TAO_EC_Filter_Builder (void);

  /// Create the filter, the caller must assume ownership of the filter
  /// returned.
  virtual TAO_EC_Filter*
      build (TAO_EC_ProxyPushSupplier *supplier,
             RtecEventChannelAdmin::ConsumerQOS& qos
             ACE_ENV_ARG_DECL) const = 0;

};

// ****************************************************************

/**
 * @class TAO_EC_Null_Filter_Builder
 *
 * @brief A simple implementation of the filter builder.
 *
 * Simply creates a Null_Filter in every case.
 */
class TAO_RTEvent_Export TAO_EC_Null_Filter_Builder : public TAO_EC_Filter_Builder
{
public:
  /// constructor.
  TAO_EC_Null_Filter_Builder (void);

  /// destructor...
  virtual ~TAO_EC_Null_Filter_Builder (void);

  // = The TAO_EC_Filter_Builder methods...
  TAO_EC_Filter* build (TAO_EC_ProxyPushSupplier *supplier,
                        RtecEventChannelAdmin::ConsumerQOS& qos
                        ACE_ENV_ARG_DECL) const;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_FILTER_BUILDER_H */
