// $Id$

#include "Payload_Receiver.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"

namespace
{
  // defaults only
  const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
  int expected   = 200;
  int maybe_lost = 400;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:e:l:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'e':
        expected = ACE_OS::atoi(get_opts.opt_arg());
        break;
      case 'l':
        maybe_lost = ACE_OS::atoi(get_opts.opt_arg());
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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG, "(%P) Starting server\n"));

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%P) Server: Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      Payload_Receiver *payload_receiver_impl;
      ACE_NEW_RETURN (payload_receiver_impl,
                      Payload_Receiver (),
                      1);
      PortableServer::ServantBase_var
        receiver_owner_transfer (payload_receiver_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (payload_receiver_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Payload_Receiver_var payload_receiver =
        Test::Payload_Receiver::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (payload_receiver.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%P) Server: Cannot open output file for writing IOR: %s",
                            ior_output_file),
                            1);
        }
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG,
                 "(%P) Server waiting for %d (+%d possible) messages...\n",
                 expected, maybe_lost));

      const ACE_Time_Value start_time = ACE_OS::gettimeofday ();
      ACE_Time_Value end_time = start_time + ACE_Time_Value (27);
      ACE_Time_Value runtime;
      int count = 0;
      int maybe_lost_count = 0;
      bool stalled = false;
      while (payload_receiver_impl->count () < expected ||
             payload_receiver_impl->count (true) < maybe_lost)
      {
        int prev_count = count;
        int prev_maybe_lost_count = maybe_lost_count;
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

      int result = 0;

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

      root_poa->destroy (1, 1);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "(%P) Ending server (result %d)\n", result));
      return result;
    }
  catch (const CORBA::Exception& ex)
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
