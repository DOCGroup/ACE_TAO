/**
 * @file Auto_Disconnect.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_AUTO_DISCONNECT_H
#define TAO_PERF_RTEC_AUTO_DISCONNECT_H

#include "ace/config-all.h"

/**
 * @class Auto_Disconnect
 *
 * @brief Automatically invoke the disconnect() operation on some
 *        RTEC client.
 */
template<class Client>
class Auto_Disconnect
{
public:
  /// Constructor
  /**
   * @param client The client
   */
  ACE_EXPLICIT Auto_Disconnect (Client *s = 0);

  /// Destructor
  ~Auto_Disconnect (void);

  /// Assingment operator
  Auto_Disconnect<Client>& operator= (Client *client);

  /// Release the client, i.e. the destructor does not call any
  /// methods.
  void release (void);

private:
  /// The client
  Client *client_;
};

#if defined(__ACE_INLINE__)
#include "Auto_Disconnect.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Auto_Disconnect.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_AUTO_DISCONNECT_H */
