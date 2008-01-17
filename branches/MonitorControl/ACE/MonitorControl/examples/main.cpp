
#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

#include "MonitorControl/MonitorControl.h"
#include "MonitorControl/MonitorPointRegistry.h"
#include "MonitorControl/ControlActionRegistry.h"

int main (int argc, char *argv [])
{
  ACE_Service_Config::open (argc,
                            argv,
                            ACE_DEFAULT_LOGGER_KEY,
                            false);

  ACE::MonitorControl::AdminManager *mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");
    
  const ACE::MonitorControl::Admin &admin = mgr->admin ();
  
  ACE::MonitorControl::MonitorPointRegistry *mpreg =
    ACE::MonitorControl::MonitorPointRegistry::instance ();

  ACE::MonitorControl::ControlActionRegistry *careg =
    ACE::MonitorControl::ControlActionRegistry::instance ();
    
  

  return 0;
}