//
// $Id$
//

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
  Oneway_Receiver (void);

  // = The skeleton methods
  virtual void receive_oneway (void);
};

#include /**/ "ace/post.h"
#endif /* ONEWAY_RECEIVER_H */
