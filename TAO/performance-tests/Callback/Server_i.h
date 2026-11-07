#ifndef TAO_SERVER_I_H
#define TAO_SERVER_I_H

#include "callbackS.h"

/// Implement the Test::Server interface
class Server_i : public POA_Test::Server
{
public:
  /// Constructor.
  Server_i () = default;
  Server_i (Server_i const &) = delete;
  Server_i (Server_i &&) = delete;
  Server_i &operator= (Server_i const &) = delete;
  Server_i &operator= (Server_i &&) = delete;

  /// Return 1 when the shutdown method has been invoked.
  bool done () const;

  /// Implement the CORBA methods
  //@{
  void set_callback (Test::Callback_ptr callback) override;

  void request (Test::TimeStamp,
                Test::Payload const &) override;

  void shutdown () override;

  //@}

private:
  /// A flag to terminate the event loop
  bool done_ {};

  /// The callback object
  Test::Callback_var callback_;
};

#if defined(__ACE_INLINE__)
#include "Server_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_SERVER_I_H */
