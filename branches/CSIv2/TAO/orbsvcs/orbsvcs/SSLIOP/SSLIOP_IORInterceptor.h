// -*- C++ -*-

//=============================================================================
/**
 * @file SSLIOP_IORInterceptor.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_IOR_INTERCEPTOR_H
#define TAO_SSLIOP_IOR_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CSIIOPC.h"
#include "tao/IORInterceptor/IORInterceptorC.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @class IORInterceptor
     *
     * @brief IOR interceptor that adds SSLIOP-specific information to
     *        a CSIv2 tagged component.
     *
     * This IOR interceptor adds SSLIOP-specific information to a
     * CSIv2 tagged component.
     */
    class IORInterceptor
      : public virtual PortableInterceptor::IORInterceptor,
        public virtual TAO_Local_RefCounted_Object
    {
    public:

      /// Constructor
      IORInterceptor (TAO_ORB_Core * orb_core,
                      CSIIOP::AssociationOptions csiv2_target_supports,
                      CSIIOP::AssociationOptions csiv2_target_requires);

      /**
       * @name Methods Required by the IOR Interceptor Interface
       *
       * These are methods that must be implemented since they are
       * pure virtual in the abstract base class.  They are the
       * canonical methods required for all IOR interceptors.
       */
      //@{
      /// Return the name of this IORInterceptor.
      virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Cleanup resources acquired by this IORInterceptor.
      virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      /// Add the tagged components to the IOR.
      virtual void establish_components (PortableInterceptor::IORInfo_ptr info
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void components_established (
          PortableInterceptor::IORInfo_ptr info
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void adapter_manager_state_changed (
          PortableInterceptor::AdapterManagerId id,
          PortableInterceptor::AdapterState state
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));

      virtual void adapter_state_changed (
          const PortableInterceptor::ObjectReferenceTemplateSeq & templates,
          PortableInterceptor::AdapterState state
          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((CORBA::SystemException));
      //@}

    private:

      /// Construct the @c IOP::TAG_TLS_SEC_TRANS tagged component
      /// containing the @c CSIIOP::TLS_SEC_TRANS structure.
      int construct_transport_mech_component (IOP::TaggedComponent & tc);

      /// Populate the @c CSIIOP::TLS_SEC_TRANS::addresses field.
      int populate_transport_address_list (
            CSIIOP::TransportAddressList & list);

    private:

      // Pointer to the @c TAO_ORB_Core within which this interceptor
      // is registered.
      TAO_ORB_Core * orb_core_;

      /// Supported CSIv2 TLS-specific association options.
      CSIIOP::AssociationOptions csiv2_target_supports_;

      /// Required CSIv2 TLS-specific association options.
      CSIIOP::AssociationOptions csiv2_target_requires_;
    };

  }  // End SSLIOP namespace

}  // End TAO namespace


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_SSLIOP_IOR_INTERCEPTOR_H */
