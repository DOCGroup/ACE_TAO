/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Filter_Builder
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_FILTER_BUILDER_H
#define TAO_EC_FILTER_BUILDER_H

#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Filter;

class TAO_ORBSVCS_Export TAO_EC_Filter_Builder
{
  // = TITLE
  //   Abstract base class for the filter builders.
  //
  // = DESCRIPTION
  //   The creation of a builder hierarchy is controlled by a
  //   Filter_Builder. The relationship between TAO_EC_Filter and
  //   TAO_EC_Filter_Builder follows the "Builder Pattern" (Gamma et
  //   al.)
  //
  //
public:
  virtual ~TAO_EC_Filter_Builder (void);
  // destructor...

  virtual TAO_EC_Filter*
      build (RtecEventChannelAdmin::ConsumerQOS& qos) const = 0;
  // Create the filter, the caller must assume ownership of the filter
  // returned.

};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Null_Filter_Builder : public TAO_EC_Filter_Builder
{
  // = TITLE
  //   A simple implementation of the filter builder.
  //
  // = DESCRIPTION
  //   Simply creates a Null_Filter in every case.
  //
public:
  TAO_EC_Null_Filter_Builder (void);
  // constructor.

  virtual ~TAO_EC_Null_Filter_Builder (void);
  // destructor...

  // = The TAO_EC_Filter_Builder methods...
  TAO_EC_Filter* build (RtecEventChannelAdmin::ConsumerQOS& qos) const;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_FILTER_BUILDER_H */
