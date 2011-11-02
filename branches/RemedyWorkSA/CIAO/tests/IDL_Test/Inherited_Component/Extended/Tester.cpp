//$Id$
/*
 * @file Tester.cpp
 *
 * @author Marcel Smit
 */


#include "DerivedC.h"
#include "ace/streams.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *tester_ior = ACE_TEXT ("file://Derived.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ek:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'k':
            tester_ior = get_opts.opt_arg ();
            break;

          case '?':
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                              "usage:  %s\n"
                              "-k <Derived IOR> (default is file://Derived.ior)\n"
                              "\n",
                              argv [0]),
                              -1);
        }
    }

  return 0;
}


int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      // initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      // create the factory object reference,
      CORBA::Object_var tester_obj =
        orb->string_to_object (tester_ior);

      // downcast the object reference to the appropriate type
      ::Inherited::Tester_var tester =
        ::Inherited::Tester::_narrow (tester_obj.in ());

      if (CORBA::is_nil (tester.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Unable to acquire 'Tester' objref\n"),
                            -1);
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Who is the culprit in the Tester?\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Uncaught CORBA exception\n"),
                        1);
    }
  return 0;
}
