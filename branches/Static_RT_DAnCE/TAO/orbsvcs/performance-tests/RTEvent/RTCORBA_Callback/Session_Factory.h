//
// $Id$
//

#ifndef SESSION_FACTORY_H
#define SESSION_FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Session_Factory interface
class Session_Factory
  : public virtual POA_Test::Session_Factory
{
public:
  /// Constructor
  Session_Factory (CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr poa);

  // = The skeleton methods
  virtual Test::Session_ptr create_new_session (Test::Callback_ptr cb);

  virtual void shutdown (void);

  PortableServer::POA_ptr _default_POA (void);

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* SESSION_FACTORY_H */
