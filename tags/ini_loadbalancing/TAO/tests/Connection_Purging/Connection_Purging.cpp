// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/Connection_Purging
//
// = FILENAME
//     Connection_Purging.cpp
//
// = DESCRIPTION
//     This program tests automatic purging in TAO. Collocation is off 
//     which helps us test the usage and purging of handles from the 
//     connection cache.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//     Irfan Pyarali <irfan@cs.wustl.edu>
//
//=========================================================================

#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/Handle_Gobbler.h"
#include "testS.h"

struct arguments
{
  int argc;
  char **argv;
};

struct Info
{
  CORBA::ORB_var orb;
  PortableServer::POA_var root_poa;
  CORBA::String_var ior;
};


// Note: The handles available may seem more than the iterations but
// bear in mind that the server and client are in the same process and
// that per RMI uses up two handles atleast and also that the ORB creation
// uses up a descriptor atleast.
static size_t keep_handles_available = 35;
static size_t iterations = 30;
static size_t remote_calls = 1;
static Info *info = 0;
static int debug = 0;
static int go_to_next_orb = 0;

class test_i : public POA_test
{
public:

  test_i (void);

  void method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  size_t counter_;
};

test_i::test_i (void)
  : counter_ (0)
{
}

void
test_i::method (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  go_to_next_orb = 1;

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::method() iteration = %d\n",
                ++this->counter_));
}

class Server_Task : public ACE_Task_Base
{
public:

  Server_Task (Info *info);
  int svc (void);

private:

  Info *info_;
};

Server_Task::Server_Task (Info *info)
  : info_ (info)
{
}

int
Server_Task::svc (void)
{
  for (size_t j = 0;
       j < remote_calls;
       ++j)
    {
      for (size_t i = 0;
           i < iterations;
           ++i)
        {
          while (!go_to_next_orb)
            this->info_[i].orb->perform_work ();

            go_to_next_orb = 0;
        }
    }

  return 0;
}

static int
parse_args (int argc,
            char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "i:a:r:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        iterations = ACE_OS::atoi (get_opts.optarg);
        break;

      case 'd':
        debug = 1;
        break;

      case 'a':
        keep_handles_available = atoi (get_opts.optarg);
        break;

      case 'r':
        remote_calls = atoi (get_opts.optarg);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "[-i iterations] \n"
                           "[-d (debug)] \n"
                           "[-a (keep handles available)] \n",
                           "[-r (remote calls per server)] \n",
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

void
setup_server_orbs (test_i &servant,
                   int argc,
                   char **argv)
{
  ACE_NEW (info,
           Info[iterations]);

  for (size_t i = 0;
       i < iterations;
       ++i)
    {
      int argc_copy = argc;
      char **argv_copy = 0;

      ACE_NEW (argv_copy,
               char *[argc]);

      int j = 0;

      for (j = 0;
           j < argc;
           ++j)
        argv_copy[j] = ACE_OS::strdup (argv[j]);

      ACE_DECLARE_NEW_CORBA_ENV;

      ACE_TRY
        {
          char orb_name[BUFSIZ];
          ACE_OS::sprintf (orb_name,
                           "%d",
                           i);

          // Initialize the ORB first.
          info[i].orb = CORBA::ORB_init (argc_copy,
                                         argv_copy,
                                         orb_name,
                                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          for (j = 0;
               j < argc;
               ++j)
            ACE_OS::free (argv_copy[j]);

          delete[] argv_copy;

          // Obtain the RootPOA.
          CORBA::Object_var object = info[i].orb->resolve_initial_references ("RootPOA",
                                                                              ACE_TRY_ENV);

          // Get the POA_var object from Object_var.
          info[i].root_poa =
            PortableServer::POA::_narrow (object.in (),
                                          ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Get the POAManager of the RootPOA.
          PortableServer::POAManager_var poa_manager =
            info[i].root_poa->the_POAManager (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          poa_manager->activate (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          PortableServer::ObjectId_var id =
            info[i].root_poa->activate_object (&servant,
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          object = info[i].root_poa->id_to_reference (id.in (),
                                                      ACE_TRY_ENV);
          ACE_TRY_CHECK;

          info[i].ior =
            info[i].orb->object_to_string (object.in (),
                                           ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in setting up servers");
          ACE_ASSERT (0);
        }
      ACE_ENDTRY;
    }
}

void
setup_client_orb (CORBA::ORB_out client_orb,
                  int argc,
                  char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the client ORB first.
      client_orb = CORBA::ORB_init (argc,
                                    argv,
                                    0,
                                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in initializing client ORB");
      ACE_ASSERT (0);
    }
  ACE_ENDTRY;
}

void
invoke_remote_calls (CORBA::ORB_ptr client_orb)
{
  ACE_DECLARE_NEW_CORBA_ENV;
    
  ACE_TRY 
    {
      for (size_t j = 0;
           j < remote_calls;
           ++j)
        {
          if (debug)
            ACE_DEBUG ((LM_DEBUG, "remote_call %d\n", j));

          for (size_t i = 0;
               i < iterations;
               ++i)
            {
              if (debug)
                ACE_DEBUG ((LM_DEBUG, "invoke_remote_calls::iteration %d\n", i));

              CORBA::Object_var object =
                client_orb->string_to_object (info[i].ior.in (),
                                              ACE_TRY_ENV);
              ACE_TRY_CHECK;
                  
              test_var test_object =
                test::_narrow (object.in (),
                                   ACE_TRY_ENV);
              ACE_TRY_CHECK;
 
              size_t invoke_exception = 0;
              for (int try_invoke = 0; try_invoke < 3; ++try_invoke)
                {
                  ACE_TRY_EX (try_again_block)
                    {
                      invoke_exception = 0;
                      test_object->method (ACE_TRY_ENV);
                      ACE_TRY_CHECK_EX (try_again_block);
                    }
                  ACE_CATCHANY
                    {
                      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "invoke_remote_calls::Exception!");

                      if (debug)
                        ACE_DEBUG ((LM_DEBUG, "Trying again...invoke_remote_calls::iteration %d\n", i));

                      invoke_exception = 1;
                    }
                  ACE_ENDTRY;

                  if (invoke_exception != 0)
                    continue;
                  else
                    break;
                }

              if (invoke_exception == 1)
                ACE_ASSERT (0);
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in invoke_remote_calls on client side");
      ACE_ASSERT (0);
    }
  ACE_ENDTRY;
}

void
orbs_shutdown (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for (size_t i = 0;
           i < iterations;
           ++i)
        {
          info[i].orb->shutdown (1,
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;
          
          info[i].root_poa->destroy (1,
                                     1,
                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
     {
       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in shutting orbs");
       ACE_ASSERT (0);
     }
  ACE_ENDTRY;
  
  delete[] info;
}

int
main (int argc,
      char *argv [])
{
  int argc_copy = argc;
  char **argv_copy = 0;

  ACE_NEW_RETURN (argv_copy,
                  char *[argc],
                  -1);

  int j = 0;

  for (j = 0;
       j < argc;
       ++j)
    argv_copy[j] = ACE_OS::strdup (argv[j]);

  CORBA::ORB_var client_orb;

  setup_client_orb (client_orb.out (),
                    argc,
                    argv);

  int result = parse_args (argc,
                           argv);
  if (result != 0)
    return result;

  test_i servant;

  setup_server_orbs (servant,
                     argc_copy,
                     argv_copy);

  for (j = 0;
       j < argc;
       ++j)
    ACE_OS::free (argv_copy[j]);

  delete[] argv_copy;

  Server_Task server_task (info);
  result = server_task.activate (THR_BOUND);
  ACE_ASSERT (result == 0);

  // Consume all handles in the process, leaving us
  // <keep_handles_available> to play with.
  ACE_Handle_Gobbler handle_gobbler;
  result = handle_gobbler.consume_handles (keep_handles_available);
  ACE_ASSERT (result == 0);

  invoke_remote_calls (client_orb.in ());
  //orbs_shutdown ();

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

// = Handle Gobbler
template class ACE_Node<ACE_HANDLE>;
template class ACE_Unbounded_Set<ACE_HANDLE>;
template class ACE_Unbounded_Set_Iterator<ACE_HANDLE>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

// = Handle Gobbler
#pragma instantiate ACE_Node<ACE_HANDLE>
#pragma instantiate ACE_Unbounded_Set<ACE_HANDLE>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_HANDLE>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
