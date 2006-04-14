// $Id$

#include "testS.h"

class server_i : public POA_server
{
public:
  server_i (int quiet,
            CORBA::ORB_ptr orb);

  void ping (CORBA::UShort time_to_live
             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void start (client_ptr c,
              CORBA::UShort time_to_live
              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  int quiet_;
  // Be quiet.

  client_var client_;
  // Pointer to server.

  CORBA::ORB_var orb_;
  // Pointer to orb.
};
