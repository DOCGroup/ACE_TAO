//
// $Id$
//

#ifndef HELLOWORLD_H
#define HELLOWORLD_H
#include /**/ "ace/pre.h"

#include "tao/corba.h"
#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::HelloWorld interface
///
class HelloWorld
  : public virtual POA_Test::HelloWorld
  , public virtual PortableServer::RefCountServantBase
{
public:

  HelloWorld (void);
  /// Constructor

  virtual char * get_string (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* HELLOWORLD_H */
