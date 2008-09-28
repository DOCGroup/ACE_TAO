/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  @author Matt Murphy <murphym@cs.uri.edu>
 */
//=============================================================================

#include "Object1_i.h"
#include <orbsvcs/RTCosScheduling/RTCosScheduling_ServerScheduler_i.h>

#include "tao/Version.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("server.ior");
char *node_        = 0;
char *file_        = 0;
char *shared_file_ = 0;
char *object_      = 0;
int num_threads_   = 5;
int use_realtime_  = 1;

// ORB Thread

class ORB_Thread : public ACE_Task<ACE_SYNCH>
{
public:

ORB_Thread(CORBA::ORB_var orb) : orb_(orb)
{
}

int
svc(void)
{

  try
    {
      orb_->run();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("svc");
      return 1;
    }

  ACE_OS::exit(0);
  return 0;
}


private:
  CORBA::ORB_var orb_;
};


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  // We set the '-' flag for getopts because we have to do
  //  this parsing before the ORB does it's
  //parsing, and we'd confuse it to reorder
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("-:N:F:S:T:A:X:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'N':
            node_ = get_opts.opt_arg ();
            break;
          case 'F':
            file_ = get_opts.opt_arg ();
            break;
          case 'S':
            shared_file_ =  get_opts.opt_arg ();
            break;
          case 'T':
            num_threads_ = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case 'A':
            object_ = ACE_OS::strdup(get_opts.opt_arg ());
            break;
          case 'X':
            use_realtime_ = ACE_OS::atoi(get_opts.opt_arg ());
            break;

        }
     }
  if (node_ == 0)
    node_ = ACE_OS::strdup("1");
  if (file_ == 0)
    file_ = ACE_OS::strdup("schedule.cfg");
  if (shared_file_ == 0)
    shared_file_ = ACE_OS::strdup("Scheduling_Service_Shared_Memory");
  if (object_ == 0)
    object_ = ACE_OS::strdup("Server2");

  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  try
    {
      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "testORB");

      /// get the root poa
      CORBA::Object_var object= orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var rootPOA =
        PortableServer::POA::_narrow (object.in ());

      /// Create a manager for the POA
      PortableServer::POAManager_var poa_manager = rootPOA->the_POAManager ();

      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);

      poa_policy_list[0] =
        rootPOA->create_lifespan_policy(PortableServer::TRANSIENT);

      // Create the RT Scheduling service
      TAO::RTCosScheduling_ServerScheduler_i *server_sched = 0;
      ACE_NEW_THROW_EX(server_sched,
                       TAO::RTCosScheduling_ServerScheduler_i(
                         node_,
                         file_,
                         shared_file_,
                         num_threads_ ),
                       CORBA::NO_MEMORY());

      /// Create the POA so RT Policies are set
      PortableServer::POA_var RTPOA =
        server_sched->create_POA(rootPOA.in(),
                                 "my_RT_POA",
                                 poa_manager.in(),
                                 poa_policy_list);


      Object1_impl * servant = 0;
      ACE_NEW_THROW_EX(servant,
                       Object1_impl(),
                       CORBA::NO_MEMORY());

      PortableServer::ObjectId_var id =
        RTPOA->activate_object(servant);

      CORBA::Object_var testObject =
        RTPOA->id_to_reference(id.in());

      CORBA::String_var testObject_IORString =
        orb->object_to_string (testObject.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", testObject_IORString.in ());
      ACE_OS::fclose (output_file);

      if (use_realtime_)
        {
          /// Schedule the object
          try
            {
              server_sched->schedule_object(testObject.inout(),
                                            object_);
            }
          catch (const RTCosScheduling::UnknownName& ex)
            {
              ex._tao_print_exception (
                "Unknown object passed to schedule_object\n");
            }
        }

      // Activate the manager and run the event loop
      poa_manager->activate ();

      // Need to set the main thread pthread scope and pthread policy to
      // the values that are specified in svc.conf.  This change was
      // recommended by irfan@oomworks.com
      long flags = THR_NEW_LWP | THR_JOINABLE |

      orb->orb_core ()->orb_params ()->thread_creation_flags ();

      ORB_Thread ot(orb);
      ot.activate(flags);
      ot.wait();

      ACE_DEBUG((LM_DEBUG,
                 "Server is shutting down\n\n"));

      delete server_sched;

      /// clean up
      rootPOA->destroy (1, 1);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}
