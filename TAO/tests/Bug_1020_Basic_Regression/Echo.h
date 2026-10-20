
/**
 * @file Echo.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef BUG_1020_BASIC_REGRESSION_ECHO_H
#define BUG_1020_BASIC_REGRESSION_ECHO_H
#include "TestS.h"

class Echo : public POA_Test::Echo
{
public:
  Echo (CORBA::ORB_ptr orb,
        int abort_counter);
  Echo (Echo const &) = delete;
  Echo &operator= (Echo const &) = delete;
  Echo (Echo &&) = delete;
  Echo &operator= (Echo &&) = delete;

  void echo_payload (Test::Payload &data) override;

private:
  CORBA::ORB_var orb_;

  int abort_counter_;

  TAO_SYNCH_MUTEX mutex_;
};

#endif /* BUG_1020_BASIC_REGRESSION_ECHO_H*/
