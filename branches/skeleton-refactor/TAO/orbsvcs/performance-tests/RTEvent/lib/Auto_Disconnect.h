/**
 * @file Auto_Disconnect.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_AUTO_DISCONNECT_H
#define TAO_PERF_RTEC_AUTO_DISCONNECT_H

#include "tao/corba.h"
#include "ace/Auto_Functor.h"

/**
 * @class Disconnect
 *
 * @brief Helper functor to call the disconnect() method of a class.
 */
template<class Client>
class Disconnect
{
public:
  void operator() (Client *client);
};

/**
 * @class Auto_Disconnect
 *
 * @brief Automatically invoke the disconnect() operation on some
 *        RTEC client.
 */
template<class Client>
class Auto_Disconnect : public ACE_Utils::Auto_Functor<Client,Disconnect<Client> >
{
public:
  /// Constructor
  /**
   * @param client The client
   */
  explicit Auto_Disconnect (Client *s = 0);

  /// Assignment operator
  Auto_Disconnect<Client>& operator= (Client *client);
};

#if defined(__ACE_INLINE__)
#include "Auto_Disconnect.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Auto_Disconnect.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_AUTO_DISCONNECT_H */
