#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager_T.h"

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
                             ACE_TEXT ("(%P|%t) Usage: client -k <ior>\n")),
                            -1);
        }
    }

  if (ior == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) client: -k <IOR> is required\n")),
                      -1);
  return 0;
}

static size_t
cache_size (CORBA::ORB_ptr orb)
{
  TAO_ORB_Core *core = orb->orb_core ();
  return core->lane_resources ().transport_cache ().current_size ();
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
                           ACE_TEXT ("(%P|%t) Narrow to Test::OnewayIdle failed\n")),
                          1);

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) Sending oneway ping\n")));
      test->ping ();

      ACE_Time_Value run_time (3);
      orb->run (run_time);

      size_t const size = cache_size (orb.in ());
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) client transport cache size after ORB run = %B\n"),
                  size));

      if (size != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ERROR: expected client transport cache size 0, got %B\n"),
                           size),
                          1);

      orb->destroy ();
    }
  catch (const CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) client caught CORBA exception %C\n"),
                  ex._name ()));
      return 1;
    }

  return 0;
}
