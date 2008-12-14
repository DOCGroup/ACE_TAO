// -*- C++ -*-
// $Id$

#include <vector>
#include <fstream>
#include <algorithm>

#include "ace/Get_Opt.h"
#include "orbsvcs/orbsvcs/LWFT/LWFT_Client_Init.h"

#include "WorkerC.h"
#include "FTClient_Timer_Handler.h"
#include "ace/Sig_Handler.h"
#include "ace/Reactor.h"

std::string ior = "file://decoram_server.ior";
std::string server_id = "server";
double period = 100;
double execution_time = 15;

FTClient_Timer_Handler timeout_handler;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv,
                        "k:e:p:h" //client options
                        );
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        server_id = get_opts.opt_arg ();
        break;
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'e':
        execution_time = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'p':
        period = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case 'h':
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\t-i <server_id> (defaults to %s)\n"
                           "\t-k <ior> (defaults to %s)\n",
                           server_id.c_str (),
                           ior.c_str ()),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      LWFT_Client_Init ci;
      CORBA::ORB_var orb = ci.init (argc, argv);

      int result = parse_args (argc, argv);
      
      if (result != 0)
        {
          return result;
        }
 
      CORBA::Object_var object = orb->string_to_object (ior.c_str ());

      DeCoRAM::Worker_var worker =
        DeCoRAM::Worker::_narrow (object.in ());

      timeout_handler.set_orb (orb.in ());
      timeout_handler.set_worker (worker.in ());

      // add a the handler for the SIGINT signal here                                                              
      ACE_Sig_Handler sig_handler;
      sig_handler.register_handler (SIGINT, &timeout_handler);
      
      // register the timer handler with the ORB reactor
      ACE_Time_Value period_tv;
      period_tv.msec (static_cast<long> (period));

      orb->orb_core ()->reactor ()->schedule_timer (&timeout_handler,
                                                    0,
                                                    ACE_Time_Value::zero,
                                                    period_tv);

      orb->run ();

      orb->orb_core ()->reactor ()->cancel_timer (&timeout_handler);

      timeout_handler.dump_to_file ( + "-history.txt");
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client - exception caught:");
      return -1;
    }

  return 0;
}
