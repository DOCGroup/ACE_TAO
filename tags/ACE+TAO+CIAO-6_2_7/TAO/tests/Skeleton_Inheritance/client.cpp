// $Id$

#include "idl_3C.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

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

      ::Skeleton_Inheritance::Interface_3_var if_3 =
        ::Skeleton_Inheritance::Interface_3::_narrow(tmp.in ());

      if (CORBA::is_nil (if_3.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ::Skeleton_Inheritance::Interface_1 "
                             "reference <%s>\n",
                             ior),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "Invoking operation 3\n"));
      CORBA::String_var if_result_3 = if_3->operation_3 ();
      ACE_DEBUG ((LM_DEBUG, "Invoking operation 2\n"));
      CORBA::String_var if_result_2 = if_3->operation_2 ();
      ACE_DEBUG ((LM_DEBUG, "Invoking operation 1\n"));
      CORBA::String_var if_result_1 = if_3->operation_1 ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - strings returned:\n\t<%C>\n\t<%C>\n\t<%C>\n",
                  if_result_1.in (),
                  if_result_2.in (),
                  if_result_3.in ()));

      if (if_3->_is_a("no_name"))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: no_name is known.\n"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK: no_name is unknown.\n"));
        }
      const char * name = "IDL:Skeleton_Inheritance/Interface_1:1.0";
      if (if_3->_is_a(name))
        {
          ACE_DEBUG ((LM_DEBUG, "OK: <%C> is known.\n",
                      name));
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "ERROR: <%C> is unknown\n",
                      name));
        }

      if_3->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
