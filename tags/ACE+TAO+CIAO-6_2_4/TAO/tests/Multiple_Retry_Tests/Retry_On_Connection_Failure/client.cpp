// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
bool oneway_call = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'o':
        oneway_call = ACE_OS::atoi (get_opts.opt_arg ()) != 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-o <oneway flag> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      if (oneway_call)
        {
          CORBA::String_var the_string = CORBA::string_dup( "Test" );
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client sending string <%C> oneway\n",
                      the_string.in ()));
          hello->set_string (the_string);
        }
      else
        {
          CORBA::String_var the_string = hello->get_string ();
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Client getting string <%C>\n",
                      the_string.in ()));
        }

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
