// -*- C++ -*-
// HTTP_Server.h

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "HTTP_Handler.h"

#include "ace/Service_Object.h"
#include "ace/Thread_Manager.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Task.h"

// Specialize ACE_SOCK_Acceptor to lock around accept();
template <class LOCK>
class LOCK_SOCK_Acceptor : public ACE_SOCK_Acceptor
{
public:
  int accept (
        ACE_SOCK_Stream &new_stream,
        ACE_Addr *remote_addr = 0,
        ACE_Time_Value *timeout = 0,
        int restart = 1
        ) const;

private:
  LOCK lock_;
};

typedef LOCK_SOCK_Acceptor<ACE_Thread_Mutex> HTTP_SOCK_Acceptor;
typedef ACE_Oneshot_Acceptor
	<HTTP_Handler, HTTP_SOCK_Acceptor, HTTP_SOCK_Acceptor::PEER_ADDR>
        HTTP_Acceptor;

class HTTP_Server : public ACE_Service_Object
{
public:
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);

private:
  int port_;
  int threads_;
  ACE_Thread_Manager tm_;
  HTTP_Acceptor acceptor_;
};

class HTTP_Task : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  HTTP_Task (HTTP_Acceptor &acceptor, ACE_Thread_Manager &tm)
    : ACE_Task<ACE_NULL_SYNCH>(&tm), acceptor_(acceptor)
  {}

  virtual int open (void *args = 0);
  virtual int close (u_long flags = 0);

  virtual int svc (void);

  virtual int put (ACE_Message_Block *, ACE_Time_Value * = 0) {return 0;}
  // defining this is not required in the newer ACE

private:
  HTTP_Acceptor &acceptor_;
};

ACE_STATIC_SVC_DECLARE (HTTP_Server)

#endif
