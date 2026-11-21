#include <atomic>

#include "TestS.h"

class Client : public POA_Test::Client
{
public:
  Client () = default;
  ~Client () override = default;

  Client (Client const &) = delete;
  Client (Client &&) = delete;
  Client &operator= (Client const &) = delete;
  Client &operator= (Client &&) = delete;

  void reply (char const *msg) override;

  int reply_count () const { return this->reply_count_.load (); }

private:
  std::atomic<int> reply_count_ {};
};

