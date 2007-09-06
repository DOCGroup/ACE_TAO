#ifndef HANDLER_H
#define HANDLER_H

#include "NM_MonitorS.h"

namespace CIAO
{
  class DomainDataManager;
}

class Monitor_Handler_I : public POA_Onl_Monitor::AMI_NM_MonitorHandler
{
  public:    

    Monitor_Handler_I (CIAO::DomainDataManager* manager);
    void get_resource_data (const ::Deployment::Domain & ami_return_val);
    
    void get_resource_data_excep (
        ::Messaging::ExceptionHolder * excep_holder);
    
    void monitor_app_QoS (void);
    
    void monitor_app_QoS_excep (
        ::Messaging::ExceptionHolder * excep_holder);

  protected:
    CIAO::DomainDataManager* manager_;
};

#endif
