// $Id$

#ifndef METRICS_LOGGER_H
#define METRICS_LOGGER_H

#if ! defined (VXWORKS)

// Comment out both of these to disable output
#  define METRICS_LOGGER_ERROR_OUTPUT_ENABLED
#  define METRICS_LOGGER_DEBUG_OUTPUT_ENABLED

#endif /* ! VXWORKS */

// Comment this out to disable event passing
#define TAO_METRICS_LOGGER_SENDS_EVENTS

#include "ace/OS.h"
#include "ace/Task.h"
#include "ace/Singleton.h"
#include "ace/Functor.h"
#include "ace/Hash_Map_Manager.h"

//#include "tao/TAO.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/MetricsS.h"
#include "orbsvcs/RtecEventCommS.h"

#include "Metrics_Utils.h"

#if defined (METRICS_LOGGER_SENDS_EVENTS)
#include "MetricsC.h"
#include "DOVE_Supplier.h"
#endif /* METRICS_LOGGER_SENDS_EVENTS */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Metrics_Export TAO_Metrics_Logger_Data
{
  // = TITLE
  //   Defines a class for logger data.
  //
  // = DESCRIPTION
  //   This class is used to transfer metrics data to the logger class.
public:

  enum LOGGER_DATA_TYPE {QOS_TYPE, TIMEPROBE_TYPE, BANNER_TYPE};

  TAO_Metrics_Logger_Data (LOGGER_DATA_TYPE data_type,
                    Metrics::Time interval = ORBSVCS_Time::zero ())
    : data_type_ (data_type),
      interval_ (interval),
      qos_params_ (0),
      timeprobe_params_ (0),
      banner_ (0)
    {
    }
  // Constructor.

  ~TAO_Metrics_Logger_Data ()
    {
      delete qos_params_;
      delete timeprobe_params_;
      delete banner_;
    }
  // Destructor.

  LOGGER_DATA_TYPE data_type_;
  // Identifies type of data.

  Metrics::Time interval_;
  // Time interval over which data were collected.

  Metrics::QoSParameter_Set * qos_params_;
  // Points to a QoS parameter data set.

  Metrics::TimeprobeParameter_Set * timeprobe_params_;
  // Points to a timeprobe data set.

  char * banner_;
  // Banner for entry in the log and/or visualization stream.
};



class TAO_Metrics_Export TAO_Metrics_Logger
  : public ACE_Task<ACE_MT_SYNCH>, public POA_Metrics::QoSLogger
  // = TITLE
  //   This class implements a Metrics QoS logger.
  //
  // = DESCRIPTION
  //   This class allows clients to log monitored QoS.
{
public:

  typedef ACE_Hash_Map_Manager_Ex<CORBA::ULong,
	                          CORBA::String_var *,
                                  ACE_Hash<CORBA::ULong>,
                                  ACE_Equal_To<CORBA::ULong>,
                                  ACE_Null_Mutex> PROBE_NAME_MAP;
  // Type of map used for O(1) lookup of probe names.

  typedef ACE_Hash_Map_Entry<CORBA::ULong, CORBA::String_var> PROBE_NAME_MAP_ENTRY;
  // Type of map used for O(1) lookup of RT_Infos by their handles.

  TAO_Metrics_Logger (int generate_events = 0, int generate_log = 0, int generate_export_file = 1, const char *log_filename = 0);
  // Default constructor.

  virtual ~TAO_Metrics_Logger ();
  // Destructor.

  virtual int svc (void);
  // Active object loop for processing logged data.

  virtual void send_banner (
        const char * banner
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
/*
  void send_banner (const char *banner,
                    CORBA::Environment &ACE_TRY_ENV =
                        CORBA::Environment::default_environment ());
  // Sends a banner to be written to the log file and to the visualization browser.
*/

  void process_banner (const char *banner);
  // Writes banner to the log file and/or to the visualization browser.

  void log_aggregate_QoS (const Metrics::QoSParameter_Set & qos_params,
                          Metrics::Time interval,
                          CORBA::Environment &ACE_TRY_ENV =
                            CORBA::Environment::default_environment ());
  // Reports the aggregate QoS information for all operations
  // to the passed QoS logger.

  void process_aggregate_QoS (const Metrics::QoSParameter_Set & qos_params,
                              Metrics::Time interval);
  // Processes the aggregate QoS information for all operations
  // to the passed QoS logger.

  void set_identities (const Metrics::ProbeIdentity_Set & probe_identities,
                       CORBA::Environment &ACE_TRY_ENV =
                         CORBA::Environment::default_environment ());
  // Binds the names of various timeprobes to their identifiers so
  // that ids alone can be used (for efficiency) in passing data.

  void set_identity (const Metrics::ProbeIdentity_t & probe_identity,
                     CORBA::Environment &ACE_TRY_ENV =
                       CORBA::Environment::default_environment ());
  // Binds the names of various timeprobes to their identifiers so
  // that ids alone can be used (for efficiency) in passing data.

  void log_timeprobe_data (const Metrics::TimeprobeParameter_Set & timeprobe_params,
                           Metrics::Time interval,
                           CORBA::Environment &ACE_TRY_ENV =
                             CORBA::Environment::default_environment ());
  // Reports timeprobe data collected since last run.

  void process_timeprobe_data (const Metrics::TimeprobeParameter_Set & timeprobe_params,
                               Metrics::Time interval);
  // Processes timeprobe data collected since last run.

protected:

  int generate_events_;
  // Indicates whether or not to generate events for displays.

  int generate_log_;
  // Indicates whether or not to generate a text log.

  const char *log_filename_;
  // Name of the text log file (if any)

  int generate_totals_;
  // Indicates whether information of totals are logged.

  int generate_export_file_;
  // Indicates whether information of totals are logged.

  int log_started_;
  // Indicates whether or not the log has been started already.

  FILE *log_file_;
  // Handle to the text log file (if any).

  FILE *export_file_;
  // Handle to the excel export log file (if any).

  PROBE_NAME_MAP probe_name_map_;
  // Maps probe ids to their names.

#if defined (METRICS_LOGGER_SENDS_EVENTS)
  DOVE_Supplier dove_supplier_;
  // DOVE Event supplier.

  Metrics::QoSData qos_data_;
  // QoS data structure.

  Metrics::UtilData util_data_;
  // Utilization data structure.

#endif /* METRICS_LOGGER_SENDS_EVENTS */

  // Only write the file once every OPEN_CLOSE_BANNER_CYCLE banner
  // changes
  int saveFileCnt_;
};

typedef ACE_Singleton<Metrics::QoSLogger_var, ACE_SYNCH_MUTEX>
TAO_METRICS_LOGGER_REF_SINGLETON;

typedef ACE_Singleton<TAO_Metrics_Logger, ACE_SYNCH_MUTEX>
TAO_METRICS_LOGGER_SINGLETON;


#endif /* METRICS_LOGGER_H */






