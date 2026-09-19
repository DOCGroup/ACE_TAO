#ifndef IDLE_TRANSPORT_TIMEOUT_ECHO_I_H
#define IDLE_TRANSPORT_TIMEOUT_ECHO_I_H

#include "testS.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager_T.h"

/**
 * @class Echo_i
 *
 * @brief Servant implementation for Test::Echo.
 *
 * Provides ping/cache_size/shutdown operations used by the
 * Idle_Transport_Timeout regression test.
 */
class Echo_i : public virtual POA_Test::Echo
{
public:
  explicit Echo_i (CORBA::ORB_ptr orb);

  // Test::Echo operations
  bool ping (::CORBA::Long sleep_time, ::CORBA::Long cache_size_expected, ::Test::Echo_ptr server, ::CORBA::Long sleep_time_server, ::CORBA::Long cache_size_expected_in_server1, ::CORBA::Long cache_size_expected_in_server2) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* IDLE_TRANSPORT_TIMEOUT_ECHO_I_H */
