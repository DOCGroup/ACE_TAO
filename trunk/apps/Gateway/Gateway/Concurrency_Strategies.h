/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Concurrency_strategies.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CONCURRENCY_STRATEGIES)
#define _CONCURRENCY_STRATEGIES

#include "ace/Synch.h"

// The following typedefs are used in order to parameterize the
// synchronization policies without changing the source code!

// If we don't have threads then use the single-threaded synchronization.
#if !defined (ACE_HAS_THREADS)
#define SYNCH_STRATEGY ACE_NULL_SYNCH
typedef ACE_Null_Mutex MAP_MUTEX;
#else /* ACE_HAS_THREADS */

// Note that we only need to make the ACE_Task thread-safe if we are
// using the multi-threaded Thr_Consumer_Proxy...
#if defined (USE_OUTPUT_MT) 
#define SYNCH_STRATEGY ACE_MT_SYNCH
#else
#define SYNCH_STRATEGY ACE_NULL_SYNCH
#endif /* USE_OUTPUT_MT || USE_INPUT_MT */

// Note that we only need to make the ACE_Map_Manager thread-safe if
// we are using the multi-threaded Thr_Supplier_Proxy.  In this
// case, we use an RW_Mutex since we'll lookup Consumers far more
// often than we'll update them.
#if defined (USE_INPUT_MT)
typedef ACE_RW_Mutex MAP_MUTEX;
#else
typedef ACE_Null_Mutex MAP_MUTEX;
#endif /* USE_INPUT_MT */
#endif /* ACE_HAS_THREADS */

// = Forward decls
class Thr_Consumer_Proxy;
class Thr_Supplier_Proxy;
class Consumer_Proxy;
class Supplier_Proxy;

#if defined (ACE_HAS_THREADS) && (defined (USE_OUTPUT_MT) || defined (USE_INPUT_MT))
#if defined (USE_OUTPUT_MT)
typedef Thr_Consumer_Proxy CONSUMER_HANDLER;
#else
typedef Consumer_Proxy CONSUMER_HANDLER;
#endif /* USE_OUTPUT_MT */

#if defined (USE_INPUT_MT)
typedef Thr_Supplier_Proxy SUPPLIER_HANDLER;
#else
typedef Supplier_Proxy SUPPLIER_HANDLER;
#endif /* USE_INPUT_MT */
#else
// Instantiate a non-multi-threaded Gateway.
typedef Supplier_Proxy SUPPLIER_HANDLER;
typedef Consumer_Proxy CONSUMER_HANDLER;
#endif /* ACE_HAS_THREADS */

#endif /* _CONCURRENCY_STRATEGIES */
