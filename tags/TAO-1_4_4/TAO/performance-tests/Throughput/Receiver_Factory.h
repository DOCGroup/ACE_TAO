//
// $Id$
//

#ifndef THROUGHPUT_RECEIVER_FACTORY_H
#define THROUGHPUT_RECEIVER_FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver_Factory interface
class Receiver_Factory
  : public virtual POA_Test::Receiver_Factory
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Receiver_Factory (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual Test::Receiver_ptr create_receiver (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Keep a reference to the ORB in order to shutdown the app
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* THROUGHPUT_RECEIVER_FACTORY_H */
