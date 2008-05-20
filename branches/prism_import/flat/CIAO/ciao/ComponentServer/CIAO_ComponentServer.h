/**
 * @file CIAO_ComponentServer.h
 * @author William R. Otte
 * 
 * Defines interface for the bootstrap element of the CIAO Component
 * Server.
 */

#ifndef CIAO_COMPONENTSERVER_H_
#define CIAO_COMPONENTSERVER_H_

#include <ace/String_Base.h>
#include <ace/Task.h>
#include <tao/ORB.h>
#include <ciao/Logger/Logger_Service.h>

namespace CIAO
{
  namespace Deployment
  {  
    class ComponentServer_Configurator;
    

    class ComponentServer_Task : ACE_Task_Base
    {
    public:
      ComponentServer_Task (int argc, ACE_TCHAR **argv);
      
      int svc (void);
      
      struct Error
      {
        Error (const ACE_CString &err) : err_(err) {}
        
        ACE_CString err_;
      };
      
      void run (void);
      
    private:
      void parse_args (int argc, ACE_TCHAR **argv);
      
      void get_log_level (int argc, ACE_TCHAR **argv);
      
      void set_log_level (void);
      
      void configure_logging_backend (void);
      
      void usage (void);
      
      const char * sched_policy_name (int sched_policy);
      
      void check_supported_priorities (void);
      
      
      CORBA::ORB_var orb_;
      
      auto_ptr<CIAO::Logger_Service> logger_;
      
      ACE_CString uuid_;
      ACE_CString callback_ior_str_;
      ACE_CString output_file_;
      auto_ptr<ComponentServer_Configurator> configurator_;
      
    };
  }
}

#endif
