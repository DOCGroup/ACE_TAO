// -*- C++ -*-

//=============================================================================
/**
 *  @file    Non_Servant_Upcall.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_NONSERVANT_UPCALL_H
#define TAO_NONSERVANT_UPCALL_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declaration
class TAO_Root_POA;
class TAO_Object_Adapter;

namespace TAO
{
  namespace Portable_Server
  {
    /**
     * @class Non_Servant_Upcall
     *
     * @brief This class helps us with a recursive thread lock without
     * using a recursive thread lock.  Non_Servant_Upcall has a
     * magic constructor and destructor.  We unlock the
     * Object_Adapter lock for the duration of the non-servant
     * (i.e., adapter activator and servant activator) upcalls;
     * reacquiring once the upcalls complete.  Even though we are
     * releasing the lock, other threads will not be able to make
     * progress since
     * <Object_Adapter::non_servant_upcall_in_progress_> has been
     * set.
     */
    class TAO_PortableServer_Export Non_Servant_Upcall
    {
    public:

      /// Constructor.
      Non_Servant_Upcall (::TAO_Root_POA &poa);

      /// Destructor.
      ~Non_Servant_Upcall (void);

      ::TAO_Root_POA &poa (void) const;

    protected:

      TAO_Object_Adapter &object_adapter_;
      TAO_Root_POA &poa_;
      Non_Servant_Upcall *previous_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Non_Servant_Upcall.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_NONSERVANT_UPCALL_H */
