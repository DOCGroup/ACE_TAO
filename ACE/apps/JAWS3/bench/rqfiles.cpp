#include <cmath>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "ace/Get_Opt.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Select_Reactor_T.h"
#include "ace/Reactor_Token_T.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"

static std::vector<std::string> requests;
static int number_of_outstanding_requests {};

class HTTP_Sink_Svc_Handler
  : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  HTTP_Sink_Svc_Handler () = default;

  //FUZZ: disable check_for_lack_ACE_OS
  int open (void *) override
  {
  //FUZZ: enable check_for_lack_ACE_OS
    ACE_Reactor::instance ()
    ->register_handler (this, ACE_Event_Handler::WRITE_MASK);
    return 0;
  }

  int handle_input (ACE_HANDLE handle) override
  {
    char buf[8 * 1024];
    int result = this->peer ().recv (buf, sizeof (buf));
    if (result <= 0)
      {
        ACE_Reactor::instance ()
        ->cancel_wakeup (handle, ACE_Event_Handler::READ_MASK);

        if (--number_of_outstanding_requests == 0)
          ACE_Reactor::instance ()->end_event_loop ();

        return -1;
      }
    return 0;
  }

  int handle_output (ACE_HANDLE handle) override
  {
    int const random_number = static_cast<int> (
      (ACE_OS::rand () / (1.0 + RAND_MAX)) * requests.size ());
    std::string const &random_request = requests[random_number];
    ssize_t const result = this->peer ().send_n (random_request.c_str (), random_request.size ());
    ACE_Reactor::instance ()
    ->cancel_wakeup (handle, ACE_Event_Handler::WRITE_MASK);
    if (result < 0)
      return -1;
    ACE_Reactor::instance ()
    ->schedule_wakeup (handle, ACE_Event_Handler::READ_MASK);
    return 0;
  }

  HTTP_Sink_Svc_Handler (HTTP_Sink_Svc_Handler const &) = delete;
  HTTP_Sink_Svc_Handler (HTTP_Sink_Svc_Handler &&) = delete;
  HTTP_Sink_Svc_Handler &operator= (HTTP_Sink_Svc_Handler const &) = delete;
  HTTP_Sink_Svc_Handler &operator= (HTTP_Sink_Svc_Handler &&) = delete;
};

using HTTP_Sink_Connector = ACE_Connector<HTTP_Sink_Svc_Handler, ACE_SOCK_CONNECTOR>;

class HTTP_Make_Request_Event_Handler
  : public ACE_Event_Handler
{
public:
  explicit HTTP_Make_Request_Event_Handler (ACE_Time_Value const &request_rate,
                                            int number_of_requests = -1,
                                            char const *website = nullptr)
    : number_of_requests_ (number_of_requests)
    , addr_ (website ? website : "localhost:5432")
    , timer_id_ (ACE_Reactor::instance ()->schedule_timer (
                   this, nullptr, ACE_Time_Value::zero, request_rate))
  {
    number_of_outstanding_requests = number_of_requests;
  }

  ~HTTP_Make_Request_Event_Handler () override
  {
    if (this->timer_id_ != -1)
      ACE_Reactor::instance ()->cancel_timer (this->timer_id_);
  }

  int handle_timeout (ACE_Time_Value const &, void const *) override
  {
    if (this->number_of_requests_ > 0)
      this->number_of_requests_--;

    int tries = 0;
    int result = 0;

    do
      {
        HTTP_Sink_Svc_Handler *handler {};
        result = this->connector_.connect (handler, this->addr_,
                                           ACE_Synch_Options::asynch);
    ++tries;
      }
    while (result < 0 && tries < 100);

    if (ACE_Reactor::instance ()->event_loop_done ())
      this->number_of_requests_ = 0;

    if (this->number_of_requests_ == 0 || result < 0)
      ACE_Reactor::instance ()->cancel_timer (this->timer_id_);

    if (result < 0)
      {
        ACE_Reactor::instance ()->end_event_loop ();
        return -1;
      }

    return 0;
  }

private:
  int number_of_requests_;

  HTTP_Sink_Connector connector_;
  ACE_INET_Addr addr_;

  long timer_id_;

  HTTP_Make_Request_Event_Handler (HTTP_Make_Request_Event_Handler const &) = delete;
  HTTP_Make_Request_Event_Handler (HTTP_Make_Request_Event_Handler &&) = delete;
  HTTP_Make_Request_Event_Handler &operator= (HTTP_Make_Request_Event_Handler const &) = delete;
  HTTP_Make_Request_Event_Handler &operator= (HTTP_Make_Request_Event_Handler &&) = delete;
};

class Signal_Handler
  : public ACE_Event_Handler
{
public:
  Signal_Handler () = default;

  int handle_signal (int signo, siginfo_t *, ucontext_t *) override
  {
    switch (signo)
      {
      case SIGINT:
        ACE_Reactor::instance ()->end_event_loop ();
        break;

      default:
        break;
      }

    return 0;
  }

  Signal_Handler (Signal_Handler const &) = delete;
  Signal_Handler (Signal_Handler &&) = delete;
  Signal_Handler &operator= (Signal_Handler const &) = delete;
  Signal_Handler &operator= (Signal_Handler &&) = delete;
};

using ACE_Select_Reactor_Noop_Token = ACE_Reactor_Token_T<ACE_Noop_Token>;
using ACE_Select_NULL_LOCK_Reactor = ACE_Select_Reactor_T<ACE_Select_Reactor_Noop_Token>;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  std::unique_ptr<ACE_Select_NULL_LOCK_Reactor> select_reactor {
    std::make_unique<ACE_Select_NULL_LOCK_Reactor> ()};
  std::unique_ptr<ACE_Reactor> reactor {
    std::make_unique<ACE_Reactor> (select_reactor.get (), true)};
  // The reactor owns its implementation; the singleton owns the reactor.
  select_reactor.release ();
  ACE_Reactor::instance (reactor.get (), true);
  reactor.release ();

  // Install signal handlers
  Signal_Handler signal_handler;

  ACE_Reactor::instance ()->register_handler (SIGINT, &signal_handler);
  ACE_OS::signal (SIGPIPE, SIG_IGN);

  ACE_Get_Opt options (argc, argv, ACE_TEXT("f:r:n:w:"));

  // f -- file list
  // r -- request rate in requests per second
  // n -- number of requests to generate
  // w -- website in form of hostname:port (e.g., www.yahoo.com:80)

  char const *filelist_name {};
  double request_rate {1.0};
  char const *website {};
  int number_of_requests {};

  int c {};
  while ((c = options ()) != -1)
    {
      switch (c)
        {
        case 'f':
          filelist_name = options.optarg;
          break;
        case 'r':
          request_rate = ACE_OS::atof (options.optarg);
          if (request_rate == 0.0)
            request_rate = 1.0;
          break;
        case 'n':
          number_of_requests = ACE_OS::atoi (options.optarg);
          break;
        case 'w':
          website = options.optarg;
          break;
        default:
          break;
        }
    }

  if (filelist_name == nullptr)
    ACE_OS::abort ();

  double rq_interval_sec {};
  double const rq_interval_usec = 1000000 * std::modf (1 / request_rate, &rq_interval_sec);
  ACE_Time_Value const tv (static_cast<long> (rq_interval_sec), static_cast<long> (rq_interval_usec));

  // Read in the file list and create requests

  char buf[BUFSIZ] {};
    {
      std::unique_ptr<FILE, int (*) (FILE *)> fp (
        std::fopen (filelist_name, "r"), std::fclose);
      if (!fp)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot open request file %s\n", filelist_name), 1);
      while (std::fgets (buf, sizeof (buf), fp.get ()) != nullptr)
        {
          requests.emplace_back ("GET /");
          requests.back () += buf;
          requests.back () += "\r\n";
        }
    }
  if (requests.empty ())
    ACE_ERROR_RETURN ((LM_ERROR, "Request file %s contains no requests\n", filelist_name), 1);

  // Create a series of requests

  std::unique_ptr<HTTP_Make_Request_Event_Handler> request_handler =
    std::make_unique<HTTP_Make_Request_Event_Handler> (tv, number_of_requests, website);

  while (!ACE_Reactor::instance ()->event_loop_done ())
    ACE_Reactor::instance ()->handle_events ();
  ACE_Reactor::instance ()->remove_handler (SIGINT, ACE_Event_Handler::SIGNAL_MASK);

  return 0;
}

