#include "testS.h"

class client_i : public POA_client
{
public:
  client_i (bool quiet,
            server_ptr s);

  void ping (CORBA::UShort time_to_live) override;

  client_i (client_i const &) = delete;
  client_i (client_i &&) = delete;
  client_i &operator= (client_i const &) = delete;
  client_i &operator= (client_i &&) = delete;

private:
  /// Be quiet.
  bool quiet_ {};

  /// Pointer to the server.
  server_var server_;
};
