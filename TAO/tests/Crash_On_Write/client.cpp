// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int iterations = 2000;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:"));
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
single_iteration (Test::Oneway_Receiver_ptr oneway_receiver)
{
  try
    {
      oneway_receiver->receive_oneway ();
      ACE_Time_Value tv (0, 40000);
      ACE_OS::sleep (tv);
    }
  catch (const CORBA::Exception&)
    {
      return 1;
    }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      Test::Oneway_Receiver_var oneway_receiver =
        Test::Oneway_Receiver::_narrow(tmp.in ());

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
          int result = single_iteration (oneway_receiver.in ());
          if (result)
            exception_count++;
          else
            normal_count++;

          if (i % 100 == 0)
            ACE_DEBUG ((LM_DEBUG, "Finished iteration %d\n", i));

          // Lets not waste any more time than we have to
          if (exception_count > 10 && normal_count > 0)
            break;
        }

      if (exception_count == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: no exceptions detected\n"));

      if (normal_count == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: no request was succesful\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
