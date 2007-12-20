#ifndef TM_DAEMON_H
#define TM_DAEMON_H

#include "TM_DaemonS.h"
#include "DomainDataManager.h"
#include "Handler_i.h"
#include <vector>
#include <string>

using namespace std;

namespace CIAO
{
  namespace TM_Daemon
  {
    class Daemon_impl: public POA_CIAO::TM_Daemon::Daemon
    {

    public:
      Daemon_impl (CORBA::ORB_ptr orb,
//                   const char *deployment_plan,
		   vector<string> deployment_plans,
                   const char *mapping,
                   const char *domain,
                   ::PortableServer::POA_ptr poa,
                   size_t threads = 2);

      virtual ~Daemon_impl ();

      virtual ::Deployment::Domain * getInitialDomain ();

      virtual ::Deployment::Domain * getSnapshot ();

      virtual void shutdown ();

      virtual int run ();


    private:

      CORBA::ORB_var orb_;

      DomainDataManager* manager_;

      size_t thread_count_;

      Onl_Monitor::AMI_NM_MonitorHandler_var handler_;

    };
  }
}

#endif
