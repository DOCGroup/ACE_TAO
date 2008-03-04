/**
 * $Id$
 * @file Plan_Launcher_Impl.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * Contains the Plan_Launcher_i class, which can be used by
 * applications to launch component assemblies.
 */
#ifndef PLAN_LAUNCHER_BASE_IMPL_H
#define PLAN_LAUNCHER_BASE_IMPL_H

#include "Plan_Launcher_Base_Impl_Export.h"
#include "ciao/CIAO_common.h"
#include "DAnCE/Interfaces/ExecutionManagerDaemonC.h"
#include "DAnCE/Deployment/Deployment_ConnectionC.h"
#include "ace/Map_Manager.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"
#include "ace/Vector_T.h"

namespace DAnCE
{
namespace Plan_Launcher
{

/**
 * @class Plan_Launcher_Impl
 * @brief This class launches and manages deployment plans.
 */
class Plan_Launcher_Base_Impl_Export Plan_Launcher_Base_Impl
{
public:
  class Deployment_Failure
  {
public:
    Deployment_Failure(const ACE_CString &error) :
      error_(error)
    {
    }
    ;
    ACE_CString error_;
  };

  Plan_Launcher_Base_Impl(CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[]);

  virtual ~Plan_Launcher_Base_Impl();

  void execute();

  static Deployment::DeploymentPlan* read_cdr_plan_file(
      const char *deployment_plan_uri)
  ACE_THROW_SPEC((Deployment_Failure));
  static void write_cdr_plan_file(const char * filename,
      const Deployment::DeploymentPlan & plan)
  ACE_THROW_SPEC((Deployment_Failure));

protected:

  /**
   * @brief Launch a plan, given an IDL deployment plan
   * @param plan A valid IDL deployment plan
   * @returns a string containing the UUID of the plan. Null indicates failure.
   */
  virtual const char * launch_plan(const ::Deployment::DeploymentPlan &plan);

  /// Returns the DAM associated with a given plan URI
  ::Deployment::DomainApplicationManager_ptr get_dam(const char *uuid);

  /// Tears down a plan given the UUID
  bool teardown_plan(const char *uuid);

  void teardown_plan(::Deployment::DomainApplicationManager_ptr dam);

  void destroy_dam(::Deployment::DomainApplicationManager_ptr dam);

  virtual void stop_plan();
  void usage(const ACE_TCHAR* program = 0);
  void parse_args(int argc, ACE_TCHAR *argv[]);

  static bool is_empty(const ACE_CString & s)
  {
    return 0 == s.length();
  }
  ;
  void write_dam_ior(::Deployment::DomainApplicationManager_ptr dam);

  void create_external_connections(const ::Deployment::DeploymentPlan &plan,
      Deployment::Connections& conn);

  virtual void check_mode_consistentness();
  static ACE_CString expand_env_vars(const char * src);

  /// Cached ORB pointer
  CORBA::ORB_var orb_;
  /// Object reference of the ExecutionManager
  ::DAnCE::ExecutionManagerDaemon_var em_;

  ACE_Vector<ACE_CString> cdr_plan_urls_;
  ACE_CString plan_uuid_;
  ACE_CString em_ior_;
  ACE_CString dam_ior_;
  ACE_Vector<ACE_CString> xml_plan_urls_;
  ACE_CString cdr_dest_url_;

  unsigned long mode_;
  static const unsigned long MODE_START_PLAN = 0x0001;
  static const unsigned long MODE_STOP_PLAN = 0x0002;
  static const unsigned long MODE_USE_NAMING = 0x0004;
  static const unsigned long MODE_WRITE_CDR = 0x0008;

}; // class Plan_Launcher_Impl

} // Plan_Launcher
} // DAnCE


#endif /* PLAN_LAUNCHER_BASE_IMPL_H */
