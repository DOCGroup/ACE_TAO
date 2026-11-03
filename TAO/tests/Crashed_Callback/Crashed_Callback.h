
#ifndef CRASHED_CALLBACK_CRASHED_CALLBACK_H
#define CRASHED_CALLBACK_CRASHED_CALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Crashed_Callback interface
class Crashed_Callback
  : public virtual POA_Test::Crashed_Callback
{
public:
  /// Constructor
  Crashed_Callback () = default;

  // = The skeleton methods
  CORBA::Boolean are_you_there () override;

  void crash_now_please () override;

  void test_oneway () override;

  Crashed_Callback (Crashed_Callback const&) = delete;
  Crashed_Callback (Crashed_Callback&&) = delete;
  Crashed_Callback& operator= (Crashed_Callback const&) = delete;
  Crashed_Callback& operator= (Crashed_Callback&&) = delete;
};

#include /**/ "ace/post.h"
#endif /* CRASHED_CALLBACK_CRASHED_CALLBACK_H */
