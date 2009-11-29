// $Id$
/**
 * @file CIAO_RTComponentServer_Task.h
 * @author William R. Otte
 *
 * Defines interface for the bootstrap element of the CIAO Component
 * Server.
 */

#ifndef CIAO_COMPONENTSERVER_TASK_H_
#define CIAO_COMPONENTSERVER_TASK_H_

#include "ace/String_Base.h"
#include "ace/Task.h"
#include "tao/ORB.h"
#include "ciao/Logger/Logger_Service.h"
#include "tao/RTPortableServer/RTPortableServer.h"

namespace CIAO
{
  namespace Deployment
  {
    class RTComponentServer_Task : ACE_Task_Base
    {
    public:
      RTComponentServer_Task (int argc, ACE_TCHAR **argv);

      int svc (void);

      struct Error
      {
        Error (const ACE_CString &err);
        ACE_CString err_;
      };

      void run (void);

    private:
      void parse_args (int argc, ACE_TCHAR **argv);

      void configure_logging_backend (void);

      void usage (void);

      CORBA::ORB_var orb_;

      auto_ptr<CIAO::Logger_Service> logger_;

      ACE_CString uuid_;
      ACE_TString callback_ior_str_;
      ACE_TString output_file_;

      CORBA::ULong static_threads_;
      CORBA::ULong dynamic_threads_;
    };
  }
}

#endif
