#include "testC.h"
#include "ace/Get_Opt.h"

int
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      // The selected IOR points to either this literal or an argv entry.
      ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp = orb->string_to_object (ior);

      Test_var const hello = Test::_narrow (tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      AnySeq myseq;
      myseq.length (2);
      Container mycontainer;
      Inner myinner;
      myinner.value1 = 1;
      myinner.value2 = 2;
      myinner.value3 = 3;
      myinner.value4 = 4;
      myinner.value5 = 5;
      mycontainer.contents <<= myinner;
      myseq[0] <<= mycontainer;
      myseq[1] <<= mycontainer;
      AnySeq_var const params = hello->RunTest (myseq);

      for (CORBA::ULong count = 0; count < params->length (); ++count)
      {
        Container const *container {};
        if (!(params[count] >>= container))
        {
          ACE_ERROR ((LM_ERROR, "ERROR, failed extract\n"));
        }
        else
        {
          Inner const *inner {};
          if (!(container->contents >>= inner))
          {
            ACE_ERROR ((LM_ERROR, "ERROR, failed extract\n"));
          }
          else
            ACE_DEBUG ((LM_DEBUG, "%d %d %d %d %d\n",
                        inner->value1, inner->value2, inner->value3, inner->value4, inner->value5));
        }
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
