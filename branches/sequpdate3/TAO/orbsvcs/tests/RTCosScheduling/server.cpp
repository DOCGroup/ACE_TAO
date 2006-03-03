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

const char *ior_output_file = "server.ior";
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
svc(void) {
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "svc");
      return 1;
    }
  ACE_ENDTRY;

  exit(0);
  return 0;
}


private:
  CORBA::ORB_var orb_;
};


int
parse_args (int argc, char *argv[])
{
  // We set the '-' flag for getopts because we have to do
  //  this parsing before the ORB does it's
  //parsing, and we'd confuse it to reorder
  ACE_Get_Opt get_opts (argc, argv, "-:N:F:S:T:A:X:");
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      CORBA::ORB_var orb=
        CORBA::ORB_init (argc,
                         argv,
                         "testORB"
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// get the root poa
      CORBA::Object_var object=
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var rootPOA=
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      /// Create a manager for the POA
      PortableServer::POAManager_var poa_manager =
        rootPOA->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                                 poa_policy_list
                                 ACE_ENV_ARG_PARAMETER);


      Object1_impl * servant = 0;
      ACE_NEW_THROW_EX(servant,
                       Object1_impl(),
                       CORBA::NO_MEMORY());
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        RTPOA->activate_object(servant
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var testObject =
        RTPOA->id_to_reference(id.in()
                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      CORBA::String_var testObject_IORString =
        orb->object_to_string (testObject.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


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
          ACE_TRY_EX(INNER)
            {
              server_sched->schedule_object(testObject.inout(),
                                            object_ ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK_EX(INNER);
            }
          ACE_CATCH(RTCosScheduling::UnknownName, ex)
            {
              ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION,
                                 "Unknown object passed to schedule_object\n");
            }
          ACE_ENDTRY;
        }

      // Activate the manager and run the event loop
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);

      // Need to set the main thread pthread scope and pthread policy to
      // the values that are specified in svc.conf.  This change was
      // recommended by irfan@oomworks.com
      long flags = THR_NEW_LWP | THR_JOINABLE |

#if TAO_MAJOR_VERSION > 1 \
  || (TAO_MAJOR_VERSION==1 && TAO_MINOR_VERSION > 3 ) \
  || (TAO_MAJOR_VERSION==1 && TAO_MAJOR_VERSION==3 && TAO_MINOR_VERSION > 1)

      orb->orb_core ()->orb_params ()->thread_creation_flags ();

#else /* TAO version is 1.3.1 or lower */
      orb->orb_core ()->orb_params ()->scope_policy () |
      orb->orb_core ()->orb_params ()->sched_policy ();
#endif
      ORB_Thread ot(orb);
      ot.activate(flags);
      ot.wait();


      ACE_DEBUG((LM_DEBUG,
                 "Server is shutting down\n\n"));


      delete server_sched;

      /// clean up
      rootPOA->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                            "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
