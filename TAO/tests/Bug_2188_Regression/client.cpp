#include "brokenC.h"
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

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior = ACE_TEXT ("file://broken.ior");
  ServerAdmin_var saobj;
  CORBA::ORB_var orb;

  try
    {
      orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, ior) != 0)
        return 1;

      CORBA::Object_var const tmp =
        orb->string_to_object (ior);

      saobj = ServerAdmin::_narrow (tmp.in ());

      if (CORBA::is_nil (saobj.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerAdmin reference <%s>\n",
                             ior),
                            1);
        }

      ArrayTest_var const atobj = saobj->target ();

      CharArray_var char_array = CharArray_alloc ();
      for (int i {}; i < 10; ++i)
        char_array.inout ()[i] = static_cast<CORBA::Char> ('a' + i);
      atobj->a_charArray (char_array.in ());

      ACE_DEBUG ((LM_DEBUG, "a_charArray worked OK.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
    }

  if (!CORBA::is_nil (saobj.in ()))
    saobj->shutdown ();

  if (!CORBA::is_nil (orb.in ()))
    orb->destroy ();

  return 0;
}
