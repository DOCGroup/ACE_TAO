/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Defaults
//
// = DESCRIPTION
//   In this file we set the compile time defaults for the event
//   channel.
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

#ifndef TAO_EC_DEFAULTS_H
#define TAO_EC_DEFAULTS_H


// Control consumer and supplier reconnections
#define TAO_EC_DEFAULT_CONSUMER_RECONNECT 0
#define TAO_EC_DEFAULT_SUPPLIER_RECONNECT 0

// Control the maximum degree of concurrency tolerated by the EC, some
// kind of limit is required to avoid starvation of delayed write
// operations.
#define TAO_EC_DEFAULT_BUSY_HWM 1024
#define TAO_EC_DEFAULT_MAX_WRITE_DELAY 2048

// The defaults for the default factory.
// The TAO_EC_Default_Factory is a run-time configurable factory for
// the event channel.  Using the service configurator the user can
// specify what strategies should the factory produce.
// This macros control the default values for those strategies.
//
#define TAO_EC_DEFAULT_DISPATCHING 0         /* reactive */
#define TAO_EC_DEFAULT_CONSUMER_FILTER 1     /* type, disj. conj. timers */
#define TAO_EC_DEFAULT_SUPPLIER_FILTER 1     /* per supplier */
#define TAO_EC_DEFAULT_TIMEOUT 0             /* reactive */
#define TAO_EC_DEFAULT_OBSERVER 0            /* null */
#define TAO_EC_DEFAULT_SCHEDULING 0          /* null */
#define TAO_EC_DEFAULT_SUPPLIER_SET 1        /* delayed MT-safe */
#define TAO_EC_DEFAULT_CONSUMER_LOCK 1       /* thread */
#define TAO_EC_DEFAULT_SUPPLIER_LOCK 1       /* thread */
#define TAO_EC_DEFAULT_CONSUMER_ADMIN_LOCK 1 /* thread */
#define TAO_EC_DEFAULT_SUPPLIER_ADMIN_LOCK 1 /* thread */

#endif /* TAO_EC_DEFAULTS_H */
