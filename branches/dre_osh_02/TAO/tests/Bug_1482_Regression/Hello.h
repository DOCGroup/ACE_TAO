//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::ULong next_prime (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Mutex
  TAO_SYNCH_MUTEX mutex_;

  /// Latest prime
  CORBA::ULong latest_prime_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* HELLO_H */
