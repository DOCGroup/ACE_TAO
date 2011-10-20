// $Id$

#include "testC.h"
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

      Test_var hello = Test::_narrow(tmp.in ());

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
      AnySeq_var params =  hello->RunTest (myseq);

      for (CORBA::ULong count = 0; count < params->length(); ++count)
      {
        Container* container = 0;
        if (!(params[count] >>= container))
        {
          ACE_ERROR ((LM_ERROR, "ERROR, failed extract\n"));
        }
        else
        {
          Inner* inner = 0;
          if (!(container->contents >>= inner))
          {
            ACE_ERROR ((LM_ERROR, "ERROR, failed extract\n"));
          }
          else
            ACE_DEBUG ((LM_DEBUG, "%d %d %d %d %d\n", inner->value1, inner->value2, inner->value3, inner->value4, inner->value5));
        }
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
