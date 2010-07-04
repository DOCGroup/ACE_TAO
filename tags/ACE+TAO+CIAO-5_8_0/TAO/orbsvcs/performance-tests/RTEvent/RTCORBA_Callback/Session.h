//
// $Id$
//

#ifndef SESSION_H
#define SESSION_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Session interface
class Session
  : public virtual POA_Test::Session
{
public:
  /// Constructor
  Session (Test::Callback_ptr cb,
           PortableServer::POA_ptr poa);

  // = The skeleton methods
  virtual void sample (Test::Timestamp the_timestamp);

  virtual void shutdown (void);

  virtual PortableServer::POA_ptr _default_POA (void);

private:
  Test::Callback_var cb_;

  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* SESSION_H */
