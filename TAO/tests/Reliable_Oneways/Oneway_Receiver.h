
#ifndef ONEWAY_RECEIVER_H
#define ONEWAY_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Oneway_Receiver interface
class Oneway_Receiver
  : public virtual POA_Test::Oneway_Receiver
{
public:
  explicit Oneway_Receiver (CORBA::ORB_ptr orb);
  Oneway_Receiver (Oneway_Receiver const &) = delete;
  Oneway_Receiver (Oneway_Receiver &&) = delete;
  Oneway_Receiver &operator= (Oneway_Receiver const &) = delete;
  Oneway_Receiver &operator= (Oneway_Receiver &&) = delete;

  // = The skeleton methods
  void raise_no_permission () override;

  void destroy () override;

  Test::Shutdown_Helper_ptr get_shutdown_helper () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* ONEWAY_RECEIVER_H */
