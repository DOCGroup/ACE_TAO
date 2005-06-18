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
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Session_Factory (CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr poa);

  // = The skeleton methods
  virtual Test::Session_ptr create_new_session (Test::Callback_ptr cb
                                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* SESSION_FACTORY_H */
