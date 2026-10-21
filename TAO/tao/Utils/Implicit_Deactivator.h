// -*- C++ -*-

//=============================================================================
/**
 * @file Implicit_Deactivator.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_IMPLICIT_DEACTIVATOR_H
#define TAO_UTILS_IMPLICIT_DEACTIVATOR_H
#include /**/ "ace/pre.h"
#include "tao/Utils/utils_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "ace/Auto_Functor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Utils
  {
    /**
     * @struct Implicit_Deactivation_Functor
     *
     * @brief Implements a functor for the Implicit_Deactivator class.
     */
    struct TAO_UTILS_Export Implicit_Deactivation_Functor
    {
      using argument = PortableServer::ServantBase *;

      // Deactivate an implicitly activated servant
      void operator() (PortableServer::ServantBase * servant) noexcept;
    };

    /**
     * @class Implicit_Deactivator
     *
     * @brief Helper class to deactivate implicitly activated servants.
     *
     */
    using Implicit_Deactivator =
      ACE_Utils::Auto_Functor<PortableServer::ServantBase, Implicit_Deactivation_Functor>;
  } // namespace Utils
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_UTILS_IMPLICIT_DEACTIVATOR_H*/
