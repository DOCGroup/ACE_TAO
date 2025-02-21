#include "TestC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

int process_result = 1;

int parse_args (int argc, ACE_TCHAR *argv[])
{
ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-f <ior> "
                           "\n",
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object(ior);

      Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (hello.in ()))
      {
         ACE_ERROR_RETURN ((LM_DEBUG,
                            "Nil Test::Hello reference <%C>\n",
                            ior),
                           1);
      }

      CORBA::String_var the_string = hello->get_string ();

      ACE_DEBUG ((LM_DEBUG,
                  "Hello from file IOR: %C\n",
                  the_string.in()));

      Test::Hello_var helloRef = hello->get_Hello();

      if (CORBA::is_nil(helloRef.in()))
      {
         ACE_DEBUG ((LM_DEBUG,
                     ACE_TEXT("Correctly detected nil Object reference.\n")));
         CORBA::Object_var var = CORBA::Object::_duplicate(helloRef.in());
         if (CORBA::is_nil(var))
         {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT("Duplicated Object reference works too.\n")));
            process_result = 0;
         }
         else
         {
            ACE_ERROR_RETURN ((LM_DEBUG,
                               "ERROR:\n%C\n$%C\n",
                               "CORBA::is_nil(Hello_ptr) returned false!"
                               "CORBA::is_nil(Object_var) returned true!"),
                              1);
         }
      }
      else
      {
         CORBA::Object_ptr obj = helloRef.in();
         if (CORBA::is_nil(obj))
         {
            ACE_DEBUG ((LM_DEBUG,
                        "ERROR (causes exception further down):\n%C\n%C\n",
                        "CORBA::is_nil(Hello_ptr) returned false!"
                        "CORBA::is_nil(Object_ptr) returned true!"));
         }
         CORBA::String_var aString = helloRef->get_string();
         ACE_DEBUG ((LM_DEBUG,
                     "Received Hello: %C\n",
                     aString.in()));
      }

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return process_result;
}
