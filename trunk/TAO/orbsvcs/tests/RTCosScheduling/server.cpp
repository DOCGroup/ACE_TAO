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
#include "ace/streams.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"


char *node_        = NULL;
char *file_        = NULL;
char *shared_file_ = NULL;
char *object_      = NULL;
int num_threads_   = 5;
int use_realtime_    = 1;

// ORB Thread

class ORB_Thread : public ACE_Task<ACE_SYNCH>
{
public:

ORB_Thread(CORBA::ORB_var orb) : orb_(orb)
{
}

int
svc(void) {
    orb_->run();
    ACE_TRY_CHECK;

    exit(0);
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
  if (node_ == NULL)
    node_ = ACE_OS::strdup("1");
  if (file_ == NULL)
    file_ = ACE_OS::strdup("schedule.cfg");
  if (shared_file_ == NULL)
    shared_file_ = ACE_OS::strdup("Scheduling_Service_Shared_Memory");
  if (object_ == NULL)
    object_ = ACE_OS::strdup("Server2");

  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  ACE_TRY_NEW_ENV
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
        rootPOA->the_POAManager (ACE_ENV_ARG_PARAMETER);
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
                         num_threads_),
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


      ofstream server_IOR_file ("server.ior");
      server_IOR_file << testObject_IORString.in () << endl;


      if (use_realtime_)
        {
          /// Schedule the object
          ACE_TRY_NEW_ENV
            {
              server_sched->schedule_object(testObject.inout(),
                                            object_);
            }
          ACE_CATCH(RTCosScheduling::UnknownName, ex)
            {
              ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION,
                                 "Unknown object passed to schedule_object\n");
            }
          ACE_ENDTRY
          ACE_TRY_CHECK;
        }


      /// Activate the manager and run the event loop
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
