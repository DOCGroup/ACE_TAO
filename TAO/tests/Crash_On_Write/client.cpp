// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Crash_On_Write, client, "$Id$")

const char *ior = "file://test.ior";
int iterations = 2000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:i:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-i iterations "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
single_iteration (Test::Oneway_Receiver_ptr oneway_receiver
                  TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      oneway_receiver->receive_oneway (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_Time_Value tv (0, 40000);
      ACE_OS::sleep (tv);
    }
  ACE_CATCHANY
    {
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Oneway_Receiver_var oneway_receiver =
        Test::Oneway_Receiver::_narrow(tmp.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (oneway_receiver.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil process reference <%s>\n",
                             ior),
                            1);
        }

      int normal_count = 0;
      int exception_count = 0;
      for (int i = 0; i != iterations; ++i)
        {
          int result = single_iteration (oneway_receiver.in ()
                                         TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          if (result)
            exception_count++;
          else
            normal_count++;

          if (i % 500 == 0)
            ACE_DEBUG ((LM_DEBUG, "Finished iteration %d\n", i));
        }

      if (exception_count == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: no exceptions detected\n"));

      if (normal_count == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: no request was succesful\n"));

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
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
