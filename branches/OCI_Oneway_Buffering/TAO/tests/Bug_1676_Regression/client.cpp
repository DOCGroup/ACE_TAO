// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
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

void get_stringList2(Test::Hello_var hello);

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Hello_var hello =
        Test::Hello::_narrow(tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - get_stringList\n"));

      Test::StringList_var seq =
        hello->get_stringList (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i<seq->length(); i++) {
        CORBA::String_var the_string = CORBA::string_dup ((*seq)[i]);
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));
      }

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - get_stringList2 with initialization\n"));

      Test::StringList_var seq2;
      hello->get_stringList2(true, seq2.out());
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i<seq2->length(); i++) {
        CORBA::String_var the_string = seq2.in()[i];
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));
      }

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - get_stringList2 without initialization\n"));

      get_stringList2(hello);

      ACE_DEBUG ((LM_DEBUG, "\n(%P|%t) - mod_stringList\n"));

      hello->mod_stringList(seq.inout());
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i<seq->length(); i++) {
        CORBA::String_var the_string = CORBA::string_dup ((*seq)[i]);
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));
	  }

      hello->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

void get_stringList2(Test::Hello_var hello)
{
    Test::StringList_var seq2;

  ACE_TRY_NEW_ENV
    {
      // Shutdown the ORB and block until the shutdown is complete.
      hello->get_stringList2(false, seq2.out());
      ACE_TRY_CHECK;

      for (CORBA::ULong i = 0; i<seq2->length(); i++) {
        CORBA::String_var the_string = seq2.in()[i];
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));
	  }
    }
  ACE_CATCH(CORBA::BAD_PARAM, ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - catched expected exception BAD_PARAM\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "get_stringList2");
    }
  ACE_ENDTRY;


    return;
}
