// -*- C++ -*-
// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"

#include "TestC.h"

void shutdownORB(CORBA::ORB_ptr orb, const char * orbid)
{
  orb->shutdown(0);
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%s> is shutdown\n",
              orbid));

  orb->destroy();
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%s> is destoyed\n",
              orbid));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    const char *orbidA = "orbidA";
    const char *orbidAfile = "file://iorA.ior";
    const char *orbidBfile = "file://iorB.ior";

    CORBA::ORB_var orbA = CORBA::ORB_init(argc, argv, orbidA);

    CORBA::Object_var objA = orbA->string_to_object(orbidAfile);
    Test::Hello_var helloA(Test::Hello::_narrow(objA.in ()));
    CORBA::String_var resA = helloA->get_string();

    CORBA::Object_var objB = orbA->string_to_object(orbidBfile);
    Test::Hello_var helloB(Test::Hello::_narrow(objB.in ()));
    CORBA::String_var resB = helloB->get_string();

    ACE_DEBUG ((LM_DEBUG,
                "got resA: <%s> and resB: <%s>",
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
