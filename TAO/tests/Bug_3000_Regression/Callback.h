
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
  Callback ();

  virtual void test_oneway ();

  bool received_callback ();

 private:
  bool received_callback_;
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_CALLBACK_H */
