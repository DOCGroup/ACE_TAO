// $Id$

#include "RT_Class.h"

#include "ace/Task.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Get_Opt.h"

int use_rt   = 0;
int nthreads = 1;
ACE_TCHAR const * hi_endpoint = ACE_TEXT ("localhost:12345");
ACE_TCHAR const * lo_endpoint = ACE_TEXT ("localhost:23456");

int
parse_args (int argc, ACE_TCHAR *argv[]);

class Task : public ACE_Task_Base
{
public:
  Task (ACE_TCHAR const *endpoint);

  virtual int svc();

private:
  ACE_TCHAR const *endpoint_;

  ACE_Reactor reactor_;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  /// Move the test to the real-time class if it is possible.
  RT_Class rt_class;

  if (parse_args (argc, argv) != 0)
    return 1;

  Task hi_task (hi_endpoint);
  Task lo_task (lo_endpoint);

  hi_task.activate(rt_class.thr_sched_class() | THR_NEW_LWP | THR_JOINABLE,
                   1, 1, rt_class.priority_high());

  lo_task.activate(rt_class.thr_sched_class() | THR_NEW_LWP | THR_JOINABLE,
                   nthreads, 1, rt_class.priority_low());
  lo_task.wait();

  hi_task.wait();

  return 0;
}

// ****************************************************************

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  Svc_Handler(ACE_Reactor * reactor = 0);

  virtual int handle_input(ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);
};

typedef ACE_Acceptor<Svc_Handler,ACE_SOCK_ACCEPTOR> Acceptor;

Svc_Handler::Svc_Handler (ACE_Reactor * reactor)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH> (0, 0, reactor)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Accepted connection\n"));
}

int
Svc_Handler::handle_input(ACE_HANDLE h)
{
  const size_t bufsize = BUFSIZ;
  char buf[bufsize];

  while(1)
    {
      ssize_t n = this->peer().recv(buf, bufsize);
      if (n == 0) {
        ACE_ERROR((LM_ERROR,
                   "Connection %d closed while reading data\n",
                   h, ""));
        break;
      } else if (n == -1) {
        if(errno == EWOULDBLOCK)
          return 0;

        ACE_ERROR((LM_ERROR,
                   "Error on <%d> while reading %p\n",
                   h, ""));
        break;
      }

      ssize_t k = this->peer().send(buf, n);
      if(k == 0) {
        ACE_ERROR((LM_ERROR,
                   "Connection <%d> closed while writing\n",
                   h, ""));
        break;
      } else if (k == -1) {
        if(errno == EWOULDBLOCK)
          return 0;

        ACE_ERROR((LM_ERROR,
                   "Error on <%d> while writing %p\n",
                   h, ""));
        break;
      } else if (k != n) {
        ACE_ERROR((LM_ERROR,
                   "Short write on <%d>\n",
                   h, ""));
        break;
      }
    }

  return -1;
}

int
Svc_Handler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask m)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Closed connection\n"));
  this->peer().close();
  int r =
    this->ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>::handle_close (h,m);
  return r;
}

// ****************************************************************

Task::Task(ACE_TCHAR const * endpoint)
  : endpoint_ (endpoint)
  , reactor_ (new ACE_TP_Reactor)
{
  ACE_INET_Addr local_sap (endpoint_);
  Acceptor * acceptor = new Acceptor;

  if(acceptor->open(local_sap, &reactor_, ACE_NONBLOCK) == -1)
    {
      ACE_ERROR((LM_ERROR, "Cannot open endpoint <%s>\n", endpoint_));
    }
}

int
Task::svc()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting thread for %s\n", endpoint_));
  (void) reactor_.run_reactor_event_loop();
  return 0;
}

// ****************************************************************

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("h:l:n:r"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        hi_endpoint = get_opts.opt_arg ();
        break;

      case 'l':
        lo_endpoint = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'r':
        use_rt = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-r (use RT-CORBA) "
                           "-n nthreads "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}
