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
"                 -f <name of input data file>\n"
"                 [-O[RBport] ORB port number]\n"
"                 [-m <count> of messages to send (2000)]\n"
"                 [-b <count> at which to break navigation event\n"
"                             stream out onto its own channel (1000)]\n"
"                 [-n <usec> pause between navigation events (100000)]\n"
"                 [-w <usec> pause between weapons events (100000)]\n"
"                 [-d to dump scheduler header files]\n"
"                 [-s to suppress data updates by EC]\n"
"                 [-r to use runtime schedulers]\n"
"                 [-p to suppress prioritization of operations]\n";

DualEC_Supplier::DualEC_Supplier (int argc, char** argv)
: nav_pause_ (0, 100000),
  weap_pause_ (0, 100000),
  channel_hi_name_ (1),
  channel_lo_name_ (1),
  sched_hi_name_ (1),
  sched_lo_name_ (1),
  sched_hi_impl_ (0),
  sched_lo_impl_ (0),
  ec_hi_impl_ (0),
  ec_lo_impl_ (0),
  argc_(argc),
  argv_(argv),
  total_messages_ (2000),
  break_count_(-1),
  input_file_name_(0),
  update_data_ (1),
  dump_schedule_headers_ (0),
  use_runtime_schedulers_ (0),
  suppress_priority_ (0),
  hi_schedule_file_name_ ("DualEC_Runtime_Hi.h"),
  lo_schedule_file_name_ ("DualEC_Runtime_Lo.h"),
  nav_roll_ (0),
  nav_pitch_ (0)
{
  TAO_TRY
    {
      this->sched_hi_name_.length (1);
      this->sched_hi_name_[0].id = CORBA::string_dup ("DUAL_SCHED_HI");
      TAO_CHECK_ENV;

      this->sched_lo_name_.length (1);
      this->sched_lo_name_[0].id = CORBA::string_dup ("DUAL_SCHED_LO");
      TAO_CHECK_ENV;

      this->channel_hi_name_.length (1);
      this->channel_hi_name_[0].id = CORBA::string_dup ("DUAL_EC_HI");
      TAO_CHECK_ENV;

      this->channel_lo_name_.length (1);
      this->channel_lo_name_[0].id = CORBA::string_dup ("DUAL_EC_LO");
      TAO_CHECK_ENV;

      this->terminator_ = terminator_impl_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("DualEC_Supplier::DualEC_Supplier : could "
                                 "not resolve reference to terminator");
  }
  TAO_ENDTRY;

  // Initialize the high priority RT_Info data
  rt_info_nav_hi_.entry_point = "DUALEC_NAV_HI";
  rt_info_nav_hi_.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
  rt_info_nav_hi_.worst_case_execution_time = ORBSVCS_Time::zero ();
  rt_info_nav_hi_.typical_execution_time = ORBSVCS_Time::zero ();
  rt_info_nav_hi_.cached_execution_time = ORBSVCS_Time::zero ();
  rt_info_nav_hi_.period = 2500000;
  rt_info_nav_hi_.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;
  rt_info_nav_hi_.quantum = ORBSVCS_Time::zero ();
  rt_info_nav_hi_.threads = 1;
  rt_info_nav_hi_.info_type = RtecScheduler::OPERATION;
  rt_info_weap_hi_ = rt_info_nav_hi_;
  rt_info_weap_hi_.entry_point = "DUALEC_WEAP_HI";
  rt_info_dummy_hi_ = rt_info_nav_hi_;
  rt_info_dummy_hi_.entry_point = "DUALEC_DUMMY_HI";

  // Initialize the low priority RT_Info data
  rt_info_nav_lo_.entry_point = "DUALEC_NAV_LO";
  rt_info_nav_lo_.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
  rt_info_nav_lo_.worst_case_execution_time = ORBSVCS_Time::zero ();
  rt_info_nav_lo_.typical_execution_time = ORBSVCS_Time::zero ();
  rt_info_nav_lo_.cached_execution_time = ORBSVCS_Time::zero ();
  rt_info_nav_lo_.period = 10000000;
  rt_info_nav_lo_.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
  rt_info_nav_lo_.quantum = ORBSVCS_Time::zero ();
  rt_info_nav_lo_.threads = 1;
  rt_info_nav_lo_.info_type = RtecScheduler::OPERATION;
  rt_info_weap_lo_ = rt_info_nav_lo_;
  rt_info_weap_lo_.entry_point = "DUALEC_WEAP_LO";
  rt_info_dummy_lo_ = rt_info_nav_lo_;
  rt_info_dummy_lo_.entry_point = "DUALEC_DUMMY_LO";
}

DualEC_Supplier::~DualEC_Supplier ()
{
  TAO_TRY
    {
      this->navigation_Supplier_.disconnect ();
      this->weapons_Supplier_.disconnect ();

      // Unbind the schedulers from the NS.
      this->naming_context_->unbind (this->sched_hi_name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->naming_context_->unbind (this->sched_lo_name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Unbind the ECs from the NS.
      this->naming_context_->unbind (this->channel_hi_name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      this->naming_context_->unbind (this->channel_lo_name_, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("DualEC_Supplier::~DualEC_Supplier");
    }
  TAO_ENDTRY;

  // @@TBD - destroy the ECs
  // @@TBD - destroy the schedulers
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
  ACE_Scheduler_Factory::POD_RT_Info * rt_info_nav_hi = 
    (suppress_priority_) ? 0 : &rt_info_nav_hi_;
  ACE_Scheduler_Factory::POD_RT_Info * rt_info_weap_hi = 
    (suppress_priority_) ? 0 : &rt_info_weap_hi_;
  ACE_Scheduler_Factory::POD_RT_Info * rt_info_nav_lo = 
    (suppress_priority_) ? 0 : &rt_info_nav_lo_;
  ACE_Scheduler_Factory::POD_RT_Info * rt_info_weap_lo = 
    (suppress_priority_) ? 0 : &rt_info_weap_lo_;

  if (this->navigation_Supplier_.connect ("MIB_unknown", 
                                          "DUAL_EC_HI",
                                          "DUAL_SCHED_HI",
                                           rt_info_nav_hi) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not connect navigation supplier to DUAL_EC_HI"),
                        -1);
    }
 
 if (this->navigation_Supplier_.connect ("MIB_unknown", 
                                          "DUAL_EC_LO",
                                          "DUAL_SCHED_LO",
                                           rt_info_nav_lo) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not connect navigation supplier to DUAL_EC_LO"),
                        -1);
    }

  if (this->weapons_Supplier_.connect ("MIB_unknown", 
                                       "DUAL_EC_HI",
                                       "DUAL_SCHED_HI",
                                       rt_info_weap_hi) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not connect weapons supplier to DUAL_EC_HI"),
                        -1);
    }
 
 if (this->weapons_Supplier_.connect ("MIB_unknown", 
                                      "DUAL_EC_LO",
                                      "DUAL_SCHED_LO",
                                      rt_info_weap_lo) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Could not connect weapons supplier to DUAL_EC_LO"),
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


// Run navigation event generation thread.

void *
DualEC_Supplier::run_nav_thread (void *arg)
{
  DualEC_Supplier * sup = 
    ACE_static_cast (DualEC_Supplier *, arg);

  TAO_TRY
    {
      ACE_Unbounded_Queue_Iterator<Navigation *>
        nav_iter (sup->navigation_data_);

      if (nav_iter.done ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DualEC_Supplier::run_event_thread: "
                             "there is no navigation data\n"), 0);
        }

      CORBA::Any any;

      long total_sent = 0;

      do
      {
        // Insert the event data
        Navigation **nav;

        if ((nav_iter.next (nav)) && (nav) && (*nav))
          {
            any.replace (_tc_Navigation, *nav, 0, TAO_TRY_ENV);

            // Sleep briefly to avoid too much livelock (a little is good). 
            ACE_OS::sleep (sup->nav_pause_);

            // If the break count has been reached, change the
            // channel that is being used by the NAV supplier
            if (total_sent == sup->break_count_)
              {
               ACE_DEBUG ((LM_DEBUG,
                           "breaking out nav at event: %d\n",
                           sup->break_count_));

                sup->navigation_Supplier_.use_next_connection ();
              }

            sup->navigation_Supplier_.notify (any);
          }
        else
          {
            ACE_ERROR ((LM_ERROR,
                        "DualEC_Supplier::run_nav_thread:"
                        "Could Not access navigation data"));
          }

        if (total_sent < 5)
          ACE_DEBUG ((LM_DEBUG,
                      "Pushing event data.\n"));
        else if (total_sent == 5)
          ACE_DEBUG ((LM_DEBUG,
                      "Everything is running. Going to be mute.\n"));

        nav_iter.advance ();

        if (nav_iter.done ())
          nav_iter.first ();

      }
      while (++total_sent < sup->total_messages_);

    }
  TAO_CATCHANY
    {
    }
  TAO_ENDTRY;

  return 0;
}


// Run weapons event generation thread.

void *
DualEC_Supplier::run_weap_thread (void *arg)
{
  DualEC_Supplier * sup = 
    ACE_static_cast (DualEC_Supplier *, arg);

  TAO_TRY
    {
      ACE_Unbounded_Queue_Iterator<Weapons *>
        weap_iter (sup->weapons_data_);

      if (weap_iter.done ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "DualEC_Supplier::run_event_thread: "
                             "there is no navigation data\n"), 0);
        }

      CORBA::Any any;

      long total_sent = 0;

      do
      {
        // Insert the event data
        Weapons **weap;

        if ((weap_iter.next (weap)) && (weap) && (*weap))
          {
            any.replace (_tc_Weapons, *weap, 0, TAO_TRY_ENV);

            // Sleep briefly to avoid too much livelock (a little is good). 
            ACE_OS::sleep (sup->weap_pause_);

            sup->weapons_Supplier_.notify (any);
          }
        else
          {
            ACE_ERROR ((LM_ERROR,
                        "DualEC_Supplier::run_weap_thread:"
                        "Could Not access weapons data"));
          }

        if (total_sent < 5)
          ACE_DEBUG ((LM_DEBUG,
                      "Pushing event data.\n"));
        else if (total_sent == 5)
          ACE_DEBUG ((LM_DEBUG,
                      "Everything is running. Going to be mute.\n"));

        weap_iter.advance ();

        if (weap_iter.done ())
          weap_iter.first ();

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
  // @@TBD - look at a command line modified setting,
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

          this->naming_context_->bind (this ->sched_hi_name_, 
                                       this->sched_hi_.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          naming_context_->bind (this->sched_lo_name_, 
                                 this->sched_lo_.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Register high and low priority rt_infos with the 
          // schedulers to force priority differentiation.

          this->sched_hi_rt_info_hi_ = 
            this->sched_hi_->
              create (this->rt_info_dummy_hi_.entry_point, 
                      TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_hi_->
            set (this->sched_hi_rt_info_hi_,
                 ACE_static_cast (RtecScheduler::Criticality_t,
	                          this->rt_info_dummy_hi_.criticality),
                 this->rt_info_dummy_hi_.worst_case_execution_time,
                 this->rt_info_dummy_hi_.typical_execution_time,
                 this->rt_info_dummy_hi_.cached_execution_time,
                 this->rt_info_dummy_hi_.period,
                 ACE_static_cast (RtecScheduler::Importance_t,
                                  this->rt_info_dummy_hi_.importance),
                 this->rt_info_dummy_hi_.quantum,
                 this->rt_info_dummy_hi_.threads,
                 ACE_static_cast (RtecScheduler::Info_Type_t,
                                  this->rt_info_dummy_hi_.info_type),
                 TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_hi_rt_info_lo_ = 
            this->sched_hi_->
              create (this->rt_info_dummy_lo_.entry_point, 
                      TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_hi_->
            set (this->sched_hi_rt_info_lo_,
                 ACE_static_cast (RtecScheduler::Criticality_t,
	                          this->rt_info_dummy_lo_.criticality),
                 this->rt_info_dummy_lo_.worst_case_execution_time,
                 this->rt_info_dummy_lo_.typical_execution_time,
                 this->rt_info_dummy_lo_.cached_execution_time,
                 this->rt_info_dummy_lo_.period,
                 ACE_static_cast (RtecScheduler::Importance_t,
                                  this->rt_info_dummy_lo_.importance),
                 this->rt_info_dummy_lo_.quantum,
                 this->rt_info_dummy_lo_.threads,
                 ACE_static_cast (RtecScheduler::Info_Type_t,
                                  this->rt_info_dummy_lo_.info_type),
                 TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_hi_rt_info_hi_ = 
            this->sched_lo_->
              create (this->rt_info_dummy_hi_.entry_point, 
                      TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_lo_->
            set (this->sched_hi_rt_info_hi_,
                 ACE_static_cast (RtecScheduler::Criticality_t,
	                          this->rt_info_dummy_hi_.criticality),
                 this->rt_info_dummy_hi_.worst_case_execution_time,
                 this->rt_info_dummy_hi_.typical_execution_time,
                 this->rt_info_dummy_hi_.cached_execution_time,
                 this->rt_info_dummy_hi_.period,
                 ACE_static_cast (RtecScheduler::Importance_t,
                                  this->rt_info_dummy_hi_.importance),
                 this->rt_info_dummy_hi_.quantum,
                 this->rt_info_dummy_hi_.threads,
                 ACE_static_cast (RtecScheduler::Info_Type_t,
                                  this->rt_info_dummy_hi_.info_type),
                 TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_hi_rt_info_lo_ = 
            this->sched_lo_->
              create (this->rt_info_dummy_lo_.entry_point, 
                      TAO_TRY_ENV);

          TAO_CHECK_ENV;

          this->sched_lo_->
            set (this->sched_hi_rt_info_lo_,
                 ACE_static_cast (RtecScheduler::Criticality_t,
	                          this->rt_info_dummy_lo_.criticality),
                 this->rt_info_dummy_lo_.worst_case_execution_time,
                 this->rt_info_dummy_lo_.typical_execution_time,
                 this->rt_info_dummy_lo_.cached_execution_time,
                 this->rt_info_dummy_lo_.period,
                 ACE_static_cast (RtecScheduler::Importance_t,
                                  this->rt_info_dummy_lo_.importance),
                 this->rt_info_dummy_lo_.quantum,
                 this->rt_info_dummy_lo_.threads,
                 ACE_static_cast (RtecScheduler::Info_Type_t,
                                  this->rt_info_dummy_lo_.info_type),
                 TAO_TRY_ENV);

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

      naming_context_->bind (this->channel_hi_name_, 
                             this->ec_hi_.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      naming_context_->bind (this->channel_lo_name_, 
		                     this->ec_lo_.in (), TAO_TRY_ENV);
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
      this->load_schedule_data ();

      // Sleep for 10 seconds to give time for registrations.
      ACE_DEBUG ((LM_DEBUG,
		  "\nDUAL_SCHED_HI, DUAL_SCHED_LO, DUAL_EC_HI and "
                  "DUAL_EC_LO are registered with the Naming Service.\n"
                  "Sleeping 10 seconds before generating events\n"));
      ACE_Time_Value tv (10, 0);
      ACE_OS::sleep (tv);

      // Spawn thread to run over the navigation data and generate events.
      ACE_Thread_Manager event_thread_manager;
      event_thread_manager.spawn (DualEC_Supplier::run_nav_thread, 
                                  this);

      // Spawn thread to run over the weapons data and generate events.
      event_thread_manager.spawn (DualEC_Supplier::run_weap_thread, 
                                  this);

      // Wait for the threads that are generating events.
      event_thread_manager.wait ();

      // Shut down the ORB via the termination servant
      this->terminator_->shutdown ();
      TAO_CHECK_ENV;

      // Wait for the thread that runs the orb event loop.
      orb_thread_manager.wait ();

      // Clean up the navigation data.
      ACE_Unbounded_Queue_Iterator<Navigation *>
        nav_iter (this->navigation_data_);
      Navigation **nav_temp;
      for (nav_iter.first ();
           nav_iter.done () == 0;
           nav_iter.advance ())
        if (nav_iter.next (nav_temp) && nav_temp)
          delete (*nav_temp);

      // Clean up the weapons data.
      ACE_Unbounded_Queue_Iterator<Weapons *>
        weap_iter (this->weapons_data_);
      Weapons **weap_temp;
      for (weap_iter.first ();
           weap_iter.done () == 0;
           weap_iter.advance ())
        if (weap_iter.next (weap_temp) && weap_temp)
          delete (*weap_temp);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;

}

void
DualEC_Supplier::load_schedule_data ()
{
  Navigation * nav = 0;
  Weapons * weap = 0;

  Schedule_Viewer_Data data;

  // constants for periods (in units of one hundred nanoseconds)
  const TimeBase::TimeT ONE_HZ_PERIOD = 10000000;
  const TimeBase::TimeT FIVE_HZ_PERIOD = ONE_HZ_PERIOD / 5 ;
  const TimeBase::TimeT TEN_HZ_PERIOD = ONE_HZ_PERIOD / 10;
  const TimeBase::TimeT TWENTY_HZ_PERIOD = ONE_HZ_PERIOD / 20;

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
                  scan_count = sscanf (temp, "%s %lf %lf %lu %lu %lu %lu",
                                       data.operation_name,
                                       &data.utilitzation,
                                       &data.overhead,
                                       &data.arrival_time,
                                       &data.deadline_time,
                                       &data.completion_time,
                                       &data.computation_time);
                  if (scan_count != 7)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "DOVE_Supplier::load_schedule_data: "
                                  "scanned incorrect number of data elements: %d\n", scan_count));
                      return;
                    }

 
                  if ((strcmp(data.operation_name, "high_20") == 0) ||
                      (strcmp(data.operation_name, "low_20") == 0))
                    {
                      ACE_NEW (weap, Weapons);
                      if (weap == 0)
                        {                       
                          ACE_ERROR ((LM_ERROR,
                                      "DOVE_Supplier::load_schedule_data: "
                                      "failed to allocate Weapons\n"));
                          return;
                        }

                      weap->criticality = 1;
                      weap->deadline_time = TWENTY_HZ_PERIOD;
                      weap->number_of_weapons = 2;
                      weap->weapon1_identifier = CORBA::string_alloc (30);
                      strcpy (weap->weapon1_identifier.inout (),"Photon Torpedoes");
                      weap->weapon1_status =(ACE_OS::rand() % 4) == 0 ? 0 : 1 ;
                      weap->weapon2_identifier = CORBA::string_alloc (30);
                      strcpy (weap->weapon2_identifier.inout (),"Quantum Torpedoes");
                      weap->weapon2_status = (ACE_OS::rand() % 4) == 0 ? 0 : 1;
                      weap->weapon3_identifier = CORBA::string_alloc (1);
                      strcpy (weap->weapon3_identifier.inout (), "");
                      weap->weapon3_status = 0;
                      weap->weapon4_identifier = CORBA::string_alloc (1);
                      strcpy (weap->weapon4_identifier.inout (), "");
                      weap->weapon4_status = 0;
                      weap->weapon5_identifier = CORBA::string_alloc (1);
                      strcpy (weap->weapon5_identifier.inout (), "");
                      weap->weapon5_status = 0;
                      weap->utilization =       0.0;
                      weap->overhead =          0.0;
                      weap->arrival_time =      ORBSVCS_Time::zero ();
                      weap->completion_time  =  ORBSVCS_Time::zero ();
                      weap->computation_time =  ORBSVCS_Time::zero ();
                      weap->update_data =       update_data_;

                      // Insert the data into the queue.
                      weapons_data_.enqueue_tail (weap);
                    }
                  else
                    {
                      ACE_NEW (nav, Navigation);
                      if (nav == 0)
                        {                       
                          ACE_ERROR ((LM_ERROR,
                                      "DOVE_Supplier::load_schedule_data: "
                                  "failed to allocate Navigation\n"));
                          return;
                        }

                      nav->criticality = 0;
                      nav->deadline_time = TWENTY_HZ_PERIOD;
                      nav->position_latitude = ACE_OS::rand() % 90;
                      nav->position_longitude = ACE_OS::rand() % 180;
                      nav->altitude = ACE_OS::rand() % 100;
                      nav->heading = ACE_OS::rand() % 180;
                      this->nav_roll_ = (this->nav_roll_ >= 180) ? -180 : this->nav_roll_ + 1;
                      nav->roll = this->nav_roll_;
                      this->nav_pitch_ =  (this->nav_pitch_ >= 90) ? -90 : this->nav_pitch_ + 1;
                      nav->pitch = this->nav_pitch_;
                      nav->utilization =       0.0;
                      nav->overhead =          0.0;
                      nav->arrival_time =      ORBSVCS_Time::zero ();
                      nav->completion_time =   ORBSVCS_Time::zero ();
                      nav->computation_time =  ORBSVCS_Time::zero ();
                      nav->update_data =       this->update_data_;
                      nav->utilization = (double) (20.0 + ACE_OS::rand() % 10);
                      nav->overhead = (double) (ACE_OS::rand() % 10);

                      // Insert the data into the queue.
                      navigation_data_.enqueue_tail (nav);
                    }
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
}


// Get command line options.

unsigned int
DualEC_Supplier::get_options (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "f:m:b:n:w:dsrp");
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

        case 'b':
          temp = ACE_OS::atoi (get_opt.optarg);
          if (temp > 0)
            {
              this->break_count_ = (u_int) temp;
               ACE_DEBUG ((LM_DEBUG,
                           "Break count: %d\n",
                           this->break_count_));
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: break count must be > 0",
                               argv[0]),
                               1);
          break;

        case 'n':
          temp = ACE_OS::atoi (get_opt.optarg);
          if (temp >= 0)
            {
              this->nav_pause_ = 
                ACE_Time_Value(0, ACE_static_cast (long, temp));
              ACE_DEBUG ((LM_DEBUG,
                          "Navigation pause: %d usec\n",
                          temp));
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: navigation pause must be >= 0",
                               argv[0]),
                               1);
          break;


        case 'w':
          temp = ACE_OS::atoi (get_opt.optarg);
          if (temp >= 0)
            {
              this->weap_pause_ = 
                ACE_Time_Value(0, ACE_static_cast (long, temp));
              ACE_DEBUG ((LM_DEBUG,
                          "Weapons pause: %d usec\n",
                          temp));
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: weapons pause must be >= 0",
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
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  int min_priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO);

  // Set all threads in the process into the RT scheduling class
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              min_priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
          ACE_DEBUG ((LM_DEBUG,
                      "%s: user is not superuser, "
                      "so remain in time-sharing class\n", argv[0]));
      else
          ACE_ERROR ((LM_ERROR,
                      "%s: ACE_OS::sched_params failed\n", argv[0]));
    }



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
template class ACE_Node<Navigation *>;
template class ACE_Unbounded_Queue<Navigation *>;
template class ACE_Unbounded_Queue_Iterator<Navigation *>;
template class ACE_Node<Weapons *>;
template class ACE_Unbounded_Queue<Weapons *>;
template class ACE_Unbounded_Queue_Iterator<Weapons *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<Navigation *>
#pragma instantiate ACE_Unbounded_Queue<Navigation *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<Navigation *>
#pragma instantiate ACE_Node<Weapons *>
#pragma instantiate ACE_Unbounded_Queue<Weapons *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<Weapons *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
