// $Id$

#include "Sender_i.h"
#include "Server_Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Oneways_Invoking_Twoways, server, "$Id$")

const char *ior_output_file = "test.ior";
int number_of_oneways = 10;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'n' :
        number_of_oneways = ACE_OS::atoi (get_opts.opt_arg ());
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-n <number of oneways>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Sender_i *sender_impl;
      ACE_NEW_RETURN (sender_impl,
                      Sender_i (orb.in ()),
                      1);
      PortableServer::ServantBase_var receiver_owner_transfer(sender_impl);

      Test::Sender_var sender =
        sender_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (sender.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Server_Task server_task (orb.in (),
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }
      ACE_Thread_Manager::instance ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "Now terminating test\n"));

      CORBA::ULong activeobjects = sender_impl->get_active_objects ();
      if (((number_of_oneways * activeobjects) !=
            sender_impl->get_number_received ()) && activeobjects != 0)
        {
          ACE_ERROR ((LM_ERROR, "Error, expected %d oneways, received %d\n",
                      number_of_oneways,
                      sender_impl->get_number_received()));
        }
      else
        {
          if (sender_impl->get_number_received () == 0)
            {
              ACE_ERROR ((LM_ERROR, "Error: Received no calls\n"));
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG, "Corrected amount received\n"));
            }
        }

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
