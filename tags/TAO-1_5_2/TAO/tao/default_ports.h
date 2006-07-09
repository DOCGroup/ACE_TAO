// -*- C++ -*-

//=============================================================================
/**
 *  @file     default_ports.h
 *
 *  $Id$
 *
 *  Port reservation details for all the services.
 *
 *  @author  Copyright 1995 by Sun Microsystems, Inc.
 *  @author  Chris Cleeland
 *  @author  Douglas C. Schmidt
 */
//=============================================================================

#ifndef TAO_DEFAULT_PORTS_H
#define TAO_DEFAULT_PORTS_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// The default UDP multicast port number for locating the TAO Naming
// Service.
#if !defined (TAO_DEFAULT_NAME_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_NAME_SERVER_REQUEST_PORT 10013
#endif /* TAO_DEFAULT_NAME_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Naming Service.
#if !defined (TAO_DEFAULT_NAME_SERVER_REPLY_PORT)
#define TAO_DEFAULT_NAME_SERVER_REPLY_PORT 10014
#endif /* TAO_DEFAULT_NAME_SERVER_REPLY_PORT */

// The default UDP multicast port number for locating the TAO Trading
// Service.
#if !defined (TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT 10016
#endif /* TAO_DEFAULT_TRADING_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Trading Service.
#if !defined (TAO_DEFAULT_TRADING_SERVER_REPLY_PORT)
#define TAO_DEFAULT_TRADING_SERVER_REPLY_PORT 10017
#endif /* TAO_DEFAULT_TRADING_SERVER_REPLY_PORT */

// The default UDP multicast port number for locating the TAO
// Implementation Repository Service.
#if !defined (TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT 10018
#endif /* TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Implementation Repository Service.
#if !defined (TAO_DEFAULT_IMPLREPO_SERVER_REPLY_PORT)
#define TAO_DEFAULT_IMPLREPO_SERVER_REPLY_PORT 10019
#endif /* TAO_DEFAULT_IMPLREPO_SERVER_REPLY_PORT */

// The default UDP multicast port number for locating the TAO
// Interface Repository Service.
#if !defined (TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT)
#define TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT 10020
#endif /* TAO_DEFAULT_INTERFACEREPO_SERVER_REQUEST_PORT */

// The default UDP port number for replying to a location request to
// the TAO Interface Repository Service.
#if !defined (TAO_DEFAULT_INTERFACEREPO_SERVER_REPLY_PORT)
#define TAO_DEFAULT_INTERFACEREPO_SERVER_REPLY_PORT 10021
#endif /* TAO_DEFAULT_INTERFACEREPO_SERVER_REPLY_PORT */

#include /**/ "ace/post.h"
#endif /*TAO_DEFAULT_PORTS_H*/
