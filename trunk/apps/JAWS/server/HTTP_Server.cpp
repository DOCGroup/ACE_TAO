// $Id$

#include "ace/Get_Opt.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"

#include "IO.h"
#include "HTTP_Server.h"

//DONE// James, please make sure that you don't have "free floating"
//DONE// enums since they will inevitably cause portability problems.

class HTTP_Server_Anchor
{
public:
  enum 
  { 
    POOL = 0,
    PER_REQUEST = 1 
  };

  enum 
  { 
    SYNCH = 0,
    ASYNCH = 2 
  };
};

void
HTTP_Server::parse_args (int argc,
			 char *argv[])
{
  int c;
  int thr_strategy = 0;
  int io_strategy = 0;
  char *prog = argc > 0 ? argv[0] : "Sock_Server";

  // Set some defaults
  this->port_ = 0;
  this->threads_ = 0;
  this->backlog_ = 0;
  this->throttle_ = 0;

  ACE_Get_Opt get_opt (argc, argv, "p:n:t:i:b:");

  while ((c = get_opt ()) != -1)
    switch (c) 
      {
      case 'p':
	this->port_ = ACE_OS::atoi (get_opt.optarg);
	break;
      case 'n':
	this->threads_ = ACE_OS::atoi (get_opt.optarg);
	break;
      case 't':
	// POOL        -> thread pool
	// PER_REQUEST -> thread per request
	// THROTTLE    -> thread per request with throttling
        if (ACE_OS::strcmp (get_opt.optarg, "POOL") == 0)
          thr_strategy = HTTP_Server_Anchor::POOL;
        else if (ACE_OS::strcmp (get_opt.optarg, "PER_REQUEST") == 0)
          {
            thr_strategy = HTTP_Server_Anchor::PER_REQUEST;
            this->throttle_ = 0;
          }
        else if (ACE_OS::strcmp (get_opt.optarg, "THROTTLE") == 0)
          {
            thr_strategy = HTTP_Server_Anchor::PER_REQUEST;
            this->throttle_ = 1;
          }
	break;
      case 'i':
	// SYNCH  -> synchronous I/O
	// ASYNCH -> asynchronous I/O
        if (ACE_OS::strcmp (get_opt.optarg, "SYNCH") == 0)
          io_strategy = HTTP_Server_Anchor::SYNCH;
        else if (ACE_OS::strcmp (get_opt.optarg, "ASYNCH") == 0)
          io_strategy = HTTP_Server_Anchor::ASYNCH;
	break;
      case 'b':
	this->backlog_ = ACE_OS::atoi (get_opt.optarg);
	break;
      default:
	break;
      }
  
  if (this->port_ == 0)
    this->port_ = 5432;
  if (this->threads_ == 0)
    this->threads_ = 5;
  if (this->backlog_ == 0)
    this->backlog_ = this->threads_;

  this->strategy_ = thr_strategy + io_strategy;

  prog = prog;
  ACE_DEBUG ((LM_DEBUG,
              "in HTTP_Server::init, %s port = %d, number of threads = %d\n",
              prog, this->port_, this->threads_));
}

int
HTTP_Server::init (int argc, char *argv[])
{
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);
  ACE_UNUSED_ARG (sig);

  this->parse_args (argc, argv);

  switch (this->strategy_) 
    {
    case 2:
      return this->asynch_thread_pool ();      

    case 1: 
      return this->thread_per_request ();
      
    case 0: 
    default: 
      return this->synch_thread_pool ();      
    } 
  return 0;
}

int
HTTP_Server::fini (void)
{
  this->tm_.close ();
  return 0;
}

int
HTTP_Server::synch_thread_pool (void)
{
  if (this->acceptor_.open (ACE_INET_Addr (this->port_), 1,
                            PF_INET, this->backlog_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Acceptor::open"), -1);
  
  for (int i = 0; i < this->threads_; i++) 
    {
      Synch_Thread_Pool_Task *t;

      ACE_NEW_RETURN (t,
		      Synch_Thread_Pool_Task (this->acceptor_, this->tm_),
                      -1);

      if (t->open () != 0) 
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Thread_Pool_Task::open"), -1);
    }      

  this->tm_.wait ();
  return 0;
}

Synch_Thread_Pool_Task::Synch_Thread_Pool_Task (HTTP_Acceptor &acceptor,
                                                ACE_Thread_Manager &tm)
  : ACE_Task<ACE_NULL_SYNCH> (&tm),
    acceptor_ (acceptor)
{
}

int
Synch_Thread_Pool_Task::open (void *args)
{
  ACE_UNUSED_ARG (args);

  if (this->activate (THR_DETACHED | THR_NEW_LWP) == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Synch_Thread_Pool_Task::open"),
                      -1);

  return 0;
}

int
Synch_Thread_Pool_Task::svc (void)
{
  Synch_HTTP_Handler_Factory factory;

  for (;;) 
    {
      ACE_SOCK_Stream stream;

      if (this->acceptor_.accept (stream) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Acceptor::accept"), -1);

      ACE_Message_Block *mb;
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (HTTP_Handler::MAX_REQUEST_SIZE + 1),
                      -1);

      HTTP_Handler *handler = factory.create_http_handler ();
      handler->open (stream.get_handle (), *mb);
      mb->release ();
      ACE_DEBUG ((LM_DEBUG,
                  " (%t) in Synch_Thread_Pool_Task::svc, recycling\n"));
    }
  
  // This stinks, because I am afraid that if I remove this line, some
  // compiler will issue a warning that this routine could exit
  // without returning a value.  But, leaving it in makes the VXWORKS
  // compiler complain about an unreachable statement.

#if ! defined(VXWORKS)
  return 0;
#endif /* VXWORKS */
}

int
HTTP_Server::thread_per_request (void)
{
  int grp_id = -1;

#if 0
  ::thr_create (0, 0,
                Thread_Per_Request_Task::REAPER, (void *) this,
                THR_NEW_LWP, 0);
#endif

  // thread per request
  if (this->acceptor_.open (ACE_INET_Addr (this->port_), 1,
                            PF_INET, this->backlog_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Acceptor::open"), -1);
  
  ACE_SOCK_Stream stream;
  const ACE_Time_Value wait_time (0,10);

  for (;;) 
    {
      if (this->acceptor_.accept (stream) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Acceptor::accept"), -1);

      Thread_Per_Request_Task *t;
      ACE_NEW_RETURN (t, Thread_Per_Request_Task (stream.get_handle (),
                                                  this->tm_),
                      -1);


      if (t->open (&grp_id) != 0) 
	ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n", "Thread_Per_Request_Task::open"),
                          -1);

      // Throttling is not allowing too many threads to run away.
      // Should really use some sort of condition variable here.
      if (!this->throttle_) 
	continue;

      // This works because each task has only one thread.
      while (this->tm_.num_tasks_in_group (grp_id) > this->threads_)
	this->tm_.wait (&wait_time);
    }

  // This stinks, because I am afraid that if I remove this line, some
  // compiler will issue a warning that this routine could exit
  // without returning a value.  But, leaving it in makes the VXWORKS
  // compiler complain about an unreachable statement.

#if ! defined(VXWORKS)
  return 0;
#endif /* VXWORKS */
}

Thread_Per_Request_Task::Thread_Per_Request_Task (ACE_HANDLE handle,
						  ACE_Thread_Manager &tm)
  : ACE_Task<ACE_NULL_SYNCH> (&tm),
    handle_ (handle)
{
}

int
Thread_Per_Request_Task::open (void *args)
{
  int status = -1;
  int *grp_id = &status;

  if (args != 0)
    grp_id = (int *) args;

#if defined (ACE_WIN32)
  if (*grp_id == -1)
    status = *grp_id = this->activate (THR_DETACHED | THR_NEW_LWP);
  else
    status = this->activate (THR_DETACHED | THR_NEW_LWP, 1, 0, -1, *grp_id, 0);
#else
  status = ::thr_create (0, 0,
                         Thread_Per_Request_Task::DEBUG_TPR, (void *) this,
                         THR_DETACHED | THR_DAEMON | THR_NEW_LWP, 0);
#endif

  if (status == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Thread_Per_Request_Task::open"),
                      -1);
  return 0;
}

int
Thread_Per_Request_Task::svc (void)
{
  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb, ACE_Message_Block (HTTP_Handler::MAX_REQUEST_SIZE + 1),
                  -1);
  Synch_HTTP_Handler_Factory factory;
  HTTP_Handler *handler = factory.create_http_handler ();
  handler->open (this->handle_, *mb);
  mb->release ();
  return 0;
}

void *
Thread_Per_Request_Task::DEBUG_TPR (void *task)
{
  Thread_Per_Request_Task *tpr_task = (Thread_Per_Request_Task *) task;
  tpr_task->svc ();
  tpr_task->close (0);
  // ::thr_exit (0);
  return 0;
}

void *
Thread_Per_Request_Task::REAPER (void *task)
{
  Thread_Per_Request_Task *tpr_task = (Thread_Per_Request_Task *) task;
  tpr_task = tpr_task;

#if !defined (ACE_WIN32)
  while (1)
    if (::thr_join(0, 0, 0) == 0)
      ACE_DEBUG ((LM_DEBUG, " (%t) REAPER joined a thread\n"));
#endif

  return 0;
}

int
Thread_Per_Request_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
	      " (%t) Thread_Per_Request_Task::svc, dying\n"));
  delete this;
  return 0;
}

// Understanding the code below requires understanding of the
// WindowsNT asynchronous completion notification mechanism.

// (1) The application submits an asynchronous I/O request to the
//     operating system and a special handle with it (Asynchronous
//     Completion Token).
// (2) The operating system commits to performing the I/O request,
//     while application does its own thing.
// (3) Operating system finishes the I/O request and places ACT onto
//     the I/O Completion Port, which is a queue of finished
//     asynchronous requests.
// (4) The application eventually checks to see if the I/O request
//     is done by checking the I/O Completion Port, and retrieves the
//     ACT.

int
HTTP_Server::asynch_thread_pool (void)
{  
// This only works on Win32
#if defined (ACE_WIN32)
  // Create the appropriate acceptor for this concurrency strategy and
  // an appropriate handler for this I/O strategy
  ACE_Asynch_Acceptor<Asynch_HTTP_Handler_Factory> acceptor;

  // Tell the acceptor to listen on this->port_, which makes an
  // asynchronous I/O request to the OS.
  if (acceptor.open (ACE_INET_Addr (this->port_),
		     HTTP_Handler::MAX_REQUEST_SIZE + 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n",
                       "ACE_Asynch_Acceptor::open"), -1);
  
  // Create the thread pool.
  for (int i = 0; i < this->threads_; i++) 
    {
      // Register threads with the proactor and thread manager.
      Asynch_Thread_Pool_Task *t;
      ACE_NEW_RETURN (t,
		      Asynch_Thread_Pool_Task (*ACE_Service_Config::proactor (),
					       this->tm_),
		      -1);
      if (t->open () != 0) 
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n",
                           "Thread_Pool_Task::open"), -1);
      // The proactor threads are waiting on the I/O Completion Port.
    }   

  // Wait for the threads to finish.
  return this->tm_.wait ();
#endif /* ACE_WIN32 */
  return -1;
}

// This only works on Win32
#if defined (ACE_WIN32)

Asynch_Thread_Pool_Task::Asynch_Thread_Pool_Task (ACE_Proactor &proactor,
                                                  ACE_Thread_Manager &tm)
  : ACE_Task<ACE_NULL_SYNCH> (&tm),
    proactor_ (proactor)
{
}

int
Asynch_Thread_Pool_Task::open (void *args)
{
  if (this->activate () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Asynch_Thread_Pool_Task::open"),
                      -1);

  return 0;
}

int
Asynch_Thread_Pool_Task::svc (void)
{
  for (;;) 
    if (this->proactor_.handle_events () == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Proactor::handle_events"),
                        -1);

  return 0;
}

#endif /* ACE_WIN32 */

// Define the factory function.
ACE_SVC_FACTORY_DEFINE (HTTP_Server)
 
// Define the object that describes the service.
ACE_STATIC_SVC_DEFINE (HTTP_Server, "HTTP_Server", ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (HTTP_Server),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ, 0)

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
