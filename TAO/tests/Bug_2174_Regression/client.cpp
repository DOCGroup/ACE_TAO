#include "testC.h"
#include "ace/Get_Opt.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://test.ior");
  // 1 : fault in test
  // 2 : _non_existent() returned false
  // 3 : _non_existent() returned true
  // 4 : CORBA::OBJECT_NOT_EXIST exception
  // 5 : CORBA::TRANSIENT exception
  int result {1};

  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const object =
        orb->string_to_object (ior);

      Simple_Server_var const server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      CORBA::Boolean const non_existent = server->_non_existent ();

      if (non_existent)
        result = 3;
      else
        result = 2;

      ACE_DEBUG ((LM_DEBUG,
                  "client (%P) _non_existent() returned %d\n",
                  static_cast<int> (non_existent)));

      orb->destroy ();
    }
  catch (CORBA::OBJECT_NOT_EXIST const &ex)
    {
      ex._tao_print_exception ("ERROR: Exception caught:");
      result = 4;
    }
  catch (CORBA::TRANSIENT const &)
    {
      result = 5;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR: Exception caught:");
      result = 6;
    }
  return result;
}
