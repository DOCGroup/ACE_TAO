// $Id$

#ifndef _CLIENT_TASK_
#define _CLIENT_TASK_

#include "Client.h"

#include "ace/ARGV.h"

class Client_Task : public ACE_Task_Base
{
 public:

  Client_Task (const std::string& args)
    : args_ (args)
  { }

  virtual int svc ()
  {
    ACE_ARGV my_args (args_.c_str());

    {
      Client client (my_args.argc(), my_args.argv());
      client.run();
    }

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client_Task::svc>\n"));
    return 0;
  }

 private:
  std::string args_;
};

#endif //_CLIENT_TASK_
