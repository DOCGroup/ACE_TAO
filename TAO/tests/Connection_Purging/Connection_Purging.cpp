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
//     This program tests automatic purging in TAO.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//     Irfan Pyarali <irfan@cs.wustl.edu>
//
//=========================================================================

#include "testS.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"

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

static size_t iterations = 20;
static Info *info = 0;
static int debug = 0;

class test_i : public POA_test
{
public:

  void method (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

void
test_i::method (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "test_i::method()\n"));
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
  for (size_t i = 0;
       i < iterations;
       ++i)
    {
      this->info_[i].orb->run ();
    }

  return 0;
}

static int
parse_args (int argc,
            char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "i:d");
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

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "[-i iterations] \n"
                           "[-d (debug)] \n"
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
  for (size_t i = 0;
       i < iterations;
       ++i)
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      ACE_TRY
        {
          CORBA::Object_var object = client_orb->string_to_object (info[i].ior.in (),
                                                                   ACE_TRY_ENV);
          ACE_TRY_CHECK;

          test_var test_object =
            test::_narrow (object.in (),
                           ACE_TRY_ENV);
          ACE_TRY_CHECK;

          test_object->method (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          info[i].orb->shutdown (1,
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in running client side");
          ACE_ASSERT (0);
        }
      ACE_ENDTRY;
    }
}

void
cleanup (void)
{
  for (size_t i = 0;
       i < iterations;
       ++i)
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      ACE_TRY
        {
          info[i].root_poa->destroy (1,
                                     1,
                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in cleaning up");
          ACE_ASSERT (0);
        }
      ACE_ENDTRY;
    }

  delete[] info;
}

int
main (int argc,
      char **argv)
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
                     argc,
                     argv);


  for (j = 0;
       j < argc;
       ++j)
    ACE_OS::free (argv_copy[j]);

  delete[] argv_copy;

  Server_Task server_task (info);
  result = server_task.activate (THR_BOUND);
  ACE_ASSERT (result == 0);

  invoke_remote_calls (client_orb.in ());

  cleanup ();

  return 0;
}
