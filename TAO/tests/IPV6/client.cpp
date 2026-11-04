#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  bool expect_error {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'x':
        options.expect_error = true;
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Test::Hello_var const hello = Test::Hello::_narrow (object.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Hello reference <%s>\n"),
                             options.ior.c_str ()),
                            1);
        }

      CORBA::String_var const the_string = hello->get_string ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      hello->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      if (options.expect_error)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - expected exception occurred!\n%s\n",
                      ex._info().c_str()));

          return 0;
        }
      else
        {
          ex._tao_print_exception ("Exception caught:");
          return 1;
        }
    }

  if (options.expect_error)
    {
      ACE_DEBUG ((LM_ERROR, "(%P|%t) client - expected error did not occur!\n"));

      return 1;
    }

  return 0;
}
