//
// $Id$
//

#ifndef FACTORY_H
#define FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Factory interface
class Factory
  : public virtual POA_Test::Factory
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Factory (CORBA::ORB_ptr orb);

  // = The skeleton methods

  virtual Test::Simple_ptr create_simple_object (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Keep a reference to the ORB
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* FACTORY_H */
