// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs/Scheduling_Service/Scheduling_Service
//
// = FILENAME
//    Scheduling_Service.h
//
// = DESCRIPTION
//      This class implements the functionality of a Scheduling Service in
//      a stand-alone process.
//
// = AUTHORS
//      Chris Gill (cdgill@cs.wustl.edu) Based on the original
//      Scheduling Service program by David Levine
//      (levine@cs.wustl.edu) and Carlos O'Ryan (coryan@cs.wustl.edu),
//      and on the Naming Service program implementation by Marina
//      Spivak (marina@cs.wustl.edu) and Nagarajan Surendran
//      (naga@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_SCHEDULING_SERVICE_H
#define TAO_SCHEDULING_SERVICE_H

#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/Sched/Reconfig_Scheduler.h"
#if defined (TAO_USES_STRATEGY_SCHEDULER)
#include "orbsvcs/Sched/Strategy_Scheduler.h"
#endif /* defined (TAO_USES_STRATEGY_SCHEDULER) */
#include "orbsvcs/Sched/Config_Scheduler.h"

class TAO_Scheduling_Service
{
  // = TITLE
  //   Defines a class that encapsulates the implementation of the
  //   TAO Scheduling Service.
  //
  // = DESCRIPTION
  //   This class makes use of the <TAO_Reconfig_Scheduler> and
  //   <ACE_Config_Scheduler> classes to implement the Scheduling Service.
public:
  TAO_Scheduling_Service (void);
  // Default Constructor.

  TAO_Scheduling_Service (int argc, char *argv[]);
  // Constructor taking the command-line arguments.

  int init (int argc, char *argv[]);
  // Initialize the Scheduling Service with the arguments.

  int run (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Run the TAO_Scheduling_Service.

  ~TAO_Scheduling_Service (void);
  // Destructor.

private:
  typedef ACE_Config_Scheduler CONFIG_SCHED_TYPE;

// The templatized method parameters needed by the reconfig scheduler
// class template are hopelessly broken on pre-2.8 versions of g++.
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)

  typedef TAO_Reconfig_Scheduler<TAO_MUF_Reconfig_Sched_Strategy,
                                 TAO_SYNCH_MUTEX> RECONFIG_SCHED_TYPE;

#endif  /* __GNUC__ */

  enum Scheduler_Type {CONFIG, RECONFIG};

  int parse_args (int argc, char *argv[]);
  // parses the arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.  This must be the first thing in the

  POA_RtecScheduler::Scheduler *scheduler_impl_;
  // Scheduler instance.

  const char *ior_file_name_;
  // Name of the IOR output file.

  const char *pid_file_name_;
  // Name of the process id output file.

  const char *service_name_;
  // Name used to register the service.

  Scheduler_Type scheduler_type_;
  // Type of scheduler with which to instantiate the service.

  CORBA::ORB_var orb_;
  // A reference to the ORB, to shut it down properly.
};

#endif /* TAO_SCHEDULING_SERVICE_H */
