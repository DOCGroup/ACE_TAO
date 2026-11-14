#include "TestC.h"
#include "ace/Get_Opt.h"

static int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior>\n"),
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      ACE_TCHAR const *ior = ACE_TEXT ("file://obj.ior");

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test::Basic reference <%s>\n"),
                             ior),
                            1);
        }

      for (int i {}; i < 5; ++i)
        {
          CORBA::String_var the_string =
            basic->get_string ();

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client request handled by object at <%s>\n",
                      the_string.in ()));
        }

      basic->remove_member ();

      basic->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in client.cpp:");
      return 1;
    }

  return 0;
}
