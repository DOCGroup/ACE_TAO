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
#include "ace/pre.h"
#include "Auto_Functor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

namespace TAO
{
  namespace Utils
  {

    /**
     * @struct ORB_Destroyer_Functor
     *
     * @brief Implements a functor for the ORB_Destroyer class.
     */
    struct ORB_Destroyer_Functor
    {
      typedef CORBA::ORB_ptr argument;

      /// Destroy the ORB
      void operator() (CORBA::ORB_ptr orb)
        ACE_THROW_SPEC (());
    };

    /**
     * @class ORB_Destroyer
     *
     * @brief Helper class to destroy an ORB.
     *
     */
    typedef Auto_Functor<
      CORBA::ORB,
      ORB_Destroyer_Functor>
    ORB_Destroyer;

  } // namespace Utils
} // namespace TAO

#include "ace/post.h"
#endif /* TAO_UTILS_ORB_DESTROYER_H*/
