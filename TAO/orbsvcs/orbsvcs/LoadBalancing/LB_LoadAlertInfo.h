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

  /// True if the LoadAlert object has been alerted about a given load
  /// condition.  False otherwise.
  CORBA::Boolean alerted;

};


#include "ace/post.h"

#endif  /* TAO_LB_LOAD_ALERT_INFO_H */
