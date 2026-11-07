
#ifndef CLIENT_LEAKS_PROCESS_FACTORY_H
#define CLIENT_LEAKS_PROCESS_FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Process_Factory interface
class Process_Factory
  : public virtual POA_Test::Process_Factory
{
public:
  /// Constructor
  explicit Process_Factory (CORBA::ORB_ptr orb);

  Process_Factory (Process_Factory const &) = delete;
  Process_Factory (Process_Factory &&) = delete;
  Process_Factory &operator= (Process_Factory const &) = delete;
  Process_Factory &operator= (Process_Factory &&) = delete;

  /// Return true if the shutdown message has been received already
  bool shutdown_received () const;

  // = The skeleton methods
  Test::Process_ptr create_new_process () override;

  void noop () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Set to true when the test has finished
  bool shutdown_received_ {};
};

#include /**/ "ace/post.h"
#endif /* CLIENT_LEAKS_PROCESS_FACTORY_H */
