//
// $Id$
//

#ifndef CLIENT_LEAKS_PROCESS_FACTORY_H
#define CLIENT_LEAKS_PROCESS_FACTORY_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Process_Factory interface
class Process_Factory
  : public virtual POA_Test::Process_Factory
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Process_Factory (CORBA::ORB_ptr orb);

  /// Return 1 if the shutdown message has been received already
  int shutdown_received (void);

  // = The skeleton methods
  virtual Test::Process_ptr create_new_process (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,Test::Spawn_Failed));

  virtual void noop (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Set to 1 when the test has finished
  int shutdown_received_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* CLIENT_LEAKS_PROCESS_FACTORY_H */
