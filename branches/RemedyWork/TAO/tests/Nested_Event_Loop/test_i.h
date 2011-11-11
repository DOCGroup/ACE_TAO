// $Id$

#include "testS.h"

class server_i : public POA_server
{
public:
  server_i (CORBA::ORB_ptr orb);

  void loop (client_ptr remote_partner,
             CORBA::ULong event_loop_depth,
             CORBA::ULong event_loop_iterations);

  void run_no_ops (client_ptr remote_partner,
                   CORBA::ULong iterations);

  void no_op (client_ptr remote_partner,
              const act &act_for_iterations,
              const act &act_for_flag);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

class client_i : public POA_client
{
public:
  client_i (server_ptr remote_partner);

  void loop (CORBA::ULong event_loop_depth,
             CORBA::ULong event_loop_iterations);

  void oneway_no_op (const act &act_for_iterations,
                     const act &act_for_flag);

  void twoway_no_op (void);

private:
  server_var remote_partner_;
  client_var self_;
};
