// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Current.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_SSLIOP_CURRENT_H
#define TAO_SSLIOP_CURRENT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_Current_Impl.h"

#include "orbsvcs/SSLIOPC.h"
#include "tao/ORB_Core.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    class Current;
    typedef Current * Current_ptr;
    typedef TAO_Pseudo_Var_T<Current> Current_var;
    typedef TAO_Pseudo_Out_T<Current> Current_out;

    /**
     * @class Current
     *
     * @brief Implementation of the TAO SSLIOP::Current extension.
     *
     * This object can be used to obtain SSL session related
     * information about the current execution context.  For example,
     * SSL peer certificate chains for the current request can be
     * obtained from this object.
     */
    class Current
      : public ::SSLIOP::Current,
        public ::CORBA::LocalObject
    {
    public:
      typedef Current_ptr _ptr_type;
      typedef Current_var _var_type;
      typedef Current_out _out_type;

      /// Constructor.
      Current (TAO_ORB_Core *orb_core);

      /// Return the peer certificate associated with the current
      /// request.
      virtual ::SSLIOP::ASN_1_Cert * get_peer_certificate ();

      /**
       * Return the certificate chain associated with the current
       * execution context.  If no SSL session is being used for the
       * request or upcall, then the NoContext exception is raised.
       * On the client side, the chain does include the peer (server)
       * certficate.  However, the certificate chain on the server
       * side does NOT contain the peer (client) certificate.
       */
      virtual ::SSLIOP::SSL_Cert * get_peer_certificate_chain ();

      /**
       * This method is mostly useful as an inexpensive means of
       * determining whether or not SSL session state is available.
       *
       * @return @c true if the current execution context is not
       *         within a SSL session.
       */
      virtual CORBA::Boolean no_context ();

      /// Set the TSS slot ID assigned to this object.
      void tss_slot (size_t slot);

      /// Setup the Current.
      void setup (Current_Impl *& prev_impl,
                  Current_Impl * new_impl,
                  bool &setup_done);

      /// Teardown the Current for this request.
      void teardown (Current_Impl *prev_impl, bool &setup_done);

      /**
       * @name Downcast and Reference Counting Methods
       *
       * These are basically the same methods generated by the IDL
       * compiler for all IDL interfaces.
       */
      //@{
      // The static operations.
      static Current_ptr _duplicate (Current_ptr obj);

      static Current_ptr _narrow (CORBA::Object_ptr obj);

      static Current_ptr _nil ()
      {
        return (Current_ptr)0;
      }

      virtual const char* _interface_repository_id () const;
      //@}

    protected:
      /// Destructor
      ~Current ();

      /// Set the TSS SSLIOP::Current implementation.
      int implementation (Current_Impl *impl);

      /// Return the TSS SSLIOP::Current implementation.
      Current_Impl *implementation ();

    private:
      Current (const Current &) = delete;
      void operator= (const Current &) = delete;

    private:
      /// TSS slot assigned to this object.
      size_t tss_slot_;

      /// Pointer to the ORB Core corresponding to the ORB with which this
      /// object is registered.
      TAO_ORB_Core * const orb_core_;
    };
  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "orbsvcs/SSLIOP/SSLIOP_Current.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CURRENT_H */
