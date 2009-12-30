// $Id$
/**
 * @file CIAO_ComponentServer_Task.h
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

namespace CIAO
{
  namespace Deployment
  {
    class ComponentServer_Task : ACE_Task_Base
    {
    public:
      ComponentServer_Task (int argc, ACE_TCHAR **argv);

      int svc (void);

      struct Error
      {
        Error (const ACE_CString &err);
        ACE_CString err_;
      };

      void run (void);

    private:
      void parse_args (int argc, ACE_TCHAR **argv);

      void usage (void);

      CORBA::ORB_var orb_;

      ACE_CString uuid_;
      ACE_TString callback_ior_str_;
      ACE_TString output_file_;
    };
  }
}

#endif
