//
// $Id$
//

#ifndef ROUNDTRIP_HANDLER_H
#define ROUNDTRIP_HANDLER_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Basic_Stats.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Roundtrip interface
class Roundtrip_Handler
  : public virtual POA_Test::AMI_RoundtripHandler
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Roundtrip_Handler (int expected_callbacks);

  /// Return the number of pending callbacks
  int pending_callbacks (void) const;

  /// Dump the results
  void dump_results (ACE_UINT32 gsf);

  // = The skeleton methods
  virtual void test_method (Test::Timestamp send_time
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void test_method_excep (Test::AMI_RoundtripExceptionHolder *holder
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void shutdown_excep (Test::AMI_RoundtripExceptionHolder *holder
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// The number of callbacks not received yet
  int pending_callbacks_;

  /// Collect the latency results
  ACE_Basic_Stats latency_stats_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
