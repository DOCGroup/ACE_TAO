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

  // = The skeleton methods
  virtual Test::Process_ptr create_new_process (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,Test::Spawn_Failed));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* CLIENT_LEAKS_PROCESS_FACTORY_H */
