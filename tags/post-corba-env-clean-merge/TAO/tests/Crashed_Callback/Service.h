//
// $Id$
//

#ifndef CRASHED_CALLBACK_SERVICE_H
#define CRASHED_CALLBACK_SERVICE_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Service interface
class Service
  : public virtual POA_Test::Service
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Service (void);

  /// Dump the results.
  /**
   * In case of failure this exposes any incomplete runs.
   */
  void dump_results (void);

  // = The skeleton methods
  virtual void run_test (Test::Crashed_Callback_ptr callback
                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Call the are_you_there callback multiple times.
  /**
   * @return Number of exceptions raised
   */
  int call_are_you_there (Test::Crashed_Callback_ptr callback
                          TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC (());

  /// Call the test_oneway method mutiple times.
  /**
   * @return Number of exceptions raised
   */
  int call_test_oneway (Test::Crashed_Callback_ptr callback
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC (());

private:
  /// Count the number of tests executed
  int test_count_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* CRASHED_CALLBACK_SERVICE_H */
