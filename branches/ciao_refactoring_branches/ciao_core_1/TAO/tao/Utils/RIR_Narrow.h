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

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/ORB.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
      static _ptr_type narrow (CORBA::ORB_ptr orb, char const * id);

      /// Use resolve_initial_references to find an object and then
      /// narrow it.
      static _ptr_type narrow (PortableInterceptor::ORBInitInfo_ptr orb,
                               char const * id);

    private:
      static _ptr_type narrow_object (CORBA::Object_ptr object);
    };

  } // namespace Utils
} // namespace TAO


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# include "tao/Utils/RIR_Narrow.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#include /**/ "ace/post.h"

#endif /*TAO_UTILS_RIR_NARROW_H*/
