
#include "TestS.h"

class Client : public POA_Test::Client
{
public:
  Client ()
    : POA_Test::Client(),
      reply_count_ (0) {}
  virtual ~Client ();

  virtual void reply (
    const char * msg);

  int reply_count () { return this->reply_count_; }

private:
  int  reply_count_;
};

