/**
 * @file Implicit_Deactivator.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Implicit_Deactivator__h_
#define Implicit_Deactivator__h_

#include "Auto_Functor.h"

#include "tao/PortableServer/PortableServer.h"

namespace TAO
{
namespace Utils
{

/**
 * @struct Implicit_Deactivation_Functor
 *
 * @brief Implements a functor for the Implicit_Deactivator class.
 */
struct Implicit_Deactivation_Functor
{
  typedef PortableServer::ServantBase * argument;

  // Deactivate an implicitly activated servant
  void operator() (PortableServer::ServantBase * servant) throw();
};

/**
 * @class Implicit_Deactivator
 *
 * @brief Helper class to deactivate implicitly activated servants.
 *
 */
typedef Auto_Functor<
            PortableServer::ServantBase,
            Implicit_Deactivation_Functor>
        Implicit_Deactivator;

} // namespace Utils
} // namespace TAO

#endif // Implicit_Deactivator__h_
