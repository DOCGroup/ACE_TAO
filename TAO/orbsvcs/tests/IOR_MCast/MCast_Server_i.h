#ifndef MCAST_SERVER_I_H
#define MCAST_SERVER_I_H

#include "MCastS.h"

class MCast_Server_i : public POA_MCast::Server {
public:
  MCast_Server_i () = default;

  MCast_Server_i (MCast_Server_i const&) = delete;
  MCast_Server_i (MCast_Server_i&&) = delete;
  MCast_Server_i& operator= (MCast_Server_i const&) = delete;
  MCast_Server_i& operator= (MCast_Server_i&&) = delete;

  // Simple method just to see that the client contacted the
  // server. If it does, true is returned.
  CORBA::Boolean connect_server () override;
};

#endif /* MCAST_SERVER_I_H */
