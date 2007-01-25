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
  virtual void sample (Test::Timestamp the_timestamp)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableServer::POA_ptr _default_POA (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  Test::Callback_var cb_;

  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* SESSION_H */
