// -*- C++ -*-
#ifndef SIMPLE_H
#define SIMPLE_H

#include "TestS.h"
#include "Factory.h"

#if defined (_MSC_VER)
# pragma warning(push)
#endif /* _MSC_VER */

/// Implement the Test::Simple interface
class Simple
  : public virtual POA_Test::Simple
{
public:
  explicit Simple (int server_id, int pong_delay = 0);

  Simple (Simple const &) = delete;
  Simple &operator= (Simple const &) = delete;
  Simple (Simple &&) = delete;
  Simple &operator= (Simple &&) = delete;

  /// Default _non_existent: always returns false.
  CORBA::Boolean _non_existent () override;

  char* get_string () override;

  void exit () override;

private:
  int const server_id_;
  int const pong_delay_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SIMPLE_H */
