#include "ace/Get_Opt.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"
#include "cuts/BDC/Benchmark_Data_CollectorC.h"
#include "cuts/CUTSC.h"
#include <iostream>
#include <string>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

const char *bdc_ior = "file://BDC.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:p:");
  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'k':
            bdc_ior = get_opts.opt_arg ();
            break;

          case '?':  // display help for use of the server.
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "usage:  %s\n"
                               "-k <Benchmark Data Collector IOR> (default is file://BDC.ior)\n"
                               "\n",
                               argv [0]),
                              -1);
        }
    }
  return 0;
}

static CUTS::Path_Sequence createPathSeq (const char* path)
{
  CORBA::String_var critical_path (path);
  ACE_Tokenizer tok (critical_path);
  CUTS::Path_Element path_element;
  CUTS::Path_Sequence path_seq;
  tok.delimiter_replace (';', 0);
  for (char *p = tok.next (); p; p = tok.next ())
    {
      ACE_Tokenizer temp_tok (p);
      temp_tok.delimiter_replace (',', 0);
      path_element.node = CORBA::string_dup (temp_tok.next ());
      path_element.src = CORBA::string_dup (temp_tok.next ());
      path_element.dst = CORBA::string_dup (temp_tok.next ());

      CORBA::ULong len = path_seq.length();
      path_seq.length (len + 1);
      path_seq [len] = path_element;
    }

  /*for (CORBA::ULong i = 0; i < path_seq.length(); ++i)
    {
      cout << "Node: " << path_seq[i].node << endl
           << "Source Port: " << path_seq[i].src << endl
           << "Destination Port: " << path_seq[i].dst << endl;
    }*/
  return path_seq;
}


int
main (int argc, char *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "");

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj = orb->string_to_object (bdc_ior);

      CUTS::Benchmark_Data_Collector_var
        comp = CUTS::Benchmark_Data_Collector::_narrow (obj.in());

      if (CORBA::is_nil (comp.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire Benchmark_Data_Collector objref\n"),
                            -1);
        }

      CUTS::BDC_Control_Handle_var controls = comp->provide_controls();

      if (CORBA::is_nil (controls.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire controls objref\n"),
                            -1);
        }

      CUTS::Path_Measurement_var
        online_measurements = comp->provide_online_measurements();

      if (CORBA::is_nil (online_measurements.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire online_measurements objref\n"),
                            -1);
        }

      string pathId ("SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.PlannerOne,CommandEvent,CommandEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.SensorMain,CommandEvent,TrackEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.PlannerOne,TrackEvent,SituationEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.PlannerTwo,SituationEvent,AssessmentEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.ConfigOp,AssessmentEvent,CommandEvent;SLICE.CoWorkEr_ComponentImplementations.SLICE.SLICE.EffectorMain,CommandEvent,CommandEvent");
      /*cerr << "Displaying statistics for path: " << pathId << endl;*/

      CUTS::Path_Sequence pathSeq = createPathSeq (pathId.c_str());

	  CORBA::Long
        pathCookie = online_measurements->bind_to_path (pathSeq,
                                                        CUTS::Path_Measurement::BPF_BOTH);

      ACE_Time_Value timeout (5);

      while (1)
        {
          // controls->collect_performance_data ();

          CUTS::Event_Time_Info event_info
            = online_measurements->execution_time (pathCookie);
          CUTS::Time_Info time = event_info.time;
          ACE_DEBUG ((LM_DEBUG, "ID %d: Execution time (min/avg/max): "
                      "(%d ms/%d ms/%d ms)\t"
                      "No. of Events sampled: (%d)\n",
                      pathCookie,
                      time.min,
                      time.total,
                      time.max,
                      event_info.max_events));
          ACE_OS::sleep (timeout);
        }
    }

  catch (const CUTS::ID_Not_Found& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "execution_time");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught CUTS::ID_Not_Found exception\n"),
                        1);
    }

  catch (const CUTS::Operation_Failed& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "execution_time");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught CUTS::Operation_Failed exception\n"),
                        1);
    }
  catch (const CUTS::Binding_Failed& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "bind_to_path");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught CUTS::Binding_Failed exception\n"),
                        1);
    }

  catch (const CORBA::Exception& ex)
    {
      ACE_PRINT_EXCEPTION (ex, "CORBA::Exception\n");
      ACE_ERROR_RETURN ((LM_ERROR, "Caught CORBA::Exception\n"), 1);
    }

  return 0;
}
