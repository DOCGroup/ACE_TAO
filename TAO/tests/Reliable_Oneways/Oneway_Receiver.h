
#ifndef ONEWAY_RECEIVER_H
#define ONEWAY_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Oneway_Receiver interface
class Oneway_Receiver
  : public virtual POA_Test::Oneway_Receiver
{
public:
  /// Constructor
  Oneway_Receiver (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void raise_no_permission ();

  virtual void destroy ();

  virtual Test::Shutdown_Helper_ptr get_shutdown_helper ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* ONEWAY_RECEIVER_H */
