//
// $Id$
//

#include "AMH_Servant.h"
#include "ace/Get_Opt.h"
#include "Timer_Handler.h"


AMH_Servant::AMH_Servant (CORBA::ORB_ptr orb)
  : sleep_time_ (0)
{
  // @@ Mayur, why do you obtain the reactor each time this method is
  //    called?  Why not just cache it once in the constructor.
  //    Furthermore, you don't appear to need the ORB pseudo-reference
  //    anything other than to obtain the Reactor.  Why not just
  //    remove the cached ORB pseudo-reference altogether?
  //
  // Mayur: Good point.  Never crossed my mind :)
  this->reactor_ = orb->orb_core ()->reactor ();
}

AMH_Servant::~AMH_Servant (void)
{
}

int
AMH_Servant::parse_args (int &argc, char **argv)
{
  // *** To get correct behaviour, SET POSIXLY_CORECT=1 on Linux
  // systems!!! ***
  ACE_Get_Opt get_opts (argc, argv, "s:");
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
              for (int i = count_argv; i < argc; i++)
                argv [i] = argv [i+2];
            }

            // Decrement the value of this->argc_ to reflect the removal
            // of '-s' option.
            argc = argc - 2;

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
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      // @@ Mayur, the below Timer_Handler instance will leak if your
      //    schedule_timer() call below fails (which you do not check
      //    for).  If the schedule_timer() call fails, then you should
      //    delete() the Timer_Handler instance.
      // 
      // Done.

      // Handler will 'kill' itself when it is done.
      Timer_Handler *handler = new Timer_Handler (_tao_rh,
                                                  send_time);

      // We assume the schedule_timer method is thread-safe
      long l = this->reactor_->schedule_timer (handler,
                                               (void *) 0,
                                               ACE_Time_Value (0, this->sleep_time_));
      ACE_TRY_CHECK;

      if (l == -1) //schedule timer failed
        {
          // free memory
          delete handler;

          // send a no-op to the client
          _tao_rh->test_method (ACE_static_cast (Test::Timestamp, 0));

          // just in case we add code later on after this if stmt
          return;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Exception in start_test \n");
    }
  ACE_ENDTRY;

  // Footnote: This method is thread-safe:
  // - We have no common state (and don't alter any).
  // - We do things only through function calls which means we are using
  //   the thread-specific stack for that function call.
}

void
AMH_Servant::start_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      _tao_rh->start_test ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Exception in start_test \n");
    }
  ACE_ENDTRY;
}

void
AMH_Servant::end_test (Test::AMH_RoundtripResponseHandler_ptr _tao_rh
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      _tao_rh->end_test ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Exception in end_test \n");
    }
  ACE_ENDTRY;
}


//Global Footnote: This whole class seems to be thread-safe (and look ma, no locks! :)
