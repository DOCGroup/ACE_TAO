//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "tao/corba.h"
#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Hello interface
///
class Hello
  : public virtual POA_Test::Hello
  , public virtual PortableServer::RefCountServantBase
{
public:

  /// Constructor
  Hello (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  virtual Test::HelloWorld_ptr get_helloworld (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  PortableServer::POA_var poa_;

  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* HELLO_H */
