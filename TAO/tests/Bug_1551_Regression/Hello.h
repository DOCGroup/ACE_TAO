#ifndef HELLO_H
#define HELLO_H
/**
 * @file
 *
 * $Id$
 */
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class Hello
  : public virtual POA_Test::Hello
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Hello(CORBA::ORB_ptr orb, bool simulate_crashes);

  virtual void short_sleep (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

  bool simulate_crashes_;

  ACE_Atomic_Op<ACE_Thread_Mutex,long> atomic_counter_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* HELLO_H */
