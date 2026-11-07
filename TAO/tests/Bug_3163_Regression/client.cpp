#include "testC.h"
#include "ace/Get_Opt.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior, int &message_size)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:v:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'v':
        message_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  int message_size {};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior, message_size) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (ior);

      ctest_var const hello = ctest::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      ctest::UCSeq_var data;

      hello->ctestfn (message_size, data.out ());

      CORBA::ULong const received = data->length ();
      ACE_DEBUG ((LM_DEBUG, "Received %u bytes\n", received));

      hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}


