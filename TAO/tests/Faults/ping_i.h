
//=============================================================================
/**
 *  @file   ping_i.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_PING_I_H
#define TAO_PING_I_H

#include "pingS.h"

/**
 * @class PingObject_i
 *
 * @brief A simple implementation of the <ping> interface
 *
 * Used to verify that the client can tolerate server crashes with
 * oneway invocations and that there are no problems with
 * persistent object references after the crash.
 */
class PingObject_i : public POA_PingObject
{
public:
  /// ctor
  explicit PingObject_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr the_poa);
  PingObject_i (PingObject_i const &) = delete;
  PingObject_i (PingObject_i &&) = delete;
  PingObject_i &operator= (PingObject_i const &) = delete;
  PingObject_i &operator= (PingObject_i &&) = delete;

  // = The PingObject interface methods
  void ping (PingObject_ptr callback) override;
  void pong () override;
  void shutdown () override;

  PortableServer::POA_ptr _default_POA () override;

private:
  /// The orb
  CORBA::ORB_var orb_ {};

  /// The poa
  PortableServer::POA_var poa_ {};
};

#if defined(__ACE_INLINE__)
#include "ping_i.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PING_I_H */
