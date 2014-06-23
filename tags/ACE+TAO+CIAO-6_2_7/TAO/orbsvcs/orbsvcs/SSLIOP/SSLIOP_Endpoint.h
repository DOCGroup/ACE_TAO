// -*- C++ -*-

//=============================================================================
/**
 *  @file     SSLIOP_Endpoint.h
 *
 *  $Id$
 *
 *  SSLIOP implementation of PP Framework Endpoint interface.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_ENDPOINT_H
#define TAO_SSLIOP_ENDPOINT_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOP/SSLIOP_OwnCredentials.h"

#include "orbsvcs/SSLIOPC.h"
#include "orbsvcs/SecurityC.h"

#include "tao/IIOP_Endpoint.h"
#include "ace/INET_Addr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /// Tag for storing multiple ssl endpoints within a single profile.
  const ACE_UINT32 TAG_SSL_ENDPOINTS = 0x54414f01U;
}

// namespace TAO
// {
//   namespace SSLIOP
//   {
    /**
     * @class Endpoint
     *
     * @brief SSLIOP-specific implementation of PP Framework Endpoint
     *        interface.
     *
     *
     */
    class TAO_SSLIOP_Export TAO_SSLIOP_Endpoint : public TAO_Endpoint
    {
    public:

      friend class TAO_SSLIOP_Profile;

      /// Constructor
      TAO_SSLIOP_Endpoint (const ::SSLIOP::SSL *ssl_component,
                TAO_IIOP_Endpoint *iiop_endp);

      /// Destructor.
      virtual ~TAO_SSLIOP_Endpoint (void);


      /// Need to have an assignment operator since the SSLIOP_Profile class
      /// may have to reorder its list of endpoints based on filtering by
      /// the EndpointPolicy.
      TAO_SSLIOP_Endpoint & operator= (const TAO_SSLIOP_Endpoint& other);

      /**
       * @name TAO_Endpoint Methods
       *
       * See Endpoint.h for their documentation.
       */
      //@{
      virtual TAO_Endpoint *next (void);
      virtual int addr_to_string (char *buffer, size_t length);

      /// Return true if this endpoint is equivalent to @a
      /// other_endpoint The relationship is defined as equivalency of
      /// their qop, hostname and ssl ports (if non-zero).
      /// Two endpoints may be equivalent even if their iiop counterparts are
      /// not. In fact, there are cases (as with the LPL processing)
      /// when those counterparts are not known at all.
      CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

      /// Return a copy of the corresponding endpoints by allocating
      /// memory.
      virtual TAO_Endpoint *duplicate (void);

      /// Return a hash value for this object.  Note that only the IP
      /// address and port are used to generate the hash value. This may
      /// cause a few more hash table collisions in the transport cache,
      /// because a synthesized SSLIOP endpoints for an address will
      /// have the same hash value as a fully qualified one. The
      /// redeeming feature is that it makes / bi-directional SSLIOP work
      /// by allowing descendent class (Synthetic_Endpoint) instances to
      /// be used as keys in the cache manager and match other fully
      /// qualified endpoint. (which were  used earlier to cache a
      /// particular transport)
      virtual CORBA::ULong hash (void);
      //@}

      /**
       * @name SSLIOP_Endpoint-specific Methods
       */
      //@{
      /// Return SSL component corresponding to this endpoint.
      const ::SSLIOP::SSL &ssl_component (void) const;

      /// Accessor to our IIOP counterpart.
      TAO_IIOP_Endpoint *iiop_endpoint (void) const;

      /// Mutator to our IIOP counterpart.
      /**
       * @param endpoint The new endpoint
       * @param destroy If set to @c true, the TAO::SSLIOP::Endpoint
       *                object retains ownership of the given
       *                TAO_IIOP_Endpoint.
       */
      void iiop_endpoint (TAO_IIOP_Endpoint *endpoint, bool destroy);

      /// Return the SSLIOP-specific ACE_INET_Addr.
      const ACE_INET_Addr &object_addr (void) const;

      /// Set the Quality-of-Protection, establishment of trust, and
      /// credentials for this endpoint. This is all done in one function
      /// so that the guard may be used uniformly.
      void set_sec_attrs (::Security::QOP qop,
                          const ::Security::EstablishTrust &trust,
                          const TAO::SSLIOP::OwnCredentials_ptr creds);

      /// Get the Quality-of-Protection settings for this endpoint.
      ::Security::QOP qop (void) const;

      /// Get the establishment of trust settings for this endpoint.
      ::Security::EstablishTrust trust (void) const;

      /// Get the credentials for this endpoint.
      /**
       * @note This method does not follow C++ mapping memory
       *       management rules.  Specifically, no duplication or
       *       reference counting occurs in this method.  This is so
       *       that no additional locks occur when checking the
       *       transport cache.
       */
      TAO::SSLIOP::OwnCredentials * credentials (void) const;
      //@}


      /// Credentials are not supplied by the constructor, and it is
      /// valid to have a nil credential, for instance if the
      /// SSL_use_certificate() method returns 0. Therefore it is
      /// necessary to have a new method to distinguish between a
      /// credential that is nil because it has not been set, vs one
      /// that was set to nil explicitly.
      int credentials_set (void) const;

    protected:

      /// Cache the SSL tagged component in a decoded format. Notice
      /// that we do not need to marshal this object!
      ::SSLIOP::SSL ssl_component_;

    private:

      /// Cached instance of ACE_INET_Addr for use in making invocations,
      /// etc.
      mutable ACE_INET_Addr object_addr_;

      /// IIOP Endpoints can be strung into a list.  Return the next
      /// endpoint in the list, if any.
      TAO_SSLIOP_Endpoint *next_;

      /// IIOP counterpart.
      /**
       * Since SSLIOP is an 'extension' of IIOP, each SSLIOP_Endpoint
       * contains SSL-specific information plus a pointer to the
       * IIOP_Endpoint containing the IIOP portion of our address.
       */
      TAO_IIOP_Endpoint *iiop_endpoint_;

      /// Flag that determines whether or not the iiop_endpoint_ member is
      /// deallocated with delete().
      bool destroy_iiop_endpoint_;

      /// Quailty-of-Protection settings for this endpoint object.
      ::Security::QOP qop_;

      /// Establishment of trust settings for this endpoint object.
      ::Security::EstablishTrust trust_;

      /// SSLIOP-specific credentials for this endpoint object.
      TAO::SSLIOP::OwnCredentials_var credentials_;

      /// A flag indicating that credentials_ was explicitly initialized
      int credentials_set_;
    };

    /**
     * @class SSLIOP_Synthetic_Endpoint
     *
     * @brief SSLIOP-specific implementation of PP Framework Endpoint
     *        interface, representing synthetic endpoints. An endpoints
     *        is synthetic whenever there is insuficient data to fully
     *        initialize an SSLIOP endpoint: qop, trust, credentials,
     *        etc. Such as when creating an SSLIOP endpoint in response
     *        of a Listen Point List or accepting a connection.
     *
     *        LPL and IOR-originated endpoints can now compare as
     *        equivalent, if they denote the same host, port and
     *        protection.  That would have given some false
     *        positives in some very obscure cases (same SSL port, but
     *        different protection or undelying IIOP port, or vice versa)
     *        The "synthetic eVndpoint" has its very own is_equivalent()
     *        to help eliminate any false positives and make the process
     *        more clear.
     *
     */
    class TAO_SSLIOP_Export TAO_SSLIOP_Synthetic_Endpoint : public TAO_SSLIOP_Endpoint
    {
    public:

      /// Constructor
      TAO_SSLIOP_Synthetic_Endpoint (TAO_IIOP_Endpoint *iiop_endp);

      /// Destructor.
      virtual ~TAO_SSLIOP_Synthetic_Endpoint (void);

      /**
       * Return true if this endpoint is equivalent to @param
       * other_endpoint.
       * Two synthetic endpoints are equivalent if their iiop counterparts are
       * equivalent, and, if both have non-zero ssl ports, their ssl
       * ports are the same.
       */
      CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

      /// Return a copy of the corresponding endpoints by allocating
      /// memory.
      virtual TAO_Endpoint *duplicate (void);

    private:
      TAO_SSLIOP_Synthetic_Endpoint (const ::SSLIOP::SSL *ssl);

    };

//   }  // End SSLIOP namespace.
// }  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/SSLIOP/SSLIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_ENDPOINT_H */
