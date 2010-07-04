#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Application_Controlled,
           client,
           "$Id$")

const ACE_TCHAR *ior = ACE_TEXT("file://obj.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
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
  // Indicates sucessful parsing of the command line
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
        orb->string_to_object (ior);

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in ());

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Basic reference <%s>\n",
                             ior),
                            1);
        }

      for (int i = 0; i < 5; i++)
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client.cpp:");
      return 1;
    }

  return 0;
}
