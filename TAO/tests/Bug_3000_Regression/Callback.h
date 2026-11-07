
#ifndef CALLBACK_CALLBACK_H
#define CALLBACK_CALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Callback interface
class Callback
  : public virtual POA_Test::Callback
{
public:
  Callback () = default;
  Callback (Callback const &) = delete;
  Callback &operator= (Callback const &) = delete;
  Callback (Callback &&) = delete;
  Callback &operator= (Callback &&) = delete;

  void test_oneway () override;

  bool received_callback () const;

private:
  bool received_callback_ {};
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_CALLBACK_H */
