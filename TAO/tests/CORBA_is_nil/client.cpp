#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include <iostream>

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
                            "Nil Test::Hello reference <%s>\n",
                            ior),
                           1);
      }

      CORBA::String_var the_string = hello->get_string ();

      std::cout << "Hello from file IOR: " << the_string.in() << std::endl;

      Test::Hello_var helloRef = hello->get_Hello();

      if (CORBA::is_nil(helloRef.in()))
      {
         std::cout << "Correctly detected nil Object reference." << std::endl;
         process_result = 0;
      }
      else
      {
         CORBA::Object_ptr obj = helloRef.in();
         if (CORBA::is_nil(obj))
         {
            std::cout << "CORBA::is_nil(Hello_ptr) returned false!\n"
                      << "CORBA::is_nil(Object_ptr) returned true!" 
                      << std::endl;
         }
         CORBA::String_var aString = helloRef->get_string();
         std::cout << "Received Hello: " << aString.in() << std::endl;
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

