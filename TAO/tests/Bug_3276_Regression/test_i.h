#ifndef _TEST_I_H_
#define _TEST_I_H_

#include "testS.h"

class Simple_Server_i : public POA_Simple_Server
{
public:
  // ctor
  explicit Simple_Server_i (CORBA::ORB_ptr orb);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;
  void remote_call () override;

  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
};

#endif /* _TEST_I_H_ */
