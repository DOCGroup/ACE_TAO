//
// $Id$
//

#ifndef CLIENT_LEAKS_STARTUP_CALLBACK_H
#define CLIENT_LEAKS_STARTUP_CALLBACK_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Startup_Callback interface
class Startup_Callback
  : public virtual POA_Test::Startup_Callback
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Startup_Callback (void);

  /// Return 1 if the process has started already
  /**
   * If the process has started then <the_process> returns the object
   * reference for the new process
   */
  int process_has_started (Test::Process_out the_process);

  // = The skeleton methods
  virtual void started (Test::Process_ptr the_process
                        TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Synchronize changes to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// Store the process.
  Test::Process_var process_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* CLIENT_LEAKS_STARTUP_CALLBACK_H */
