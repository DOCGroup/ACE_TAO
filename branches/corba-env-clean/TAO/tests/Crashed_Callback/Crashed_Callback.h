//
// $Id$
//

#ifndef CRASHED_CALLBACK_CRASHED_CALLBACK_H
#define CRASHED_CALLBACK_CRASHED_CALLBACK_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Crashed_Callback interface
class Crashed_Callback
  : public virtual POA_Test::Crashed_Callback
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Crashed_Callback (void);

  // = The skeleton methods
  virtual CORBA::Boolean are_you_there (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void crash_now_please (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void test_oneway (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* CRASHED_CALLBACK_CRASHED_CALLBACK_H */
