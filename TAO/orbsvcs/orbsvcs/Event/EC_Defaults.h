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

// Any of the definitions below can be overriden in the config.h file
// or the compilation line.

// Control consumer and supplier reconnections
#ifndef  TAO_EC_DEFAULT_CONSUMER_RECONNECT
# define TAO_EC_DEFAULT_CONSUMER_RECONNECT 0
#endif /*  TAO_EC_DEFAULT_CONSUMER_RECONNECT */

#ifndef TAO_EC_DEFAULT_SUPPLIER_RECONNECT
#define TAO_EC_DEFAULT_SUPPLIER_RECONNECT 0
#endif /* TAO_EC_DEFAULT_SUPPLIER_RECONNECT */

// Control callbacks when proxies are disconnected
#ifndef TAO_EC_DEFAULT_DISCONNECT_CALLBACKS
# define TAO_EC_DEFAULT_DISCONNECT_CALLBACKS 0
#endif /* TAO_EC_DEFAULT_DISCONNECT_CALLBACKS */

// Control the maximum degree of concurrency tolerated by the EC, some
// kind of limit is required to avoid starvation of delayed write
// operations.
#ifndef TAO_EC_DEFAULT_BUSY_HWM
# define TAO_EC_DEFAULT_BUSY_HWM 1024
#endif /* TAO_EC_DEFAULT_BUSY_HWM */

#ifndef TAO_EC_DEFAULT_MAX_WRITE_DELAY
# define TAO_EC_DEFAULT_MAX_WRITE_DELAY 2048
#endif /* TAO_EC_DEFAULT_MAX_WRITE_DELAY */

// The defaults for the default factory.
// The TAO_EC_Default_Factory is a run-time configurable factory for
// the event channel.  Using the service configurator the user can
// specify what strategies should the factory produce.
// This macros control the default values for those strategies.
//
#ifndef TAO_EC_DEFAULT_DISPATCHING
# define TAO_EC_DEFAULT_DISPATCHING 0         /* reactive */
#endif /* TAO_EC_DEFAULT_DISPATCHING */

#ifndef TAO_EC_DEFAULT_CONSUMER_FILTER
# define TAO_EC_DEFAULT_CONSUMER_FILTER 1     /* type, disj. conj. timers */
#endif /* TAO_EC_DEFAULT_CONSUMER_FILTER */

#ifndef TAO_EC_DEFAULT_SUPPLIER_FILTER
# define TAO_EC_DEFAULT_SUPPLIER_FILTER 1     /* per supplier */
#endif /* TAO_EC_DEFAULT_SUPPLIER_FILTER */

#ifndef TAO_EC_DEFAULT_TIMEOUT
# define TAO_EC_DEFAULT_TIMEOUT 0             /* reactive */
#endif /* TAO_EC_DEFAULT_TIMEOUT */

#ifndef TAO_EC_DEFAULT_OBSERVER
# define TAO_EC_DEFAULT_OBSERVER 0            /* null */
#endif /* TAO_EC_DEFAULT_OBSERVER */

#ifndef TAO_EC_DEFAULT_SCHEDULING
# define TAO_EC_DEFAULT_SCHEDULING 0          /* null */
#endif /* TAO_EC_DEFAULT_SCHEDULING */

#ifndef TAO_EC_DEFAULT_SUPPLIER_SET
# define TAO_EC_DEFAULT_SUPPLIER_SET 1        /* delayed MT-safe */
#endif /* TAO_EC_DEFAULT_SUPPLIER_SET */

#ifndef TAO_EC_DEFAULT_CONSUMER_LOCK
# define TAO_EC_DEFAULT_CONSUMER_LOCK 1       /* thread */
#endif /* TAO_EC_DEFAULT_CONSUMER_LOCK */

#ifndef TAO_EC_DEFAULT_SUPPLIER_LOCK
# define TAO_EC_DEFAULT_SUPPLIER_LOCK 1       /* thread */
#endif /* TAO_EC_DEFAULT_SUPPLIER_LOCK */

#ifndef TAO_EC_DEFAULT_CONSUMER_ADMIN_LOCK
# define TAO_EC_DEFAULT_CONSUMER_ADMIN_LOCK 1 /* thread */
#endif /* TAO_EC_DEFAULT_CONSUMER_ADMIN_LOCK */

#ifndef TAO_EC_DEFAULT_SUPPLIER_ADMIN_LOCK
# define TAO_EC_DEFAULT_SUPPLIER_ADMIN_LOCK 1 /* thread */
#endif /* TAO_EC_DEFAULT_SUPPLIER_ADMIN_LOCK */

#endif /* TAO_EC_DEFAULTS_H */
