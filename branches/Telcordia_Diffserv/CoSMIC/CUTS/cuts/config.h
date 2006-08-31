// -*- C++ -*-

//=============================================================================
/**
 * @file    CUTS.h
 *
 * $Id$
 *
 * This files contains macro definitions that are global to the CUTS
 * module, such as exporting and importing objects
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_CONFIG_H_
#define _CUTS_CONFIG_H_

#if defined (__CUTS_INLINE__)
# define CUTS_INLINE inline
#else
# define CUTS_INLINE
#endif

#define CUTS_USERNAME           "cuts"

#define CUTS_PASSWORD           "cuts"

#define CUTS_DEFAULT_HOSTNAME   "localhost"

#define CUTS_DEFAULT_PORT       3306

#define CUTS_INIT_ACTIVATION_RECORD_COUNT  5

#define CUTS_PORT_AGENT_THREAD_COUNT 1

#define CUTS_EVENT_HANDLER_THREAD_COUNT 1

#endif  // !defined _CUTS_CONFIG_H_
