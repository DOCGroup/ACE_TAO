// $Id$

#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "tao/ORB.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/Config_Handlers/XML_File_Intf.h"
#include "DAnCE/tools/Convert_Plan/Convert_Plan_Impl.h"
#include "Split_Plan_impl.h"


bool cdr_encoded_ = false;
const char *input_filename = 0;

void
usage (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("usage: dance_split_plan [options]\n")
              ACE_TEXT ("Applies the split plan algorithm and saves the result to")
              ACE_TEXT ("CDR encoded plans\n")
              ACE_TEXT ("\t-c <plan>\t\tCDR Encoded input plan\n")
              ACE_TEXT ("\t-x <plan>\t\tXML Encoded input plan\n")
              ));
}

bool
parse_args (int argc, ACE_TCHAR *argv [])
{
  DANCE_DEBUG (9, (LM_TRACE, DLINFO ACE_TEXT ("dance_split_plan options: ")));
  
  for (int i = 0; i < argc; ++i)
    {
      DANCE_DEBUG (9, (LM_TRACE, ACE_TEXT("\t%s\n"), argv[i]));
    }
  
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("c:x:h"), 0);
  
  int c;
  ACE_CString s;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'c':
          cdr_encoded_ = true;
          input_filename = get_opt.opt_arg ();
          break;
          
        case 'x':
          cdr_encoded_ = false;
          input_filename = get_opt.opt_arg ();
          break;

        case 'h':
          usage ();
          return false;
        default: 
          usage ();
          return false;
        }
    }
  
  return true;
}

int 
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  DANCE_DISABLE_TRACE ();
  
  int retval = 0;

  try
    {
      DAnCE::Logger_Service
        * dlf = ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");
      
      if (dlf)
        {
          dlf->init (argc, argv);
        }
      
      DANCE_DEBUG (6, (LM_TRACE, DLINFO
                       ACE_TEXT("PlanLauncher - initializing ORB\n")));
      
      // Need an ORB for the Config handlers
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      
      if (!parse_args (argc, argv))
        {
          return -1;
        }

      auto_ptr<Deployment::DeploymentPlan> plan;
      
      if (!cdr_encoded_)
        {
          plan.reset (DAnCE::Convert_Plan::read_xml_plan (input_filename));
        }
      else 
        {
          plan.reset (DAnCE::Convert_Plan::read_cdr_plan (input_filename));
        }
      
      if (plan.get () == 0)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO "Split_Plan - "
                           "Unable to convert provided plan into IDL representation\n"));
          return 0;
        }
      
      DAnCE::Split_Plan::TNodePlans plans;
      DAnCE::Split_Plan split (*plan, plans);
      split.split_plan ();
      
      for (DAnCE::Split_Plan::TNodePlans::iterator iter_plans = plans.begin();
           iter_plans != plans.end();
           ++iter_plans)
        {
          ACE_CString name ((*iter_plans).ext_id_);
          DANCE_DEBUG (3, (LM_DEBUG, DLINFO "Split_Plan - "
                           "Writing sub plan for node %C\n",
                           name.c_str ()));

          name += input_filename;
          DAnCE::Convert_Plan::write_cdr_plan (name.c_str (), (*iter_plans).int_id_);
        }
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("PlanLauncher");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "Split_Plan - error: unknown c++ exception\n"));
      retval = -1;
    }
  
  return retval;
}
