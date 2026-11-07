
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
  Session_Factory (Session_Factory const &) = delete;
  Session_Factory (Session_Factory &&) = delete;
  Session_Factory &operator= (Session_Factory const &) = delete;
  Session_Factory &operator= (Session_Factory &&) = delete;

  // = The skeleton methods
  Test::Session_ptr create_new_session (Test::Callback_ptr cb) override;

  void shutdown () override;

  PortableServer::POA_ptr _default_POA () override;

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  PortableServer::POA_var poa_;
};

#include /**/ "ace/post.h"
#endif /* SESSION_FACTORY_H */
