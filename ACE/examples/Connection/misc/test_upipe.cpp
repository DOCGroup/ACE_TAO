// $Id$

// This short program illustrates in implementation of the classic
// "bounded buffer" program using ACE_UPIPEs.  This program also shows
// how the ACE_Connector and ACE_Acceptor patterns work when used with
// ACE_UPIPEs.

#include "ace/Acceptor.h"
#include "ace/UPIPE_Acceptor.h"
#include "ace/UPIPE_Connector.h"
#include "ace/Connector.h"
#include "ace/UPIPE_Addr.h"



#if defined (ACE_HAS_THREADS)

#include "test_upipe.h"

class Server : public ACE_Strategy_Acceptor <Server_Service, ACE_UPIPE_ACCEPTOR>
{
  // = TITLE
  //     Defines the interface for a factory that accepts connections
  //     and creates/activates Server_Service objects.
public:
  Server (ACE_Thread_Manager *thr_mgr,
          ACE_Reactor *reactor)
    : reactor_ (reactor),
      thr_mgr_ (thr_mgr)
    {
      ACE_TRACE ("Server::Server");
    }

  virtual int init (int argc, ACE_TCHAR *argv[])
    {
      ACE_TRACE ("Server::init");
      const ACE_TCHAR *l_addr = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;

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

class Client : public ACE_Connector <Client_Service, ACE_UPIPE_CONNECTOR>
{
  // = TITLE
  //     Defines the interface for a factory that connects
  //     a Client_Service with a Server.
public:
  Client (ACE_Thread_Manager *thr_mgr)
    : thr_mgr_ (thr_mgr)
    {
      ACE_TRACE ("Client::Client");
    }

  virtual int init (int argc, ACE_TCHAR *argv[])
  {
    ACE_TRACE ("Client::init");

    const ACE_TCHAR *r_addr = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;

    ACE_UPIPE_Addr remote_addr (r_addr);

    Client_Service *cs;

    ACE_NEW_RETURN (cs, Client_Service (this->thr_mgr_), -1);

    return this->connect (cs, remote_addr);
  }

private:
  ACE_Thread_Manager *thr_mgr_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Service_Config svc_conf;
  ACE_Thread_Manager thr_mgr;

  Client peer_connector (&thr_mgr);
  Server peer_acceptor (&thr_mgr, ACE_Reactor::instance ());

  // Establish the connection between Acceptor and Connector.

  if (peer_acceptor.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("init")), -1);
  else if (peer_connector.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("init")), -1);

  // Wait for threads to exit.
  thr_mgr.wait ();
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "your platform does not support threads\n"),
                    1);
}
#endif /* ACE_HAS_THREADS */
