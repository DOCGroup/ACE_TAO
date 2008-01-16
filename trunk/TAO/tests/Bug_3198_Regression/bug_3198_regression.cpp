// $Id$

#include "tao/ORB_Core.h"
#include "tao/SystemException.h"
#include "tao/PolicyC.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Bug_3198_Regression,
           bug_3198_regression,
           "$Id$")

const char *ior = "corbaloc:iiop:localhost:12345/MyObjectKey";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
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


int main (int argc, char *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

    CORBA::Object_var obj = orb->string_to_object (ior);
    CORBA::PolicyList_var policies;
    CORBA::Boolean result = obj->_validate_connection (policies.out());
    if (result)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) got true from _validate_connection\n"),
                          1);
    }
  }
  catch (const CORBA::TRANSIENT&)
  {
    ACE_DEBUG ((LM_DEBUG, "Caught transient as expected\n"));
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Exception caught:");
    return 1;
  }
  return 0;
}










































