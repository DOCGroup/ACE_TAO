#ifndef _TEST_I_H_
#define _TEST_I_H_

#include "testS.h"

class Simple_Server_i : public POA_Simple_Server
{
public:
  Simple_Server_i (CORBA::ORB_ptr orb);
  // ctor

  void remote_call () override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* _TEST_I_H_ */
