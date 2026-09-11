#ifndef TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H
#define TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H

#include "testS.h"

class OnewayIdle_i : public virtual POA_Test::OnewayIdle
{
public:
  explicit OnewayIdle_i (CORBA::ORB_ptr orb);

  void ping (CORBA::Long wait_seconds) override;
  CORBA::Long observed_cache_size () override;
  void shutdown () override;

private:
  CORBA::ORB_var orb_;
  CORBA::Long observed_cache_size_ { -1 };
};

#endif /* TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H */
