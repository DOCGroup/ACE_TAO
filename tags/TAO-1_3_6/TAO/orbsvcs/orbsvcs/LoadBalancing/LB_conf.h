// -*- C++ -*-

//=============================================================================
/**
 * @file LB_conf.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_CONF_H
#define TAO_LB_CONF_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#ifndef TAO_LB_PULL_HANDLER_INTERVAL
/// The interval in seconds the load balancer queries registered load
/// monitors for loads.
const long TAO_LB_PULL_HANDLER_INTERVAL = 5;
#endif  /* TAO_LB_PULL_HANDLER_INTERVAL */

#ifndef TAO_LB_PULL_HANDLER_RESTART
/// The time in seconds the load balancer's "pull handler" is
/// restarted after being fired.
const long TAO_LB_PULL_HANDLER_RESTART = 5;
#endif  /* TAO_LB_PULL_HANDLER_RESTART */


#include /**/ "ace/post.h"

#endif  /* TAO_LB_CONF_H */
