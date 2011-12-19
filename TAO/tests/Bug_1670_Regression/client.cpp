/**
 * @file client.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "TestC.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      Baz::C_var cobject =
        Baz::C::_narrow (object.in ());

      if (CORBA::is_nil (cobject.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                                "Cannot narrow Baz::C object <%s>\n",
                                ior),
                            1);
        }

      CORBA::Long result;
      result = cobject->op1 ();
      ACE_ASSERT(result == 1);

      result = cobject->op2 ();
      ACE_ASSERT(result == 2);

      result = cobject->op3 ();
      ACE_ASSERT(result == 3);

      result = cobject->op4 ();
      ACE_ASSERT(result == 4);

      Foo::Bar::B_var bobject =
        Foo::Bar::B::_narrow (object.in ());

      if (CORBA::is_nil (bobject.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                                "Cannot narrow Foo::Bar::B object <%s>\n",
                                ior),
                            1);
        }

      result = bobject->op3 ();
      ACE_ASSERT(result == 3);

      Foo::Bar::A_var aobject =
        Foo::Bar::A::_narrow (object.in ());

      if (CORBA::is_nil (aobject.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                                "Cannot narrow Foo::Bar::A object <%s>\n",
                                ior),
                            1);
        }

      result = aobject->op1 ();
      ACE_ASSERT(result == 1);

      result = aobject->op2 ();
      ACE_ASSERT(result == 2);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("");
      return 1;
    }

  return 0;
}
