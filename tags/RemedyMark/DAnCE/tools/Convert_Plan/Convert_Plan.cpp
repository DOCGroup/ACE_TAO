// $Id$

#include "ace/Dynamic_Service.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "tao/ORB.h"
#include "dance/Deployment/Deployment_DeploymentPlanC.h"
#include "dance/Deployment/Deployment_PlanErrorC.h"
#include "dance/Logger/Log_Macros.h"
#include "dance/Logger/Logger_Service.h"
#include "Convert_Plan_Impl.h"

const ACE_TCHAR *input_filename = 0;
const ACE_TCHAR *output_filename = 0;

void
usage (void)
{
  DANCE_TRACE ("usage");
  ACE_DEBUG ((
    LM_DEBUG, ACE_TEXT ("usage: dance_split_plan [options]\n")
              ACE_TEXT ("Converts a provided plan to CDR.  ")
              ACE_TEXT ("If no output file is nominated, ")
              ACE_TEXT ("the input XML plan will be validated")
              ACE_TEXT ("against schema and the program will exit\n")
              ACE_TEXT ("\t-x <plan>\t\tXML Encoded input plan\n")
              ACE_TEXT ("\t-o <file>\t\tOutput destination for ")
              ACE_TEXT ("converted plan\n")
              ));
}

bool
parse_args (int argc, ACE_TCHAR *argv [])
{
  DANCE_TRACE ("parse_args");
  DANCE_DEBUG (DANCE_LOG_TRACE,
    (LM_TRACE, DLINFO ACE_TEXT ("dance_split_plan options: ")));

  for (int i = 0; i < argc; ++i)
    {
      DANCE_DEBUG (DANCE_LOG_TRACE, (LM_TRACE, ACE_TEXT("\t%s\n"), argv[i]));
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
      DAnCE::Logger_Service * dlf =
        ACE_Dynamic_Service<DAnCE::Logger_Service>::instance ("DAnCE_Logger");

      if (dlf)
        {
          dlf->init (argc, argv);
        }

      DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE, DLINFO
                       ACE_TEXT("Convert_Plan - initializing ORB\n")));

      // Need an ORB for the Config handlers
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        {
          return -1;
        }

      if (!input_filename)
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, DLINFO ACE_TEXT ("Convert_Plan - ")
            ACE_TEXT ("No input filename provided\n")));
          return -1;
        }

      auto_ptr <Deployment::DeploymentPlan> plan (
        DAnCE::Convert_Plan::read_xml_plan (input_filename));

      if (!plan.get ())
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR, (LM_ERROR,
            DLINFO ACE_TEXT ("Convert_Plan - ")
            ACE_TEXT ("Unable to load an XML based plan\n")));
          return -1;
        }

#ifdef GEN_OSTREAM_OPS
  {
    std::ostringstream plan_stream;
    plan_stream << *plan << std::endl;
    DANCE_DEBUG (DANCE_LOG_TRACE, (LM_TRACE,
      DLINFO "Convert_Plan - Input plan: %C\n",
                  plan_stream.str ().c_str ()));
  }
#endif /* GEN_OSTREAM_OPS */

      if (!output_filename)
        {
          DANCE_DEBUG (DANCE_LOG_TERMINAL_ERROR, (LM_DEBUG,
            DLINFO ACE_TEXT ("Convert_Plan - ")
                   ACE_TEXT ("XML Input file validated correctly with no ")
                   ACE_TEXT ("output file specifed, exiting\n")));
          return 0;
        }

      DAnCE::Convert_Plan::write_cdr_plan (output_filename, *plan);
    }
  catch (const ::Deployment::PlanError &ex)
    {
      DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
        (LM_ERROR, ACE_TEXT ("Convert_Plan - ")
        ACE_TEXT ("Config error while parsing plan ")
        ACE_TEXT ("<%C>, <%C>\n"),
        ex.name.in (),
        ex.reason.in ()));
      retval = -1;
    }
  catch (const CORBA::Exception &ex)
    {
      // another one of these. This is probably printing something outside of
      // our logging level framework
      ex._tao_print_exception ("Convert_Plan");
      retval = -1;
    }
  catch (...)
    {
      DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
        (LM_ERROR, ACE_TEXT ("Convert_Plan - ")
        ACE_TEXT ("error: unknown c++ exception\n")));
      retval = -1;
    }

  return retval;
}
