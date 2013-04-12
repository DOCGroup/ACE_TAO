// $Id$

#include "TestC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Get_Opt.h"


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var tmp =
        orb->string_to_object ("corbaloc:iiop:1.2@localhost:9931/NameService");

      CosNaming::NamingContext_var root =
        CosNaming::NamingContext::_narrow (tmp.in ());

      if (CORBA::is_nil (root.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil NamingService reference\n")),
                             1);
        }

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("**** Narrowed root NamingContext\n")));

      CosNaming::Name name;
      name.length(3);
      name[0].id = CORBA::string_dup ("nsB");
      name[1].id = CORBA::string_dup ("example");
      name[2].id = CORBA::string_dup ("Hello");

      try
        {
          tmp = root->resolve (name);
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("**** Resolved #nsB/example/Hello\n")));

          Test::Hello_var hello =
            Test::Hello::_narrow (tmp.in ());

          if (CORBA::is_nil (hello.in ()))
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 ACE_TEXT ("Nil Test::Hello reference\n")),
                                 1);
            }

          CORBA::String_var the_string = hello->get_string ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) - string returned <%C>\n"),
                      the_string.in ()));

          hello->shutdown ();
        }
      catch (const CosNaming::NamingContext::CannotProceed&)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Caught correct exception\n")));
        }

      orb->shutdown ();
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Exception caught:"));
      return 1;
    }

  return 0;
}
