// $Id$

#include "Metrics_Logger.h"
#include "ace/Auto_Ptr.h"
#include "ace/ace_wchar.h"

ACE_RCSID(Metrics_Logger, Metrics_Logger, "$Id$")

const int    OPEN_CLOSE_BANNER_CYCLE = 4;


//////////////////////////////
// class TAO_Metrics_Logger //
//////////////////////////////

// Default constructor.

TAO_Metrics_Logger::TAO_Metrics_Logger (int generate_events,
                                        int generate_log,
                                        int generate_export_file,
                                        const char *log_filename)
  : generate_events_ (generate_events),
#ifdef VXWORKS
    generate_log_ (1),
    log_filename_ ("logger.txt"),
#else
    generate_log_ (generate_log),
    log_filename_ (log_filename),
#endif
    generate_totals_(0),
    generate_export_file_(generate_export_file),
    log_started_ (0),
    log_file_ (stdout),
    export_file_(stdout),
    saveFileCnt_(0)
{
#ifdef VXWORKS
  taskPrioritySet( taskIdSelf(), 145 );
#endif

#if defined (METRICS_LOGGER_SENDS_EVENTS)
  if (this->generate_events_)
    {
      // Connect the underlying DOVE event supplier.
      if (this->dove_supplier_.connect () < 0)
	{
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
          ACE_ERROR ((LM_ERROR, "failed to connect DOVE event supplier\n"));
#endif
          this->generate_events_ = 0;
	}
    }
#endif /* METRICS_LOGGER_SENDS_EVENTS */

  if (generate_log_)
    {
      if (log_filename_)
        {
          log_file_ = ACE_OS::fopen (log_filename_, "w+");
          if (log_file_ == NULL)
            {
              generate_log_ = 0;
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
              ACE_ERROR ((LM_ERROR,
                          "Logger failed to open log file %s\n",
                          log_filename_));
#endif
              return;
            }

          if (generate_export_file_)
          {
             export_file_ = ACE_OS::fopen ("remote_logger_export.excel", "w+");
             if (export_file_ == NULL)
             {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
                ACE_ERROR ((LM_ERROR, "Logger failed to open log file %s\n", log_filename_));
#endif
                return;
             }
          }
         log_started_ = 1;
       	}

      // Write a start of logging session message.
      time_t current_time = ACE_OS::time ();
      ACE_OS::fprintf (log_file_,
                       "\n\nTAO_Metrics Metrics Logging Session: %s\n\n",
                       ACE_OS::ctime (& current_time));

      ACE_OS::fprintf (export_file_,
                       "\n\nTAO_Metrics Metrics Logging Session: %s\n\n",
                       ACE_OS::ctime (& current_time));
      if (log_filename_)
        {
          ACE_OS::fflush (log_file_);
          ACE_OS::fflush (export_file_);
//          ACE_OS::fclose (log_file_);
        }
    }
}


// Destructor.

TAO_Metrics_Logger::~TAO_Metrics_Logger ()
{
  if (generate_log_)
    {
      if (log_filename_)
        {
//        log_file_ = ACE_OS::fopen (log_filename_, "a+");
	}

      ACE_OS::fprintf (log_file_, "\n\n");

      ACE_OS::fflush (log_file_);

      ACE_OS::fprintf (export_file_, "\n\n");
      ACE_OS::fflush (export_file_);

      if (log_filename_)
        {
//        ACE_OS::fclose (log_file_);
       	}
    }

#if defined (METRICS_LOGGER_SENDS_EVENTS)
  if (this->generate_events_)
    {
      this->dove_supplier_.disconnect ();
    }
#endif /* METRICS_LOGGER_SENDS_EVENTS */
}


// Active object loop for processing logged data.

int
TAO_Metrics_Logger::svc (void)
{
  ACE_Message_Block *mb;

  // Loop forever.
  while (1)
    if (this->getq (mb) < 0)
      {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) getq failed in TAO_Metrics_Logger::svc"));
#endif
        return -1;
      }
    else if (! mb)
      {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) null message block pointer in TAO_Metrics_Logger::svc"));
#endif
        return -1;
      }
    else if (! mb->rd_ptr ())
      {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) null message block pointer in TAO_Metrics_Logger::svc"));
#endif
        return -1;
      }
    else
      {
        // Process message.

        TAO_Metrics_Logger_Data *data = (TAO_Metrics_Logger_Data *) mb->rd_ptr ();
        switch (data->data_type_)
	  {
            case TAO_Metrics_Logger_Data::QOS_TYPE:
              this->process_aggregate_QoS (*data->qos_params_, data->interval_);
              delete data;
              delete mb;
              break;

            case TAO_Metrics_Logger_Data::TIMEPROBE_TYPE:
              this->process_timeprobe_data (*data->timeprobe_params_, data->interval_);
              delete data;
              delete mb;
              break;

            case TAO_Metrics_Logger_Data::BANNER_TYPE:
              this->process_banner (data->banner_);
              delete data;
              delete mb;
              break;

            default:
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) unrecognized data type in TAO_Metrics_Logger::svc"));
#endif
              return -1;
     }
    }

// Modified by BAP.  Remove unreachable code.
//  return 0;
}


// Sends a banner to be written to the log file and to the visualization browser.

void
TAO_Metrics_Logger::
send_banner (
        const char * banner
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
   // Package up the data and put it on the task queue.

  TAO_Metrics_Logger_Data *data;
  ACE_NEW (data,
           TAO_Metrics_Logger_Data (TAO_Metrics_Logger_Data::BANNER_TYPE));
  ACE_NEW (data->banner_, char [ACE_OS::strlen (banner) + 1]);
  ACE_OS::strcpy (data->banner_, banner);

  ACE_Message_Block *mb;
  ACE_NEW (mb,
           ACE_Message_Block ((char *) data,
                              sizeof (TAO_Metrics_Logger_Data)));

  if (this->putq (mb) < 0)
    {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_Logger::send_banner putq failed"));
#endif
    }
}

void
TAO_Metrics_Logger::process_banner (const char *banner)
{
  if (this->generate_log_)
    {
      if (log_filename_)
        {
//        log_file_ = ACE_OS::fopen (log_filename_, "a+");
        }

      ACE_OS::fprintf (log_file_,
                       "%s\n", banner);
      ACE_OS::fprintf (export_file_,
                       "%s\n", banner);
      if (log_filename_)
        {
          ACE_OS::fflush (log_file_);
          ACE_OS::fflush (export_file_);

          // Close and then reopen the file.  We can't afford to open & close all of the time.
       	  if (( saveFileCnt_++ % OPEN_CLOSE_BANNER_CYCLE) == 0 )
            {
              ACE_OS::fclose (log_file_);
              log_file_ = ACE_OS::fopen (log_filename_, "a+");

              ACE_OS::fclose (export_file_);
              export_file_ = ACE_OS::fopen ("remote_logger_export.excel", "a+");
            }
        }
    }
}


// Reports the aggregate QoS information for all operations
// to the passed QoS logger.

void
TAO_Metrics_Logger::log_aggregate_QoS (const Metrics::QoSParameter_Set & qos_params,
                                       Metrics::Time interval,
                                       CORBA::Environment &ACE_TRY_ENV)
{
// This is a temporary patch to eliminate this data from the log.  It was
// done to bypass a long rebuild that would have been required otherwise.
// Remove this patch later.  Brian Mendel

  //Added to remove Linux warning (Boeing Extension)
  ACE_UNUSED_ARG(ACE_TRY_ENV);
  ACE_UNUSED_ARG(qos_params);
  ACE_UNUSED_ARG(interval);
  // Package up the data and put it on the task queue.
#if 0
  TAO_Metrics_Logger_Data *data;
  ACE_NEW (data,
           TAO_Metrics_Logger_Data (TAO_Metrics_Logger_Data::QOS_TYPE,
                                    interval));
  ACE_NEW (data->qos_params_,
           Metrics::QoSParameter_Set (qos_params));

  ACE_Message_Block *mb;
  ACE_NEW (mb,
           ACE_Message_Block ((char *) data,
                              sizeof (TAO_Metrics_Logger_Data)));

  if (this->putq (mb) < 0)
    {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_Logger::log_aggregate_QoS putq failed"));
#endif
    }
#endif
}


// Processes the aggregate QoS information for all operations
// to the passed QoS logger.

void
TAO_Metrics_Logger::process_aggregate_QoS (const Metrics::QoSParameter_Set & qos_params,
                                           Metrics::Time interval)
{
  //Added to remove Linux warning (Boeing Extension)
  ACE_UNUSED_ARG(interval);

  if (this->generate_log_)
    {
      if (log_filename_)
        {
//        log_file_ = ACE_OS::fopen (log_filename_, "a+");
        }

      ACE_OS::fprintf (log_file_,
                       "\nName  missed     made cancelled\n");

      ACE_OS::fprintf (export_file_,
                       "\n\n Name \t missed \t made \t cancelled\n");
    }

#if defined (METRICS_LOGGER_SENDS_EVENTS)
  if (this->generate_events_)
    {
      this->qos_data_.hrt_deadlines_missed = 0;
      this->qos_data_.hrt_deadlines_made = 0;
      this->qos_data_.hrt_operations_cancelled = 0;
      this->qos_data_.srt_deadlines_missed = 0;
      this->qos_data_.srt_deadlines_made = 0;
      this->qos_data_.srt_operations_cancelled = 0;
    }
#endif /* METRICS_LOGGER_SENDS_EVENTS */

  for (u_long i = 0; i < qos_params.length (); ++i)
    {
#if defined (METRICS_LOGGER_SENDS_EVENTS)
      if (this->generate_events_)
        {
          if (qos_params [i].is_hrt)
	    {
              this->qos_data_.hrt_deadlines_missed +=
                qos_params [i].deadlines_missed;
              this->qos_data_.hrt_deadlines_made +=
                qos_params [i].deadlines_made;
              this->qos_data_.hrt_operations_cancelled +=
                qos_params [i].operations_cancelled;
	    }
          else
	    {
              this->qos_data_.srt_deadlines_missed +=
                qos_params [i].deadlines_missed;
              this->qos_data_.srt_deadlines_made +=
                qos_params [i].deadlines_made;
              this->qos_data_.srt_operations_cancelled +=
                qos_params [i].operations_cancelled;
	    }
	}
#endif /* METRICS_LOGGER_SENDS_EVENTS */

      if (this->generate_log_)
        {
          ACE_OS::fprintf (log_file_,
                           "%-50s %8lu %8lu %8lu\n",
                           (const char *)qos_params [i].entry_point,
                           qos_params [i].deadlines_missed,
                           qos_params [i].deadlines_made,
                           qos_params [i].operations_cancelled);
          ACE_OS::fprintf (export_file_,
                           " %s \t %lu \t %lu \t %lu\n",
                           (const char *)qos_params [i].entry_point,
                           qos_params [i].deadlines_missed,
                           qos_params [i].deadlines_made,
                           qos_params [i].operations_cancelled);
        }
    }

#if defined (METRICS_LOGGER_SENDS_EVENTS)
  if (this->generate_events_)
    {
      // Push an event to the visualization browser.
      CORBA::Any any;

      ACE_TRY_NEW_ENV
        {
          any.replace (Metrics::_tc_QoSData, &qos_data_, 0, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics_Logger::process_aggregate_QoS");
#endif
          return;
        }
      ACE_ENDTRY;

      this->dove_supplier_.notify (any);
    }
#endif /* METRICS_LOGGER_SENDS_EVENTS */

  if (this->generate_log_)
    {
      if (log_filename_)
        {
          ACE_OS::fflush (log_file_);
//          ACE_OS::fclose (log_file_);
        }
    }
}


// Binds the names of various timeprobes to their identifiers so
// that ids alone can be used (for efficiency) in passing data.

void
TAO_Metrics_Logger::set_identities (const Metrics::ProbeIdentity_Set &
                                      probe_identities,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  for (u_long i = 0; i < probe_identities.length (); ++i)
    {
      this->set_identity (probe_identities [i], ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_Metrics_Logger::set_identity (const Metrics::ProbeIdentity_t & probe_identity,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // Look up the existing name that is bound, creating one if there is
  // not one there already.
  CORBA::String_var *name = 0;
  if (probe_name_map_.find (probe_identity.probe_id, name) != 0
      || name == 0)
    {
      ACE_NEW_THROW_EX (name,
                        CORBA::String_var (probe_identity.probe_name),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      // What is this used for?  BRM
      auto_ptr<CORBA::String_var> name_ptr (name);

      if (probe_name_map_.rebind (probe_identity.probe_id, name) < 0)
	{
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
          ACE_ERROR ((LM_ERROR, "TAO_Metrics_Logger::set_identity bind failed.\n"));
#endif
          return;
	}

      // DEBUG
      const char * name_cstr = name->in ();
      ACE_DEBUG ((LM_DEBUG,
                  "Logger registered name [%s] with id [%u]\n",
                  name_cstr, probe_identity.probe_id));

      name_ptr.release ();
    }
  else
    {
      // DEBUG
      const char * old_name_cstr = name->in ();

      // Unconditionally set the name to the new name.
      *name = probe_identity.probe_name;

      // DEBUG
      const char * new_name_cstr = name->in ();
      ACE_DEBUG ((LM_DEBUG,
                  "Logger CHANGED name from [%s] to [%s] for id [%u]\n",
                  old_name_cstr, new_name_cstr, probe_identity.probe_id));
    }
}


// Reports timeprobe data collected since last run.

void
TAO_Metrics_Logger::log_timeprobe_data (const Metrics::TimeprobeParameter_Set &
                                        timeprobe_params,
                                        Metrics::Time interval,
                                        CORBA::Environment &ACE_TRY_ENV)
{
  //Added to remove Linux warning (Boeing Extension)
  ACE_UNUSED_ARG(ACE_TRY_ENV);

  // Package up the data and put it on the task queue.

  TAO_Metrics_Logger_Data *data;
  ACE_NEW (data,
           TAO_Metrics_Logger_Data (TAO_Metrics_Logger_Data::TIMEPROBE_TYPE,
                                    interval));
  ACE_NEW (data->timeprobe_params_,
           Metrics::TimeprobeParameter_Set (timeprobe_params));

  ACE_Message_Block *mb;
  ACE_NEW (mb,
           ACE_Message_Block ((char *) data,
                              sizeof (TAO_Metrics_Logger_Data)));

  if (this->putq (mb) < 0)
    {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_Logger::log_timeprobe_data putq failed"));
#endif
    }
}


// Processes timeprobe data collected since last run.
void
TAO_Metrics_Logger::process_timeprobe_data (const Metrics::TimeprobeParameter_Set & timeprobe_params,
                                            Metrics::Time interval)
{
   if (this->generate_log_)
   {
      if (log_filename_)
      {
         //        log_file_ = ACE_OS::fopen (log_filename_, "a+");
      }

      ACE_OS::fprintf (log_file_,
                       "\n\n\n"
                       "Interval (usec):       %9lu  \n\n"
                       "Probe                                        elapsed_time(usec)    Start Time (usecs)    Stop Time (usec)\n\n",
                       // Modified by BAP.  This really should be corrected since we are truncating the interval here.
                       // We can get away with it here for now since the intervals are small enough to not roll over.
                       (u_long) ACE_U64_TO_U32(interval)/10 );

      ACE_OS::fprintf (export_file_,
                       "\n\n\n"
                       "Interval (usec): \t %9lu \t \n\n"
                       "Probe \t elapsed_time(usec) \tStart Time (usecs) \t Stop Time (usec)\n\n",
                       // Modified by BAP.  This really should be corrected since we are truncating the interval here.
                       // We can get away with it here for now since the intervals are small enough to not roll over.
                       (u_long) ACE_U64_TO_U32(interval)/10 );
   }

#if defined (METRICS_LOGGER_SENDS_EVENTS)
   Metrics::Time queue_time = 0;
   Metrics::Time hrt_op_time = 0;
   Metrics::Time srt_op_time = 0;
#endif /* METRICS_LOGGER_SENDS_EVENTS */

   for (u_long i = 0; i < timeprobe_params.length (); ++i)
   {
#if defined (METRICS_LOGGER_SENDS_EVENTS)
      if (! timeprobe_params [i].is_full_interval)
      {
         if (timeprobe_params [i].is_operation)
         {
            if (timeprobe_params [i].is_hrt)
            {
               hrt_op_time += timeprobe_params [i].interval;
            }
            else
            {
               srt_op_time += timeprobe_params [i].interval;
            }
         }
         else
         {
            queue_time += timeprobe_params [i].interval;
         }
      }
#endif /* METRICS_LOGGER_SENDS_EVENTS */

      if (this->generate_log_)
      {
         // Look up the name of the probe in the hash map.
         // If successful, use the name, and if not just use the probe id.
         CORBA::String_var *probe_name = 0;
         char format_spec[255];
         char export_spec[255];

         if (probe_name_map_.find (timeprobe_params [i].probe_id, probe_name) == 0
             && probe_name != 0)
         {
            if (timeprobe_params [i].cross_thread_probe &&
                timeprobe_params [i].is_full_interval)
            {
               if (generate_totals_)
               {
#if defined (ACE_LACKS_LONGLONG_T)
                  // This needs to be fixed for platforms that do not have 64 bit support.  This will result
                  // in overruns as currently written
                  ACE_OS::fprintf (log_file_,
                                   "  %-40s  (total)  %20lu   %20lu   %20lu  *** Cross Thread Event ***\n",
                                   (const char *) *probe_name,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
                  ACE_OS::fprintf (export_file_,
                                   "  %s (total) \t %lu \t %lu \t %lu \t ***Cross Thread Event***\n",
                                   (const char *) *probe_name,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
#else
                  // Build up the format specifiers since the specifier or not portable
                  ACE_OS::strcpy(format_spec, "  %-40s  (total)  ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " *** Cross Thread Event ***\n");

                  ACE_OS::strcpy(export_spec, "  %-40s  (total)  \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t*** Cross Thread Event ***\n");

                  ACE_OS::fprintf (log_file_,
                                   format_spec,
                                   (const char *) *probe_name,
                                   timeprobe_params [i].interval / 10,
                                   timeprobe_params [i].start_event_time / 10,
                                   timeprobe_params [i].stop_event_time / 10);
                  ACE_OS::fprintf (export_file_,
                                   export_spec,
                                   (const char *) *probe_name,
                                   timeprobe_params [i].interval / 10,
                                   timeprobe_params [i].start_event_time / 10,
                                   timeprobe_params [i].stop_event_time / 10);
#endif
               }
            }
            else
            {
#if defined (ACE_LACKS_LONGLONG_T)
                  // This needs to be fixed for platforms that do not have 64 bit support.  This will result
                  // in overruns as currently written
               ACE_OS::fprintf (log_file_,
                                "  %-40s   %20lu   %20lu   %20lu   *** Cross Thread Event ***\n",
                                (const char *) *probe_name,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
               ACE_OS::fprintf (export_file_,
                                "  %s \t %lu \t %lu \t %lu \t ***Cross Thread Event***\n",
                                (const char *) *probe_name,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
#else
                  // Build up the format specifiers since the specifier or not portable
                  ACE_OS::strcpy(format_spec, "  %-40s  ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " *** Cross Thread Event ***\n");

                  ACE_OS::strcpy(export_spec, "  %-40s  \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t *** Cross Thread Event ***\n");

                  ACE_OS::fprintf (log_file_,
                                   format_spec,
                                   (const char *) *probe_name,
                                   timeprobe_params [i].interval / 10,
                                   timeprobe_params [i].start_event_time / 10,
                                   timeprobe_params [i].stop_event_time / 10);
                  ACE_OS::fprintf (export_file_,
                                   export_spec,
                                   (const char *) *probe_name,
                                   timeprobe_params [i].interval / 10,
                                   timeprobe_params [i].start_event_time / 10,
                                   timeprobe_params [i].stop_event_time / 10);
#endif
            }

         }
         else
         {
            if (timeprobe_params [i].cross_thread_probe &&
                timeprobe_params [i].is_full_interval)
            {
               if (generate_totals_)
#if defined (ACE_LACKS_LONGLONG_T)
                  ACE_OS::fprintf (log_file_,
                                   "  [id (total): %8lu] %9lu   %9lu   %9lu   *** Cross Thread Event ***\n",
                                   timeprobe_params [i].probe_id,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
                  ACE_OS::fprintf (export_file_,
                                   "  [id (total): %lu] \t %lu \t %lu \t %lu \t ***Cross Thread Event***\n",
                                   timeprobe_params [i].probe_id,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                   (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
#else
                  // Build up the format specifiers since the specifier or not portable
                  ACE_OS::strcpy(format_spec, "  [id (total): %8lu] ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " ");
                  ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(format_spec, " *** Cross Thread Event ***\n");

                  ACE_OS::strcpy(export_spec, "  [id (total): %8lu] \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t ");
                  ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
                  ACE_OS::strcat(export_spec, " \t*** Cross Thread Event ***\n");

                  if (probe_name == 0)
                  {
                     ACE_DEBUG((LM_DEBUG, "Found a probe with id: %d with no name\n", timeprobe_params [i].probe_id));
                  }
                  else
                  {
                     ACE_OS::fprintf (log_file_,
                                      format_spec,
                                      (const char *) *probe_name,
                                      timeprobe_params [i].interval / 10,
                                      timeprobe_params [i].start_event_time / 10,
                                      timeprobe_params [i].stop_event_time / 10);
                     ACE_OS::fprintf (export_file_,
                                      export_spec,
                                      (const char *) *probe_name,
                                      timeprobe_params [i].interval / 10,
                                      timeprobe_params [i].start_event_time / 10,
                                      timeprobe_params [i].stop_event_time / 10);
                  }
#endif
            }
            else
#if defined (ACE_LACKS_LONGLONG_T)
               ACE_OS::fprintf (log_file_,
                                "  [id: %14lu]   %9lu  %9lu   %9lu   \n",
                                timeprobe_params [i].probe_id,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
               ACE_OS::fprintf (export_file_,
                                " [id: %lu] \t %lu \t %lu \t %lu\n",
                                timeprobe_params [i].probe_id,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].interval)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].start_event_time)/10,
                                (u_long) ACE_U64_TO_U32(timeprobe_params [i].stop_event_time)/10);
#else
               // Build up the format specifiers since the specifier or not portable
               ACE_OS::strcpy(format_spec, "  [id: %14lu]   ");
               ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
               ACE_OS::strcat(format_spec, " ");
               ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
               ACE_OS::strcat(format_spec, " ");
               ACE_OS::strcat(format_spec, ACE_UINT64_FORMAT_SPECIFIER);
               ACE_OS::strcat(format_spec, " *** Cross Thread Event ***\n");

               ACE_OS::strcpy(export_spec, " [id: %lu] \t ");
               ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
               ACE_OS::strcat(export_spec, " \t ");
               ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
               ACE_OS::strcat(export_spec, " \t ");
               ACE_OS::strcat(export_spec, ACE_UINT64_FORMAT_SPECIFIER);
               ACE_OS::strcat(export_spec, " *** Cross Thread Event ***\n");

               if (probe_name == 0)
               {
                  ACE_DEBUG((LM_DEBUG, "Found a probe with id: %d with no name\n", timeprobe_params [i].probe_id));
               }
               else
               {
                  ACE_OS::fprintf (log_file_,
                                   format_spec,
                                   (const char *) *probe_name,
                                   timeprobe_params [i].interval / 10,
                                   timeprobe_params [i].start_event_time / 10,
                                   timeprobe_params [i].stop_event_time / 10);
                  ACE_OS::fprintf (export_file_,
                                   export_spec,
                                   (const char *) *probe_name,
                                   timeprobe_params [i].interval / 10,
                                   timeprobe_params [i].start_event_time / 10,
                                   timeprobe_params [i].stop_event_time / 10);
               }
#endif

         }
      }  // End if (this->generate_log_)
   }

#if defined (METRICS_LOGGER_SENDS_EVENTS)
   if (this->generate_events_)
   {
      CORBA::Any any;

          util_data_.hrt_op_utilization =
            ACE_static_cast (
              CORBA::Double,
              ACE_UINT64_DBLCAST_ADAPTER (hrt_op_time))
            / ACE_static_cast (
                CORBA::Double,
                ACE_UINT64_DBLCAST_ADAPTER (interval));

          util_data_.srt_op_utilization =
            ACE_static_cast (
              CORBA::Double,
              ACE_UINT64_DBLCAST_ADAPTER (srt_op_time))
            / ACE_static_cast (
                CORBA::Double,
                ACE_UINT64_DBLCAST_ADAPTER (interval));

          util_data_.queue_utilization =
            ACE_static_cast (
              CORBA::Double,
              ACE_UINT64_DBLCAST_ADAPTER (queue_time))
            / ACE_static_cast (
                CORBA::Double,
                ACE_UINT64_DBLCAST_ADAPTER (interval));

          ACE_TRY_NEW_ENV
	    {
              any.replace (Metrics::_tc_UtilData, &util_data_, 0, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
#if defined (METRICS_LOGGER_ERROR_OUTPUT_ENABLED)
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics_Logger::process_aggregate_QoS");
#endif
              return;
            }
          ACE_ENDTRY;

	  this->dove_supplier_.notify (any);
        }
#endif /* METRICS_LOGGER_SENDS_EVENTS */

  if (this->generate_log_)
    {
      if (log_filename_)
       	{
        ACE_OS::fflush (log_file_);
        ACE_OS::fflush (export_file_);
//	  ACE_OS::fclose (log_file_);
       	}
    }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// Already instantiated in Transport_Mux_Strategy ... otherwise VxWorks gets dual
// definition link error
//template class ACE_Equal_To<CORBA::ULong>;
// The below template already instantiated in ../ace/Funchtor.h
// template class ACE_Hash<CORBA::ULong>;
template class ACE_Hash_Map_Manager_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<CORBA::ULong, CORBA::String_var *>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Equal_To<CORBA::ULong>
//#pragma instantiate ACE_Hash<CORBA::ULong>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CORBA::ULong, CORBA::String_var *, ACE_Hash<CORBA::ULong>, ACE_Equal_To<CORBA::ULong>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<CORBA::ULong, CORBA::String_var *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

