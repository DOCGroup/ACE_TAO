// HTTP_Server.cpp

#include "ace/Get_Opt.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"

#include "JAWS/server/IO.h"
#include "JAWS/server/HTTP_Server.h"

#if !defined (HTTP_SERVER_C)
#define HTTP_SERVER_C

template <class LOCK> int
LOCK_SOCK_Acceptor<LOCK>::accept (ACE_SOCK_Stream &ns,
                                  ACE_Addr *ra,
                                  ACE_Time_Value *to,
                                  int r) const
{
  ACE_Guard<LOCK> m ((LOCK &)this->lock_);

  return ACE_SOCK_Acceptor::accept (ns, ra, to, r);
}

void
HTTP_Server::parse_args (int argc, char *argv[])
{
  int c;
  char *prog = argc > 0 ? argv[0] : "Sock_Server";

  // Set some defaults
  this->port_ = 0;
  this->threads_ = 0;

  ACE_Get_Opt get_opt(argc, argv, "p:n:s:");
  while ((c = get_opt()) != -1)
    switch (c) 
      {
      case 'p':
	this->port_ = ACE_OS::atoi(get_opt.optarg);
	break;
      case 'n':
	this->threads_ = ACE_OS::atoi(get_opt.optarg);
	break;
      case 's':
	// 0 -> synch thread pool
	// 1 -> thread per request
	// 2 -> asynch thread pool
	this->strategy_ = ACE_OS::atoi(get_opt.optarg);
	break;
      default:
	break;
      }
  
  if (this->port_ == 0) this->port_ = 5432;
  if (this->threads_ == 0) this->threads_ = 5;
  
  ACE_DEBUG ((LM_DEBUG, "in HTTP_Server::init, %s port = %d, number of threads = %d\n",
              prog, this->port_, this->threads_));
  
}

int
HTTP_Server::init (int argc, char *argv[])
{
#if !defined (ACE_WIN32)
  sigignore (SIGPIPE);
#endif

  this->parse_args (argc, argv);

  if (this->acceptor_.open (ACE_INET_Addr (this->port_), 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Acceptor::open"), -1);
  
  switch (this->strategy_) 
    {
    case 2:
      return this->asynch_thread_pool ();      
      break;

    case 1: 
      return this->thread_per_request ();
      break;
      
    case 0: 
    default: 
      return this->synch_thread_pool ();      
      break;
    } 
  return 0;
}

int
HTTP_Server::fini (void)
{
  this->tm_.close();
  return 0;
}

int
HTTP_Server::synch_thread_pool (void)
{
  for (int i = 0; i < this->threads_; i++) 
    {
      Synch_Thread_Pool_Task *t;
      ACE_NEW_RETURN (t, Synch_Thread_Pool_Task (this->acceptor_, this->tm_), -1);
      if (t->open () != 0) 
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Thread_Pool_Task::open"), -1);
    }      
  this->tm_.wait();
  return 0;
}

Synch_Thread_Pool_Task::Synch_Thread_Pool_Task (HTTP_Acceptor &acceptor, ACE_Thread_Manager &tm)
  : ACE_Task<ACE_NULL_SYNCH>(&tm), acceptor_(acceptor)
{
}

int
Synch_Thread_Pool_Task::open (void *args)
{
  if (this->activate () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Synch_Thread_Pool_Task::open"), -1);

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
      ACE_NEW_RETURN (mb, ACE_Message_Block (HTTP_Handler::MAX_REQUEST_SIZE + 1), -1);
      HTTP_Handler *handler = factory.create_http_handler ();
      handler->open (stream.get_handle (), *mb);
      mb->release ();
      ACE_DEBUG ((LM_DEBUG, "(%t) in Synch_Thread_Pool_Task::svc, recycling\n"));
    }
  
  return 0;
}

int
HTTP_Server::thread_per_request (void)
{
  // thread per request
  for (;;) 
    {
      ACE_SOCK_Stream stream;
      if (this->acceptor_.accept (stream) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "HTTP_Acceptor::accept"), -1);
      Thread_Per_Request_Task *t;
      ACE_NEW_RETURN (t, Thread_Per_Request_Task (stream.get_handle (), this->tm_), -1);
      if (t->open () != 0) 
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Thread_Per_Request_Task::open"), -1);
    }      
  return 0;
}

Thread_Per_Request_Task::Thread_Per_Request_Task (ACE_HANDLE handle,
						  ACE_Thread_Manager &tm)
  : ACE_Task<ACE_NULL_SYNCH>(&tm),
    handle_ (handle)
{
}

int
Thread_Per_Request_Task::open (void *args)
{
  if (this->activate () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Thread_Per_Request_Task::open"), -1);

  return 0;
}

int
Thread_Per_Request_Task::svc (void)
{
  ACE_Message_Block *mb;
  ACE_NEW_RETURN (mb, ACE_Message_Block (HTTP_Handler::MAX_REQUEST_SIZE + 1), -1);
  Synch_HTTP_Handler_Factory factory;
  HTTP_Handler *handler = factory.create_http_handler ();
  handler->open (this->handle_, *mb);
  mb->release ();
  return 0;
}

int
Thread_Per_Request_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread_Per_Request_Task::svc, dying\n"));
  delete this;
  return 0;
}


int
HTTP_Server::asynch_thread_pool (void)
{  
// This only works on Win32
#if defined (ACE_WIN32)
  ACE_Asynch_Acceptor<Asynch_HTTP_Handler_Factory> acceptor;
  if (acceptor.open (ACE_INET_Addr (this->port_),
		     HTTP_Handler::MAX_REQUEST_SIZE + 1) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Acceptor::open"), -1);
  
  for (int i = 0; i < this->threads_; i++) 
    {
      Asynch_Thread_Pool_Task *t;
      ACE_NEW_RETURN (t, Asynch_Thread_Pool_Task (*ACE_Service_Config::proactor(), this->tm_), -1);
      if (t->open () != 0) 
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Thread_Pool_Task::open"), -1);
    }   
  return this->tm_.wait();
#endif /* ACE_WIN32 */
  return -1;
}

// This only works on Win32
#if defined (ACE_WIN32)

Asynch_Thread_Pool_Task::Asynch_Thread_Pool_Task (ACE_Proactor &proactor, ACE_Thread_Manager &tm)
  : ACE_Task<ACE_NULL_SYNCH> (&tm), proactor_ (proactor)
{
}

int
Asynch_Thread_Pool_Task::open (void *args)
{
  if (this->activate () == -1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Asynch_Thread_Pool_Task::open"), -1);

  return 0;
}

int
Asynch_Thread_Pool_Task::svc (void)
{
  for (;;) 
    {
      if (this->proactor_.handle_events () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Proactor::handle_events"), -1);	
    }
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

// This is necessary for gcc to work with templates
#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class LOCK_SOCK_Acceptor<ACE_Thread_Mutex>;
template class ACE_Oneshot_Acceptor<HTTP_Handler, ONESHOT_SOCK_ACCEPTOR>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#endif /* HTTP_SERVER_C */
