#include "Client_Task.h"
#include "Server_Task.h"

#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

#include <memory>

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

  std::unique_ptr<Server_Task> server_task_;
  std::unique_ptr<Client_Task> client_task_;

  bool s_init_;
  bool shutdown_;
};

void MyMain::print_usage ()
{}

bool MyMain::init_server (const ACE_TCHAR* args)
{
  std::string my_args (ACE_TEXT_ALWAYS_CHAR (args));
  // main thread and extra thread for backdoor operations
  int thread_pool = 2;

#ifdef ACE_HAS_CPP14
  server_task_ = std::make_unique<Server_Task> (my_args);
#else
  server_task_.reset (new Server_Task (my_args));
#endif

  ACE_ASSERT (server_task_);

  server_task_->activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, thread_pool);

  int duration = 4; // wait 3 seconds for initialization
  ACE_Time_Value current = ACE_High_Res_Timer::gettimeofday_hr ();
  ACE_Time_Value timeout = current + ACE_Time_Value (duration);

  while (current < timeout)
  {
    if (server_task_->ready ())
    {
      break;
    }
    ACE_Time_Value sleep_time;
    sleep_time.msec (10);
    ACE_OS::sleep (sleep_time);
    current += sleep_time;
  }

  if (!server_task_->ready ())
  {
    server_task_->force_shutdown ();
    server_task_->wait ();
    server_task_.reset ();
    return false;
  }

  return true;
}

bool MyMain::init_client (const ACE_TCHAR* args)
{
  std::string my_args (ACE_TEXT_ALWAYS_CHAR (args));
  int thread_pool = 1;

#ifdef ACE_HAS_CPP14
  client_task_ = std::make_unique<Client_Task> (my_args);
#else
  client_task_.reset (new Client_Task (my_args));
#endif

  ACE_ASSERT (client_task_);

  client_task_->activate (THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, thread_pool);

  return true;
}

MyMain::MyMain (int argc, ACE_TCHAR* argv[])
  : s_init_ (false)
  , shutdown_ (false)
{
  argc--;
  for (int p = 1; p <= argc; p++)
  {
    if (ACE_OS::strcmp (argv[p], ACE_TEXT ("-?")) == 0)
    {
      print_usage ();
      return;
    }

    if (ACE_OS::strcasecmp (argv[p], ACE_TEXT ("-s")) == 0)
    {
      const ACE_TCHAR* s_args = (((p + 1) <= argc) ? argv[p + 1] : 0);
      s_init_ = this->init_server (s_args);
      p++;
    }
    else if (ACE_OS::strcasecmp (argv[p], ACE_TEXT ("-c")) == 0)
    {
      const ACE_TCHAR* s_args = (((p + 1) <= argc) ? argv[p + 1] : 0);
      if (s_init_)
      {
        this->init_client (s_args);
      }
      p++;
    }
  }
}

void MyMain::run ()
{
  if (server_task_)
  {
    server_task_->wait ();
  }

  if (client_task_)
  {
    client_task_->wait ();
  }

  shutdown_ = true;
}

MyMain::~MyMain ()
{
  if (!shutdown_)
  {
    this->run ();
  }
}

int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  MyMain my_main (argc, argv);

  my_main.run ();

  return 0;
}
