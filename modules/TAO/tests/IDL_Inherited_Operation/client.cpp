// $Id$

#include "InheritedOpC.h"
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  Outer::Derived_var target;

  try
    {
      orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp = orb->string_to_object (ior);

      target = Outer::Derived::_narrow (tmp.in ());

      if (CORBA::is_nil (target.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Outer::Derived ")
                             ACE_TEXT ("reference <%s>\n"),
                             ior),
                            1);
        }

      target->BaseOp ();
      target->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::BAD_OPERATION&)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("client - target operation ")
                  ACE_TEXT ("BaseOp returned BAD_OPERATION\n")));
      target->shutdown ();
      orb->destroy  ();
      return 1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client - Exception caught:");
      target->shutdown ();
      orb->destroy  ();
      return 1;
    }

  return 0;
}
