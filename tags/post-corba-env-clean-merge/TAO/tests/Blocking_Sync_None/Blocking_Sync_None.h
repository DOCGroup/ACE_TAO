//
// $Id$
//

#ifndef BLOCKING_SYNC_NONE_H
#define BLOCKING_SYNC_NONE_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Blocking_Sync_None interface
class Blocking_Sync_None
  : public virtual POA_Test::Blocking_Sync_None
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Blocking_Sync_None (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void slow_operation (const Test::Payload &the_payload,
                               CORBA::ULong sleep_microseconds
                               TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* BLOCKING_SYNC_NONE_H */
