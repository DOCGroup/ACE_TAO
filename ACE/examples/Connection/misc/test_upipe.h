/* -*- C++ -*- */

#ifndef ACE_TEST_UPIPE_H
#define ACE_TEST_UPIPE_H

#include "ace/OS_NS_unistd.h"
#include "ace/Svc_Handler.h"
#include "ace/Service_Config.h"
#include "ace/UPIPE_Stream.h"

using SVC_HANDLER = ACE_Svc_Handler<ACE_UPIPE_STREAM, ACE_NULL_SYNCH>;

class Server_Service : public SVC_HANDLER
  // = TITLE
  //     Defines the interface for a service that recvs data from its
  //     client and writes the data to its stdout.
{
public:
  explicit Server_Service (ACE_Thread_Manager *thr_mgr = nullptr)
    : SVC_HANDLER (thr_mgr)
  {
  }
  ~Server_Service () override = default;
  Server_Service (Server_Service const &) = delete;
  Server_Service (Server_Service &&) = delete;
  Server_Service &operator= (Server_Service const &) = delete;
  Server_Service &operator= (Server_Service &&) = delete;

  int open (void *) override
    {
      ACE_TRACE ("Server_Service::open");
      return 0;
    }

  int svc () override
    {
      ACE_TRACE ("Server_Service::svc");

      char buf[BUFSIZ] {};
      while (true)
        {
          ssize_t const n = this->peer ().recv (buf, sizeof buf);
          if (n <= 0)
            break;

          ACE_OS::write (ACE_STDOUT, buf, n);
        }

      return 0;
    }
};

class Client_Service : public SVC_HANDLER
  // = TITLE
  //     Defines the interface for a service that recvs data from its
  //     stdin and forward the data to its server.
{
public:
  explicit Client_Service (ACE_Thread_Manager *thr_mgr = nullptr)
    : SVC_HANDLER (thr_mgr)
    {
      ACE_TRACE ("Client_Service::Client_Service");
    }
  ~Client_Service () override = default;
  Client_Service (Client_Service const &) = delete;
  Client_Service (Client_Service &&) = delete;
  Client_Service &operator= (Client_Service const &) = delete;
  Client_Service &operator= (Client_Service &&) = delete;

  int open (void *) override
    {
      ACE_TRACE ("Client_Service::open");
      return this->activate (THR_DETACHED | THR_NEW_LWP);
    }

  int svc () override
    {
      ACE_TRACE ("Client_Service::svc");
      char buf[BUFSIZ] {};
      while (true)
        {
          ssize_t const n = ACE_OS::read (ACE_STDIN, buf, sizeof buf);
          if (n <= 0)
            break;

          this->peer ().send (buf, n);
        }

      this->peer ().close ();
      return 0;
    }
};

#endif /* ACE_TEST_UPIPE_H */
