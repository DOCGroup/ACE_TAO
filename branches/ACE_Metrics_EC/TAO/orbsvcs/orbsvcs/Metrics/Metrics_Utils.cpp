// $Id$

#ifndef METRICS_UTILS_CPP
#define METRICS_UTILS_CPP

#include "Metrics_Utils.h"
#include "Metrics_LocalCache.h"

/////////////////////////////
// Class TAO_Metrics_Utils //
/////////////////////////////

// Static constants

const u_int WSOA_Metrics_Handles::WSOA_METRICS_MAX_TILING_FACTOR = 64;

const u_int TAO_Metrics_Utils::METRICS_BUFSIZE = 64;

const u_int TAO_Metrics_Utils::METRICS_ENQUEUE_PROBE_ID_BASE = 0;
const u_int TAO_Metrics_Utils::METRICS_DEQUEUE_PROBE_ID_BASE = 1000;
const u_int TAO_Metrics_Utils::METRICS_SCHED_PROBE_ID_BASE = 2000;
const u_int TAO_Metrics_Utils::METRICS_QUO_PROBE_ID_BASE = 3000;
const u_int TAO_Metrics_Utils::METRICS_RTARM_PROBE_ID_BASE = 4000;
const u_int TAO_Metrics_Utils::METRICS_TNS_PROBE_ID_BASE = 5000;
const u_int TAO_Metrics_Utils::METRICS_TEEC_PROBE_ID_BASE = 6000;
const u_int TAO_Metrics_Utils::METRICS_UPCALL_MONITOR_ADAPTER_ID_BASE = 7000;

// Static variable

RtecScheduler::handle_t TAO_Metrics_Utils::timeoutEvent_ = 72;



// Static method to set the mission state in the logger.

void
TAO_Metrics_Utils::set_mission_state (int i, Metrics::QoSLogger_ptr logger)
{
  if (CORBA::is_nil (logger))
    {
#if defined (METRICS_UTILS_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_Utils::set_mission_state:\n"
                          "Logger IOR reference is nil.\n"));
#endif
      return;
    }

  char banner [TAO_Metrics_Utils::METRICS_BUFSIZE];
  ACE_OS::memset (banner, 0, TAO_Metrics_Utils::METRICS_BUFSIZE);
  ACE_OS::sprintf (banner, "\n\n*** Mission State %d ***\n\n", i);

  ACE_TRY_NEW_ENV
    {
      logger->send_banner (banner);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
#if defined (METRICS_UTILS_ERROR_OUTPUT_ENABLED)
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO_Metrics_LocalCache::set_mission_state:"
                           " logger call failed\n");
#endif
    }
  ACE_ENDTRY;
}

// Sets the logger reference.

Metrics::QoSLogger_var
TAO_Metrics_Utils::get_logger (const char * logger_ior_str,
                               CORBA::ORB_ptr orb)
{
  Metrics::QoSLogger_var logger;
  if (logger_ior_str && orb)
    {
      ACE_TRY_NEW_ENV
        {
          CORBA::Object_var obj =
            orb->string_to_object (logger_ior_str);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (obj.in ()))
            {
#if defined (METRICS_UTILS_ERROR_OUTPUT_ENABLED)
              ACE_ERROR ((LM_ERROR,
                          "TAO_Metrics_Utils::get_logger:\n"
                          "Logger IOR string produced a "
                          "nil reference:\n[%s]",
                          logger_ior_str));
#endif
              return logger;
            }
          else
            {
              logger = Metrics::QoSLogger::_narrow (obj.in ());
              ACE_TRY_CHECK;

              if (CORBA::is_nil (logger.in ()))
                {
#if defined (METRICS_UTILS_ERROR_OUTPUT_ENABLED)
                  ACE_ERROR ((LM_ERROR,
                             "TAO_Metrics_LocalCache::set_logger:\n"
                             "Could not narrow IOR to logger:\n[%s]",
                             logger_ior_str));
#endif
                  return logger;
                }
            }
        }
      ACE_CATCHANY
        {
#if defined (METRICS_UTILS_ERROR_OUTPUT_ENABLED)
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "TAO_Metrics_LocalCache::set_logger exception");
#endif
          return logger;
        }
      ACE_ENDTRY;
    }
#if defined (METRICS_UTILS_ERROR_OUTPUT_ENABLED)
  else
    {
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_LocalCache::set_logger:\n"
                  "must pass a non-zero orb and ior pointer\n"));
    }
#endif
  return logger;
}


////////////////////////////////
// class WSOA_Metrics_Handles //
////////////////////////////////

// Default constructor - registers all WSOA METRICS INSTRUMENTATION handles.

WSOA_Metrics_Handles::WSOA_Metrics_Handles ()
  : tiles_requested_ (0)
  , compute_metrics_handle_(0)
  , RTARM_triggered_adaptation_metrics_handle_(0)
  , RTARM_region_transition_metrics_handle_(0)
  , QuO_contract_eval_metrics_handle_(0)
  , tile_request_delegate_metrics_handle_(0)
  , image_received_metrics_handle_(0)
  , image_processed_metrics_handle_(0)
  , image_quiescent_metrics_handle_(0)
  , tile_received_first_metrics_handle_(0)
  , tile_queuing_first_metrics_handle_(0)
  , tile_decompression_first_metrics_handle_(0)
  , tile_decompression_handle_(0)
  , tile_ipm_first_metrics_handle_(0)
  , tile_processed_first_metrics_handle_(0)
  , zlib_compression_handle_(0)
  , zlib_decompression_handle_(0)
{
#ifdef TAO_USES_REPORTING_METRICS
     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "TAO_compute_scheduling",
                                   TAO_Metrics_Utils::SCHED,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->compute_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "RTARM_triggered_adaptation",
                                   TAO_Metrics_Utils::RTARM,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->RTARM_triggered_adaptation_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "RTARM_region_transition",
                                   TAO_Metrics_Utils::RTARM,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->RTARM_region_transition_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "QuO_contract_eval",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->QuO_contract_eval_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_request_delegate",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->tile_request_delegate_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "image_received",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->image_received_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "image_processed",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->image_processed_metrics_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "image_quiescent",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->image_quiescent_metrics_handle_);

     char registration_name[64];
     u_int i;

     // Store the first tile received handle, and make room for the others.
     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_received_0",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->tile_received_first_metrics_handle_);

     RtecScheduler::handle_t temp_metrics_handle = 0;
     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_received_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                       temp_metrics_handle);
     }

     // Store the first tile processed handle, and make room for the others.
     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_processed_0",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->tile_processed_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_processed_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
        REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                      base_metrics,
                                      registration_name,
                                      TAO_Metrics_Utils::QUO,
                                      *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                      temp_metrics_handle);
     }

     // Store the first tile processed handle, and make room for the others.
     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_queuing_0",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->tile_queuing_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_queuing_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                       temp_metrics_handle);
     }

     // Store the first tile processed handle, and make room for the others.
     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_decompression_0",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->tile_decompression_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_decompression_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                       temp_metrics_handle);
     }

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_ipm_0",
                                   TAO_Metrics_Utils::QUO,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->tile_ipm_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_ipm_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                       temp_metrics_handle);
     }

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "zlib Compression",
                                   TAO_Metrics_Utils::SCHED,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->zlib_compression_handle_);

     REGISTER_METRICS_REPORTING_PROBE_RETURN(TAO_METRICS_REPORTING_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "zlib Compression",
                                   TAO_Metrics_Utils::SCHED,
                                   *TAO_METRICS_LOGGER_REF_SINGLETON::instance(),
                                   this->zlib_decompression_handle_);
#else /* ! TAO_USES_REPORTING_METRICS */
     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "TAO_compute_scheduling",
                                   TAO_Metrics_Utils::SCHED,
                                   this->compute_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "RTARM_triggered_adaptation",
                                   TAO_Metrics_Utils::RTARM,
                                   this->RTARM_triggered_adaptation_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "RTARM_region_transition",
                                   TAO_Metrics_Utils::RTARM,
                                   this->RTARM_region_transition_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "QuO_contract_eval",
                                   TAO_Metrics_Utils::QUO,
                                   this->QuO_contract_eval_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_request_delegate",
                                   TAO_Metrics_Utils::QUO,
                                   this->tile_request_delegate_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "image_received",
                                   TAO_Metrics_Utils::QUO,
                                   this->image_received_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "image_processed",
                                   TAO_Metrics_Utils::QUO,
                                   this->image_processed_metrics_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "image_quiescent",
                                   TAO_Metrics_Utils::QUO,
                                   this->image_quiescent_metrics_handle_);

     char registration_name[64];
     u_int i;

     // Store the first tile received handle, and make room for the others.
     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_received_0",
                                   TAO_Metrics_Utils::QUO,
                                   this->tile_received_first_metrics_handle_);

     //RtecScheduler::handle_t temp_metrics_handle;
     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_received_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       temp_metrics_handle);
     }

     // Store the first tile processed handle, and make room for the others.
     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_processed_0",
                                   TAO_Metrics_Utils::QUO,
                                   this->tile_processed_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_processed_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
        REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                      base_metrics,
                                      registration_name,
                                      TAO_Metrics_Utils::QUO,
                                      temp_metrics_handle);
     }

     // Store the first tile processed handle, and make room for the others.
     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_queuing_0",
                                   TAO_Metrics_Utils::QUO,
                                   this->tile_queuing_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_queuing_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       temp_metrics_handle);
     }

     // Store the first tile processed handle, and make room for the others.
     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_decompression_0",
                                   TAO_Metrics_Utils::QUO,
                                   this->tile_decompression_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_decompression_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       temp_metrics_handle);
     }

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "tile_ipm_0",
                                   TAO_Metrics_Utils::QUO,
                                   this->tile_ipm_first_metrics_handle_);

     for (i = 1; i <= WSOA_METRICS_MAX_TILING_FACTOR; ++i)
     {
             ::sprintf (registration_name, "tile_ipm_%d", i);

         // We just throw the handles away rather than storing them,
             // as they're always calculated by offset from the first.
         REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       temp_metrics_handle);

         REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                       base_metrics,
                                       registration_name,
                                       TAO_Metrics_Utils::QUO,
                                       tile_decompression_handle_);
     }

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "zlib Compression",
                                   TAO_Metrics_Utils::SCHED,
                                   this->zlib_compression_handle_);

     REGISTER_METRICS_PROBE_RETURN(TAO_METRICS_CACHE_SINGLETON_TYPE::instance(),
                                   base_metrics,
                                   "zlib Compression",
                                   TAO_Metrics_Utils::SCHED,
                                   this->zlib_decompression_handle_);
#endif /* TAO_USES_REPORTING_METRICS */
}



#if !defined (__ACE_INLINE__)
#include "Metrics_Utils.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<WSOA_Metrics_Handles, ACE_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<WSOA_Metrics_Handles, ACE_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* METRICS_UTILS_CPP */
