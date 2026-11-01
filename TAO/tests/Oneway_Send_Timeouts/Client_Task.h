#ifndef _CLIENT_TASK_
#define _CLIENT_TASK_

#include "Client.h"

#include "ace/ARGV.h"
#include "ace/Task.h"

#include <string>

class Client_Task : public ACE_Task_Base
{
public:
  explicit Client_Task (std::string const &args)
    : args_ (args)
  {}

  Client_Task (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

  int svc () override
  {
    ACE_ARGV my_args (args_.c_str ());

    {
      Client client (my_args.argc (), my_args.argv ());
      client.run ();
    }

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client_Task::svc>\n"));
    return 0;
  }

private:
  std::string args_;
};

#endif //_CLIENT_TASK_
