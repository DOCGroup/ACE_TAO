//
// $Id$
//

#ifndef ONEWAY_BUFFERING_ADMIN_H
#define ONEWAY_BUFFERING_ADMIN_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Oneway_Buffering_Admin interface
class Oneway_Buffering_Admin
  : public virtual POA_Test::Oneway_Buffering_Admin
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Oneway_Buffering_Admin (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::ULong request_count (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong bytes_received_count (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void request_received (CORBA::ULong payload_length,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void flush (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;

  /// Keep track of the number of requests received
  CORBA::ULong request_count_;

  /// Keep track of the number of requests received
  CORBA::ULong bytes_received_count_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ONEWAY_BUFFERING_H */
