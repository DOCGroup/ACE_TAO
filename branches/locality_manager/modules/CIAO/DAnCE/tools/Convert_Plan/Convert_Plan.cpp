// $Id$

#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "tao/ORB.h"
#include "DAnCE/Deployment/Deployment_DeploymentPlanC.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "DAnCE/Logger/Logger_Service.h"
#include "Convert_Plan_Impl.h"


const ACE_TCHAR *input_filename = 0;
const ACE_TCHAR *output_filename = 0;
void
usage (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("usage: dance_split_plan [options]\n")
              ACE_TEXT ("Converts a provided plan to CDR.  If no output file is nominated, ")
              ACE_TEXT ("the input XML plan will be validated against schema and the program will exit\n")
              ACE_TEXT ("\t-x <plan>\t\tXML Encoded input plan\n")
              ACE_TEXT ("\t-o <file>\t\tOutput destination for converted plan\n")
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
  
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("x:o:h"), 0);
  
  int c;
  ACE_CString s;
  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
        case 'x':
          input_filename = get_opt.opt_arg ();
          break;
          
        case 'o':
          output_filename = get_opt.opt_arg ();
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

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

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
                       ACE_TEXT("Convert_Plan - initializing ORB\n")));
      
      // Need an ORB for the Config handlers
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      
      if (!parse_args (argc, argv))
        {
          return -1;
        }
      
      if (!input_filename)
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan - " 
                           "No input filename provided\n"));
          return -1;
        }

      auto_ptr <Deployment::DeploymentPlan> plan (DAnCE::Convert_Plan::read_xml_plan (input_filename));
      
      if (!plan.get ())
        {
          DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan - "
                           "Unable to load an XML based plan\n"));
          return -1;
        }

#ifdef GEN_OSTREAM_OPS
  {
    std::ostringstream plan_stream;
    plan_stream << *plan << std::endl;
    DANCE_DEBUG (9, (LM_TRACE, DLINFO "Convert_Plan - Input plan: %C\n",
                  plan_stream.str ().c_str ()));
  }
#endif /* GEN_OSTREAM_OPS */
  
      if (!output_filename)
        {
          DANCE_DEBUG (2, (LM_DEBUG, DLINFO "Convert_Plan - "
                           "XML Input file validated correctly with no output file specifed, exiting\n"));
          return 0;
        }

      DAnCE::Convert_Plan::write_cdr_plan (output_filename, *plan);
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Convert_Plan");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (1, (LM_ERROR, "Convert_Plan - error: unknown c++ exception\n"));
      retval = -1;
    }
  
  return retval;
}
