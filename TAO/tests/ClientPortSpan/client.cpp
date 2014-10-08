// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
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

      try
        {

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - calling get_string\n"));
          CORBA::String_var the_string = hello->get_string ();
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      the_string.in ()));
          ACE_OS::sleep (1);
          hello->shutdown ();
        }
      catch (const CORBA::TRANSIENT& )
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - caught expected transient\n"));
          result = 1;
        }
      catch (const CORBA::COMM_FAILURE& )
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - caught expected comm failure\n"));
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      result = 100;
    }

  return result;
}
