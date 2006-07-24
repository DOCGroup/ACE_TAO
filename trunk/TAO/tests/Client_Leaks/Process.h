//
// $Id$
//

#ifndef CLIENT_LEAKS_PROCESS_H
#define CLIENT_LEAKS_PROCESS_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Process interface
class Process
  : public virtual POA_Test::Process
{
public:
  /// Constructor
  Process (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::Long get_process_id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_LEAKS_PROCESS_H */
