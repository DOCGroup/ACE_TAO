// $Id$

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"

//FUZZ: disable check_for_math_include/
#include <math.h>

static char **requests;
static int number_of_urls;
static int number_of_outstanding_requests;

class HTTP_Sink_Svc_Handler
  : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  int open (void *)
  {
    ACE_Reactor::instance ()
    ->register_handler (this, ACE_Event_Handler::WRITE_MASK);
    return 0;
  }

  int handle_input (ACE_HANDLE handle)
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

  int handle_output (ACE_HANDLE handle)
  {
    int random_number;
    random_number  = (int) ((::rand () / (1.0 + RAND_MAX)) * number_of_urls);
    const char *random_request = requests[random_number];
    size_t random_request_length = ::strlen (random_request);
    ssize_t result = 0;
    result = this->peer ().send_n (random_request, random_request_length);
    ACE_Reactor::instance ()
    ->cancel_wakeup (handle, ACE_Event_Handler::WRITE_MASK);
    if (result < 0)
      return -1;
    ACE_Reactor::instance ()
    ->schedule_wakeup (handle, ACE_Event_Handler::READ_MASK);
    return 0;
  }

};

typedef ACE_Connector <HTTP_Sink_Svc_Handler, ACE_SOCK_CONNECTOR>
        HTTP_Sink_Connector;


class HTTP_Make_Request_Event_Handler
  : public ACE_Event_Handler
{
public:

  HTTP_Make_Request_Event_Handler (const ACE_Time_Value &request_rate,
                                   int number_of_requests = -1,
                                   const char *website = 0)
    : number_of_requests_ (number_of_requests)
    , addr_ (website ? website : "localhost:5432")
  {
    number_of_outstanding_requests = number_of_requests;
    this->timer_id_
      = ACE_Reactor::instance ()->schedule_timer (this, 0, 0, request_rate);
  }

  int handle_timeout (const ACE_Time_Value &, const void *)
  {
    if (this->number_of_requests_ > 0)
      this->number_of_requests_--;

    int tries = 0;
    int result = 0;

    do
      {
        HTTP_Sink_Svc_Handler *handler = 0;
        result = this->connector_.connect (handler, this->addr_,
                                           ACE_Synch_Options::asynch);
        tries++;
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

  int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
  {
    delete this;
    return 0;
  }

private:

  int number_of_requests_;
  int number_of_outstanding_requests_;

  HTTP_Sink_Connector connector_;
  ACE_INET_Addr addr_;

  long timer_id_;

};

class Signal_Handler
  : public ACE_Event_Handler
{
public:

  int handle_signal (int signo, siginfo_t *, ucontext_t *)
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

};

typedef ACE_Select_Reactor_Token_T<ACE_Noop_Token>
        ACE_Select_Reactor_Noop_Token;
typedef ACE_Select_Reactor_T<ACE_Select_Reactor_Noop_Token>
        ACE_Select_NULL_LOCK_Reactor;

int
main (int argc, char *argv[])
{
  ACE_Select_NULL_LOCK_Reactor *select_reactor;
  select_reactor = new ACE_Select_NULL_LOCK_Reactor;
  ACE_Reactor::instance (new ACE_Reactor (select_reactor, 1), 1);

  // Install signal handlers
  Signal_Handler *signal_handler = new Signal_Handler;

  ACE_Reactor::instance ()->register_handler (SIGINT, signal_handler);
  ACE_OS::signal (SIGPIPE, SIG_IGN);

  ACE_Get_Opt options (argc, argv, "f:r:n:w:");

  // f -- file list
  // r -- request rate in requests per second
  // n -- number of requests to generate
  // w -- website in form of hostname:port (e.g., www.yahoo.com:80)

  char *filelist_name = 0;
  double request_rate = 1.0;
  char *website = 0;
  int number_of_requests = 0;

  int c;
  while ((c = options ()) != -1)
    {
      switch (c)
        {
        case 'f':
          filelist_name = options.optarg;
          break;
        case 'r':
          request_rate = ::atof (options.optarg);
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

  if (filelist_name == 0)
    ::abort ();

  double rq_interval_sec;
  double rq_interval_usec;
  rq_interval_usec = 1000000 * ::modf (1/request_rate, &rq_interval_sec);
  ACE_Time_Value tv ((long) rq_interval_sec, (long) rq_interval_usec);

  // Scan file for number of lines.

  FILE *fp = ::fopen (filelist_name, "r+b");
  while ((c = ::fgetc (fp)) != EOF)
    {
      if (c == '\n')
        number_of_urls++;
    }
  ::fclose (fp);

  requests = (char **) ::malloc (number_of_urls * sizeof (char *));

  // Read in the file list and create requests

  int i = 0;
  static char buf[BUFSIZ];
  fp = ::fopen (filelist_name, "r+b");
  while (::fgets (buf, sizeof (buf), fp) != NULL)
    {
      static char rq[BUFSIZ];
      sprintf (rq, "GET /%s\r\n", buf);
      requests[i++] = ::strdup (rq);
    }
  ::fclose (fp);

  // Create a series of requests

  HTTP_Make_Request_Event_Handler *eh;
  eh = new HTTP_Make_Request_Event_Handler (tv, number_of_requests, website);

  while (! ACE_Reactor::instance ()->event_loop_done ())
    ACE_Reactor::instance ()->handle_events ();

  // Cleanup

  for (i = 0; i < number_of_urls; i++)
    ::free (requests[i]);
  ::free (requests);

  return 0;
}

