// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  void test_method (CORBA::Short priority
                    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Short priority
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Use RTCurrent to find out the CORBA priority of the current
  // thread.

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (obj.in ()))
    ACE_THROW (CORBA::INTERNAL ());

  CORBA::Short servant_thread_priority =
    current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Print out the info.
  if (servant_thread_priority != priority)
    ACE_DEBUG ((LM_DEBUG,
                "ERROR: servant thread priority is not equal"
                "to method argument.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Server_Declared priority: %d  "
              "Servant thread priority: %d\n",
              priority, servant_thread_priority));
}

void
Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

//*************************************************************************

const char *ior_output_file1 = "test1.ior";
const char *ior_output_file2 = "test2.ior";
CORBA::Short poa_priority = -1;
CORBA::Short object_priority = -1;

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:o:a:b:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        ior_output_file1 = get_opts.opt_arg ();
        break;

      case 'o':
        ior_output_file2 = get_opts.opt_arg ();
        break;

      case 'a':
        result = ::sscanf (get_opts.opt_arg (),
                           "%hd",
                           &poa_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-a> option"),
                            -1);
        break;

      case 'b':
        result = ::sscanf (get_opts.opt_arg (),
                           "%hd",
                           &object_priority);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p <iorfile1> "
                           "-o <iorfile2> "
                           "-a <poa_priority> "
                           "-b <object_priority> "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (poa_priority < 0
      || object_priority < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Valid poa and object priorities must be"
                       " specified.\nSee README file for more info\n"),
                      -1);

  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
    return 0;
}

int
create_object (RTPortableServer::POA_ptr poa,
               CORBA::ORB_ptr orb,
               Test_i *server_impl,
               CORBA::Short priority,
               const char *filename
               ACE_ENV_ARG_DECL)
{
  // Register with poa.
  PortableServer::ObjectId_var id;

  if (priority > -1)
    id = poa->activate_object_with_priority (server_impl,
                                             priority
                                             ACE_ENV_ARG_PARAMETER);
  else
    id = poa->activate_object (server_impl ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (-1);

  CORBA::Object_var server =
    poa->id_to_reference (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Print out the IOR.
  CORBA::String_var ior =
    orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG, "<%s>\n\n", ior.in ()));

  // Print ior to the file.
  if (filename != 0)
    {
      FILE *output_file= ACE_OS::fopen (filename, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           filename),
                          -1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);
    }

  return 0;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      // RTORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      // RootPOA.
      object =
        this->orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create child POA with SERVER_DECLARED PriorityModelPolicy,
      // and MULTIPLE_ID id uniqueness policy (so we can use one
      // servant to create several objects).
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (2);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::SERVER_DECLARED,
                                              poa_priority
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_policy_list[1] =
        root_poa->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTPortableServer::POA_var rt_poa =
        RTPortableServer::POA::_narrow (child_poa.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_poa.in (), "RTPOA") == -1)
        return -1;

      // Servant.
      Test_i server_impl (this->orb_.in ());

      // Create object 1 (it will inherit POA's priority).
      int result;
      ACE_DEBUG ((LM_DEBUG, "\nActivated object one as "));
      result = create_object (rt_poa.in (), this->orb_.in (), &server_impl,
                              -1, ior_output_file1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result == -1)
        return -1;

      // Create object 2 (override POA's priority).
      ACE_DEBUG ((LM_DEBUG, "\nActivated object two as "));
      result = create_object (rt_poa.in (), this->orb_.in (), &server_impl,
                              object_priority, ior_output_file2 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result == -1)
        return -1;

      // Activate POA manager.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Start ORB event loop.
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected exception caught in Server_Declared test server:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in());

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      int result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
