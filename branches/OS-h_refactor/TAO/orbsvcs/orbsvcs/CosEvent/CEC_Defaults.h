/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_Defaults.h
 *
 *  $Id$
 *
 * In this file we set the compile time defaults for the event
 * channel.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_DEFAULTS_H
#define TAO_CEC_DEFAULTS_H
#include "ace/pre.h"

#ifndef TAO_CEC_ENABLE_DEBUG_MESSAGES
#define TAO_CEC_ENABLE_DEBUG_MESSAGES 0
#endif /* TAO_CEC_ENABLE_DEBUG_MESSAGES */

// Any of the definitions below can be overriden in the config.h file
// or the compilation line.

// Control consumer and supplier reconnections
#ifndef  TAO_CEC_DEFAULT_CONSUMER_RECONNECT
# define TAO_CEC_DEFAULT_CONSUMER_RECONNECT 0
#endif /*  TAO_CEC_DEFAULT_CONSUMER_RECONNECT */

#ifndef TAO_CEC_DEFAULT_SUPPLIER_RECONNECT
#define TAO_CEC_DEFAULT_SUPPLIER_RECONNECT 0
#endif /* TAO_CEC_DEFAULT_SUPPLIER_RECONNECT */

// Control callbacks when proxies are disconnected
#ifndef TAO_CEC_DEFAULT_DISCONNECT_CALLBACKS
# define TAO_CEC_DEFAULT_DISCONNECT_CALLBACKS 0
#endif /* TAO_CEC_DEFAULT_DISCONNECT_CALLBACKS */

// Control the maximum degree of concurrency tolerated by the EC, some
// kind of limit is required to avoid starvation of delayed write
// operations.
#ifndef TAO_CEC_DEFAULT_BUSY_HWM
# define TAO_CEC_DEFAULT_BUSY_HWM 1024
#endif /* TAO_CEC_DEFAULT_BUSY_HWM */

#ifndef TAO_CEC_DEFAULT_MAX_WRITE_DELAY
# define TAO_CEC_DEFAULT_MAX_WRITE_DELAY 2048
#endif /* TAO_CEC_DEFAULT_MAX_WRITE_DELAY */

// The defaults for the default factory.
// The TAO_CEC_Default_Factory is a run-time configurable factory for
// the event channel.  Using the service configurator the user can
// specify what strategies should the factory produce.
// This macros control the default values for those strategies.
//
#ifndef TAO_CEC_DEFAULT_DISPATCHING
# define TAO_CEC_DEFAULT_DISPATCHING 0         /* reactive */
#endif /* TAO_CEC_DEFAULT_DISPATCHING */

#ifndef TAO_CEC_DEFAULT_PULLING_STRATEGY
# define TAO_CEC_DEFAULT_PULLING_STRATEGY 0         /* reactive */
#endif /* TAO_CEC_DEFAULT_PULLING_STRATEGY */

#ifndef TAO_CEC_DEFAULT_CONSUMER_COLLECTION
# define TAO_CEC_DEFAULT_CONSUMER_COLLECTION 0x003 /* delayed MT-safe */
#endif /* TAO_CEC_DEFAULT_CONSUMER_COLLECTION */

#ifndef TAO_CEC_DEFAULT_SUPPLIER_COLLECTION
# define TAO_CEC_DEFAULT_SUPPLIER_COLLECTION 0x003 /* delayed MT-safe */
#endif /* TAO_CEC_DEFAULT_SUPPLIER_COLLECTION */

#ifndef TAO_CEC_DEFAULT_CONSUMER_LOCK
# define TAO_CEC_DEFAULT_CONSUMER_LOCK 1       /* thread */
#endif /* TAO_CEC_DEFAULT_CONSUMER_LOCK */

#ifndef TAO_CEC_DEFAULT_SUPPLIER_LOCK
# define TAO_CEC_DEFAULT_SUPPLIER_LOCK 1       /* thread */
#endif /* TAO_CEC_DEFAULT_SUPPLIER_LOCK */

#ifndef TAO_CEC_DEFAULT_DISPATCHING_THREADS
# define TAO_CEC_DEFAULT_DISPATCHING_THREADS 1 /* */
#endif /* TAO_CEC_DEFAULT_DISPATCHING_THREADS */

#ifndef TAO_CEC_DEFAULT_DISPATCHING_THREADS_FLAGS
# define TAO_CEC_DEFAULT_DISPATCHING_THREADS_FLAGS THR_NEW_LWP|THR_BOUND|THR_SCHED_DEFAULT
#endif /* TAO_CEC_DEFAULT_DISPATCHING_THREADS */

#ifndef TAO_CEC_DEFAULT_DISPATCHING_THREADS_PRIORITY
# define TAO_CEC_DEFAULT_DISPATCHING_THREADS_PRIORITY ACE_THR_PRI_OTHER_DEF
#endif /* TAO_CEC_DEFAULT_DISPATCHING_THREADS_PRIORITY */

#ifndef TAO_CEC_DEFAULT_DISPATCHING_THREADS_FORCE_ACTIVE
# define TAO_CEC_DEFAULT_DISPATCHING_THREADS_FORCE_ACTIVE 1
#endif /* TAO_CEC_DEFAULT_DISPATCHING_THREADS_FORCE_ACTIVE */

#ifndef TAO_CEC_DEFAULT_REACTIVE_PULLING_PERIOD
# define TAO_CEC_DEFAULT_REACTIVE_PULLING_PERIOD 5000000 /* usecs */
#endif /* TAO_CEC_DEFAULT_REACTIVE_PULLING_PERIOD */

#ifndef TAO_CEC_DEFAULT_ORB_ID
# define TAO_CEC_DEFAULT_ORB_ID "" /* */
#endif /* TAO_CEC_DEFAULT_ORB_ID */

#ifndef TAO_CEC_DEFAULT_CONSUMER_CONTROL
# define TAO_CEC_DEFAULT_CONSUMER_CONTROL 0 /* null */
#endif /* TAO_CEC_DEFAULT_CONSUMER_CONTROL */

#ifndef TAO_CEC_DEFAULT_SUPPLIER_CONTROL
# define TAO_CEC_DEFAULT_SUPPLIER_CONTROL 0 /* null */
#endif /* TAO_CEC_DEFAULT_SUPPLIER_CONTROL */

#ifndef TAO_CEC_DEFAULT_CONSUMER_CONTROL_PERIOD
# define TAO_CEC_DEFAULT_CONSUMER_CONTROL_PERIOD 5000000 /* usecs */
#endif /* TAO_CEC_DEFAULT_CONSUMER_CONTROL_PERIOD */

#ifndef TAO_CEC_DEFAULT_SUPPLIER_CONTROL_PERIOD
# define TAO_CEC_DEFAULT_SUPPLIER_CONTROL_PERIOD 5000000 /* usecs */
#endif /* TAO_CEC_DEFAULT_SUPPLIER_CONTROL_PERIOD */

#include "ace/post.h"
#endif /* TAO_CEC_DEFAULTS_H */
