
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
  explicit Process (CORBA::ORB_ptr orb);

  Process (Process const&) = delete;
  Process (Process&&) = delete;
  Process& operator= (Process const&) = delete;
  Process& operator= (Process&&) = delete;

  // = The skeleton methods
  CORBA::Long get_process_id () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_LEAKS_PROCESS_H */
