// $Id$

// ============================================================================
//
// = FILENAME
//    DualEC_Sup.cpp
//
// = DESCRIPTION
//   Event Supplier for visualizing scheduling behavior, using arrival
//   and dispatch data logged by an event channel dispatch command object
//
// = AUTHOR
//    Chris Gill (cdgill@cs.wustl.edu)
//
//    Adapted from the DOVE simulation event supplier
//    originally
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//    modified
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include <limits.h>

#include "tao/corba.h"
#include "tao/TAO.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
//#include "ace/Profile_Timer.h"
#include "ace/OS.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "DualEC_Sup.h"
#include "NavWeapC.h"

ACE_RCSID(Event_Supplier, DualEC_Sup, "$Id$")

static const char usage [] =
"[[-?]\n"
"                 [-O[RBport] ORB port number]\n"
"                 [-m <count> of messages to send [100]\n"
"                 [-f <name of input data file>]\n"
"                 [-t <count> of event generating threads]]\n"
"                 [-d to dump scheduler header files]\n"
"                 [-s to suppress data updates by EC]]\n"
"                 [-r to use runtime schedulers]\n"
"                 [-p to suppress prioritization of operations]]\n";

  int event_thread_count_;

DualEC_Supplier::DualEC_Supplier (int argc, char** argv)
: sched_hi_impl_ (0),
  sched_lo_impl_ (0),
  ec_hi_impl_ (0),
  ec_lo_impl_ (0),
  weapons_Supplier (&dOVE_Supplier_Hi_),
  navigation_Supplier (&dOVE_Supplier_Lo_),
  argc_(argc),
  argv_(argv),
  total_messages_(10),
  input_file_name_(0),
  update_data_ (1),
  dump_schedule_headers_ (0),
  use_runtime_schedulers_ (0),
  suppress_priority_ (0),
  hi_schedule_file_name_ ("DualEC_Runtime_Hi.h"),
  lo_schedule_file_name_ ("DualEC_Runtime_Lo.h"),
  event_thread_count_ (1)
{
  TAO_TRY
    {
      this->terminator_ = terminator_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DualEC_Supplier::DualEC_Supplier : could "
                                 "not resolve reference to terminator");
  }
  TAO_ENDTRY;

  // Seed the visualization data
  navigation_.roll = navigation_.pitch = 0;

  // Initialize the high priority RT_Info data
  rt_info_data_hi_.entry_point = "DUALEC_SUP_HI";
  rt_info_data_hi_.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
  rt_info_data_hi_.worst_case_execution_time = ORBSVCS_Time::zero;
  rt_info_data_hi_.typical_execution_time = ORBSVCS_Time::zero;
  rt_info_data_hi_.cached_execution_time = ORBSVCS_Time::zero;
  rt_info_data_hi_.period = 2500000;
  rt_info_data_hi_.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;
  rt_info_data_hi_.quantum = ORBSVCS_Time::zero;
  rt_info_data_hi_.threads = 1;
  rt_info_data_hi_.info_type = RtecScheduler::OPERATION;

  // Initialize the low priority RT_Info data
  rt_info_data_lo_.entry_point = "DUALEC_SUP_LO";
  rt_info_data_lo_.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  rt_info_data_lo_.worst_case_execution_time = ORBSVCS_Time::zero;
  rt_info_data_lo_.typical_execution_time = ORBSVCS_Time::zero;
  rt_info_data_lo_.cached_execution_time = ORBSVCS_Time::zero;
  rt_info_data_lo_.period = 10000000;
  rt_info_data_lo_.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  rt_info_data_lo_.quantum = ORBSVCS_Time::zero;
  rt_info_data_lo_.threads = 1;
  rt_info_data_lo_.info_type = RtecScheduler::OPERATION;
}

DualEC_Supplier::~DualEC_Supplier ()
{
  this->dOVE_Supplier_Hi_.disconnect ();
  this->dOVE_Supplier_Lo_.disconnect ();

  // CDG - TBD - unregister the ECs from the NS
  // CDG - TBD - destroy the ECs
}

int
DualEC_Supplier::init ()
{
  this->get_options (argc_, argv_);

  TAO_TRY
  {
    // Connect to the RootPOA.
    CORBA::Object_var poaObject_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references("RootPOA");

    if (CORBA::is_nil (poaObject_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to initialize the POA.\n"),
                        1);

    this->root_POA_var_ =
      PortableServer::POA::_narrow (poaObject_var.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;

    this->poa_manager_ =
       root_POA_var_->the_POAManager (TAO_TRY_ENV);
    TAO_CHECK_ENV;

    // Get the Naming Service object reference.
    CORBA::Object_var namingObj_var =
      TAO_ORB_Core_instance()->orb()->resolve_initial_references ("NameService");

    if (CORBA::is_nil (namingObj_var.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to get the Naming Service.\n"),
                        -1);

    this->naming_context_ =
      CosNaming::NamingContext::_narrow (namingObj_var.in (),
                                       TAO_TRY_ENV);
    TAO_CHECK_ENV;

  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DualEC_Supplier::init");
    return -1;
  }
  TAO_ENDTRY;

  // Create two scheduling service instances.
  if (this->create_schedulers () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not create schedulers"),
                        -1);
    }

  // Create two event channels.
  if (this->create_event_channels () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not create event channels"),
                        -1);
    }

  // Connect suppliers to the respective event channels.
  ACE_Scheduler_Factory::POD_RT_Info * rt_info_ptr_hi = 
    (suppress_priority_) ? 0 : &rt_info_data_hi_;
  if (this->dOVE_Supplier_Hi_.connect ("MIB_unknown", 
                                       "DUAL_EC_HI",
                                       "DUAL_SCHED_HI",
                                       rt_info_ptr_hi) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not connect to DUAL_EC_HI"),
                        -1);
    }
  ACE_Scheduler_Factory::POD_RT_Info * rt_info_ptr_lo = 
    (suppress_priority_) ? 0 : &rt_info_data_lo_;
  if (this->dOVE_Supplier_Lo_.connect ("MIB_unknown", 
                                       "DUAL_EC_LO",
                                       "DUAL_SCHED_LO",
                                       rt_info_ptr_lo) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not connect to DUAL_EC2"),
                        -1);
    }

  return 0;
}

// Private class that implements a termination servant.

void 
DualEC_Supplier::Terminator::shutdown (CORBA::Environment &_env)
{
  TAO_TRY
    {
      TAO_ORB_Core_instance ()->orb ()->shutdown ();
    }
  TAO_CATCHANY
    {
    }
  TAO_ENDTRY;
}


// Run the ORB event loop.

void *
DualEC_Supplier::run_orb (void *) 
{
  TAO_TRY
    {
      TAO_ORB_Core_instance ()->orb ()->run (); 
    }
  TAO_CATCHANY
    {
    }
  TAO_ENDTRY;

  return 0;
}

// Run event generation thread.
void *
DualEC_Supplier::run_event_thread (void *arg)
{
  DualEC_Supplier * sup = 
    ACE_static_cast (DualEC_Supplier *, arg);

  TAO_TRY
    {
      ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *>
        schedule_iter (sup->schedule_data_);

      if (schedule_iter.done ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DualEC_Supplier::run_event_thread: "
                             "there is no scheduling data\n"), 0);
        }

      CORBA::Any any;

      unsigned long total_sent = 0;

      do
      {
        // Insert the event data
        int data_is_nav = 0;
        sup->insert_event_data (any,
                                schedule_iter,
                                data_is_nav);

        // deliver it over the wire, according to the type of data
        if (data_is_nav)  
          {
            sup->navigation_Supplier->notify (any);
          }
        else
          {
            sup->weapons_Supplier->notify (any);
          }

        if (total_sent < 5)
          ACE_DEBUG ((LM_DEBUG,
                      "Pushing event data.\n"));
        else if (total_sent == 5)
          ACE_DEBUG ((LM_DEBUG,
                      "Everything is running. Going to be mute.\n"));
      }
      while (++total_sent < sup->total_messages_);

    }
  TAO_CATCHANY
    {
    }
  TAO_ENDTRY;

  return 0;
}



// Create two scheduling service instances, register
// them with the Naming Service.

int
DualEC_Supplier::create_schedulers (void)
{
  // CDG - TBD - look at a command line modified setting,
  // create either a runtime or a config scheduler for 
  // each instance

  TAO_TRY
    {
      if (use_runtime_schedulers_)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Runtime Schedulers not implemented\n"), 
                            -1);
        }
      else
        {
          // Create Event Service Implementations, passing in the respective
          // Scheduling Service Implementations (which must already be created).

          ACE_NEW_RETURN (this->sched_hi_impl_,
                          ACE_Config_Scheduler,
                          -1);

          this->sched_hi_ = sched_hi_impl_->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          ACE_NEW_RETURN (this->sched_lo_impl_,
                          ACE_Config_Scheduler,
                          -1);

          this->sched_lo_ = sched_lo_impl_->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Register Scheduling Service Implementations with Naming Service

          CosNaming::Name sched_hi_name (1);
          sched_hi_name.length (1);
          sched_hi_name[0].id = CORBA::string_dup ("DUAL_SCHED_HI");
          naming_context_->bind (sched_hi_name, sched_hi_.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CosNaming::Name sched_lo_name (1);
          sched_lo_name.length (1);
          sched_lo_name[0].id = CORBA::string_dup ("DUAL_SCHED_LO");
          naming_context_->bind (sched_lo_name, sched_lo_.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("DualEC_Supplier::create_schedulers");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


// Create two event service instances, registers
// them with the Naming Service.

int 
DualEC_Supplier::create_event_channels (void)
{
  TAO_TRY
    {
      // Create Event Service Implementations, passing in the respective
      // Scheduling Service Implementations (which must already be created).
      ACE_NEW_RETURN (this->ec_hi_impl_,
                      ACE_EventChannel (sched_hi_.in (), 
                                        1,
                                        ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                        &default_module_factory_),
                      -1);

      this->ec_hi_ = ec_hi_impl_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_NEW_RETURN (this->ec_lo_impl_,
                      ACE_EventChannel (sched_lo_.in (), 
                                        1,
                                        ACE_DEFAULT_EVENT_CHANNEL_TYPE,
                                        &default_module_factory_),
                      -1);

      this->ec_lo_ = ec_lo_impl_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Register Event Service Implementations with Naming Service

      CosNaming::Name channel_hi_name (1);
      channel_hi_name.length (1);
      channel_hi_name[0].id = CORBA::string_dup ("DUAL_EC_HI");
      naming_context_->bind (channel_hi_name, ec_hi_.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name channel_lo_name (1);
      channel_lo_name.length (1);
      channel_lo_name[0].id = CORBA::string_dup ("DUAL_EC_LO");
      naming_context_->bind (channel_lo_name, ec_lo_.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DualEC_Supplier::create_event_channels");
    return -1;
  }
  TAO_ENDTRY;

  return 0;
}

void
DualEC_Supplier::compute_schedules (void)
{
  TAO_TRY
    {
#if defined (__SUNPRO_CC)
          // Sun C++ 4.2 warns with the code below:
          //   Warning (Anachronism): Temporary used for non-const
          //   reference, now obsolete.
          //   Note: Type "CC -migration" for more on anachronisms.
          //   Warning (Anachronism): The copy constructor for argument
          //   infos of type RtecScheduler::RT_Info_Set_out should take
          //   const RtecScheduler::RT_Info_Set_out&.
          // But, this code is not CORBA conformant, because users should
          // not define instances of _out types.


          RtecScheduler::RT_Info_Set_out infos_out_hi (this->infos_hi_);
          RtecScheduler::Config_Info_Set_out configs_out_hi (this->configs_hi_);
          RtecScheduler::Scheduling_Anomaly_Set_out anomalies_out_hi (this->anomalies_hi_);
          sched_hi_->compute_scheduling
            (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             infos_out_hi, configs_out_hi, anomalies_out_hi, TAO_TRY_ENV);
          TAO_CHECK_ENV;

          RtecScheduler::RT_Info_Set_out infos_out_lo (this->infos_lo_);
          RtecScheduler::Config_Info_Set_out configs_out_lo (this->configs_lo_);
          RtecScheduler::Scheduling_Anomaly_Set_out anomalies_out_lo (this->anomalies_lo_);
          sched_lo_->compute_scheduling
            (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             infos_out_lo, configs_out_lo, anomalies_out_lo, TAO_TRY_ENV);
          TAO_CHECK_ENV;

#else  /* ! __SUNPRO_CC */

          sched_hi_->compute_scheduling
            (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             this->infos_hi_.out (), this->configs_hi_.out (), 
             this->anomalies_hi_.out (), TAO_TRY_ENV);
           TAO_CHECK_ENV;

          sched_lo_->compute_scheduling
            (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD),
             this->infos_lo_.out (), this->configs_lo_.out (), 
             this->anomalies_lo_.out (), TAO_TRY_ENV);
           TAO_CHECK_ENV;

#endif /* ! __SUNPRO_CC */

      if (dump_schedule_headers_ && (this->hi_schedule_file_name_ != 0))
        {
          ACE_Scheduler_Factory::dump_schedule (infos_hi_.in (),
                                                configs_hi_.in (),
                                                anomalies_hi_.in (),
                                                this->hi_schedule_file_name_);
          TAO_CHECK_ENV;
        }

      if (dump_schedule_headers_ && (this->lo_schedule_file_name_ != 0))
        {
          ACE_Scheduler_Factory::dump_schedule (infos_lo_.in (),
                                                configs_lo_.in (),
                                                anomalies_lo_.in (),
                                                this->lo_schedule_file_name_);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;
}

void
DualEC_Supplier::start_generating_events (void)
{
  TAO_TRY
    {
      // Activate the POA manager.
      poa_manager_->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Spawn a thread that runs the orb event loop
      ACE_Thread_Manager orb_thread_manager;
      orb_thread_manager.spawn (DualEC_Supplier::run_orb);

      // Compute the scheduling information, based on the provided RT_Infos.
      this->compute_schedules ();

      // Load the scheduling data for the simulation.
      this->load_schedule_data (this->schedule_data_);

      // Spawn <event_thread_count_> threads to run 
      // over the data and generate events
      ACE_Thread_Manager event_thread_manager;
      event_thread_manager.spawn_n (event_thread_count_,
                                    DualEC_Supplier::run_event_thread, 
                                    this);

      // Wait for the threads that are generating events.
      event_thread_manager.wait ();

      // Shut down the ORB via the termination servant
      this->terminator_->shutdown ();
      TAO_CHECK_ENV;

      // Wait for the thread that runs the orb event loop.
      orb_thread_manager.wait ();

      // clean up the scheduling data
      ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *>
        schedule_iter (this->schedule_data_);
      Schedule_Viewer_Data **data_temp;
      for (schedule_iter.first ();
           schedule_iter.done () == 0;
           schedule_iter.advance ())
        if (schedule_iter.next (data_temp) && data_temp)
          delete (*data_temp);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

}

void
DualEC_Supplier::load_schedule_data
      (ACE_Unbounded_Queue<Schedule_Viewer_Data *> &schedule_data)
{
  Schedule_Viewer_Data *data = 0;

  if (this->input_file_name_)
    {
      // Open the scheduler data input file and read its contents into
      // a queue.
      FILE *input_file;

      int scan_count = 0;
      input_file = ACE_OS::fopen(this->input_file_name_, "r");

      if (input_file)
        {
          // Get a line at a time from the data file and parse it.
          char input_buf[BUFSIZ];
          while (ACE_OS::fgets (input_buf, BUFSIZ, input_file))
            {
              // Run through leading whitespace.
              char *temp = input_buf;
              while (*temp && isspace (*temp))
                ++temp;

              // If there is anything besides whitespace in the line
              // read, scan its fields into the scheduling data
              // structure.
              if (ACE_OS::strlen (temp) > 0)
                {
                  ACE_NEW (data, Schedule_Viewer_Data);
                  scan_count = sscanf (temp, "%s %lf %lf %lu %lu %lu %lu",
                                       data->operation_name,
                                       &data->utilitzation,
                                       &data->overhead,
                                       &data->arrival_time,
                                       &data->deadline_time,
                                       &data->completion_time,
                                       &data->computation_time);
                  if (scan_count != 7)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "DOVE_Supplier::start_generating_events: "
                                  "scanned incorrect number of data elements: %d\n", scan_count));

                      delete data;
                      return;
                    }

                  // Insert the data into the queue.
                  schedule_data.enqueue_tail (data);
                }
            }
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "DOVE_Supplier::start_generating_events: "
                      "could not open input file [%s].\n",
                      this->input_file_name_));
          return;
        }
    }
  else
  {
    u_long last_completion = 0;

    // Just create 10 dummy scheduling records and use them.
    for (int i = 0; i < 10; ++i)
    {
      ACE_NEW (data, Schedule_Viewer_Data);

      char *oper_name = 0;
      switch (i % 4)
      {
      case 0:
        oper_name = "high_20";
        break;

      case 1:
        oper_name = "low_20";
        break;

      case 2:
        oper_name = "high_10";
        break;

      case 3:
      default:
        oper_name = "low_10";
        break;
      }

      ACE_OS::strncpy (data->operation_name,
                       oper_name,
                       BUFSIZ-1);

      data->utilitzation = (double)(20.0+ACE_OS::rand() %10);
      data->overhead = (double)(ACE_OS::rand() %20);

      data->arrival_time = ACE_OS::rand() % 200;
      data->computation_time = (ACE_OS::rand() % 100) + 10;

      data->completion_time = last_completion + (ACE_OS::rand() % 100) + 100;
      data->completion_time =
        data->completion_time <  data->arrival_time + data->computation_time
        ? data->arrival_time + data->computation_time
        : data->completion_time;

      last_completion = data->completion_time;

      data->deadline_time = data->completion_time + (ACE_OS::rand() % 200) - 50;

      // insert the data into the queue.
      schedule_data.enqueue_tail (data);
    }
  }
}

// This function fills in the random data into the anys transported by
// the event channel.

void
DualEC_Supplier::insert_event_data (CORBA::Any &data,
                                    ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *> &schedule_iter,
                                    int &data_is_nav)
{
  static u_long last_completion = 0;

  // constants for periods (in units of one hundred nanoseconds)
  const TimeBase::TimeT ONE_HZ_PERIOD = 10000000;
  const TimeBase::TimeT FIVE_HZ_PERIOD = ONE_HZ_PERIOD / 5 ;
  const TimeBase::TimeT TEN_HZ_PERIOD = ONE_HZ_PERIOD / 10;
  const TimeBase::TimeT TWENTY_HZ_PERIOD = ONE_HZ_PERIOD / 20;

  TAO_TRY
  {
    Schedule_Viewer_Data **sched_data;

    if ((schedule_iter.next (sched_data)) && (sched_data) && (*sched_data))
    {
      if ((strcmp((*sched_data)->operation_name, "high_20") == 0) ||
           (strcmp((*sched_data)->operation_name, "low_20") == 0)  ||
           (strcmp((*sched_data)->operation_name, "high_1") == 0)  ||
           (strcmp((*sched_data)->operation_name, "low_1") == 0))
      {
        data_is_nav = 1;     
  
        if ((strcmp((*sched_data)->operation_name, "high_20") == 0) ||
            (strcmp((*sched_data)->operation_name, "high_1") == 0))
          {
            navigation_.criticality = 1;
          }
        else
          {
            navigation_.criticality = 0;
          }

        if ((strcmp((*sched_data)->operation_name, "high_20") == 0) ||
            (strcmp((*sched_data)->operation_name, "low_20") == 0))
          {
            navigation_.deadline_time = TWENTY_HZ_PERIOD;
          }
        else
          {
            navigation_.criticality = ONE_HZ_PERIOD;
          }

        navigation_.position_latitude = ACE_OS::rand() % 90;
        navigation_.position_longitude = ACE_OS::rand() % 180;
        navigation_.altitude = ACE_OS::rand() % 100;
        navigation_.heading = ACE_OS::rand() % 180;
        navigation_.roll = (navigation_.roll >= 180) ? -180 : navigation_.roll + 1;
        navigation_.pitch =  (navigation_.pitch >= 90) ? -90 : navigation_.pitch + 1;

        navigation_.utilization =       0.0;
        navigation_.overhead =          0.0;
        navigation_.arrival_time =      ORBSVCS_Time::zero;
        navigation_.completion_time =   ORBSVCS_Time::zero;
        navigation_.computation_time =  ORBSVCS_Time::zero;
        navigation_.update_data =       update_data_;


        // because the scheduler data does not supply these values
        navigation_.utilization = (double) (20.0 + ACE_OS::rand() % 10);
        navigation_.overhead = (double) (ACE_OS::rand() % 10);

        data.replace (_tc_Navigation, &navigation_, 0, TAO_TRY_ENV);
      }
      else if ((strcmp((*sched_data)->operation_name, "high_10") == 0) ||
               (strcmp((*sched_data)->operation_name, "low_10") == 0)  ||
                (strcmp((*sched_data)->operation_name, "high_5") == 0)  ||
                (strcmp((*sched_data)->operation_name, "low_5") == 0))
      {
        data_is_nav = 0;     

        if ((strcmp((*sched_data)->operation_name, "high_10") == 0) ||
            (strcmp((*sched_data)->operation_name, "high_5") == 0))
          {
            weapons_.criticality = 1;
          }
        else
          {
            weapons_.criticality = 0;
          }

        if ((strcmp((*sched_data)->operation_name, "high_10") == 0) ||
            (strcmp((*sched_data)->operation_name, "low_10") == 0))
          {
            navigation_.deadline_time = TEN_HZ_PERIOD;
          }
        else
          {
            navigation_.criticality = FIVE_HZ_PERIOD;
          }


        weapons_.number_of_weapons = 2;
        weapons_.weapon1_identifier = CORBA::string_alloc (30);
        strcpy (weapons_.weapon1_identifier.inout (),"Photon Torpedoes");
        weapons_.weapon1_status =(ACE_OS::rand() % 4) == 0 ? 0 : 1 ;
        weapons_.weapon2_identifier = CORBA::string_alloc (30);
        strcpy (weapons_.weapon2_identifier.inout (),"Quantum Torpedoes");
        weapons_.weapon2_status = (ACE_OS::rand() % 4) == 0 ? 0 : 1;
        weapons_.weapon3_identifier = CORBA::string_alloc (1);
        strcpy (weapons_.weapon3_identifier.inout (), "");
        weapons_.weapon3_status = 0;
        weapons_.weapon4_identifier = CORBA::string_alloc (1);
        strcpy (weapons_.weapon4_identifier.inout (), "");
        weapons_.weapon4_status = 0;
        weapons_.weapon5_identifier = CORBA::string_alloc (1);
        strcpy (weapons_.weapon5_identifier.inout (), "");
        weapons_.weapon5_status = 0;
        weapons_.utilization =       0.0;
        weapons_.overhead =          0.0;
        weapons_.arrival_time =      ORBSVCS_Time::zero;
        weapons_.completion_time  =  ORBSVCS_Time::zero;
        weapons_.computation_time =  ORBSVCS_Time::zero;
        weapons_.update_data =       update_data_;


        data.replace (_tc_Weapons, &weapons_, 0, TAO_TRY_ENV);
      }
      else {
        ACE_ERROR ((LM_ERROR,
                    "DualEC_Supplier::insert_event_data:"
                    "unrecognized operation name [%s]",
                    (*sched_data)->operation_name));
      }

      TAO_CHECK_ENV;


            if (last_completion > (*sched_data)->completion_time)
              last_completion = 0;

      if ((*sched_data)->completion_time >= last_completion)
      {
              ACE_Time_Value pause (0,
                                    (*sched_data)->completion_time -
                                      last_completion);
              ACE_OS::sleep (pause);
              last_completion = (*sched_data)->completion_time;
      }
    }
    else
      ACE_ERROR ((LM_ERROR,
                  "DualEC_Supplier::insert_event_data:"
                  "Could Not access scheduling data"));

    schedule_iter.advance ();

    if (schedule_iter.done ())
      schedule_iter.first ();
  }
  TAO_CATCHANY
  {
    ACE_ERROR ((LM_ERROR,
                "(%t)Error in DualEC_Supplier::insert_event_data.\n"));
  }
  TAO_ENDTRY;
}



// Function get_options.

unsigned int
DualEC_Supplier::get_options (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "f:m:t:dsrp");
  int opt;
  int temp;

  while ((opt = get_opt ()) != -1)
    {
      switch (opt)
        {
        case 'f':
          this->input_file_name_ = get_opt.optarg;

          if (!this->input_file_name_ || ACE_OS::strlen (this->input_file_name_) > 0)
            ACE_DEBUG ((LM_DEBUG,"Reading file!\n"));
          else
            {
              this->input_file_name_ = 0;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "%s: file name must be specified with -f option",
                                 argv[0]),
                                1);
            }
          break;

        case 'm':
          temp = ACE_OS::atoi (get_opt.optarg);
          if (temp > 0)
            {
              this->total_messages_ = (u_int) temp;
               ACE_DEBUG ((LM_DEBUG,
                           "Messages to send: %d\n",
                           this->total_messages_));
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: message count must be > 0",
                               argv[0]),
                               1);
          break;

        case 't':
          temp = ACE_OS::atoi (get_opt.optarg);
          if (temp > 0)
            {
              this->event_thread_count_ = (u_int) temp;
               ACE_DEBUG ((LM_DEBUG,
                           "Messages to send: %d\n",
                           this->total_messages_));
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: thread count must be > 0",
                               argv[0]),
                               1);
          break;

        case 'd':

          this->dump_schedule_headers_ = 1;
          break;

        case 's':
          update_data_ = 0;
          break;

        case  'r':
          use_runtime_schedulers_ = 1;
          break;

        case 'p':
          suppress_priority_ = 1;
          break;

        default:
        case '?':
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s %s\n",
                      argv[0], usage));
          ACE_OS::exit (0);
          break;
        }
    }

  if (argc != get_opt.optind)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s: too many arguments\n"
                       "Usage: %s %s\n",
                       argv[0],
                       argv[0],
                       usage),
                      1);

  return 0;
}

// function main

int
main (int argc, char *argv [])
{
  TAO_TRY
    {
      // Initialize ORB.
      TAO_ORB_Manager orb_Manager;

      orb_Manager.init (argc,
                        argv,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;


      // Create the demo supplier.
      DualEC_Supplier *event_Supplier_ptr;

      ACE_NEW_RETURN (event_Supplier_ptr,
                      DualEC_Supplier(argc, argv),
                      -1);

      // Initialize everthing
      if (event_Supplier_ptr->init () == -1)
        exit (1);

      // now we can go ahead
      event_Supplier_ptr->start_generating_events ();

      // when done, we clean up
      delete event_Supplier_ptr;
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<Schedule_Viewer_Data *>;
template class ACE_Unbounded_Queue<Schedule_Viewer_Data *>;
template class ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<Schedule_Viewer_Data *>
#pragma instantiate ACE_Unbounded_Queue<Schedule_Viewer_Data *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
