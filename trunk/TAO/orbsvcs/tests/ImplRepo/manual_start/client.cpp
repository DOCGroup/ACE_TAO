// $Id$

#include "testC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
CORBA::ORB_var orb;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage:  %s ")
                             ACE_TEXT ("-k <ior> ")
                             ACE_TEXT ("\n"),
                             argv [0]),
                            -1);
        }
    }
  // Indicates successful parsing of the command line
  return 0;
}

void
do_test (void)
{
  CORBA::Object_var obj = orb->string_to_object (ior);
  Test_var test = Test::_narrow( obj.in () );
  ACE_ASSERT (!CORBA::is_nil(test.in ()));

  try
    {
      test->contact ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("client contact completed\n")));
      return;
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("client caught %C during contact\n"),
                  ex._name ()));
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    do_test ();
    return 0;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception (ACE_TEXT ("client:"));
  }

  return -1;
}
