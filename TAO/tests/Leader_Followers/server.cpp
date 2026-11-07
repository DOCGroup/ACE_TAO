#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Task.h"
#include "test_i.h"

#include "tao/Strategies/advanced_resource.h"

#include <cstdio>
#include <memory>
#include <string>

struct Server_Options
{
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("ior")};
  int event_loop_threads {1};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options& options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("e:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 'e':
        options.event_loop_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-o <iorfile> "
                                     "-e <number of event loop threads> "
                                     "\n"),
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

class Event_Loop_Task : public ACE_Task_Base
{
public:
  explicit Event_Loop_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
    {
    }

  Event_Loop_Task (Event_Loop_Task const&) = delete;
  Event_Loop_Task (Event_Loop_Task&&) = delete;
  Event_Loop_Task& operator= (Event_Loop_Task const&) = delete;
  Event_Loop_Task& operator= (Event_Loop_Task&&) = delete;

  int svc () override
    {
      try
        {
          this->orb_->run ();
        }
      catch (CORBA::Exception const& ex)
        {
          ex._tao_print_exception ("Exception caught in thread:");
          return -1;
        }


      return 0;
    }

private:
  // ORB reference.
  CORBA::ORB_var const orb_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      test_i servant (orb.in ());

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      test_var const server =
        test::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), std::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            -1);
        std::fprintf (output_file.get (), "%s", ior.in ());
      }

      Event_Loop_Task event_loop_task (orb.in ());

      if (event_loop_task.activate (THR_NEW_LWP | THR_JOINABLE,
                                    options.event_loop_threads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot activate event_loop threads\n")),
                          -1);

      event_loop_task.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Server: Event loop finished\n")));

      root_poa->destroy (true, true);
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
