// $Id$

#ifndef MONITORMANAGER_H
#define MONITORMANAGER_H

#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Task.h"
#include "ace/ARGV.h"
#include "ace/Atomic_Op.h"
#include "ace/Barrier.h"

#include "tao/ORB.h"

#include "orbsvcs/Notify/MonitorControl/notify_mc_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export TAO_MonitorManager : public ACE_Service_Object
{
public:
  /// Perform member initialization
  TAO_MonitorManager (void);

  virtual int init (int argc, ACE_TCHAR* argv[]);
  virtual int fini (void);

  /// Start the ORB task which includes initializtion of the ORB,
  /// creation of the monitor servant and activation.
  /// If run is called more than once, it will silently perform a no-op.
  int run (void);

  /// This will call the run() method.
  virtual int resume (void);

  /// Get the dynamic instance of the TAO_MonitorManager and
  /// shut it down
  static void shutdown (void);

private:
  class ORBTask: public ACE_Task_Base
  {
  public:
    ORBTask (void);
    virtual int svc (void);

    TAO_SYNCH_MUTEX mutex_;
    ACE_ARGV_T<ACE_TCHAR> argv_;
    CORBA::ORB_var orb_;
    ACE_TString ior_output_;
    bool use_name_svc_;
    ACE_Barrier startup_barrier_;
    ACE_TString mc_orb_name_;
  };

  bool run_;
  ORBTask task_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_NOTIFY_MONITOR_CONTROL_MANAGER "TAO_MonitorAndControl"

ACE_STATIC_SVC_DECLARE_EXPORT(TAO_Notify_MC, TAO_MonitorAndControl)
ACE_FACTORY_DECLARE(TAO_Notify_MC, TAO_MonitorAndControl)

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* MONITORMANAGER_H */
