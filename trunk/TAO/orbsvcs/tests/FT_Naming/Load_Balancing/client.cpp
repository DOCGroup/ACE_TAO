// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "ace/Time_Value.h"

const ACE_TCHAR *ior = ACE_TEXT("file://obj.ior");

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
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k <ior> ")
                           ACE_TEXT ("\n"),
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) - Acquiring Name Service\n")));

      TAO_Naming_Client name_svc;
      try {

        ACE_Time_Value timeout (10); // Wait up to 10 seconds for the naming service
        if (name_svc.init (orb.in (), &timeout) != 0)
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("client: Could not connect to ")
                             ACE_TEXT ("naming service.\n")),
                            1);
      }
      catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception (
          ACE_TEXT ("Exception caught while initializing name ")
          ACE_TEXT ("service facade:"));
        return 1;
      }

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup ("basic_name");

      CORBA::Object_var tmp;
      Test::Basic_var basic;

      // Iterate enough so we get a few wraparrounds
      for (int i = 0; i < 15; i++)
      {
        try {

          // Each time we invoke resolve, we get a different member
          tmp =
            name_svc->resolve (name);

          // Narrow it to a Basic object
          basic =
            Test::Basic::_narrow (tmp.in ());

        }
        catch (CORBA::Exception& ex)
          {
            ex._tao_print_exception (ACE_TEXT ("Error resolving name.\n"));
          }

        if (CORBA::is_nil (basic.in ()))
          {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               ACE_TEXT ("Server obj ref not obtained ")
                               ACE_TEXT ("from Load Balancing Name Service\n"),
                               ior),
                              1);
          }

        try {
          CORBA::String_var the_string =
            basic->get_string ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - Client request handled ")
                      ACE_TEXT ("by object at <%C>\n"),
                      the_string.in ()));

        }
        catch (CORBA::Exception& ex)
          {
            ex._tao_print_exception (
              ACE_TEXT ("Error invoking get_string on Basic object.\n"));
            return 1;
          }

        // Remove one member after we wrapped around to make sure naming manager can
        // handle it successufully
        if (i == 7)
          {
            try {
              // Try removing a member
              basic->remove_member ();
            }
            catch (CORBA::Exception& ex)
              {
                ex._tao_print_exception (
                  ACE_TEXT ("Error invoking get_string on Basic object.\n"));
                return 1;
              }
          }
      }

      try {

        basic->shutdown ();

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) - Shutting down server\n")));
      }
      catch (CORBA::Exception& ex)
      {
        ex._tao_print_exception (
          ACE_TEXT ("Error invoking get_string on Basic object.\n"));
        return 1;
      }
      orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("Exception caught in client.cpp:"));
    return 1;
  }

  return 0;
}
