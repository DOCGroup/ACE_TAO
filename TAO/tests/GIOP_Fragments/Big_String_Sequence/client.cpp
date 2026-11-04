#include "EchoC.h"
#include "ace/Get_Opt.h"
#include <cstring>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-f <ior> "
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
      Client_Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return -1;

      CORBA::Object_var const tmp = orb->string_to_object (
        ACE_TEXT_ALWAYS_CHAR (options.ior.c_str ()));

      Echo_var const echo = Echo::_narrow (tmp.in ());

      if (CORBA::is_nil (echo.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Nil Echo reference <%s>\n"),
                           options.ior.c_str ()),
                          -1);

      Echo::List_var list = echo->return_list ();

      ACE_DEBUG ((LM_DEBUG,
                  "Received list of length %u\n",
                  list->length ()));
      if (list->length () != 2)
        ACE_ERROR_RETURN ((LM_ERROR, "Expected length 2\n"), -1);

      char const *value = (*list)[0].in ();
      size_t length = std::strlen (value);
      ACE_DEBUG ((LM_DEBUG,
                  "First element has length %B\n",
                  length));
      for (size_t n {}; n < length; ++n)
        {
          if (value[n] != 'A')
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Character at position %B should be 'A', but is '%c'\n",
                               n,
                               value[n]),
                              -1);
        }
      value = (*list)[1].in ();
      length = std::strlen (value);
      ACE_DEBUG ((LM_DEBUG,
                  "Second element has length %B, value: <%C>\n",
                  length,
                  value));
      if (std::strcmp (value, "Hello World") != 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Expected \"Hello World\""), -1);

      Echo::WList_var const wlist = echo->return_wlist ();

      ACE_DEBUG ((LM_DEBUG,
                  "Received wide list of length %u\n",
                  wlist->length ()));
      if (wlist->length () != 2)
        ACE_ERROR_RETURN ((LM_ERROR, "Expected length 2\n"), -1);

      echo->shutdown ();
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
