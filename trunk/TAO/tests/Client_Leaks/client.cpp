// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Crashed_Callback, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 1500;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	ior = get_opts.optarg;
	break;

      case 'i':
	iterations = ACE_OS::atoi (get_opts.optarg);
	break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
			   "-k <ior> "
			   "-i <iterations> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

void
run_iteration (Test::Process_Factory_ptr process_factory,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      Test::Process_var process =
        process_factory->create_new_process (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      (void) process->get_process_id (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      process->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
    }
  ACE_ENDTRY;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::Process_Factory_var process_factory =
        Test::Process_Factory::_narrow(tmp.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (process_factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil process reference <%s>\n",
                             ior),
                            1);
        }

      for (int i = 0; i != iterations; ++i)
        {
          run_iteration (process_factory.in (),
                         ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (i % 50 == 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) - client %d / %d iterations\n",
                          i, iterations));
            }
        }

      process_factory->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
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
