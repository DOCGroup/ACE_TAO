#include "Payload_Receiver.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"
#include <memory>

namespace
{
  // defaults only
  ACE_TCHAR const *ior_output_file = ACE_TEXT("test.ior");
  int expected {200};
  int maybe_lost {400};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:l:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'e':
        expected = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'l':
        maybe_lost = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> [-e <expected>] [-l <maybe_lost>]"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "(%P) Starting server\n"));

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%P) Server: Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      Payload_Receiver *payload_receiver_impl {};
      ACE_NEW_RETURN (payload_receiver_impl,
                      Payload_Receiver (),
                      1);
      PortableServer::ServantBase_var
        receiver_owner_transfer (payload_receiver_impl);

      PortableServer::ObjectId_var const id =
        root_poa->activate_object (payload_receiver_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Payload_Receiver_var const payload_receiver =
        Test::Payload_Receiver::_narrow (object.in ());

      CORBA::String_var const ior =
        orb->object_to_string (payload_receiver.in ());

      // Write the IOR before activating the POA manager.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P) Server: Cannot open output file for writing IOR: %s"),
                             ior_output_file),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG,
                 "(%P) Server waiting for %d (+%d possible) messages...\n",
                 expected, maybe_lost));

      ACE_Time_Value const start_time = ACE_OS::gettimeofday ();
      ACE_Time_Value end_time = start_time + ACE_Time_Value (27);
      ACE_Time_Value runtime {};
      int count {};
      int maybe_lost_count {};
      bool stalled {};
      while (payload_receiver_impl->count () < expected ||
             payload_receiver_impl->count (true) < maybe_lost)
      {
        int const prev_count = count;
        int const prev_maybe_lost_count = maybe_lost_count;
        ACE_Time_Value tv (0, 100 * 1000);
        orb->run (tv);
        count = payload_receiver_impl->count ();
        maybe_lost_count = payload_receiver_impl->count (true);
        if ((count == prev_count) ||
            (maybe_lost_count == prev_maybe_lost_count))
          {
            if (!stalled)
              {
                stalled = true;
                end_time += ACE_Time_Value (3);
                runtime = ACE_OS::gettimeofday () - start_time;
              }
            else if (ACE_OS::gettimeofday () > end_time)
              {
                if (count < expected)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                               "(%P) Server: The clients stalled out after %d messages\n",
                               count));
                  }
                break; // Abort waiting for more messages
              }
          }
        else
          {
            stalled = false;
          }
      }

      if (!stalled)
        {
          runtime = ACE_OS::gettimeofday () - start_time;
        }

      int result {};

      if (count != expected)
        {
          ACE_ERROR ((LM_ERROR,
                     "(%P) Server did not receive all of the SYNC_WITH_TARGET messages\n"));
          result = 1;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P) Server got %d of %d SYNC_WITH_TARGET messages\n"
                  "        and %d of %d SYNC_WITH_TRANSPORT or SYNC_NONE messages\n"
                  "        in %d.%06d sec\n",
                  count, expected,
                  maybe_lost_count, maybe_lost,
                  runtime.sec(), runtime.usec()));

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "(%P) Ending server (result %d)\n", result));
      return result;
    }
  catch (CORBA::Exception const &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) Server terminated by: "));
      ex._tao_print_exception ("CORBA Exception:");
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) Server terminated by unknown exception\n"));
    }

  return 1;
}
