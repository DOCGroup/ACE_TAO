#ifndef _SERVER_TASK_
#define _SERVER_TASK_

#include "Server.h"

#include "ace/ARGV.h"
#include "ace/Task.h"

#include <string>

class Server_Task : public ACE_Task_Base
{
public:
  explicit Server_Task (std::string const &args)
    : args_ (args)
  {}
  ~Server_Task () override { this->force_shutdown (); }

  Server_Task (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task &&) = delete;

  int svc () override
  {
    bool initializer = false;
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
      if (!server_)
      {
        ACE_ARGV my_args (args_.c_str ());

        // Initialize Server ORB in new thread
        server_ = std::make_unique<Server> (my_args.argc (), my_args.argv ());

        ACE_ASSERT (server_);
        initializer = true;
      }
    }

    if (initializer)
    {
      server_->run (false);
      this->force_shutdown (); // servant thread is responsible for shutdown
    }
    else
    {
      server_->run (true);
    }

    return 0;
  }

  bool ready () const
  {
    if (server_)
    {
      return server_->init_;
    }

    return false;
  }

  void force_shutdown ()
  {
    if (server_)
    {
      server_->shutdown ();
    }
  }

private:
  std::string args_;
  std::unique_ptr<Server> server_;
  TAO_SYNCH_MUTEX mutex_;
};

#endif //_SERVER_TASK_
