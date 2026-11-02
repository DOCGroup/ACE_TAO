#ifndef TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H
#define TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H

#include "testS.h"

class OnewayIdle_i : public virtual POA_Test::OnewayIdle
{
public:
  explicit OnewayIdle_i (CORBA::ORB_ptr orb);

  OnewayIdle_i (OnewayIdle_i const &) = delete;
  OnewayIdle_i (OnewayIdle_i &&) = delete;
  OnewayIdle_i & operator= (OnewayIdle_i const &) = delete;
  OnewayIdle_i & operator= (OnewayIdle_i &&) = delete;

  void ping () override;

  bool test_failed () const;
  bool request_received () const;

private:
  CORBA::ORB_var orb_;
  bool test_failed_ {};
  bool request_received_ {};
};

#endif /* TRANSPORT_IDLE_TIMEOUT_ONEWAY_I_H */
