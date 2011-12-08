//
// $Id$
//

#include "MT_AMH_Server.h"
#include "tao/Strategies/advanced_resource.h"

#include "ace/Task.h"
#include "ace/Get_Opt.h"

MT_AMH_Server::MT_AMH_Server (int &argc, ACE_TCHAR **argv)
  : Base_Server (argc, argv)
{
}

MT_AMH_Server::~MT_AMH_Server (void)
{
}

void
MT_AMH_Server::usage (const char *message)
{
  static const char * usage =
    "invoke as: mt_server -o <ior_output_file>\n"
    " -n <num_threads>\n"
    "-s <sleep_time (in microseconds)>\n";

  // @@ Mayur, why don't you just place the usage message directly in
  //    the below ACE_ERROR macro?  It's not a big deal.  It's just
  //    something we normally do.
  //
  // Mayur: Seems cleaner to me this way.
  ACE_ERROR ((LM_ERROR, "%C : %C", message, usage));
}


int
MT_AMH_Server::parse_args (void)
{

  // Let the base server parse it's argumrents first
  if (Base_Server::parse_args () != 1)
    {
      this->usage ("");
      ACE_OS::exit (1);
    }

  ACE_Get_Opt get_opts (this->argc_, this->argv_, ACE_TEXT("n:"));
  int c;
  int count_argv = 0;

  while ((c = get_opts ()) != -1)
    {
      ++count_argv;
      switch (c)
        {
        case 'n':
          {
            this->nthreads_ = ACE_OS::atoi (get_opts.opt_arg ());

            {
              // Added unneeded '{ & }' just to satisfy Win32
              for (int i = count_argv; i <= this->argc_; ++i)
                this->argv_ [i] = this->argv_ [i+2];
            }

            // Decrement the value of this->argc_ to reflect the removal
            // of '-n' option.
            this->argc_ = this->argc_ - 2;
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

void
MT_AMH_Server::start_threads (void)
{
  // Each of this thread runs the event loop
  this->activate (THR_NEW_LWP | THR_JOINABLE, this->nthreads_, 1);
  this->thr_mgr ()->wait ();
}

int
MT_AMH_Server::svc (void)
{
  run_event_loop ();
  return 1;
}
