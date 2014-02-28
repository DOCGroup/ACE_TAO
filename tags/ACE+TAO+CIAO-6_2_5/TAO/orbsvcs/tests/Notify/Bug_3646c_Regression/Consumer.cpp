// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "Notify_Test_Client.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
  try
    {
      Notify_Test_Client client;
      ACE_Argv_Type_Converter argcon (argc, argv);
      status = client.init (argcon.get_argc (), argcon.get_TCHAR_argv ());

      if (status == 0)
        {
          CosNaming::NamingContext_ptr naming = client.naming_context ();
          if (CORBA::is_nil (naming))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Can't find naming service\n"),
                                1);
            }

          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup ("Channel1");
          CORBA::Object_var obj1 = naming->resolve (name);

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Successfully retrieved Challel1\n")));

          name[0].id = CORBA::string_dup ("Channel2");
          CORBA::Object_var obj2 = naming->resolve (name);

          ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Successfully retrieved Challel2\n")));
        }
    }
  catch (const CORBA::Exception& e)
    {
      e._tao_print_exception ("Error: ");
      status = 1;
    }

  return status;
}
