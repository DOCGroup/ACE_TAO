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
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Mayur, the below Timer_Handler instance will leak if your
  //    schedule_timer() call below fails (which you do not check
  //    for).  If the schedule_timer() call fails, then you should
  //    delete() the Timer_Handler instance.

  // Handler will 'kill' itself when it is done.
  Timer_Handler *handler = new Timer_Handler (send_time);

  // @@ Mayur, why do you obtain the reactor each time this method is
  //    called?  Why not just cache it once in the constructor.
  //    Furthermore, you don't appear to need the ORB pseudo-reference
  //    anything other than to obtain the Reactor.  Why not just
  //    remove the cached ORB pseudo-reference altogether?
  ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();

  // @@ Mayur, you never check the return value of the below
  //    schedule_timer() call.

  // @@ Mayur, it's not clear why you're passing a response handler
  //    reference as the argument to the handle_timeout() method.
  //    There doesn't appear any point in doing so since you create a
  //    new Timer_Handler each time this method is executed.  Why
  //    don't you just cache the reference in the Timer_Handler
  //    itself inside a "_var"?  That way you can duplicate the
  //    reference in the "right" place, i.e. the callee duplicate the
  //    object reference if it wants to hold on to it, not the caller.
  //    While this isn't strictly necessary since schedule_timer()
  //    isn't an IDL-defined method, it does make the code clearer for
  //    those who understand the C++ mapping.  Doing so also helps
  //    close a potential memory leak that can occur if the below
  //    schedule_timer() call fails (which you do not check for!).

  // We assume the schedule_timer method is thread-safe
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
                         ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Mayur you're not passing the emulated exception parameter.
  _tao_rh->start_test ();
}

void
AMH_Servant::end_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                         ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Mayur you're not passing the emulated exception parameter.
  _tao_rh->end_test ();
}


//Global Footnote: This whole class seems to be thread-safe (and look ma, no locks! :)
