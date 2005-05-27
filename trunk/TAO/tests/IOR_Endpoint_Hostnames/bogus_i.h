//
// $Id$
//

#ifndef bogus_h
#define bogus_h
#include "ace/pre.h"

#include "bogusS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
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

#endif //bogus_h
