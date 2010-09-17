// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");
int min_timeout = 0;
int max_timeout = 20;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:l:h:"));
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
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int result = 1;
// 1 : fault in test
// 2 : _non_existent() returned false
// 3 : _non_existent() returned true
// 4 : CORBA::OBJECT_NOT_EXIST exception
// 5 : CORBA::TRANSIENT exception

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      CORBA::Boolean non_existent = server->_non_existent ();

      if (non_existent)
          result = 3;
      else
          result = 2;

      ACE_DEBUG ((LM_DEBUG,
                  "client (%P) _non_existent() returned %d\n",
                  static_cast<int>(non_existent) ));

      orb->destroy ();
    }
  catch (const CORBA::OBJECT_NOT_EXIST& ex)
  {
      ex._tao_print_exception ("ERROR: Exception caught:");
      result = 4;
  }
  catch (const CORBA::TRANSIENT&)
  {
      result = 5;
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ERROR: Exception caught:");
      result = 6;
    }
  return result;
}
