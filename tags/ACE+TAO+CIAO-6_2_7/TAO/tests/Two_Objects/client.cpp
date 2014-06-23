// $Id$

#include "Two_ObjectsC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("r:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'r':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-r <ior> "
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
  // Used to declare the CORBA::Environment variable

  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Parse the arguments
      if (parse_args (argc, argv) != 0)
        return 1;

      // Convert the ior string to an object reference.
      // The object reference obtained is a reference to the factory
      // object.
      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      // Narrow the object reference to the appropriate type
      Two_Objects_Test::Object_Factory_var factory =
        Two_Objects_Test::Object_Factory::_narrow(tmp.in ());

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Two_Objects_Test::Object_Factory reference <%s>\n",
                             ior),
                            1);
        }

      Two_Objects_Test::First_var first;
      Two_Objects_Test::Second_var second;

      // Use the factory object to create the first and second interfaces
      first = factory->create_first();
      second = factory->create_second();

      // Call the oneway method
      first->oneway_method ();

      ACE_DEBUG ((LM_DEBUG, "Client : one way call done\n"));

      Two_Objects_Test::Octet_Seq_var reply_seq =
      second->twoway_method ();

      ACE_DEBUG ((LM_DEBUG, "Client : length of returned data is %d\n",
                     reply_seq->length() ));

      second->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
