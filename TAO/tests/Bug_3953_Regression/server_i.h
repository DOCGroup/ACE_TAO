#include "testS.h"

class server_i : public POA_server
{
public:
  server_i (int quiet,
            CORBA::ORB_ptr orb);
  server_i (server_i const &) = delete;
  server_i (server_i &&) = delete;
  server_i &operator= (server_i const &) = delete;
  server_i &operator= (server_i &&) = delete;

  void ping (CORBA::UShort time_to_live) override;

  void start (client_ptr c,
              CORBA::UShort time_to_live) override;

  void shutdown () override;

  bool exception () const;
private:
  int quiet_;
  // Be quiet.

  client_var client_;
  // Pointer to server.

  CORBA::ORB_var orb_;
  // Pointer to orb.

  bool exception_;
};
