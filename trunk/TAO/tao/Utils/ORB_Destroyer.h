// -*- C++ -*-

//=============================================================================
/**
 * @file ORB_Destroyer.h
 *
 * $Id$
 *
 * @author Marina Spivak <marina@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_ORB_DESTROYER_H
#define TAO_UTILS_ORB_DESTROYER_H
#include /**/ "ace/pre.h"
#include "tao/Utils/utils_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "ace/Auto_Functor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Utils
  {

    /**
     * @struct ORB_Destroyer_Functor
     *
     * @brief Implements a functor for the ORB_Destroyer class.
     */
    struct TAO_UTILS_Export ORB_Destroyer_Functor
    {
      typedef CORBA::ORB_ptr argument;

      /// Destroy the ORB
      void operator() (CORBA::ORB_ptr orb) throw ();
    };

    /**
     * @class ORB_Destroyer
     *
     * @brief Helper class to destroy an ORB.
     *
     */
    typedef ACE_Utils::Auto_Functor<
      CORBA::ORB,
      ORB_Destroyer_Functor>
    ORB_Destroyer;

  } // namespace Utils
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_UTILS_ORB_DESTROYER_H*/
