//
// $Id$
//

#ifndef bogus_h
#define bogus_h
#include /**/ "ace/pre.h"

#include "bogusS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class bogus
  : public virtual POA_Test::bogus
  , public virtual PortableServer::RefCountServantBase
{
public:
  bogus ();
  virtual ~bogus ();

  virtual void noop (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif //bogus_h
