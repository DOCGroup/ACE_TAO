/**
 * @file Shutdown.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_SHUTDOWN_H
#define TAO_PERF_RTEC_SHUTDOWN_H

#include "ace/config-all.h"
#include "ace/CORBA_macros.h"

/**
 * @class Shutdown
 *
 * @brief Helper functor to call the shutdown() method of a class.
 */
template<class Client>
class Shutdown
{
public:
  void operator() (Client *client
                   ACE_ENV_ARG_DECL);
};

#if defined(__ACE_INLINE__)
#include "Shutdown.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Shutdown.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_SHUTDOWN_H */
