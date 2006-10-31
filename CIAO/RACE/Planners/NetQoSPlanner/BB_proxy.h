// $Id$
#ifndef BB_PROXY_H
#define BB_PROXY_H

#include <list>
#include <string>
#include "BandwidthBroker/BandwidthBrokerC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"


using namespace mil::darpa::arms::mlrm;
using namespace mil::darpa::arms::mlrm::BandwidthBroker;

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_NetQoSPlanner_Impl
    {
      class FlowRequest
      {
        public:
          explicit FlowRequest (AdmissionControl_ptr bb_ref,
                                const AdmissionControl::FlowInfo &f,
                                CommonDef::QOSRequired qos_req);
          ~FlowRequest () throw ();
          AdmissionControl::AdmissionControlResult send_request ();
          int commit ();
          int rollback ();
          long get_dscp () const;
          std::string get_token () const;

        private:
          AdmissionControl_ptr BB_ref_;
          AdmissionControl::FlowInfo_var flowinfo_;
          CommonDef::QOSRequired qos_req_;
          AdmissionControl::SchedulingAction sched_action_;
          CORBA::String_var flowtoken_;
          CORBA::Long dscp_;
          AdmissionControl::AdmissionControlResult adm_ctrl_result_;
      };

      typedef std::list <FlowRequest *> CommandList;
    }
  }
}
      
#include "PlanManager.h"
      
namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_NetQoSPlanner_Impl
    {
      class PlanManager;
      class BB_Proxy
      {
        public:

          BB_Proxy ();
          ~BB_Proxy () throw ();
          bool resolve (CORBA::ORB_ptr orb);
          int flow_request (const AdmissionControl::FlowInfo &f, 
                            CommonDef::QOSRequired qos_req, 
                            long &dscp,
                            PlanManager *plan_man);
          int commit ();
          int rollback ();
          void set_plan_manager (PlanManager *plan_man);
          static void del (const FlowRequest *) throw ();

        protected:
          void populate_name (CosNaming::Name &name);

        private:
          bool resolved_;
          std::string BB_iorfile_;
          std::string BB_nameserv_context_;
          AdmissionControl_var BB_ref_;
          PlanManager *plan_man_;
      };
    } // namespace 
  } // namespace 
} // namespace


#endif // BB_PROXY_H
