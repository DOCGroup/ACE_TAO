#include "testS.h"

class client_i : public POA_client
{
public:
  client_i (bool const quiet,
            server_ptr const s);
  client_i (client_i const &) = delete;
  client_i (client_i &&) = delete;
  client_i &operator= (client_i const &) = delete;
  client_i &operator= (client_i &&) = delete;
  // Constructor.

  void ping (CORBA::UShort time_to_live) override;

private:
  bool quiet_ {};
  // Be quiet.

  server_var server_ {};
  // Pointer to server.
};
