// This short program illustrates in implementation of the classic
// @(#)test_upipe.cpp	1.1	10/18/96

// "bounded buffer" program using ACE_UPIPEs.  This program also shows
// how the ACE_Connector and ACE_Acceptor patterns work when used with
// ACE_UPIPEs.

// Enable tracing
#include "ace/Log_Msg.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"
#include "ace/UPIPE_Addr.h"

#if defined (ACE_HAS_THREADS)

typedef ACE_Svc_Handler <ACE_UPIPE_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

class Server_Service : public SVC_HANDLER
  // = TITLE
  //     Defines the interface for a service that recvs data from its
  //     client and writes the data to its stdout.
{
public:
  Server_Service (ACE_Thread_Manager * = 0) {}
  virtual int open (void *)
    {
      ACE_TRACE ("Server_Service::open");
      return 0;
    }

  virtual int svc (void)
    {
      ACE_TRACE ("Server_Service::svc");

      char buf[BUFSIZ];
      ssize_t n;

      while ((n = this->peer ().recv (buf, sizeof buf)) > 0)
	::write (1, buf, n);

      return 0;
    }
};

class Server : public ACE_Strategy_Acceptor <Server_Service, ACE_UPIPE_ACCEPTOR>
  // = TITLE
  //     Defines the interface for a factory that accepts connections
  //     and creates/activates Server_Service objects.
{
public:
  Server (ACE_Thread_Manager *thr_mgr,
	  ACE_Reactor *reactor)
    : reactor_ (reactor), 
      thr_mgr_ (thr_mgr)
    {
      ACE_TRACE ("Server::Server");
    }

  virtual int init (int argc, char *argv[])
    {
      ACE_TRACE ("Server::init");
      const char *l_addr = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;

      ACE_UPIPE_Addr local_addr (l_addr);
      
      if (this->thr_strategy_.open (this->thr_mgr_, THR_DETACHED | THR_NEW_LWP) == -1)
	return -1;
      else if (this->open (local_addr, this->reactor_,
			   0, 0, &this->thr_strategy_) == -1)
	return -1;

      // Give server a chance to register the STREAM pipe.
      ACE_OS::sleep (ACE_Time_Value (4));
      return 0;
    }

private:
  ACE_Reactor *reactor_;
  // Our instance of the reactor.
  
  ACE_Thread_Manager *thr_mgr_;
  // Our instance of a thread manager.
  
  ACE_Thread_Strategy<Server_Service> thr_strategy_;
  // Our concurrency strategy.
};

class Client_Service : public SVC_HANDLER
  // = TITLE
  //     Defines the interface for a service that recvs data from its
  //     stdin and forward the data to its server.
{
public:
  Client_Service (ACE_Thread_Manager *thr_mgr)
    : SVC_HANDLER (thr_mgr)
    {
      ACE_TRACE ("Client_Service::Client_Service");
    }

  virtual int open (void *)
    {
      ACE_TRACE ("Client_Service::open");
      return this->activate (THR_DETACHED | THR_NEW_LWP);
    }

  virtual int svc (void)
    {
      ACE_TRACE ("Client_Service::svc");
      char buf[BUFSIZ];
      ssize_t n;

      while ((n = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0)
	this->peer ().send (buf, sizeof buf);

      this->peer ().close ();
      return 0;
    }
};

class Client : public ACE_Connector <Client_Service, ACE_UPIPE_CONNECTOR>
  // = TITLE
  //     Defines the interface for a factory that connects
  //     a Client_Service with a Server.
{
public:
  Client (ACE_Thread_Manager *thr_mgr)
    : thr_mgr_ (thr_mgr)
    {
      ACE_TRACE ("Client::Client");
    }

  virtual int init (int argc, char *argv[])
  {
    ACE_TRACE ("Client::init");

    char *r_addr = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;

    ACE_UPIPE_Addr remote_addr (r_addr);

    return this->connect (new Client_Service (this->thr_mgr_), remote_addr);
  }

private:
  ACE_Thread_Manager *thr_mgr_;
};

//----------------------------------------
                                       
int main (int argc, char *argv[])                       
{                                                       
  ACE_Service_Config svc_conf;
  ACE_Thread_Manager thr_mgr;

  Client peer_connector (&thr_mgr);
  Server peer_acceptor (&thr_mgr, ACE_Service_Config::reactor ());

  // Establish the connection between Acceptor and Connector.

  if (peer_acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);
  else if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init"), -1);

  // Wait for threads to exit.
  thr_mgr.wait ();
  return 0;
}                                                       
#else
int
main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "your platform does not support threads\n"), 1);
}
#endif /* ACE_HAS_THREADS */
