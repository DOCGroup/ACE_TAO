// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
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
                           "usage:  %s -k <ior> \n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int error= 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      Test::BoundSequences_var TestThis =
        Test::BoundSequences::_narrow(tmp.in ());

      if (CORBA::is_nil (TestThis.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::BoundSequences reference <%s>\n",
                             ior),
                            1);
        }

      ////////////////////////////////////////////
      ::Test::BoundSequences::SequenceOf10Long Seq;
      ACE_DEBUG ((LM_DEBUG, "Client - Attempting to set length(11)\n"));
      try
        {
          Seq.length (11);
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly set length to %u\n",
                      Seq.length ()));
          error= 1;
        }
      catch (const ::CORBA::BAD_PARAM &)
        {
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly threw bad_param\n"));
        }
      catch (const ::CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error= 1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error= 1;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Attempting to set length(10)\n"));
      try
        {
          Seq.length (10);
          ACE_DEBUG ((LM_DEBUG, "Client - %Correctly set length to %u\n",
                      ((10 == Seq.length ()) ? "C" : "**** Inc"),
                      Seq.length ()));
          if (10 != Seq.length ())
            error= 1;
        }
      catch (const ::CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error= 1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error= 1;
        }

      //////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Accessing [0]\n"));
      try
        {
          // Just to read access Seq[0] without optimizing away
          ACE_DEBUG ((LM_DEBUG, "", Seq[0]));
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly allowed access to [0]\n"));
        }
      catch (const ::CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error= 1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error= 1;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Accessing [10]\n"));
      try
        {
          // Just to read access Seq[10] without optimizing away
          ACE_DEBUG ((LM_DEBUG, "", Seq[10]));
#if defined (TAO_CHECKED_SEQUENCE_INDEXING) && (TAO_CHECKED_SEQUENCE_INDEXING == 1)
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **** Incorrectly allowed access to [10]\n"));
          error= 1;
        }
      catch (const ::CORBA::BAD_PARAM &)
        {
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly threw bad_param\n"));
#else
          ACE_DEBUG ((LM_DEBUG, "Client - Correctly allowed access to [10]\n"));
#endif // TAO_CHECKED_SEQUENCE_INDEXING
        }
      catch (const ::CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error= 1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error= 1;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Sending Seq\n"));
      ::CORBA::ULong result;
      try
        {
          result= TestThis->SendSequenceOf10Long (Seq.length (), Seq);
          if (result != Seq.length())
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Client - **FAILED** Sent %u longs but "
                          "server received %u longs\n",
                          Seq.length(), result));
              error= 1;
            }
          else
            ACE_DEBUG ((LM_DEBUG,
                        "Client - **SUCCESS** Sent and Server got %u longs\n",
                        result));
        }
      catch (const ::CORBA::Exception &ex)
        {
          ex._tao_print_exception ("Client - **FAILED** Incorrectly threw");
          error= 1;
        }
      catch (...)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Client - **FAILED** Incorrectly threw something else\n"));
          error= 1;
        }

      ////////////////////////////////////////////
      ACE_DEBUG ((LM_DEBUG, "Client - Shutting down Server\n"));
      TestThis->shutdown ();

      ACE_DEBUG ((LM_DEBUG, "Client - Finishing\n"));
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client - Exception caught:");
      error= 1;
    }

  return error;
}
