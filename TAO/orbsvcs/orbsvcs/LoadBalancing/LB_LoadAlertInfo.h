// -*- C++ -*-

//=======================================================================
/**
 *  @file    LB_LoadAlertInfo.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_LOAD_ALERT_INFO_H
#define TAO_LB_LOAD_ALERT_INFO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosLoadBalancingC.h"


/**
 * @class TAO_LB_LoadAlertInfo
 *
 * @brief Structure that contains all LoadAlert-specific information.
 *
 * Structure that contains all LoadAlert-specific information.
 */
struct TAO_LB_LoadAlertInfo
{
  /// Constructor.
  TAO_LB_LoadAlertInfo (void);

  /// Reference to the LoadAlert object.
  CosLoadBalancing::LoadAlert_var load_alert;

  /// The location at which the member resides.
  PortableGroup::Location location;

  /// True if the LoadAlert object has been alerted about a given load
  /// condition.  False otherwise.
  CORBA::Boolean alerted;

  /// Equality operator.
  /**
   * For the purposes of the LoadAlert info set, only the location is
   * important.  In particular, if a LoadAlert object already resides
   * at the given location for a particular object group, this
   * equality operator will return true.
   */
  int operator== (const TAO_LB_LoadAlertInfo & rhs);

};


typedef ACE_Unbounded_Set<TAO_LB_LoadAlertInfo> TAO_LB_LoadAlertInfoSet;


#include "ace/post.h"

#endif  /* TAO_LB_LOAD_ALERT_INFO_H */
