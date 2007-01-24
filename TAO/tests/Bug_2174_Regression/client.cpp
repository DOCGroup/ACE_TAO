// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Bug_2174_Regression, client, "$Id$")

const char *ior = "file://test.ior";
int min_timeout = 0;
int max_timeout = 20;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:l:h:");
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int result = 1;
// 1 : fault in test
// 2 : _non_existent() returned false
// 3 : _non_existent() returned true
// 4 : CORBA::OBJECT_NOT_EXIST exception
// 5 : CORBA::TRANSIENT exception

int main (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);

      Simple_Server_var server =
        Simple_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
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
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
  {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ERROR: Exception caught:");
      result = 4;
  }
  ACE_CATCH (CORBA::TRANSIENT, ex)
  {
      result = 5;
  }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ERROR: Exception caught:");
      result = 6;
    }
  ACE_ENDTRY;
  return result;
}
