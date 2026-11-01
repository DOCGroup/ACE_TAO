
#ifndef CALLBACK_CALLBACK_H
#define CALLBACK_CALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Callback interface
class Callback
  : public virtual POA_Test::Callback
{
public:
  /// Constructor
  explicit Callback (CORBA::ORB_ptr orb);

  Callback (Callback const&) = delete;
  Callback (Callback&&) = delete;
  Callback& operator= (Callback const&) = delete;
  Callback& operator= (Callback&&) = delete;

  // = The skeleton methods
  CORBA::Boolean are_you_there (CORBA::String_out answer) override;

  void test_oneway () override;

  void shutdown () override;
private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_CALLBACK_H */
