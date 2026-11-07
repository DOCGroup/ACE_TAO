
#ifndef THROUGHPUT_RECEIVER_FACTORY_H
#define THROUGHPUT_RECEIVER_FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Receiver_Factory interface
class Receiver_Factory
  : public virtual POA_Test::Receiver_Factory
{
public:
  /// Constructor
  explicit Receiver_Factory (CORBA::ORB_ptr orb);
  Receiver_Factory (Receiver_Factory const &) = delete;
  Receiver_Factory (Receiver_Factory &&) = delete;
  Receiver_Factory &operator= (Receiver_Factory const &) = delete;
  Receiver_Factory &operator= (Receiver_Factory &&) = delete;

  // = The skeleton methods
  Test::Receiver_ptr create_receiver () override;

  void shutdown () override;

private:
  /// Keep a reference to the ORB in order to shutdown the app
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* THROUGHPUT_RECEIVER_FACTORY_H */
