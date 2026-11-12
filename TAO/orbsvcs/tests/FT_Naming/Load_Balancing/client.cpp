#include "TestC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "ace/Time_Value.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) - Acquiring Name Service\n")));

      TAO_Naming_Client name_svc;
      try
        {
          ACE_Time_Value timeout (10);
          if (name_svc.init (orb.in (), &timeout) != 0)
            ACE_ERROR_RETURN ((LM_DEBUG,
                               ACE_TEXT ("client: Could not connect to naming service.\n")),
                              1);
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            ACE_TEXT ("Exception caught while initializing name service facade:"));
          return 1;
        }

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("basic_name");

      CORBA::Object_var tmp {};
      Test::Basic_var basic {};

      // Iterate enough times to wrap around the group several times.
      for (int i {}; i < 15; ++i)
        {
          try
            {
              // Each time we invoke resolve, we get a different member.
              tmp = name_svc->resolve (name);

              // Narrow it to a Basic object.
              basic = Test::Basic::_narrow (tmp.in ());
            }
          catch (CORBA::Exception const &ex)
            {
              ex._tao_print_exception (ACE_TEXT ("Error resolving name.\n"));
              return 1;
            }

          if (CORBA::is_nil (basic.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Server object reference not obtained from ")
                               ACE_TEXT ("Load Balancing Name Service\n")),
                              1);

          try
            {
              CORBA::String_var const the_string = basic->get_string ();

              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) - Client request handled ")
                          ACE_TEXT ("by object at <%C>\n"),
                          the_string.in ()));
            }
          catch (CORBA::Exception const &ex)
            {
              ex._tao_print_exception (
                ACE_TEXT ("Error invoking get_string on Basic object.\n"));
              return 1;
            }

          if (i == 7)
            {
              try
                {
                  basic->remove_member ();
                }
              catch (CORBA::Exception const &ex)
                {
                  ex._tao_print_exception (
                    ACE_TEXT ("Error removing member from object group.\n"));
                  return 1;
                }
            }
        }

      try
        {
          basic->shutdown ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - Shutting down server\n")));
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception (
            ACE_TEXT ("Error shutting down server."));
          return 1;
        }
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Exception caught in client.cpp:"));
      return 1;
    }

  return 0;
}
