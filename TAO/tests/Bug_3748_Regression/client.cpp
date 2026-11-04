#include "TestC.h"
#include "ace/Get_Opt.h"
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      CORBA::ULong encountered_failures {};
      for (CORBA::ULong i {}; i < Test::expected_failure_number; ++i)
        {
          try
            {
              CORBA::String_var const the_string = hello->get_string ();
            }
          catch (::CORBA::Exception const &)
            {
              CORBA::ULong const modulo = Test::expected_failure_number / 10;
              if (++encountered_failures % modulo == 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Expected exception\n"));
                }
            }
        }

      if (encountered_failures == 0)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: No expected exceptions occurred.\n"));

      // By this time the server should accept the connection and
      // must handle shutdown gracefully.
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
