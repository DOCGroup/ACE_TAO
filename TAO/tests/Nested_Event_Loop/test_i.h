// $Id$

#include "testS.h"

class server_i : public POA_server
{
public:
  server_i (CORBA::ORB_ptr orb);

  void loop (client_ptr remote_partner,
             CORBA::ULong event_loop_depth,
             CORBA::ULong event_loop_iterations
             TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void run_no_ops (client_ptr remote_partner,
                   CORBA::ULong iterations
                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void no_op (client_ptr remote_partner,
              const act &act_for_iterations,
              const act &act_for_flag
              TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

class client_i : public POA_client
{
public:
  client_i (server_ptr remote_partner);

  void loop (CORBA::ULong event_loop_depth,
             CORBA::ULong event_loop_iterations
             TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void oneway_no_op (const act &act_for_iterations,
                     const act &act_for_flag
                     TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void twoway_no_op (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  server_var remote_partner_;
};
