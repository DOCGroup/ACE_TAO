// $Id$

#include "TestC.h"
#include <iostream>
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

char target = ' ';

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("abc"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
      case 'b':
      case 'c':
        target = c;
        break;
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s [-a|-b|-c]\n '%s' unknown"
                           "\n",
                           argv [0], argv[1]),
                          -1);
      }

  return target != ' ' ? 0 : -1;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    ACE_CString ior ("file://TestObject_");
    ior += target;
    ior += ".ior";
    ACE_DEBUG ((LM_DEBUG, "using target = %c, iorstr %s\n", target, ior.c_str()));
    CORBA::Object_var obj = orb->string_to_object (ior.c_str());
    Test_var test = Test::_narrow( obj.in() );
    ACE_ASSERT (!CORBA::is_nil(test.in()));

    CORBA::Long spid = test->server_pid ();
    ACE_DEBUG ((LM_DEBUG, "client target %c got pid %d\n", target, spid));
    return 0;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception ("client:");
  }

  return -1;
}
