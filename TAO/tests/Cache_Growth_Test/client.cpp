#include "TestC.h"
#include "ace/Get_Opt.h"

#include <string>

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior,
            bool &do_shutdown)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'x':
        do_shutdown = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-k <ior> "
                                     "-x <do_shutdown>"
                                     "\n"),
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
      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      bool do_shutdown {};
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      if (parse_args (argc, argv, ior, do_shutdown) != 0)
        return 1;

      CORBA::Object_var const tmp {orb->string_to_object (ior.c_str ())};

      Test::Hello_var const hello {Test::Hello::_narrow (tmp.in ())};

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             ior.c_str ()),
                            1);
        }

      CORBA::String_var const the_string {hello->get_string ()};

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      if (do_shutdown)
        {
          hello->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
