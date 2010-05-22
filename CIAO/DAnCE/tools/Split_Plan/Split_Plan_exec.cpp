// $Id$

#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
//#include "tao/ORB.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "DAnCE/Config_Handlers/XML_File_Intf.h"
#include "DAnCE/tools/Convert_Plan/Convert_Plan_Impl.h"
#include "Split_Plan.h"
#include "Node_Splitter.h"
#include "Locality_Splitter.h"

bool cdr_encoded_ = false;
const char *input_filename = 0;
char split_type = 'N';

void
usage (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("usage: dance_split_plan [options]\n")
              ACE_TEXT ("Applies the split plan algorithm and saves the result to")
              ACE_TEXT ("CDR encoded plans\n")
              ACE_TEXT ("\t-c <plan>\t\tCDR Encoded input plan\n")
              ACE_TEXT ("\t-x <plan>\t\tXML Encoded input plan\n")
              ACE_TEXT ("\t-t <split type>\t\tType of split to perform; N=node (default), L=locality\n")
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
  
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("c:x:t:h"), 0);
  
  int c;
  ACE_CString s;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'c':
          cdr_encoded_ = true;
          input_filename = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;
          
        case 'x':
          cdr_encoded_ = false;
          input_filename = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
          break;

        case 't':
          split_type = *get_opt.opt_arg ();
          if (split_type != 'N' && split_type != 'L')
            {
              DANCE_ERROR (1, (LM_ERROR, ACE_TEXT("Invalid split type specified.\n")));
              split_type = 'N';
            }
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
          plan.reset (DAnCE::Convert_Plan::read_xml_plan (ACE_TEXT_CHAR_TO_TCHAR (input_filename)));
        }
      else 
        {
          plan.reset (DAnCE::Convert_Plan::read_cdr_plan (ACE_TEXT_CHAR_TO_TCHAR (input_filename)));
        }
      
      if (plan.get () == 0)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO "Split_Plan - "
                           "Unable to convert provided plan into IDL representation\n"));
          return 0;
        }

      if (split_type == 'N')
        {
          DAnCE::Split_Plan<DAnCE::Node_Splitter> split;
          split.split_plan (*plan);

          for (DAnCE::Split_Plan<DAnCE::Node_Splitter>::TSubPlanIterator iter_plans = split.plans ().begin();
              iter_plans != split.plans ().end();
              ++iter_plans)
            {
              ACE_CString label ((*iter_plans).int_id_.label.in ());
              DANCE_DEBUG (3, (LM_DEBUG, DLINFO "Split_Plan - "
                              "Writing sub plan : %C\n",
                              label.c_str ()));

              ACE_CString name ((*iter_plans).ext_id_);
              name += "-";
              name += input_filename;
              DAnCE::Convert_Plan::write_cdr_plan (ACE_TEXT_CHAR_TO_TCHAR (name.c_str ()), 
                                                   (*iter_plans).int_id_);
            }
        }
      else
        {
          DAnCE::Split_Plan<DAnCE::Locality_Splitter> split;
          split.split_plan (*plan);

          for (DAnCE::Split_Plan<DAnCE::Locality_Splitter>::TSubPlanIterator iter_plans = split.plans ().begin();
              iter_plans != split.plans ().end();
              ++iter_plans)
            {
              ACE_CString label ((*iter_plans).int_id_.label.in ());
              DANCE_DEBUG (3, (LM_DEBUG, DLINFO "Split_Plan - "
                              "Writing sub plan : %C\n",
                              label.c_str ()));

              ACE_CString name ((*iter_plans).ext_id_);
              name += "-";
              name += input_filename;
              DAnCE::Convert_Plan::write_cdr_plan (ACE_TEXT_CHAR_TO_TCHAR (name.c_str ()),
                                                   (*iter_plans).int_id_);
            }
        }
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Split_Plan");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "Split_Plan - error: unknown c++ exception\n"));
      retval = -1;
    }
  
  return retval;
}
