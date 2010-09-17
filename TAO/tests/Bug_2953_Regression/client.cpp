// -*- C++ -*-
// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/Get_Opt.h"

#include "TestC.h"

void shutdownORB(CORBA::ORB_ptr orb, const char * orbid)
{
  orb->shutdown(0);
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is shutdown\n",
              orbid));

  orb->destroy();
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is destoyed\n",
              orbid));
}

static const ACE_TCHAR *orbidAfile = ACE_TEXT("file://iorA.ior");
static const ACE_TCHAR *orbidBfile = ACE_TEXT("file://iorB.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:b:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'a':
          orbidAfile = get_opts.optarg;
        break;
        case 'b':
          orbidBfile = get_opts.optarg;
        break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a <iorA> "
                           "-a <iorB> "
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
    if (parse_args (argc, argv) != 0)
      {
        return 1;
      }

    const char *orbidA = "orbidA";
    CORBA::ORB_var orbA = CORBA::ORB_init(argc, argv, orbidA);

    CORBA::Object_var objA = orbA->string_to_object(orbidAfile);
    Test::Hello_var helloA(Test::Hello::_narrow(objA.in ()));
    CORBA::String_var resA = helloA->get_string();

    CORBA::Object_var objB = orbA->string_to_object(orbidBfile);
    Test::Hello_var helloB(Test::Hello::_narrow(objB.in ()));
    CORBA::String_var resB = helloB->get_string();

    ACE_DEBUG ((LM_DEBUG,
                "got resA: <%C> and resB: <%C>",
                resA.in(), resB.in ()));

    helloA->shutdown ();
    helloB->shutdown ();
    shutdownORB(orbA.in (), orbidA);
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Caught unexpected exception:");

    ACE_ERROR ((LM_ERROR, "Bug_2953_Regression test failed.\n"));
    return 1;
  }

  ACE_DEBUG ((LM_DEBUG,
              "Client test completed successfully.\n"));

  return 0;
}
