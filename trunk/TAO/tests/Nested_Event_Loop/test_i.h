// $Id$

#include "testS.h"

class server_i : public POA_server
{
public:
  server_i (CORBA::ORB_ptr orb);

  void loop (client_ptr remote_partner,
             CORBA::ULong event_loop_depth,
             CORBA::ULong event_loop_iterations,
             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void run_no_ops (client_ptr remote_partner,
                   CORBA::ULong iterations,
                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void no_op (client_ptr remote_partner,
              CORBA::ULong act_for_iterations,
              CORBA::ULong act_for_flag,
              CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

class client_i : public POA_client
{
public:
  client_i (server_ptr remote_partner);

  void loop (CORBA::ULong event_loop_depth,
             CORBA::ULong event_loop_iterations,
             CORBA::Environment &)
    ACE_THROW_SPEC (());

  void oneway_no_op (CORBA::ULong act_for_iterations,
                     CORBA::ULong act_for_flag,
                     CORBA::Environment &)
    ACE_THROW_SPEC (());

  void twoway_no_op (CORBA::Environment &)
    ACE_THROW_SPEC (());

private:
  server_var remote_partner_;
};
