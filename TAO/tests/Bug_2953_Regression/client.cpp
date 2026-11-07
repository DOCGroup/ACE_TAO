// -*- C++ -*-
#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Log_Msg.h"
#include "ace/Thread.h"
#include "ace/Get_Opt.h"

#include "TestC.h"

void
shutdownORB (CORBA::ORB_ptr orb, char const *orbid)
{
  orb->shutdown (false);
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is shutdown\n",
              orbid));

  orb->destroy ();
  ACE_DEBUG ((LM_DEBUG,
              "ORB <%C> is destroyed\n",
              orbid));
}

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            ACE_TCHAR const *&orbid_a_file,
            ACE_TCHAR const *&orbid_b_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
        case 'a':
          orbid_a_file = get_opts.optarg;
          break;
        case 'b':
          orbid_b_file = get_opts.optarg;
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s -a <iorA> -b <iorB>\n",
                             argv[0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *orbid_a_file = ACE_TEXT ("file://iorA.ior");
  ACE_TCHAR const *orbid_b_file = ACE_TEXT ("file://iorB.ior");
  try
    {
      if (parse_args (argc, argv, orbid_a_file, orbid_b_file) != 0)
        return 1;

      char const * const orbid_a = "orbidA";
      CORBA::ORB_var const orb_a = CORBA::ORB_init (argc, argv, orbid_a);

      CORBA::Object_var const obj_a = orb_a->string_to_object (orbid_a_file);
      Test::Hello_var const hello_a (Test::Hello::_narrow (obj_a.in ()));
      CORBA::String_var const result_a = hello_a->get_string ();

      CORBA::Object_var const obj_b = orb_a->string_to_object (orbid_b_file);
      Test::Hello_var const hello_b (Test::Hello::_narrow (obj_b.in ()));
      CORBA::String_var const result_b = hello_b->get_string ();

      ACE_DEBUG ((LM_DEBUG,
                  "got resA: <%C> and resB: <%C>",
                  result_a.in (), result_b.in ()));

      hello_a->shutdown ();
      hello_b->shutdown ();
      shutdownORB (orb_a.in (), orbid_a);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught unexpected exception:");

      ACE_ERROR ((LM_ERROR, "Bug_2953_Regression test failed.\n"));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Client test completed successfully.\n"));

  return 0;
}
