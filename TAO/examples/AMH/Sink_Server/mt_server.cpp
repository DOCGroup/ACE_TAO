// $Id$

#include "ace/Task.h"
#include "ace/Get_Opt.h"

#include "Base_Server.h"
#include "AMH_Servant.h"

void
usage (char *message)
{
  static const char * usage = 
  "invoke as: mt_server -o <ior_output_file> \n -n <num_threads>\n -s <sleep_time (in microseconds)> \n";

  ACE_ERROR ((LM_ERROR, "%s : %s", message, usage));
}

class MT_AMH_Server
  : public Base_Server
  , public ACE_Task_Base
{
public:
  MT_AMH_Server (int* argc, char **argv)
    : Base_Server (argc, argv) {}

  ~MT_AMH_Server () {}

  // We need to parse an extra thread_count paramter for multi-threraded servers
  virtual int parse_args (void)
  {

    // Let the base server parse it's argumrents first
    if (Base_Server::parse_args () != 1)
      {
        usage (ACE_const_cast (char *, ""));
        ACE_OS::exit (1);
      }

    ACE_Get_Opt get_opts (*(this->argc_), this->argv_, "n:");
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
                for (int i = count_argv; i <= *(this->argc_); ++i)
                  this->argv_ [i] = this->argv_ [i+2];
              }

              // Decrement the value of this->argc_ to reflect the removal
              // of '-n' option.
              *(this->argc_) = *(this->argc_) - 2;
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

  void start_threads (void)
  {
    // Each of this thread runs the event loop
    this->activate (THR_NEW_LWP | THR_JOINABLE, this->nthreads_, 1);
    this->thr_mgr ()->wait ();
  }

  // the service method
  virtual int svc (void)
  {
    run_event_loop ();
    return 1;
  }

private:
  int nthreads_;
};



int
main (int argc, char *argv[])
{
  MT_AMH_Server amh_server (&argc, argv);
  amh_server.try_RT_scheduling();
  amh_server.start_orb_and_poa ();

  if (amh_server.parse_args () != 1)
    {
        usage (ACE_const_cast (char*, ""));
        ACE_OS::exit (1);
    }

  AMH_Servant servant (amh_server.orb ());

  if (servant.parse_args (&argc, argv) != 1)
    {
        usage (ACE_const_cast (char*, "sleep time unspecified"));
        ACE_OS::exit (1);
    }

  amh_server.register_servant (&servant);
  amh_server.start_threads ();

  return 1;
}
