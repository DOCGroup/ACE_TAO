#include "TestC.h"
#include "ace/Get_Opt.h"
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s -k <ior> \n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  bool error {};
  try
    {
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      std::basic_string<ACE_TCHAR> ior {ACE_TEXT ("file://test.ior")};
      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (ior.c_str ()));

      Test::BoundSequences_var const test_this =
        Test::BoundSequences::_narrow (tmp.in ());

      if (CORBA::is_nil (test_this.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::BoundSequences reference <%s>\n",
                             ior.c_str ()),
                            1);
        }

      ////////////////////////////////////////////
      ::Test::BoundSequences::SequenceOf10Long sequence {};
      ACE_DEBUG ((LM_DEBUG, "Client - Attempting to set length(11)\n"));
      try
        {
          sequence.length (11);
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly set length to %u\n",
                      sequence.length ()));
          error = true;
        }
      catch (::CORBA::BAD_PARAM const &)
        {
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly threw bad_param\n"));
        }
      catch (::CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error = true;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error = true;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Attempting to set length(10)\n"));
      try
        {
          sequence.length (10);
          ACE_DEBUG ((LM_DEBUG, "Client - %Correctly set length to %u\n",
                      ((10 == sequence.length ()) ? "C" : "**** Inc"),
                      sequence.length ()));
          if (10 != sequence.length ())
            error = true;
        }
      catch (::CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error = true;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error = true;
        }

      //////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Accessing [0]\n"));
      try
        {
          // Just to read access sequence[0] without optimizing away
          ACE_DEBUG ((LM_DEBUG, "", sequence[0]));
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly allowed access to [0]\n"));
        }
      catch (::CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error = true;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error = true;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Accessing [10]\n"));
      try
        {
          // Just to read access sequence[10] without optimizing away
          ACE_DEBUG ((LM_DEBUG, "", sequence[10]));
#if defined (TAO_CHECKED_SEQUENCE_INDEXING) && (TAO_CHECKED_SEQUENCE_INDEXING == 1)
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **** Incorrectly allowed access to [10]\n"));
          error = true;
        }
      catch (::CORBA::BAD_PARAM const &)
        {
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly threw bad_param\n"));
#else
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly allowed access to [10]\n"));
#endif // TAO_CHECKED_SEQUENCE_INDEXING
        }
      catch (::CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error = true;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error = true;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Sending Seq\n"));
      ::CORBA::ULong result {};
      try
        {
          result = test_this->SendSequenceOf10Long (sequence.length (), sequence);
          if (result != sequence.length ())
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Client - **FAILED** Sent %u longs but "
                          "server received %u longs\n",
                          sequence.length (), result));
              error = true;
            }
          else
            ACE_DEBUG ((LM_DEBUG,
                        "Client - **SUCCESS** Sent and Server got %u longs\n",
                        result));
        }
      catch (::CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error = true;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error = true;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Shutting down Server\n"));
      test_this->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "Client - Finishing\n"));
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client - Exception caught:");
      error = true;
    }

  return error;
}
