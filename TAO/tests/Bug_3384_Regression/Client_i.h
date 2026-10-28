#include "TestS.h"

class Client : public POA_Test::Client
{
public:
  Client ()
    : POA_Test::Client (),
      reply_count_ {}
  {}
  ~Client () override = default;

  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  void reply (char const *msg) override;

  int reply_count () { return this->reply_count_; }

private:
  int reply_count_;
};

