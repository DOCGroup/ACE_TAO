#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

static const char *ior = nullptr;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'k':
          ior = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Usage: client -k <ior>\n")),
                            -1);
        }
    }

  if (ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("client: -k <IOR> is required\n")),
                      -1);
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (ior);
      Test::OnewayIdle_var test = Test::OnewayIdle::_narrow (object.in ());
      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Narrow to Test::OnewayIdle failed\n")),
                          1);

      ACE_DEBUG ((LM_INFO, ACE_TEXT ("Sending oneway ping\n")));
      test->ping ();

      orb->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception (ACE_TEXT ("client exception"));
      return 1;
    }

  return 0;
}
