/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Event_Channel
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Filters compute QOS information for real-time dispatching, this
//   class encapsulates that information.
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
// ============================================================================

#ifndef TAO_EC_QOS_INFO_H
#define TAO_EC_QOS_INFO_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_QOS_Info
{
  // = TITLE
  //   A representation of QoS information for the event channel
  //   filters.
  //
  // = DESCRIPTION
  //   This first implementation is just a place-holder.
  //
public:
  TAO_EC_QOS_Info (void);
  // constructor

};

#if defined (__ACE_INLINE__)
#include "EC_QOS_Info.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_EVENT_CHANNEL_H */
