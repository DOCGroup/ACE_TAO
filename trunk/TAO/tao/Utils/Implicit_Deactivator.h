// -*- C++ -*-
//=============================================================================
/**
 * @file Implicit_Deactivator.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_IMPLICIT_DEACTIVATOR_H
#define TAO_UTILS_IMPLICIT_DEACTIVATOR_H
#include /**/ "ace/pre.h"
#include "utils_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "ace/Auto_Functor.h"


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
      typedef PortableServer::ServantBase * argument;

      // Deactivate an implicitly activated servant
      void operator() (PortableServer::ServantBase * servant)
        ACE_THROW_SPEC (());
    };

    /**
     * @class Implicit_Deactivator
     *
     * @brief Helper class to deactivate implicitly activated servants.
     *
     */
    typedef ACE_Utils::Auto_Functor<
      PortableServer::ServantBase,
      Implicit_Deactivation_Functor>
    Implicit_Deactivator;

  } // namespace Utils
} // namespace TAO

#include /**/ "ace/post.h"
#endif /*TAO_UTILS_IMPLICIT_DEACTIVATOR_H*/
