// $Id$

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#include "orbsvcs/Sched/Reconfig_Sched_Utils.h"
#include "orbsvcs/Scheduler_Factory.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Scheduler_Factory.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Initialize static class members.
RtecScheduler::Scheduler_ptr ACE_Scheduler_Factory::server_ = 0;
ACE_Scheduler_Factory::Factory_Status ACE_Scheduler_Factory::status_ =
  ACE_Scheduler_Factory::UNINITIALIZED;


RtecScheduler::Period_t ACE_Scheduler_Factory::period_default_ = 0;
RtecScheduler::Threads_t ACE_Scheduler_Factory::threads_default_ = 0;
RtecScheduler::Importance_t ACE_Scheduler_Factory::importance_default_ = RtecScheduler::MEDIUM_IMPORTANCE;
RtecScheduler::Criticality_t ACE_Scheduler_Factory::criticality_default_ = RtecScheduler::HIGH_CRITICALITY;
RtecScheduler::RT_Info_Enabled_Type_t ACE_Scheduler_Factory::rt_info_enable_state_default_ = RtecScheduler::RT_INFO_NON_VOLATILE;

RtecScheduler::Period_t ACE_Scheduler_Factory::period_default()
{
   return period_default_;
}

RtecScheduler::Threads_t ACE_Scheduler_Factory::threads_default()
{
   return threads_default_;
}

RtecScheduler::Importance_t ACE_Scheduler_Factory::importance_default()
{
   return importance_default_;
}

RtecScheduler::Criticality_t ACE_Scheduler_Factory::criticality_default()
{
   return criticality_default_;
}

void ACE_Scheduler_Factory::period_default(RtecScheduler::Period_t period_default)
{
   period_default_ = period_default;
}

void ACE_Scheduler_Factory::threads_default(RtecScheduler::Threads_t threads_default)
{
   threads_default_ = threads_default;
}

void ACE_Scheduler_Factory::importance_default(RtecScheduler::Importance_t importance_default)
{
   importance_default_ = importance_default;
}

void ACE_Scheduler_Factory::criticality_default(RtecScheduler::Criticality_t criticality_default)
{
   criticality_default_ = criticality_default;
}

RtecScheduler::RT_Info_Enabled_Type_t ACE_Scheduler_Factory::rt_info_enable_state_default()
{
   return rt_info_enable_state_default_;
}

void ACE_Scheduler_Factory::rt_info_enable_state_default(RtecScheduler::RT_Info_Enabled_Type_t rt_info_enable_state_default)
{
   rt_info_enable_state_default_ = rt_info_enable_state_default;
}

// This symbols are extern because the automatic template
// instantiation mechanism in SunCC gets confused otherwise.
int TAO_SF_config_count = -1;
ACE_Scheduler_Factory::POD_Config_Info* TAO_SF_config_info = 0;
int TAO_SF_entry_count = -1;
ACE_Scheduler_Factory::POD_RT_Info* TAO_SF_rt_info = 0;
int TAO_SF_dependency_count = -1;
ACE_Scheduler_Factory::POD_Dependency_Info* TAO_SF_dep_info = 0;

struct ACE_Scheduler_Factory_Data
{
  // = TITLE
  //   Helper struct, to encapsulate the singleton static server and
  //   ACE_TSS objects.  We can't use ACE_Singleton directly, because
  //   construction of ACE_Runtime_Scheduler takes arguments.

/* WSOA merge - commented out
  ACE_Runtime_Scheduler scheduler_;
  // The static runtime scheduler.
*/

  TAO_Reconfig_Scheduler<TAO_MUF_FAIR_Reconfig_Sched_Strategy, TAO_SYNCH_MUTEX> scheduler_;
  // The scheduler.

  ACE_TSS<ACE_TSS_Type_Adapter<RtecScheduler::Preemption_Priority_t> >
  preemption_priority_;
  // The dispatch queue number of the calling thread.  For access by
  // applications; must be set by either the application or Event
  // Channel.

  ACE_Scheduler_Factory_Data (void)
    : scheduler_ (TAO_SF_config_count,
                  TAO_SF_config_info,
                  TAO_SF_entry_count,
                  TAO_SF_rt_info,
                  TAO_SF_dependency_count,
                  TAO_SF_dep_info,
                  0),
      preemption_priority_ ()
  {
  }
};

static ACE_Scheduler_Factory_Data *ace_scheduler_factory_data = 0;

int ACE_Scheduler_Factory::use_runtime (int cc,
                                        POD_Config_Info cfgi[],
                                        int ec,
                                        POD_RT_Info rti[])
{
  if (server_ != 0 || TAO_SF_entry_count != -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("ACE_Scheduler_Factory::use_runtime - ")
                       ACE_TEXT("server already configured\n")),
                      -1);
  TAO_SF_config_count = cc;
  TAO_SF_config_info = cfgi;
  TAO_SF_entry_count = ec;
  TAO_SF_rt_info = rti;
  status_ = ACE_Scheduler_Factory::RUNTIME;

  return 0;
}

static RtecScheduler::Scheduler_ptr
static_server (void)
{
  RtecScheduler::Scheduler_ptr server_ = 0;

  // This isn't thread safe, but the static instance that it replaces
  // wasn't thread safe either.  Hola, Sr. Sandiego :-) If it needs to
  // be made thread safe, it should be protected using double-checked
  // locking.
  if (! ace_scheduler_factory_data  &&
      (ace_scheduler_factory_data =
         ACE_Singleton<ACE_Scheduler_Factory_Data,
                       ACE_Null_Mutex>::instance ()) == 0)
        return 0;

  try
    {
      server_ = ace_scheduler_factory_data->scheduler_._this ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("ACE_Scheduler_Factory - configured static server\n")));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        ACE_TEXT("ACE_Scheduler_Factory::config_runtime - ")
        ACE_TEXT("cannot allocate server\n"));
    }

  return server_;
}

int
ACE_Scheduler_Factory::use_config (CosNaming::NamingContext_ptr naming)
{
  return ACE_Scheduler_Factory::use_config (naming,
                                            "ScheduleService");
}

int
ACE_Scheduler_Factory::use_config (CosNaming::NamingContext_ptr naming,
                                   const char* name)
{
  if (server_ != 0 || TAO_SF_entry_count != -1)
    // No errors, runtime execution simply takes precedence over
    // config runs.
    return 0;

  try
    {
      CosNaming::Name schedule_name (1);
      schedule_name.length (1);
      schedule_name[0].id = CORBA::string_dup (name);
      CORBA::Object_var objref =
        naming->resolve (schedule_name);

      server_ =
        RtecScheduler::Scheduler::_narrow(objref.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      server_ = 0;
      ex._tao_print_exception (
        ACE_TEXT("ACE_Scheduler_Factory::use_config - ")
        ACE_TEXT(" exception while resolving server\n"));
    }

  status_ = ACE_Scheduler_Factory::CONFIG;
  return 0;
}

int
ACE_Scheduler_Factory::server (RtecScheduler::Scheduler_ptr sptr)
{
  if (server_ != 0 || TAO_SF_entry_count != -1)
    return -1;

  server_ = RtecScheduler::Scheduler::_duplicate (sptr);
  return 0;
}

RtecScheduler::Scheduler_ptr
ACE_Scheduler_Factory::server (void)
{
  if (server_ == 0 && TAO_SF_entry_count != -1)
    server_ = static_server ();

  if (server_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("ACE_Scheduler_Factor::server - ")
                       ACE_TEXT("no scheduling service configured\n")),
                      0);
  return server_;
}

static char header[] =
"// $Id $\n\n"
"// This file was automatically generated by the Scheduler_Factory.\n"
"// Before editing the file please consider generating it again.\n"
"\n"
"#include \"orbsvcs/Scheduler_Factory.h\"\n"
"\n";

static char footer[] =
"\n"
"// This sets up Scheduler_Factory to use the runtime version.\n"
"int scheduler_factory_setup =\n"
"  ACE_Scheduler_Factory::use_runtime (configs_size, configs, infos_size, infos);\n"
"\n"
"// EOF\n";

static char start_anomalies_found[] =
"\n// The following scheduling anomalies were detected:\n";

static char start_anomalies_none[] =
"\n// There were no scheduling anomalies.\n";

static char start_infos[] =
"\n\nstatic ACE_Scheduler_Factory::POD_RT_Info infos[] = {\n";

static char end_infos[] =
"};\n\n"
"static int infos_size = sizeof(infos)/sizeof(infos[0]);\n\n";

static char end_infos_empty[] =
"};\n\n"
"static int infos_size = 0;\n\n";

static char start_dependencies[] =
"\n\nstatic ACE_Scheduler_Factory::POD_Dependency_Info dependencies[] = {\n";

static char end_dependencies[] =
"};\n\n"
"static int dependencies_size = sizeof(dependencies)/sizeof(dependencies[0]);\n\n";

static char end_dependencies_empty[] =
"};\n\n"
"static int dependencies_size = 0;\n\n";

static char start_configs[] =
"\nstatic ACE_Scheduler_Factory::POD_Config_Info configs[] = {\n";

static char end_configs[] =
"};\n\n"
"static int configs_size = sizeof(configs)/sizeof(configs[0]);\n\n";

static char end_configs_empty[] =
"};\n\n"
"static int configs_size = 0;\n\n";

int ACE_Scheduler_Factory::dump_schedule
   (const RtecScheduler::RT_Info_Set& infos,
    const RtecScheduler::Dependency_Set& dependencies,
    const RtecScheduler::Config_Info_Set& configs,
    const RtecScheduler::Scheduling_Anomaly_Set& anomalies,
    const ACE_TCHAR* file_name,
    const char* rt_info_format,
    const char* dependency_format,
    const char* config_info_format,
    int dump_disabled_infos,
    int dump_disabled_dependencies)
{
  u_int i;
  char entry_point [BUFSIZ];

  // Default format for printing RT_Info output.
  if (rt_info_format == 0)
    rt_info_format = "{%20s, /* entry_point */\n"
                     "%10d, /* handle */\n"
                     "%10d, /* worst_case_execution_time */,\n"
                     "%10d, /* typical_execution_time */,\n"
                     "%10d, /* cached_execution_time */,\n"
                     "%10d, /* period */\n"
                     "(RtecScheduler::Criticality_t) %d, /* [ VL_C = 0, L_C = 1, M_C = 2, H_C = 3, VH_C = 4] */\n"
                     "(RtecScheduler::Importance_t) %d, /* [ VL_I = 0, L_I = 1, M_I = 2, H_I = 3, VH_I = 4] */\n"
                     "%10d, /* quantum */\n"
                     "%10d, /* threads */\n"
                     "%10d, /* priority */\n"
                     "%10d, /* preemption_subpriority */\n"
                     "%10d, /* preemption_priority */\n"
                     "(RtecScheduler::Info_Type_t) %d, /* [OPERATION = 0, CONJUNCTION = 1, DISJUNCTION = 2, REMOTE_DEPENDANT = 3] */\n"
                     "(RtecScheduler::RT_Info_Enabled_Type_t) %d } /* [RT_INFO_DISABLED = 0, RT_INFO_ENABLED = 1, RT_INFO_NON_VOLATILE = 2] */\n";


  // Default format for printing dependency output.
  if (dependency_format == 0)
      dependency_format = "  { (RtecScheduler::Dependency_Type_t) %d, %10d, "
                          "%10d, %10d,"
                          "(RtecScheduler::Dependency_Enabled_Type_t) %d }";

  // Default format for printing Config_Info output.
  if (config_info_format == 0)
    config_info_format = "  { %10d, %10d, "
                         "(RtecScheduler::Dispatching_Type_t) %d }";

  FILE* file = stdout;
  if (file_name != 0)
    {
      file = ACE_OS::fopen (file_name, ACE_TEXT("w"));
      if (file == 0)
          return -1;
    }
  ACE_OS::fprintf (file, header);

  // Indicate anomalies encountered during scheduling.

  ACE_OS::fprintf(file, (anomalies.length () > 0
                         ? start_anomalies_found
                         : start_anomalies_none));

  for (i = 0; i < anomalies.length (); ++i)
    {
      const RtecScheduler::Scheduling_Anomaly& anomaly = anomalies[i];
      switch (anomaly.severity)
        {
          case RtecScheduler::ANOMALY_FATAL:
            ACE_OS::fprintf(file, "FATAL: ");
            break;

          case RtecScheduler::ANOMALY_ERROR:
            ACE_OS::fprintf(file, "ERROR: ");
            break;

          case RtecScheduler::ANOMALY_WARNING:
            ACE_OS::fprintf(file, "// WARNING: ");
            break;

          default:
            ACE_OS::fprintf(file, "// UNKNOWN: ");
            break;
        }

      ACE_OS::fprintf (file,
                       "%s\n",
                       (const char *) anomaly.description);
    }

  // Print out banner indicating which infos are dumped.
  if (dump_disabled_infos)
    {
      ACE_OS::fprintf (file, "\n// Both enabled and disabled RT_Infos were dumped to this file.\n\n");
    }
  else
    {
      ACE_OS::fprintf (file, "\n// Only enabled RT_Infos were dumped to this file.\n\n");
    }

  // Print out operation QoS info.
  ACE_OS::fprintf (file, start_infos);

  for (i = 0;
       i < infos.length ();
       ++i)
    {
      const RtecScheduler::RT_Info& info = infos[i];

      if (dump_disabled_infos
          || info.enabled == RtecScheduler::RT_INFO_ENABLED
          || info.enabled == RtecScheduler::RT_INFO_NON_VOLATILE)
        {
      if (i != 0)
        // Finish previous line.
        ACE_OS::fprintf(file, ",\n");

      const RtecScheduler::RT_Info& info = infos[i];

      // Put quotes around the entry point name, exactly as it is stored.
      ACE_OS::sprintf (entry_point,
                       "\"%s\"",
                       (const char *) info.entry_point);

      // @@ TODO Eventually the TimeT structure will be a 64-bit
      // unsigned int, we will have to change this dump method then.
      ACE_OS::fprintf (file,
                       rt_info_format,
                       entry_point,
                       info.handle,
                       ACE_CU64_TO_CU32 (info.worst_case_execution_time),
                       ACE_CU64_TO_CU32 (info.typical_execution_time),
                       ACE_CU64_TO_CU32 (info.cached_execution_time),
                       info.period,
                       info.criticality,
                       info.importance,
                       ACE_CU64_TO_CU32 (info.quantum),
                       info.threads,
                       info.priority,
                       info.preemption_subpriority,
                           info.preemption_priority,
                           info.info_type,
                           info.enabled);
    }
    }

  // Finish last line.
  ACE_OS::fprintf(file, "\n");

  if (infos.length () > 0)
    ACE_OS::fprintf (file, end_infos);
  else
    ACE_OS::fprintf (file, end_infos_empty);

  // Print out banner indicating which dependencies are dumped.
  if (dump_disabled_dependencies)
    {
      ACE_OS::fprintf (file, "\n// Both enabled and disabled dependencies were dumped to this file.\n\n");
    }
  else
    {
      ACE_OS::fprintf (file, "\n// Only enabled dependencies were dumped to this file.\n\n");
    }

  // Print out operation dependency info.
  ACE_OS::fprintf (file, start_dependencies);

  for (i = 0;
       i < dependencies.length ();
       ++i)
    {
      const RtecScheduler::Dependency_Info& dep = dependencies[i];

      if (dump_disabled_infos
          || dep.enabled == RtecBase::DEPENDENCY_ENABLED
          || dep.enabled == RtecBase::DEPENDENCY_NON_VOLATILE)
        {
          // Finish previous line.
          if (i != 0)
            {
              ACE_OS::fprintf (file, ",\n");
            }

          ACE_OS::fprintf (file,
                           dependency_format,
                           dep.dependency_type,
                           dep.number_of_calls,
                           dep.rt_info,
                           dep.rt_info_depended_on,
                           dep.enabled);
        }
    }

  // Finish last line.
  ACE_OS::fprintf (file, "\n");

  if (dependencies.length () > 0)
    ACE_OS::fprintf (file, end_dependencies);
  else
    ACE_OS::fprintf (file, end_dependencies_empty);


  // Print out queue configuration info.
  ACE_OS::fprintf (file, start_configs);

  for (i = 0;
       i < configs.length ();
       ++i)
    {
      if (i != 0)
        // Finish previous line.
        ACE_OS::fprintf (file, ",\n");

      const RtecScheduler::Config_Info& config = configs[i];
      ACE_OS::fprintf (file,
                       config_info_format,
                       config.preemption_priority,
                       config.thread_priority,
                       config.dispatching_type);
    }

  // Finish last line.
  ACE_OS::fprintf (file, "\n");

  if (configs.length () > 0)
    ACE_OS::fprintf (file, end_configs);
  else
    ACE_OS::fprintf (file, end_configs_empty);

  ACE_OS::fprintf (file, footer);
  ACE_OS::fclose (file);
  return 0;
}

void ACE_Scheduler_Factory::log_scheduling_entry(TAO_Reconfig_Scheduler_Entry * entry, FILE* file)
{

   if( entry == 0 )
   {
      ACE_OS::fprintf (file, "Entry is NULL");
      return;
   }

   // Print out the actual rt_info data
   const char* rt_info_format = "{%20s, /* entry_point */\n"
                                 "%10d, /* handle */\n"
                                 "%10d, /* period */\n"
                                 "%10d, /* criticality */\n"
                                 "%10d, /* threads */\n"
                                 "%10d, /* priority */\n"
                                 "%10d, /* preemption_subpriority */\n"
                                 "%10d, /* preemption_priority */\n"
                                 "%10d  /* enabled */\n";

   TAO_RT_Info_Ex* actual_info = entry->actual_rt_info();

   ACE_OS::fprintf (file,
                    rt_info_format,
                    actual_info->entry_point.in(),
                    actual_info->handle,
                    actual_info->period,
                    actual_info->criticality,
                    actual_info->threads,
                    actual_info->priority,
                    actual_info->preemption_subpriority,
                    actual_info->preemption_priority,
                    actual_info->enabled);


   // Print out the current admitted tuple
   const char* admitted_tuple_format = "             {"
                                       "%13d, /* handle */\n"
                                       "%13d, /* rate_index */\n"
                                       "%13d, /* period */\n"
                                       "%13d, /* criticality */\n"
                                       "%13d, /* priority */\n"
                                       "%13d, /* preemption_subpriority */\n"
                                       "%13d, /* preemption_priority */\n"
                                       "%13d } /* enabled */\n";

   TAO_RT_Info_Tuple* current_admitted_tuple = entry->current_admitted_tuple();

   ACE_OS::fprintf(file, "\n   Current admitted Tuple:\n");
   if( current_admitted_tuple == 0 )
   {
      ACE_OS::fprintf (file, "   =>NONE_ADMITTED\n");
   }
   else
   {
      ACE_OS::fprintf (file,
                       admitted_tuple_format,
                       current_admitted_tuple->handle,
                       current_admitted_tuple->rate_index,
                       current_admitted_tuple->period,
                       current_admitted_tuple->criticality,
                       current_admitted_tuple->priority,
                       current_admitted_tuple->preemption_subpriority,
                       current_admitted_tuple->preemption_priority,
                       current_admitted_tuple->enabled);
   }

   // Print out the orig_tuple_subset_
   ACE_OS::fprintf(file, "\n      Original Tuple Subset\n   {\n");
   log_tuple_subset(entry->orig_tuple_subset(), file);
   ACE_OS::fprintf(file, "\n   }");

   // Print out the prop_tuple_subset_
   ACE_OS::fprintf(file, "\n      Propagated Tuple Subset\n   {\n");
   log_tuple_subset(entry->prop_tuple_subset(), file);
   ACE_OS::fprintf(file, "\n   }\n}");


}

void ACE_Scheduler_Factory::log_tuple_subset(TUPLE_SET & tuple_subset,
                      FILE* file)
{
   TAO_RT_Info_Tuple **tuple_ptr_ptr;
   const char* subset_tuple_format = "             {\n"
                                     "%13d, /* handle */\n"
                                     "%13d, /* rate_index */\n"
                                     "%13d, /* period */\n"
                                     "%13d, /* criticality */\n"
                                     "%13d, /* threads */\n"
                                     "%13d, /* priority */\n"
                                     "%13d, /* preemption_subpriority */\n"
                                     "%13d, /* preemption_priority */\n"
                                     "%13d } /* enabled */\n";

   TUPLE_SET_ITERATOR
      tuple_iter (tuple_subset);


   while (tuple_iter.done () == 0)
   {
      // Get a pointer to the tuple COPY under the iterator.
      if (tuple_iter.next (tuple_ptr_ptr) == 0
         || tuple_ptr_ptr == 0 || ((*tuple_ptr_ptr) == 0) )
      {
         //FUZZ: disable check_for_NULL
         ACE_OS::fprintf (file, "{ NULL TUPLE POINTER }\n");
         //FUZZ: enable check_for_NULL
      }
      else
      {

      ACE_OS::fprintf (file,
         subset_tuple_format,
         (*tuple_ptr_ptr)->handle,
         (*tuple_ptr_ptr)->rate_index,
         (*tuple_ptr_ptr)->period,
         (*tuple_ptr_ptr)->criticality,
         (*tuple_ptr_ptr)->threads,
         (*tuple_ptr_ptr)->priority,
         (*tuple_ptr_ptr)->preemption_subpriority,
         (*tuple_ptr_ptr)->preemption_priority,
         (*tuple_ptr_ptr)->enabled);
      }

      tuple_iter.advance ();
   }
}

int
ACE_Scheduler_Factory::log_scheduling_entries(TAO_Reconfig_Scheduler_Entry ** entry_ptr_array,
                                              long entry_ptr_array_size,
                                              const char* file_name)
{
   // Open the file
   FILE* file = stdout;
   if (file_name != 0)
   {
      file = ACE_OS::fopen (file_name, ACE_TEXT ("w"));
      if (file == 0)
        return -1;
    }


   // Iterate through the array.  The index is the (handle - 1) of the rt_info in the array
   for(int index = 0; index < entry_ptr_array_size; ++index)
   {
      ACE_OS::fprintf(file, "\n\nScheduler Entry Array contents\n");
      TAO_Reconfig_Scheduler_Entry * entry = entry_ptr_array[index];

      log_scheduling_entry(entry, file);


   }


   ACE_OS::fclose (file);
   return 0;

}

void
ACE_Scheduler_Factory::log_scheduling_tuples(
  TAO_RT_Info_Tuple ** tuple_ptr_array,
  long tuple_ptr_array_size,
  const char* file_name)
{
   // Open the file
   FILE* file = stdout;
   if (file_name != 0)
   {
      file = ACE_OS::fopen (file_name, ACE_TEXT ("w"));
      if (file == 0)
        return;
    }

   static const char subset_tuple_format[] =
     "             {\n"
     "%13d, /* handle */\n"
     "%13lu, /* rate_index */\n"
     "%13d, /* period */\n"
     "%13d, /* criticality */\n"
     "%13d, /* priority */\n"
     "%13d, /* preemption_subpriority */\n"
     "%13d, /* preemption_priority */\n"
     "%13d } /* enabled */\n";

   for (int ndx = 0; ndx < tuple_ptr_array_size; ndx++)
     {
       ACE_OS::fprintf(file,
                       subset_tuple_format,
                       tuple_ptr_array[ndx]->handle,
                       tuple_ptr_array[ndx]->rate_index,
                       tuple_ptr_array[ndx]->period,
                       tuple_ptr_array[ndx]->criticality,
                       tuple_ptr_array[ndx]->priority,
                       tuple_ptr_array[ndx]->preemption_subpriority,
                       tuple_ptr_array[ndx]->preemption_priority,
                       tuple_ptr_array[ndx]->enabled);
     }

   ACE_OS::fclose (file);
}
#if defined (HPUX) && !defined (__GNUG__)
  // aCC can't handle RtecScheduler::Preemption_Priority_t used as an operator
  // name.
  typedef CORBA::Long RtecScheduler_Preemption_Priority_t;
#endif /* HPUX && !g++ */

RtecScheduler::Preemption_Priority_t
ACE_Scheduler_Factory::preemption_priority (void)
{
  // Return whatever we've got.  The application or Event Channel is
  // responsible for making sure that it was set.
  if (ace_scheduler_factory_data->preemption_priority_.ts_object ())
    {
      ACE_TSS_Type_Adapter<RtecScheduler::Preemption_Priority_t> *tss =
        ace_scheduler_factory_data->preemption_priority_;
      // egcs 1.0.1 raises an internal compiler error if we implicitly
      // call the type conversion operator.  So, call it explicitly.
#if defined (HPUX) && !defined (__GNUG__)
      const RtecScheduler::Preemption_Priority_t preemption_priority =
        static_cast<RtecScheduler::Preemption_Priority_t> (tss->operator RtecScheduler_Preemption_Priority_t ());
#else
      const RtecScheduler::Preemption_Priority_t preemption_priority =
        static_cast<RtecScheduler::Preemption_Priority_t> (tss->operator RtecScheduler::Preemption_Priority_t ());
#endif /* HPUX && !g++ */
      return preemption_priority;
    }
  else
    return static_cast<RtecScheduler::Preemption_Priority_t> (-1);
}

void
ACE_Scheduler_Factory::set_preemption_priority
  (const RtecScheduler::Preemption_Priority_t preemption_priority)
{
  // Probably don't need this, because it should be safe to assume
  // that static_server () was called before this function.  But just
  // in case . . .
  if (!ace_scheduler_factory_data
      && (ace_scheduler_factory_data =
          ACE_Singleton<ACE_Scheduler_Factory_Data,
          ACE_Null_Mutex>::instance ()) == 0)
        return;

  ace_scheduler_factory_data->preemption_priority_->
#if defined (HPUX) && !defined (__GNUG__)
    // aCC can't handle the typedef.
    operator RtecScheduler_Preemption_Priority_t & () = preemption_priority;
#else
    operator RtecScheduler::Preemption_Priority_t & () = preemption_priority;
#endif /* HPUX && !g++ */
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<ACE_Scheduler_Factory_Data, ACE_Null_Mutex> *ACE_Singleton<ACE_Scheduler_Factory_Data, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

TAO_END_VERSIONED_NAMESPACE_DECL
