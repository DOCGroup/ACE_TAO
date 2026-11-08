#include "TestC.h"
#include "ace/Get_Opt.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
  bool oneway_call {};
};

static bool
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case 'o':
        options.oneway_call = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-o <oneway flag> "
                           "\n",
                           argv [0]),
                          false);
      }
  // Indicates successful parsing of the command line
  return true;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Client_Options options;

      if (!parse_args (argc, argv, options))
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (options.ior.c_str ());

      Test::Hello_var const hello = Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             options.ior.c_str ()),
                            1);
        }

      if (options.oneway_call)
        {
          CORBA::String_var const the_string = CORBA::string_dup ("Test");
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client sending string <%C> oneway\n",
                      the_string.in ()));
          hello->set_string (the_string);
        }
      else
        {
          CORBA::String_var const the_string = hello->get_string ();
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client getting string <%C>\n",
                      the_string.in ()));
        }

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
