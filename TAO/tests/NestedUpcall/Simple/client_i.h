#include "testS.h"

class client_i : public POA_client
{
public:
  client_i (bool quiet,
            server_ptr s);
  // Constructor.

  void ping (CORBA::UShort time_to_live) override;

  client_i (client_i const &) = delete;
  client_i (client_i &&) = delete;
  client_i &operator= (client_i const &) = delete;
  client_i &operator= (client_i &&) = delete;

private:
  bool quiet_ {};
  // Be quiet.

  server_var server_;
  // Pointer to server.
};
