/**
 * @file Auto_Functor.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_AUTO_FUNCTOR_H
#define TAO_PERF_RTEC_AUTO_FUNCTOR_H

#include "ace/config-all.h"

/**
 * @class Auto_Functor
 *
 * @brief Automatically invoke the disconnect() operation on some
 *        RTEC client.
 *
 * @todo This template can be extended to work with non-pointer
 *       arguments, but needs a little work to get there (and some
 *       specializations of the swap() method).
 */
template<class Client, class Functor>
class Auto_Functor
{
public:
  /// Constructor
  /**
   * @param client The client
   */
  ACE_EXPLICIT Auto_Functor (Client *s = 0);
  Auto_Functor (const Auto_Functor<Client,Functor>& rhs);
  Auto_Functor<Client,Functor>& operator= (const Auto_Functor<Client,Functor>& rhs);

  /// Destructor
  ~Auto_Functor (void);

  /// Assignment operator
  Auto_Functor<Client,Functor>& operator= (Client *client);

  /// Release the client, i.e. the destructor does not call any
  /// methods.
  void release (void);

private:
  /// The client
  Client *client_;
};

#if defined(__ACE_INLINE__)
#include "Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Auto_Functor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_AUTO_FUNCTOR_H */
