// $Id$

#include "ace/OS.h"
#include "ace/ARGV.h"
#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"

#include "tao/corba.h"
#include "tao/RTCORBA/Multi_Priority_Mapping.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/Metrics/Metrics_Logger.h"
#include "orbsvcs/Rate_Group_Info.h"

#include "Metrics_Local_Cache_Test.h"

ACE_RCSID(Metrics_Local_Cache_Test, Metrics_Local_Cache_Test, "$Id$")

const int base_port = 10015;

const unsigned int TAO_Metrics_Local_Cache_Test::number_of_hrt_rates_ = 5;
const unsigned int TAO_Metrics_Local_Cache_Test::number_of_srt_rates_ = 1;

const unsigned long TAO_Metrics_Local_Cache_Test::the_hrt_Rates_in_microseconds_[] =
{
     25000,             // 25 msec
     50000,             // 50 msec
    100000,             // 100 msec
    200000,             // 200 msec
    1000000             // 1 sec
};

const int TAO_Metrics_Local_Cache_Test::highest_rate_group_priority_ =
// VxWorks targets
#ifdef VXWORKS
      // Priorities on VxWorks range from 0 to 255 with 255 being the lowest priority.
      // Recommended priority is below the lowest net task of VxWorks.  (60 is a safe number)
     150;
#else
// Windows targets
#ifdef ACE_WIN32

#if defined (ACE_THR_PRI_FIFO_DEF)
      ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
#else
      ACE_DEFAULT_THREAD_PRIORITY;
#endif /* ACE_THR_PRI_FIFO_DEF */

// Other targets including Linux
#else
     50;
#endif

#endif

char *orb_init_params =
#if defined INFRA_HAS_TOA_GIOP_LITE && defined INFRA_HAS_TAO_DEBUG
   "-ORBObjRefStyle URL -ORBDebugLevel 1 -ORBGIOPlite -ORBCollocationStrategy direct";
#elif defined INFRA_HAS_TOA_GIOP_LITE
   "-ORBObjRefStyle URL -ORBGIOPlite -ORBCollocationStrategy direct";
#elif defined INFRA_HAS_TAO_DEBUG
   "-ORBObjRefStyle URL -ORBDebugLevel 10 -ORBCollocationStrategy direct";
#else
   "-ORBObjRefStyle URL -ORBCollocationStrategy direct";
#endif  // ORB Init options

int enable_debug = 0;
// Used to enable LM_DEBUG messages

int enable_trace = 0;
  // Used to enable LM_TRACE messages

//////////////////////////////////////////
// class TAO_Metrics_Local_Cache_Test //
//////////////////////////////////////////

// Default Constructor.

TAO_Metrics_Local_Cache_Test::TAO_Metrics_Local_Cache_Test (int local_monitor,
                                                            int local_logger)
  : orb_(CORBA::ORB::_nil()),
    logger_impl_ (0),
    upcall_monitor_impl_(0),
    H40_push_consumer_adapter_ (0),
    H20_push_consumer_adapter_ (0),
    H10_push_consumer_adapter_ (0),
    H05_push_consumer_adapter_ (0),
    H01_push_consumer_adapter_ (0),
    S40_push_consumer_adapter_ (0),
    S20_push_consumer_adapter_ (0),
    S10_push_consumer_adapter_ (0),
    S05_push_consumer_adapter_ (0),
    S01_push_consumer_adapter_ (0),
    logger_ior_file_name_ (0),
    monitor_ior_file_name_ (0),
    log_file_name_ (0),
    log_output_ (0),
    visualize_ (0),
    local_monitor_(local_monitor),
    local_logger_(local_logger),
    rate_group_info_(0),
    master_host_name_(0),
    master_host_ip_(0)
{
}


TAO_Metrics_Local_Cache_Test::TAO_Metrics_Local_Cache_Test (int argc,
                                                            char *argv[],
                                                            int local_monitor,
                                                            int local_logger)
  : orb_(CORBA::ORB::_nil()),
    logger_impl_ (0),
    H40_push_consumer_adapter_ (0),
    H20_push_consumer_adapter_ (0),
    H10_push_consumer_adapter_ (0),
    H05_push_consumer_adapter_ (0),
    H01_push_consumer_adapter_ (0),
    S40_push_consumer_adapter_ (0),
    S20_push_consumer_adapter_ (0),
    S10_push_consumer_adapter_ (0),
    S05_push_consumer_adapter_ (0),
    S01_push_consumer_adapter_ (0),
    logger_ior_file_name_ (0),
    monitor_ior_file_name_ (0),
    log_file_name_ (0),
    log_output_ (0),
    visualize_ (0),
    local_monitor_(local_monitor),
    local_logger_(local_logger),
    rate_group_info_(0)
{
  this->init (argc, argv);
}

// Destructor.

TAO_Metrics_Local_Cache_Test::~TAO_Metrics_Local_Cache_Test (void)
{
}

void
TAO_Metrics_Local_Cache_Test::generate_orb_args (char *& orb_init_str)
{
   int port = 0;
   char endpoint[255];   // Just use a large value to ensure the buffer is large enough for an endpoint
   RTCORBA::Priority the_corba_priority;

   const unsigned int number_of_rates = TAO_Metrics_Local_Cache_Test::number_of_hrt_rates_ +
                                        TAO_Metrics_Local_Cache_Test::number_of_srt_rates_;

   rate_group_info_ = new TAO_Rate_Group_Info* [number_of_rates];

   // Set the number of rate groups
   TAO_Rate_Group_Info::set_num_rate_groups(number_of_rates);

   // Set the base corba priority.  Priority of the highest endpoint
   TAO_Rate_Group_Info::set_corba_base_priority(50);

   // Set the spacing between consecutive endpoints.
   TAO_Rate_Group_Info::set_priority_spacing(1);

#ifdef ACE_WIN32 // WINDOWS
   // Priorities are not contiguous on Windows Platforms
   TAO_Rate_Group_Info::set_priorities_contiguous(0);
#else // !WINDOWS
   TAO_Rate_Group_Info::set_priorities_contiguous(1);
#endif

   unsigned int rate_ndx = 0;
   for (rate_ndx = 0; rate_ndx < number_of_rates; rate_ndx++)
   {
      rate_group_info_[rate_ndx] = new TAO_Rate_Group_Info;
   }

   int next_priority = 0;
   int prev_priority = 0;
   for (rate_ndx = 0;
        rate_ndx < TAO_Metrics_Local_Cache_Test::number_of_hrt_rates_;
        rate_ndx++)
   {
      // Rate for this HRT Rate Group
      rate_group_info_[rate_ndx]->set_rate( TAO_Metrics_Local_Cache_Test::the_hrt_Rates_in_microseconds_[rate_ndx] );

      // OS Priority of the highest rate group
      if (rate_ndx == 0)
      {
         next_priority = highest_rate_group_priority_;
      }
      else
      {
         next_priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                              prev_priority,
                                                              ACE_SCOPE_THREAD);
      }

      prev_priority = next_priority;
      rate_group_info_[rate_ndx]->set_rate_group_priority( next_priority );
   }

   if ( TAO_Metrics_Local_Cache_Test::number_of_srt_rates_ > 0 )
   {
      const unsigned int index_of_srt_rates = TAO_Metrics_Local_Cache_Test::number_of_hrt_rates_;

      // Temporary rate patch for Dynamic Queue
      rate_group_info_[index_of_srt_rates]->set_rate(1375000);
      if ( TAO_Metrics_Local_Cache_Test::number_of_hrt_rates_ == 0 )
      {
         next_priority = TAO_Metrics_Local_Cache_Test::highest_rate_group_priority_;
      }
      else
      {
         next_priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                              prev_priority,
                                                              ACE_SCOPE_THREAD);
      }

      rate_group_info_[index_of_srt_rates]->set_rate_group_priority(next_priority);  // Dynamic Rate Group Priority
   }

   TAO_Multi_Priority_Mapping priorityMapping (rate_group_info_[0]->get_rate_group_priority(),       // Priority for the highest rate group
                                              TAO_Rate_Group_Info::get_corba_base_priority(),    // The starting CORBA priority value
                                              TAO_Rate_Group_Info::get_priority_spacing(),       // Priority spacing between rate groups.
                                              TAO_Rate_Group_Info::get_priorities_contiguous(),  // Priority spacing between rate groups.
                                              ACE_SCHED_FIFO);

   u_int init_str_len = 0;
   if (master_host_ip_ != 0)
   {
      // Allocate an orb init string
      init_str_len = (number_of_rates) * (
                      21 + // " -ORBendpoint iiop://"
                      ACE_OS::strlen(this->master_host_ip_) +
                      6 +  // ":" + Port Number length
                      10 +  // "/priority="
                      5 ) +  // Port Number length
                      1 /* Null Terminator */;
   }
   else if (master_host_name_ != 0)
   {
      // Allocate an orb init string
      init_str_len = (number_of_rates) * (
                      21 + // " -ORBendpoint iiop://"
                      ACE_OS::strlen(this->master_host_name_) +
                      6 +  // ":" + Port Number length
                      10 +  // "/priority="
                      5 ) +  // Port Number length
                      1 /* Null Terminator */;
   }
   else
   {
      ACE_ERROR((LM_ERROR,"You must specify either a master host name or master host IP\n%a"));
   }


   // Include the application provided init string.
   if (init_str_len)
   {
      init_str_len += ACE_OS::strlen(orb_init_params) + 1;
   }

   orb_init_str = CORBA::string_alloc (init_str_len);
   *orb_init_str =0;

   // Build the endpoint string;
   for (rate_ndx = 0; rate_ndx < number_of_rates; rate_ndx++)
   {
      port = base_port + rate_ndx;
      priorityMapping.to_CORBA(rate_group_info_[rate_ndx]->get_rate_group_priority(), the_corba_priority);

      // Build the endpoint string
      ACE_OS::sprintf(endpoint,
                      " -ORBendpoint iiop://%s:%d/priority=%d",
                      this->master_host_name_,
                      port,
                      the_corba_priority );

      // Append to the string
           ACE_OS::strcat( orb_init_str, endpoint );
   }

   if (orb_init_str)
   {
      // Concatenate the application defined init string
      ACE_OS::strcat( orb_init_str, " " );
      ACE_OS::strcat( orb_init_str, orb_init_params );
   }

   ACE_DEBUG((LM_DEBUG, "ORB _init string is %s\n",orb_init_str ));
}

int
TAO_Metrics_Local_Cache_Test::parse_args (int argc,
                                          char *argv[])
{
   int num_orb_args = 0;

   ACE_Arg_Shifter arg_shifter (argc, argv);

   // Ignore the first argument.  Program Name
   arg_shifter.ignore_arg ();

   while (arg_shifter.is_anything_left ())
   {
      const ACE_TCHAR *current_arg = 0;

      if ( (current_arg = arg_shifter.get_the_parameter("-masterHostName")) )
      {
         this->master_host_name_ = CORBA::string_dup(current_arg);
         arg_shifter.consume_arg();
      }
      else if ( (current_arg = arg_shifter.get_the_parameter("-masterHostIP")) )
      {
         this->master_host_ip_ = CORBA::string_dup(current_arg);
         arg_shifter.consume_arg();
      }
      else if ( (current_arg = arg_shifter.get_the_parameter("-logger_ior")) )
      {
         this->logger_ior_file_name_ = CORBA::string_dup(current_arg);;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-debug") >= 0 )
      {
         enable_debug = 1;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("-trace") >= 0 )
      {
         enable_trace = 1;
         arg_shifter.consume_arg();
      }
      else if (  arg_shifter.cur_arg_strncasecmp("?") >= 0 )
      {
         ACE_DEBUG ((LM_DEBUG,
                     "Usage: %s "
                     "[-masterHostName <master host name>] "
                     "[-masterHostIP <master host IP address>] "
                     "[-logger_ior logger_ior_file_name] "

                     "[-debug] (Enable LM_DEBUG log priority)"
                     "[-trace] (Enable LM_TRACE log priority)"
                     "\n",
                     argv[0]));
      }
      else
      {
         ACE_DEBUG((LM_DEBUG, "Unrecognized Command Line Arg: %s.  Assuming user-defined parameter\n", arg_shifter.get_current()));
         arg_shifter.ignore_arg ();
            num_orb_args++;
      }
   }
   return num_orb_args;
}

// Initialize the logger driver with the command line arguments.
int
TAO_Metrics_Local_Cache_Test::init (int argc, char *argv[])
{
   char *orb_init_string = 0;

   // Parse User supplied arguments leaving any ORB init arguments
   int orb_argc = this->parse_args (argc, argv);
   if (enable_debug)
   {
      ACE_Log_Msg::enable_debug_messages();
   }
   else
   {
      ACE_Log_Msg::disable_debug_messages();
   }

   if (enable_trace)
   {
      ACE_Log_Msg::enable_debug_messages(LM_TRACE);
   }
   else
   {
      ACE_Log_Msg::disable_debug_messages(LM_TRACE);
   }

   char** orb_argv = 0;
   ACE_ARGV *the_orb_args = 0;

   if (orb_argc == 0)
   {
      this->generate_orb_args(orb_init_string);
      the_orb_args = new ACE_ARGV (orb_init_string);
      orb_argc = the_orb_args->argc ();
      orb_argv = the_orb_args->argv ();
   }
   else
   {
      // If the user specified ORB args, then assume that the user is handling
      // proper setup of the ORB.
      orb_argv = argv;
   }

   ACE_TRY_NEW_ENV
   {
      // Initialize ORB.
      this->orb_ = CORBA::ORB_init (orb_argc, orb_argv);

      // Connect to the RootPOA.
      CORBA::Object_var poa_object =
         this->orb_->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
         ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                           1);

      this->root_poa_ = PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_manager_.in ()))
      {
         this->poa_manager_ = root_poa_->the_POAManager (ACE_TRY_ENV);
         ACE_TRY_CHECK;

         // Activate the POA manager.
         poa_manager_->activate (ACE_TRY_ENV);
         ACE_TRY_CHECK;
      }

      // Create a child poa with USER_ID and PERSISTENT policies.
      CORBA::PolicyList policies;
      policies.length (2);

      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID,
                                                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[1] = this->root_poa_->create_lifespan_policy(PortableServer::PERSISTENT,
                                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;


      this->persistent_poa_ = this->root_poa_->create_POA("TheChildPOA",
                                                           this->poa_manager_.in (),
                                                           policies,
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Cleanup the POA Policies
      policies[0]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[1]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (local_logger_)
      {
         // Now construct the logger implementation, using the settings.
         ACE_NEW_THROW_EX (this->logger_impl_,
                           TAO_Metrics_Logger (this->visualize_,
                           this->log_output_,
                           1,  // 1 == generate_export_file
                           this->log_file_name_),
                           CORBA::NO_MEMORY ());
         ACE_TRY_CHECK;

         // Implicitly activate and store a reference to the logger.
         this->logger_ref_  = this->logger_impl_->_this (ACE_TRY_ENV);
         ACE_TRY_CHECK;

         // Output and log the logger IOR.
         ACE_DEBUG ((LM_DEBUG,
                     "\n\nThe TAO_Metrics logger IOR is:\n\n%s\n\n",
                     this->orb_->object_to_string (this->logger_ref_.in())));
      }
      else
      {
         if (this->logger_ior_file_name_ == 0)
         {
            ACE_ERROR((LM_ERROR, "You must specify a logger ior filename using the -logger_ior command line argument\n%a"));
         }

         u_int ior_str_len =
            ACE_OS::strlen("file:") +
            ACE_OS::strlen(this->logger_ior_file_name_);

         char *ior_str = CORBA::string_alloc (ior_str_len);
         *ior_str = 0;
         ACE_OS::strcat(ior_str, "file://");
         ACE_OS::strcat(ior_str, this->logger_ior_file_name_);

         CORBA::Object_var logger_obj = this->orb_->string_to_object ( ior_str , ACE_TRY_ENV);
         ACE_TRY_CHECK;

         // Turn Off priority selection for this reference.
         //TAO::PrioritySpecification ps;
         //ps.mode = TAO::USE_NO_PRIORITY;
         //CORBA::Any value;
         //value <<= ps;

         //CORBA::PolicyList policy_list (1);
         //policy_list.length (1);
         //policy_list[0] = this->orb_->create_policy (TAO::CLIENT_PRIORITY_POLICY_TYPE,
         //                                               value,
         //                                          ACE_TRY_ENV);
         //ACE_TRY_CHECK;


         // Obtain an object reference without endpoint profiles
         //CORBA::Object_var logger_obj_no_priority_ref = logger_obj->_set_policy_overrides (policy_list,
         //CORBA::SET_OVERRIDE,
         //                                                                                ACE_TRY_ENV);
         //ACE_TRY_CHECK;

         //this->logger_ref_ = Metrics::QoSLogger::_narrow (logger_obj_no_priority_ref.in(), ACE_TRY_ENV);
         this->logger_ref_ = Metrics::QoSLogger::_narrow (logger_obj.in(), ACE_TRY_ENV);
         ACE_TRY_CHECK;

      }

      if (local_monitor_)
      {
         // Now construct the logger implementation, using the settings.
         ACE_NEW_THROW_EX (this->upcall_monitor_impl_,
                           TAO_Metrics_ReportingUpcallMonitor (),
                           CORBA::NO_MEMORY ());
         ACE_TRY_CHECK;

         if (this->monitor_ior_file_name_ != 0)
         {
            FILE *miorf = fopen (this->monitor_ior_file_name_, "w");
            if (miorf != 0)
            {
               ACE_OS::fprintf (miorf,
                  "%s\n",
                  this->orb_->object_to_string (this->upcall_monitor_ref_.in()));
               ACE_OS::fclose (miorf);
            }
         }

         // Implicitly activate and store a reference to the upcall monitor.
         this->upcall_monitor_ref_  = this->upcall_monitor_impl_->_this (ACE_TRY_ENV);
         ACE_TRY_CHECK;
      }

      // Output and log the upcall monitor IOR.
      ACE_DEBUG ((LM_DEBUG,
                  "\n\nThe TAO_Metrics upcall monitor IOR is:\n\n%s\n\n",
                  this->orb_->object_to_string (this->upcall_monitor_ref_.in())));


      // Create all the push consumer upcall adapters.
      TAO_String_Manager string_manager;
      string_manager = (const char *) "H40"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->H40_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, H40_push_consumer_,
                           string_manager, 1, 250000, 125000, RtecScheduler::VERY_HIGH_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "H20"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->H20_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, H20_push_consumer_,
                           string_manager, 2, 500000, 250000, RtecScheduler::VERY_HIGH_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "H10"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->H10_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, H10_push_consumer_,
                           string_manager, 3, 1000000, 500000, RtecScheduler::VERY_HIGH_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "H05"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->H05_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, H05_push_consumer_,
                           string_manager, 4, 2000000, 1000000, RtecScheduler::VERY_HIGH_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "H01"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->H01_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, H01_push_consumer_,
                           string_manager, 5, 10000000, 5000000, RtecScheduler::VERY_HIGH_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "S40"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->S40_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, S40_push_consumer_,
                           string_manager, 6, 250000, 125000, RtecScheduler::VERY_LOW_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "S20"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->S20_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, S20_push_consumer_,
                           string_manager, 7, 500000, 250000, RtecScheduler::VERY_LOW_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "S10"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->S10_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, S10_push_consumer_,
                           string_manager, 8, 1000000, 500000, RtecScheduler::VERY_LOW_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "S05"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->S05_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, S05_push_consumer_,
                           string_manager, 9, 2000000, 1000000, RtecScheduler::VERY_LOW_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      string_manager = (const char *) "S01"; // forces a copy (for lame platforms w/ non-const "")
      ACE_NEW_THROW_EX (this->S01_push_consumer_adapter_,
                        METRICS_UPCALL_ADAPTER_TYPE
                          (*upcall_monitor_impl_, cache_, frame_manager_, S01_push_consumer_,
                           string_manager, 10, 10000000, 5000000, RtecScheduler::VERY_LOW_CRITICALITY, 0, logger_ref_.in()),
                        CORBA::NO_MEMORY ());
      ACE_TRY_CHECK;

      frame_manager_.register_period (10000000);  //  1 Hz
      frame_manager_.register_period ( 2000000);  //  5 Hz
      frame_manager_.register_period ( 1000000);  // 10 Hz
      frame_manager_.register_period (  500000);  // 20 Hz
      frame_manager_.register_period (  250000);  // 40 Hz
      frame_manager_.reset_all_frames ();

      // Register base metrics segments.
      cache_.register_base_metrics ("QUO",
                                    TAO_Metrics_Utils::QUO,
                                    logger_ref_.in());
      cache_.register_base_metrics ("Sched",
                                    TAO_Metrics_Utils::SCHED,
                                    logger_ref_.in());
      cache_.register_base_metrics ("RTARM",
                                    TAO_Metrics_Utils::RTARM,
                                    logger_ref_.in());

      // Register all the probes with the logger.
      cache_.register_probes (logger_ref_.in());

    }

    ACE_CATCHANY
    {
       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics_Local_Cache_Test::init");
       return -1;
    }
    ACE_ENDTRY;

  delete the_orb_args;
  return 0;
}


// Runs the TAO_Metrics_Local_Cache_Test.

int
TAO_Metrics_Local_Cache_Test::run (CORBA_Environment& ACE_TRY_ENV)
{
  int result = 0;
  ACE_Thread_Manager orb_thread_manager;

  ACE_TRY
    {
      /////////////////////////////
      // Initialization sequence //
      /////////////////////////////

      // Spawn a thread that runs the orb event loop
      orb_thread_manager.spawn (TAO_Metrics_Local_Cache_Test::run_orb, this);

      ////////////////////////
      // Operation sequence //
      ////////////////////////

      ACE_Time_Value start_tv, end_tv;
      Metrics::Time interval;
      for (int i = 0; i < 5; ++i)
      {
         char buf[32];
         ACE_OS::sprintf (buf, "mission state %d\n", i);
         logger_ref_->send_banner (buf);
         ACE_TRY_CHECK;

         ACE_DEBUG ((LM_DEBUG, "(%P | %t) %s", buf));

         // Advance to current next frame for each period.
         start_tv = ACE_OS::gettimeofday ();
         frame_manager_.update_all_frames (start_tv);

         ACE_Time_Value start_tv;
         u_int j;
         // Push a bunch of events to the consumer adapters.
         for (j = 0; j < 5; ++j)
         {
            // TBD - update these to do something interesting.
            cache_.report_base_metrics_start (0);
            cache_.report_base_metrics_stop (0);
            cache_.report_enqueue_start (0);
            cache_.report_enqueue_stop (0);
            cache_.report_enqueue_suspend (0);
            cache_.report_enqueue_resume (0);
            cache_.report_dequeue_start (0);
            cache_.report_dequeue_stop (0);
            cache_.report_dequeue_suspend (0);
            cache_.report_dequeue_resume (0);

            H01_push_consumer_adapter_->push (event_set_);
            H05_push_consumer_adapter_->push (event_set_);
            H10_push_consumer_adapter_->push (event_set_);
            H20_push_consumer_adapter_->push (event_set_);
            H40_push_consumer_adapter_->push (event_set_);

            S01_push_consumer_adapter_->push (event_set_);
            S05_push_consumer_adapter_->push (event_set_);
            S10_push_consumer_adapter_->push (event_set_);
            S20_push_consumer_adapter_->push (event_set_);
            S40_push_consumer_adapter_->push (event_set_);
         }

         end_tv = ACE_OS::gettimeofday ();
         end_tv -= start_tv;
         ORBSVCS_Time::Time_Value_to_TimeT (interval, end_tv);

         upcall_monitor_ref_->report_aggregate_QoS (logger_ref_.in(), interval);
         ACE_TRY_CHECK;

         Metrics::QoSParameter_t qos;
         Metrics::QoSParameter_t_var qos_var(&qos);
         Metrics::QoSParameter_t_out qos_out(qos_var);
         Metrics::QoSParameter_Set_var qos_set_var;
         Metrics::Handle_Set_t handle_set (10);
         handle_set.length (10);

         ACE_DEBUG ((LM_DEBUG, "Querying individual handle QoS\n"));

         for (j = 1; j <= 10; ++j)
         {
            handle_set [j-1] = j;
            upcall_monitor_ref_->get_aggregate_QoS (j, qos_out);
            ACE_TRY_CHECK;
            ACE_DEBUG ((LM_DEBUG,
                        "[handle = %u] [entry_point = %s] [made = %u] "
                        "[missed = %u] [cancelled = %u] [is_hrt = %d]\n",
                        qos_var.in().handle,
                        qos_var.in().entry_point.in(),
                        qos_var.in().deadlines_made,
                        qos_var.in().deadlines_missed,
                        qos_var.in().operations_cancelled,
                        qos_var.in().is_hrt));
         }

         ACE_DEBUG ((LM_DEBUG, "Querying handle set QoS\n"));

         upcall_monitor_ref_->get_aggregate_QoS_set (handle_set, qos_set_var);
         ACE_TRY_CHECK;
         for (j = 0; j < qos_set_var.ptr()->length (); ++j)
         {
            ACE_DEBUG ((LM_DEBUG,
                        "[handle = %u] [entry_point = %s] [made = %u] "
                        "[missed = %u] [cancelled = %u] [is_hrt = %d]\n",
                        qos_set_var[j].handle,
                        qos_set_var[j].entry_point.in(),
                        qos_set_var[j].deadlines_made,
                        qos_set_var[j].deadlines_missed,
                        qos_set_var[j].operations_cancelled,
                        qos_set_var[j].is_hrt));
         }

         cache_.flush_ACE_Metrics_Cache ();
         cache_.flush_TAO_Metrics_LocalCache ();
         cache_.output_statistics (logger_ref_.in());
         cache_.flip_supplier_and_consumer ();
         cache_.flush_ACE_Metrics_Cache ();
         cache_.flush_TAO_Metrics_LocalCache ();

         CORBA::ULong frame_id;
         frame_manager_.get_frame_data (10000000, start_tv, end_tv, frame_id);  // get  1 Hz data
         ACE_DEBUG ((LM_DEBUG,
                     "ending 1 Hz data frame: [id = %u] "
                     "[start = %u sec %u usec] [end = %u sec %u usec]\n",
                     frame_id, start_tv.sec (), start_tv.usec (), end_tv.sec (), end_tv.usec ()));

         frame_manager_.get_frame_data ( 2000000, start_tv, end_tv, frame_id);  // get  5 Hz data
         ACE_DEBUG ((LM_DEBUG,
                     "ending 5 Hz data frame: [id = %u] "
                     "[start = %u sec %u usec] [end = %u sec %u usec]\n",
                     frame_id, start_tv.sec (), start_tv.usec (), end_tv.sec (), end_tv.usec ()));

         frame_manager_.get_frame_data ( 1000000, start_tv, end_tv, frame_id);  // get 10 Hz data
         ACE_DEBUG ((LM_DEBUG,
                     "ending 10 Hz data frame: [id = %u] "
                     "[start = %u sec %u usec] [end = %u sec %u usec]\n",
                     frame_id, start_tv.sec (), start_tv.usec (), end_tv.sec (), end_tv.usec ()));

         frame_manager_.get_frame_data (  500000, start_tv, end_tv, frame_id);  // get 20 Hz data
         ACE_DEBUG ((LM_DEBUG,
                     "ending 20 Hz data frame: [id = %u] "
                     "[start = %u sec %u usec] [end = %u sec %u usec]\n",
                     frame_id, start_tv.sec (), start_tv.usec (), end_tv.sec (), end_tv.usec ()));

         frame_manager_.get_frame_data (  250000, start_tv, end_tv, frame_id);  // get 40 Hz data
         ACE_DEBUG ((LM_DEBUG,
                    "ending 40 Hz data frame: [id = %u] "
                    "[start = %u sec %u usec] [end = %u sec %u usec]\n",
                    frame_id, start_tv.sec (), start_tv.usec (), end_tv.sec (), end_tv.usec ()));
      }

      //////////////////////////
      // Termination sequence //
      //////////////////////////

      ACE_DEBUG ((LM_DEBUG, "about to clean up push consumers\n"));

      H40_push_consumer_adapter_->disconnect_push_consumer ();
      H20_push_consumer_adapter_->disconnect_push_consumer ();
      H10_push_consumer_adapter_->disconnect_push_consumer ();
      H05_push_consumer_adapter_->disconnect_push_consumer ();
      H01_push_consumer_adapter_->disconnect_push_consumer ();
      S40_push_consumer_adapter_->disconnect_push_consumer ();
      S20_push_consumer_adapter_->disconnect_push_consumer ();
      S10_push_consumer_adapter_->disconnect_push_consumer ();
      S05_push_consumer_adapter_->disconnect_push_consumer ();
      S01_push_consumer_adapter_->disconnect_push_consumer ();
    }

    ACE_CATCHANY
    {
       ACE_DEBUG ((LM_DEBUG, "caught an exception\n"));
       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics_Local_Cache_Test::run");
       result = -1;
    }
    ACE_ENDTRY;

    ACE_DEBUG ((LM_DEBUG, "about to shut down orb\n"));

    // Shut down orb, then wait for the ORB thread to exit.
    this->orb_->shutdown ();

    ACE_DEBUG ((LM_DEBUG, "about to wait on orb thread\n"));

    orb_thread_manager.wait ();

    ACE_DEBUG ((LM_DEBUG, "all done with run\n"));

    return result;
}


// Parses the command line arguments.

// Run the ORB event loop.
void *
TAO_Metrics_Local_Cache_Test::run_orb (void *that)
{
   ACE_TRY_NEW_ENV
   {
      ACE_static_cast (TAO_Metrics_Local_Cache_Test *, that)->orb_->run ();
      ACE_TRY_CHECK;
   }
   ACE_CATCHANY
   {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics_Local_Cache_Test::run_orb");
   }
   ACE_ENDTRY;

   return 0;
}


void TAO_Metrics_Local_Cache_Test_PushConsumer::
push (const RtecEventComm::EventSet &, CORBA_Environment &)
{
   ACE_Time_Value pause_tv;

   switch (state_variable_)
   {
   case 0:
      pause_tv.set (0,0);  // instantaneous -  all should succeed
      break;
   case 1:
      pause_tv.set (0, 25000); // 40 Hz may miss
      break;
   case 2:
      pause_tv.set (0, 50000); // 20 Hz may miss
      break;
   case 3:
      pause_tv.set (0, 100000); // 10 Hz may miss
      break;
   case 4:
      pause_tv.set (0, 200000); // 5 Hz may miss
      break;
   case 5:
      pause_tv.set (1, 0); // 1 Hz may miss
      break;
   case 6:
   default:
      pause_tv.set (2, 0); // 1/2 Hz - none should succeed
      break;
   }

   state_variable_ = (state_variable_ + 1) % 7;
   ACE_OS::sleep (pause_tv);
}

void TAO_Metrics_Local_Cache_Test_PushConsumer::
disconnect_push_consumer (CORBA_Environment &)
{
   ACE_DEBUG ((LM_DEBUG,
               "TAO_Metrics_Local_Cache_Test_PushConsumer::"
               "disconnect_push_consumer\n"));
}

int
main (int argc, char *argv[])
{
   ACE_TRY_NEW_ENV
   {
      TAO_Metrics_Local_Cache_Test logger_driver;

      ACE_DEBUG ((LM_DEBUG,
                  "%s; initializing TAO_Metrics logger driver\n", __FILE__));

      if (logger_driver.init (argc, argv) < 0)
      {
         ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "TAO_Metrics logger driver init failed."), 1);
      }

      ACE_DEBUG ((LM_DEBUG,
                  "%s; running TAO_Metrics logger driver\n", __FILE__));

      logger_driver.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
   }

   ACE_CATCHANY
   {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Metrics logger test driver exception in main.");
      return 1;
   }
   ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
