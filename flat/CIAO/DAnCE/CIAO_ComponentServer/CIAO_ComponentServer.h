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
#include <tao/ORB.h>

namespace CIAO
{
  namespace Deployment
  {  
    class ComponentServer
    {
    public:
      ComponentServer (int argc, ACE_TCHAR **argv);
      
      void run (void);
      
      struct Error
      {
        Error (const ACE_CString &err) : err_(err) {}
        
        ACE_CString err_;
      };
      
    private:
      void parse_args (int argc, ACE_TCHAR **argv);
      
      void get_log_level (int argc, ACE_TCHAR **argv);
      
      void set_log_level (void);
      
      void usage (void);
      
      CORBA::ORB_var orb_;
      int log_level_;
      ACE_CString uuid_;
      ACE_CString callback_ior_str_;
      
    };
  }
}

#endif
