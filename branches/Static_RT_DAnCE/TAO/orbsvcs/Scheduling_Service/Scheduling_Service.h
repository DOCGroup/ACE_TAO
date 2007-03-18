//=============================================================================
/**
 *  @file    Scheduling_Service.h
 *
 *  $Id$
 *
 *    This class implements the functionality of a Scheduling Service in
 *    a stand-alone process.
 *
 *
 *  @author   Chris Gill (cdgill@cs.wustl.edu) Based on the original
 *  Scheduling Service program by David Levine (levine@cs.wustl.edu) and
 *  Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author  and on the Naming Service program implementation by Marina   Spivak (marina@cs.wustl.edu) and Nagarajan Surendran   (naga@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_SCHEDULING_SERVICE_H
#define TAO_SCHEDULING_SERVICE_H

#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#if defined (TAO_USES_STRATEGY_SCHEDULER)
#include "orbsvcs/Sched/Strategy_Scheduler.h"
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "ace/SString.h"

/**
 * @class TAO_Scheduling_Service
 *
 * @brief Defines a class that encapsulates the implementation of the
 * TAO Scheduling Service.
 *
 * This class makes use of the TAO_Reconfig_Scheduler and
 * ACE_Config_Scheduler classes to implement the Scheduling Service.
 */
class TAO_Scheduling_Service
{
public:
  /// Default Constructor.
  TAO_Scheduling_Service (void);

  /// Constructor taking the command-line arguments.
  TAO_Scheduling_Service (int argc, ACE_TCHAR* argv[]);

  /// Initialize the Scheduling Service with the arguments.
  int init (int argc, ACE_TCHAR* argv[]);

  /// Run the TAO_Scheduling_Service.
  int run (void);

  /// Destructor.
  ~TAO_Scheduling_Service (void);

private:
  typedef ACE_Config_Scheduler CONFIG_SCHED_TYPE;

  typedef TAO_Reconfig_Scheduler<TAO_MUF_FAIR_Reconfig_Sched_Strategy,
                                 TAO_SYNCH_MUTEX> RECONFIG_SCHED_TYPE;

  enum Scheduler_Type {CONFIG, RECONFIG};

  /// parses the arguments.
  int parse_args (int argc, ACE_TCHAR* argv[]);

  /// The ORB manager.  This must be the first thing in the
  TAO_ORB_Manager orb_manager_;

  /// Scheduler instance.
  POA_RtecScheduler::Scheduler *scheduler_impl_;

  /// Name of the IOR output file.
  ACE_CString ior_file_name_;

  /// Name of the process id output file.
  ACE_CString pid_file_name_;

  /// Name used to register the service.
  ACE_CString service_name_;

  /// Type of scheduler with which to instantiate the service.
  Scheduler_Type scheduler_type_;

  /// A reference to the ORB, to shut it down properly.
  CORBA::ORB_var orb_;
};

#endif /* TAO_SCHEDULING_SERVICE_H */
