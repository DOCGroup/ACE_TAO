/* -*- C++ -*- */

// $Id$

#ifndef ACE_TEST_UPIPE_H
#define ACE_TEST_UPIPE_H

#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/UPIPE_Stream.h"

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


class Client_Service : public SVC_HANDLER
  // = TITLE
  //     Defines the interface for a service that recvs data from its
  //     stdin and forward the data to its server.
{
public:
  Client_Service (ACE_Thread_Manager *thr_mgr = 0)
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
	this->peer ().send (buf, n);

      this->peer ().close ();
      return 0;
    }
};

#endif /* ACE_TEST_UPIPE_H */
