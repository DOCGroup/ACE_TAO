//
// $Id$
//

#include "AMH_Servant.h"
#include "ace/Get_Opt.h"
#include "Timer_Handler.cpp"

AMH_Servant::AMH_Servant (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , sleep_time_ (0)
{
}

int
AMH_Servant::parse_args (int *argc, char **argv)
{
  // *** To get correct behaviour, SET POSIXLY_CORECT=1 on Linux
  // systems!!! ***
  ACE_Get_Opt get_opts (*argc, argv, "s:");
  int c;

  int count_argv = 0;
  while ((c = get_opts ()) != -1)
    {
      ++count_argv;
      switch (c)
        {
        case 's':
          {
            int time = ACE_OS::atoi (get_opts.opt_arg ());
            if (time >= 0)
              this->sleep_time_ = time;

            // Remove the option '-s' from argv []
            // to avoid any confusion that might result.
            {
              for (int i = count_argv; i <  *argc; i++)
                argv [i] = argv [i+2];
            }

            // Decrement the value of this->argc_ to reflect the removal
            // of '-s' option.
            *argc = *argc - 2;

            return 1;
          }

        case '?':
        default:
          // Don't do anything.
          break;
        }
    }
  return 0;
}

// Note: Only the following three methods will be called in the event
// loop.  If we make sure these three methods are thread safe, the
// entire class is thread-safe.

void
AMH_Servant::test_method (Test::AMH_RoundtripResponseHandler_ptr _tao_rh,
                                  Test::Timestamp send_time
                                  TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Handler will 'kill' itself when it is done.
  Timer_Handler *handler = new Timer_Handler (send_time);
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();

  reactor->schedule_timer (handler,
                           (void *) Test::AMH_RoundtripResponseHandler::_duplicate (_tao_rh),
                           ACE_Time_Value (0, this->sleep_time_));

  // Footnote: This method is thread-safe:
  // - We have no common state (and don't alter any).
  // - We allocate memory only on the heap and manipulate only that memory.
  // - We do things only through function calls which means we are using
  //   the thread-specific stack for that function call.
}

void
AMH_Servant::start_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                         TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->start_test ();
}

void
AMH_Servant::end_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                         TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  _tao_rh->end_test ();
}


//Global Footnote: This whole class seems to be thread-safe (and look ma, no locks! :)
