// $Id$

#ifndef METRICS_UTILS_H
#define METRICS_UTILS_H


#if defined (ACE_METRICS_COLLECTION)

#if defined (ACE_ENABLE_TIMEPROBES) && defined (ACE_COMPILE_TIMEPROBES)

#if ! defined (VXWORKS)

// Comment out both of these to disable output
#  define METRICS_UTILS_ERROR_OUTPUT_ENABLED
#  define METRICS_UTILS_DEBUG_OUTPUT_ENABLED

#endif /* ! VXWORKS */

#include "ace/OS.h"
#include "ace/Singleton.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/MetricsC.h"
#include "metrics_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Metrics_Export TAO_Metrics_Utils
{
  // = TITLE
  //   Defines a class for various metrics utility functions.
  //
  // = DESCRIPTION
  //   This class provides static methods for various general
  //   purpose tasks for managing and using the metrics framework.

public:

  // Type declarations

  enum Base_Metrics_Type {SCHED, QUO, RTARM, TNS, TEEC};

  static const u_int METRICS_BUFSIZE;
  // Constant size for local buffers.

  static const u_int METRICS_ENQUEUE_PROBE_ID_BASE;
  static const u_int METRICS_DEQUEUE_PROBE_ID_BASE;
  static const u_int METRICS_SCHED_PROBE_ID_BASE;
  static const u_int METRICS_QUO_PROBE_ID_BASE;
  static const u_int METRICS_TNS_PROBE_ID_BASE;
  static const u_int METRICS_TEEC_PROBE_ID_BASE;
  static const u_int METRICS_RTARM_PROBE_ID_BASE;
  static const u_int METRICS_UPCALL_MONITOR_ADAPTER_ID_BASE;
  // Constants for offsets into ranges of probe ids.

  static void set_mission_state (int i, Metrics::QoSLogger_ptr logger);
  // Sets the mission state in the logger referenced by the passed pointer.

  static void set_mission_state (int i, const char * logger_ior_str);
  // Sets the mission state in the logger referenced by the passed ior string.

  static Metrics::QoSLogger_var get_logger (const char * logger_ior_str,
                                            CORBA::ORB_ptr orb_);
  // Gets a pointer to a logger from the passed logger ior string,
  // using the passed ORB.

  static RtecScheduler::handle_t timeoutEvent (void);
  // Accessor for static timeout event handle.  This handle is ignored
  // by the QoS monitor, since by definition it will be dispatched
  // after the end-of-frame.

  static void timeoutEvent (RtecScheduler::handle_t handle);
  // Mutator for static timeout event handle.  This handle is ignored
  // by the QoS monitor, since by definition it will be dispatched
  // after the end-of-frame.

private:

  static RtecScheduler::handle_t timeoutEvent_;
};


struct TAO_Metrics_Export WSOA_Metrics_Handles
{
  // = TITLE
  //   Defines a helper class for WSOA METRICS INSTRUMENTATION.
  //
  // = DESCRIPTION
  //   This class provides a single point of initialization (registration)
  //   and storage for the handles used in WSOA metrics collection.  It is necessary
  //   to define this class at this level to cross-cut multiple layers of the
  //   architecture and provide consistent points of instrumentation across layers.

public:

  WSOA_Metrics_Handles ();
  // Default constructor - registers all handles.

  // number of tile handles to reserve in each category.
  static const u_int WSOA_METRICS_MAX_TILING_FACTOR;

  // global attribute to decide when image is downloaded.
  u_long tiles_requested_;

  // Globally accessible handles for METRICS INSTRUMENTATION
  RtecScheduler::handle_t compute_metrics_handle_;
  RtecScheduler::handle_t RTARM_triggered_adaptation_metrics_handle_;
  RtecScheduler::handle_t RTARM_region_transition_metrics_handle_;
  RtecScheduler::handle_t QuO_contract_eval_metrics_handle_;
  RtecScheduler::handle_t tile_request_delegate_metrics_handle_;
  RtecScheduler::handle_t image_received_metrics_handle_;
  RtecScheduler::handle_t image_processed_metrics_handle_;
  RtecScheduler::handle_t image_quiescent_metrics_handle_;
  RtecScheduler::handle_t tile_received_first_metrics_handle_;
  RtecScheduler::handle_t tile_queuing_first_metrics_handle_;
  RtecScheduler::handle_t tile_decompression_first_metrics_handle_;
  RtecScheduler::handle_t tile_decompression_handle_;
  RtecScheduler::handle_t tile_ipm_first_metrics_handle_;
  RtecScheduler::handle_t tile_processed_first_metrics_handle_;

  RtecScheduler::handle_t zlib_compression_handle_;
  RtecScheduler::handle_t zlib_decompression_handle_;
};

typedef ACE_Singleton<WSOA_Metrics_Handles, ACE_SYNCH_MUTEX>
WSOA_METRICS_HANDLES_SINGLETON;

/**
 * NOTE: This is a simple macro which calls the appropriate registration function with the cache to
 *       store the data from this pointer. At the ACE level there is no need to give a PROBE_TYPE
 *       because there is only one probe type (ACE_Timeprobe).
 *
 * USAGE_LEVEL: TAO
 *
 * Q1: Where is this and the next macro used? One example is on TAO/orbsvcs/orbsvcs/Metrics. No where in the ace code.
 *    So is this a ACE or TAO macro? 
 *
 * Q2: Is the function (register + METRICS_REGION) a ACE or TAO?
 * A2: TAO. Define in Metrics_Local_Cache_T.h. The only METRICS_REGION that exists is 'base_metrics'. Used
 *     in $TAO_ROOT/orbsvcs/orbsvcs/Metrics/Metrics_Utils.cpp
 *
 * @param METRICS_PTR - Initialized pointer to the Metrics_Cache. This pointer is used to register the probe
 *                      with the associated cache.
 * @param METIRCS_REGION - At present there is only one 'base_metrics'.
 * @param PROBE_NAME - String name of the probe
 * @param PROBE_TYPE - Unsigned integer representing the probe type
 * @param METRICS_HANDLE - Variable used to contain the metrics probe handle that is the result of this function
 *                         The type of the handle is a RtecScheduler::handle_t
 */
#  define REGISTER_METRICS_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_HANDLE) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_HANDLE = METRICS_PTR-> \
register_##METRICS_REGION ( \
PROBE_NAME, \
PROBE_TYPE); \
} } while (0)

/**
 * Same as above but instead of just reporting information to a cache there is also reference to a Metrics_Loger (TAO feature).
 * The logger is used to record the information. The type of probe is register with the logger
 *
 * USAGE_LEVEL: TAO
 */
#  define REGISTER_METRICS_REPORTING_PROBE_RETURN(METRICS_PTR,METRICS_REGION,PROBE_NAME,PROBE_TYPE,METRICS_LOGGER_REF,METRICS_HANDLE) \
do { if((METRICS_PTR->metrics_enabled())) { \
METRICS_HANDLE = METRICS_PTR-> \
register_##METRICS_REGION## ( \
PROBE_NAME, \
PROBE_TYPE, \
METRICS_LOGGER_REF); \
} } while (0)




#if defined (__ACE_INLINE__)
#include "Metrics_Utils.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_ENABLE_TIMEPROBES & ACE_COMPILE_TIMEPROBES */
#endif /* ACE_METRICS_COLLECTION */

#endif /* METRICS_UTILS_H */
