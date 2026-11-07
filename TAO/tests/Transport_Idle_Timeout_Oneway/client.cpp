#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager_T.h"
#include <string>

struct Client_Options
{
  std::basic_string<ACE_TCHAR> ior;
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 'k':
          options.ior = get_opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Usage: client -k <ior>\n")),
                            -1);
        }
    }

  if (options.ior.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) client: -k <IOR> is required\n")),
                      -1);
  return 0;
}

static size_t
cache_size (CORBA::ORB_ptr orb)
{
  TAO_ORB_Core * const core = orb->orb_core ();
  return core->lane_resources ().transport_cache ().current_size ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());
      Test::OnewayIdle_var const test = Test::OnewayIdle::_narrow (object.in ());
      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Narrow to Test::OnewayIdle failed\n")),
                          1);

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) Sending oneway ping\n")));
      test->ping ();

      // Y=1, X=2, server margin=1; allow two more seconds for peer closure.
      ACE_Time_Value run_time (1 + 2 + 1 + 2);
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
  catch (CORBA::Exception const & ex)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) client caught CORBA exception %C\n"),
                  ex._name ()));
      return 1;
    }

  return 0;
}
