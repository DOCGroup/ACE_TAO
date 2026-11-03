
#include "TestC.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

int
ACE_TMAIN(int, ACE_TCHAR *[])
{
  int const niterations = 10;
  int const norbs = 10;

  try
    {
      for (int i = 0; i != niterations; ++i)
        {
          for (int j = 0; j != norbs; ++j)
            {
              char buf[16] {};
              ACE_OS::sprintf (buf, "ORB_%4.4d", j);

              int argc {};
              ACE_TCHAR **argv {};
              CORBA::ORB_var orb =
                CORBA::ORB_init (argc, argv, buf);

              CORBA::Object_var object =
                orb->string_to_object ("DLL:Test_Object");

              Test_var test =
                Test::_narrow (object.in ());

              if (CORBA::is_nil (test.in ()))
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "Nil object reference.\n"),
                                    1);
                }

              CORBA::Long const count =
                test->instance_count ();

              CORBA::Long const expected_count = norbs * i + j + 1;
              if (count != expected_count)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Incorrect number of objects "
                              "(%d != %d)\n",
                              count, expected_count));
                }
              test->destroy ();
            }
        }

      for (int j = 0; j != norbs; ++j)
        {
          char buf[16] {};
          ACE_OS::sprintf (buf, "ORB_%4.4d", j);

          int argc {};
          ACE_TCHAR **argv {};
          CORBA::ORB_var orb =
            CORBA::ORB_init (argc, argv, buf);

          CORBA::Object_var obj =
            orb->resolve_initial_references ("RootPOA");

          PortableServer::POA_var poa =
            PortableServer::POA::_narrow (obj.in ());

          poa->destroy (true, true);

          orb->destroy ();
        }
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("main()");
      return 1;
    }
  return 0;
}
