// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

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
          CORBA::String_var the_string = hello->get_string ();

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      the_string.in ()));

          // The *first* invocation of the get_string server always throws a COMPLETED_MAYBE exception. For the call to
          // succeed we must have reinvoked breaking 'at most once' rules. The job is, therefore, a bogey.
          ACE_ERROR ((LM_ERROR, "Error - Regression - Test has failed. ORB has retried a COMPLETED_MAYBE\n"));
        }
      catch (const CORBA::TRANSIENT& trans)
        {
          // We are expecting a TRANSIENT COMPLETED_MAYBE.
          if (trans.completed () == CORBA::COMPLETED_MAYBE)
            {
              // Everything is Peachy
              hello->shutdown ();

              orb->destroy ();

              return 0;
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected exception caught:");
    }

  return 1;
}
