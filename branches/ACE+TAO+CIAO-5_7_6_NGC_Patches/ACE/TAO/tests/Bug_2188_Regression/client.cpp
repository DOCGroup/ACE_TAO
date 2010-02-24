// $Id$

#include "brokenC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const ACE_TCHAR *ior = ACE_TEXT("file://broken.ior");

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
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ServerAdmin_var saobj;
  CORBA::ORB_var orb;

  try
    {
      orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior);

      saobj = ServerAdmin::_narrow (tmp.in());

      if (CORBA::is_nil (saobj.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerAdmin reference <%s>\n",
                             ior),
                            1);
        }

      ArrayTest_var atobj = saobj->target();

      CharArray_slice * char_array = CharArray_alloc();
      for (int i = 0; i < 10; i++)
        char_array[i] = (char)('a' + i);
      atobj->a_charArray (char_array);

      ACE_DEBUG ((LM_DEBUG, "a_charArray worked OK.\n"));

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  saobj->shutdown();

  orb->destroy ();
  return 0;
}
