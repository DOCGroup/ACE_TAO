//
// $Id$
//

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
  Callback (void);

  virtual void test_oneway (void);

  bool received_callback (void);

 private:
  bool received_callback_;
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_CALLBACK_H */
