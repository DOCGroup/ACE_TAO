// -*- C++ -*-
//=============================================================================
/**
 * @file RIR_Narrow.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_RIR_NARROW_H
#define TAO_UTILS_RIR_NARROW_H
#include "ace/pre.h"

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"


namespace TAO
{
  namespace Utils
  {

    /**
     * @class RIR_Narrow
     *
     * @brief Helper class to obtain an initial reference and narrow it
     *        to the proper object reference.
     */
    template<class T> class RIR_Narrow
    {
    public:
      typedef typename T::_ptr_type _ptr_type;
      typedef typename T::_var_type _var_type;

      /// Use resolve_initial_references to find an object and then
      /// narrow it.
      static _ptr_type narrow (CORBA::ORB_ptr orb,
                               char const * id
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      /// Use resolve_initial_references to find an object and then
      /// narrow it.
      static _ptr_type narrow (PortableInterceptor::ORBInitInfo_ptr orb,
                               char const * id
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS);

    private:
      static _ptr_type narrow_object (CORBA::Object_ptr object
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    };

  } // namespace Utils
} // namespace TAO

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# include "RIR_Narrow.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#include "ace/post.h"
#endif /*TAO_UTILS_RIR_NARROW_H*/
