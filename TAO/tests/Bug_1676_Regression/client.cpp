#include "TestC.h"
#include "ace/Get_Opt.h"
  ACE_TCHAR const *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
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
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

void get_stringList2 (Test::Hello_var hello);

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior);

      Test::Hello_var const hello =
        Test::Hello::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - get_stringList\n"));

      Test::StringList_var seq =
        hello->get_stringList ();

      for (CORBA::ULong i {}; i < seq->length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      (*seq)[i].in ()));
        }

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - get_stringList2 with initialization\n"));

      Test::StringList_var seq2;
      hello->get_stringList2 (true, seq2.out ());

      for (CORBA::ULong i {}; i < seq2->length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      seq2.in ()[i].in ()));
        }

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - get_stringList2 without initialization\n"));

      get_stringList2 (hello);

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - mod_stringList\n"));

      hello->mod_stringList (seq.inout ());

      for (CORBA::ULong i {}; i < seq->length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      (*seq)[i].in ()));
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

void
get_stringList2 (Test::Hello_var hello)
{
  Test::StringList_var seq2;

  try
    {
      // Shutdown the ORB and block until the shutdown is complete.
      hello->get_stringList2 (false, seq2.out ());

      for (CORBA::ULong i {}; i < seq2->length (); ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                      seq2.in ()[i].in ()));
        }
    }
  catch (CORBA::BAD_PARAM const &)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - caught expected exception BAD_PARAM\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("get_stringList2");
    }
}
