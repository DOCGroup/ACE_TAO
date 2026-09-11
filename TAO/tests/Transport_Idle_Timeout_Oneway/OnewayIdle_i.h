#ifndef TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H
#define TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H

#include "testS.h"

class OnewayIdle_i : public virtual POA_Test::OnewayIdle
{
public:
  explicit OnewayIdle_i (CORBA::ORB_ptr orb);

  void ping () override;

  bool test_failed () const;

private:
  CORBA::ORB_var orb_;
  bool test_failed_ { false };
};

#endif /* TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H */
