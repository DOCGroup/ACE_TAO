// $Id$

#include "Server_Task.h"
#include "Client_Task.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/High_Res_Timer.h"
#include "ace/Auto_Ptr.h"

class MyMain
{
public:
  MyMain (int argc, ACE_TCHAR* argv[]);
  ~MyMain ();

  void run ();

private:
  void print_usage ();
  bool init_server (const ACE_TCHAR* args);
  bool init_client (const ACE_TCHAR* args);

  std::auto_ptr<Server_Task> server_task_;
  std::auto_ptr<Client_Task> client_task_;

  bool s_init_;
  bool shutdown_;
};

void
MyMain::print_usage ()
{
}

bool
MyMain::init_server (const ACE_TCHAR* args)
{
  std::string my_args (args);
  // main thread and extra thread for backdoor operations
  int thread_pool = 2;

  ACE_auto_ptr_reset (server_task_, new Server_Task (my_args));
  ACE_ASSERT (server_task_.get() != 0);

  server_task_->activate (THR_NEW_LWP | THR_JOINABLE |THR_INHERIT_SCHED
                          , thread_pool);

  int duration = 4; //wait 3 seconds for initialization
  ACE_Time_Value current = ACE_High_Res_Timer::gettimeofday_hr ();
  ACE_Time_Value timeout = current + ACE_Time_Value(duration);

  while (current < timeout) {
    if (server_task_->ready()) {
      break;
    }
    ACE_Time_Value sleep_time;
    sleep_time.msec (10);
    ACE_OS::sleep (sleep_time);
    current += sleep_time;
  }

  if (!server_task_->ready()) {
    server_task_->force_shutdown ();
    ACE_auto_ptr_reset (server_task_, (Server_Task*)0);
    return false;
  }

  return true;
}

bool
MyMain::init_client (const ACE_TCHAR* args)
{
  std::string my_args (args);
  int thread_pool = 1;

  ACE_auto_ptr_reset (client_task_, new Client_Task (my_args));
  ACE_ASSERT (client_task_.get() != 0);

  client_task_->activate (THR_NEW_LWP | THR_JOINABLE |THR_INHERIT_SCHED
                          , thread_pool);

  return true;
}

MyMain::MyMain (int argc, ACE_TCHAR* argv[])
  : s_init_ (false), shutdown_ (false)
{
  argc--;
  for (int p = 1; p <= argc; p++)
    {
      if (ACE_OS::strcmp (argv[p], "-?") == 0) {
        print_usage ();
        return;
      }

      if (ACE_OS::strcasecmp (argv[p], "-s") == 0) {
        const ACE_TCHAR* s_args = (((p+1) <= argc) ? argv[p+1] : 0);
        s_init_ = this->init_server (s_args);
        p++;
      }
      else if (ACE_OS::strcasecmp (argv[p], "-c") == 0) {
        const ACE_TCHAR* s_args = (((p+1) <= argc) ? argv[p+1] : 0);
        if (s_init_) {
          this->init_client (s_args);
        }
        p++;
      }
    }
}

void
MyMain::run ()
{
  if (server_task_.get() != 0) {
    server_task_->wait ();
  }

  if (client_task_.get() != 0) {
    client_task_->wait ();
  }

  shutdown_ = true;
}

MyMain::~MyMain ()
{
  if (!shutdown_) {
    this->run ();
  }
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  MyMain my_main (argc, argv);

  my_main.run ();

  return 0;
}
