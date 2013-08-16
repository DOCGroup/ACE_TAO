
//=============================================================================
/**
 *  @file    DualEC_Sup.h
 *
 *  $Id$
 *
 * Event supplier for visualization of scheduling behavior, using
 * arrival and dispatch data logged by an EC dispatch command object
 *
 *
 *  @author Chris Gill (cdgill@cs.wustl.edu) Adapted from the orginal DOVE simulation event supplier written by Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#ifndef DUALEC_SUP_H
#define DUALEC_SUP_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecSchedulerS.h"

#include "NavWeapC.h"
#include "NavWeapS.h"
#include "DOVE_Supplier.h"

struct Schedule_Viewer_Data
{
  char operation_name [BUFSIZ];
  double utilitzation;
  double overhead;
  u_long arrival_time;
  u_long deadline_time;
  u_long completion_time;
  u_long computation_time;
};


/**
 * @class DualEC_Supplier
 *
 * @brief Event Supplier.
 *
 * Reads information out of a file and
 * feeds it with additional navigation and
 * weapon data into the dove_supplier, which
 * is connected to the event channel
 */
class DualEC_Supplier
{
public:

  DualEC_Supplier (int argc, ACE_TCHAR** argv);

  ~DualEC_Supplier (void);

  /// Initialize: set up, resolve ORB services,
  /// connect the DOVE_Suppliers.
  int init (void);

  /// Compute schedule priorities, possibly dump
  /// data into runtime scheduler header files.
  void compute_schedules (void);

  /// here is really something going on,
  /// here we deliver the messages
  void start_generating_events (void);

  /// Load the scheduling information into memory
  void load_schedule_data ();


private:

/**
 * @class Terminator
 *
 * Private class that implements a termination servant.
 */
  class Terminator : public POA_NavWeapTerminator
  {
    void shutdown (void);
  };

  /// Run the orb event loop.
  static ACE_THR_FUNC_RETURN run_orb (void *);

  /// Run navigation event generation thread.
  static ACE_THR_FUNC_RETURN run_nav_thread (void *arg);

  /// Run weapons event generation thread.
  static ACE_THR_FUNC_RETURN run_weap_thread (void *arg);

  /// Create two scheduling service instances, registers
  /// them with the Naming Service.
  int create_schedulers (void);

  /// Create two event service instances, registers
  /// them with the Naming Service.
  int create_event_channels (void);

  /// Get command line options.
  unsigned int get_options (int argc, ACE_TCHAR *argv []);

  // Time values to pause before notifying the supplier each time.
  ACE_Time_Value nav_pause_;
  ACE_Time_Value weap_pause_;

  // POA client references.
  PortableServer::POA_var root_POA_var_;
  PortableServer::POAManager_var poa_manager_;

  // Naming Service client reference and names to use.
  CosNaming::NamingContext_var naming_context_;
  CosNaming::Name channel_hi_name_;
  CosNaming::Name channel_lo_name_;
  CosNaming::Name sched_hi_name_;
  CosNaming::Name sched_lo_name_;

  // Scheduling Service servant implementation pointers.
  POA_RtecScheduler::Scheduler * sched_hi_impl_;
  POA_RtecScheduler::Scheduler * sched_lo_impl_;

  // Scheduling Service client references.
  RtecScheduler::Scheduler_var sched_hi_;
  RtecScheduler::Scheduler_var sched_lo_;

  // Handles for the dummy rt_infos registered with the
  // schedulers in order to force priority differentiation.
  RtecScheduler::handle_t sched_hi_rt_info_hi_;
  RtecScheduler::handle_t sched_hi_rt_info_lo_;
  RtecScheduler::handle_t sched_lo_rt_info_hi_;
  RtecScheduler::handle_t sched_lo_rt_info_lo_;

  // Termination servant implementation and client reference
  Terminator terminator_impl_;
  NavWeapTerminator_var terminator_;

  // Event Service servant implementation pointers.
  POA_RtecEventChannelAdmin::EventChannel *ec_hi_impl_;
  POA_RtecEventChannelAdmin::EventChannel *ec_lo_impl_;

  // Event Service client references.
  RtecEventChannelAdmin::EventChannel_var ec_hi_;
  RtecEventChannelAdmin::EventChannel_var ec_lo_;

  DOVE_Supplier weapons_Supplier_;
  DOVE_Supplier navigation_Supplier_;

  // Data for registering RT_Infos
  ACE_Scheduler_Factory::POD_RT_Info rt_info_nav_hi_;
  ACE_Scheduler_Factory::POD_RT_Info rt_info_nav_lo_;
  ACE_Scheduler_Factory::POD_RT_Info rt_info_weap_hi_;
  ACE_Scheduler_Factory::POD_RT_Info rt_info_weap_lo_;
  ACE_Scheduler_Factory::POD_RT_Info rt_info_dummy_hi_;
  ACE_Scheduler_Factory::POD_RT_Info rt_info_dummy_lo_;

  // References for CORBA RtecScheduler data structures
  RtecScheduler::RT_Info_Set_var infos_hi_;
  RtecScheduler::Dependency_Set_var deps_hi_;
  RtecScheduler::Config_Info_Set_var configs_hi_;
  RtecScheduler::Scheduling_Anomaly_Set_var anomalies_hi_;
  RtecScheduler::RT_Info_Set_var infos_lo_;
  RtecScheduler::Dependency_Set_var deps_lo_;
  RtecScheduler::Config_Info_Set_var configs_lo_;
  RtecScheduler::Scheduling_Anomaly_Set_var anomalies_lo_;

  // Queues to store event carried data structures.
  ACE_Unbounded_Queue<Navigation *> navigation_data_;
  ACE_Unbounded_Queue<Weapons *> weapons_data_;

  int argc_;
  ACE_TCHAR **argv_;
  long total_messages_;
  long break_count_;
  ACE_TCHAR *input_file_name_;
  unsigned long update_data_;
  int dump_schedule_headers_;
  int use_runtime_schedulers_;
  int suppress_priority_;
  const ACE_TCHAR * hi_schedule_file_name_;
  const ACE_TCHAR * lo_schedule_file_name_;
  CORBA::Long nav_roll_;
  CORBA::Long nav_pitch_;
};

#endif /* DUALEC_SUP_H */
