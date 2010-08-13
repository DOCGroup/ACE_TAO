// -*- C++ -*-

//=============================================================================
/**
 *  @file    HTIOP_Acceptor.h
 *
 *  $Id$
 *
 *  HTIOP specific acceptor processing
 *
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef HTIOP_ACCEPTOR_H
#define HTIOP_ACCEPTOR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/HTIOP/HTIOP_Completion_Handler.h"
#include "orbsvcs/HTIOP/HTIOP_Connection_Handler.h"
#include "orbsvcs/HTIOP/HTIOP_Acceptor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/HTBP/HTBP_Addr.h"

#include "tao/Acceptor_Impl.h"
#include "tao/Transport_Acceptor.h"
#include "tao/GIOP_Message_Version.h"
#include "tao/Object_KeyC.h"

#include "ace/SOCK_Acceptor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
namespace ACE
{
  namespace HTBP
  {
    class Environment;
  }
}
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace HTIOP
  {

    // TAO HTIOP_Acceptor concrete call definition

    /**
     * @class HTIOP_Acceptor
     *
     * @brief HTIOP_Acceptor
     *
     * The HTIOP-specific bridge class for the concrete acceptor.
     * Based on IIOP_Acceptor
     */
    class HTIOP_Export Acceptor : public TAO_Acceptor
    {
    public:
      /// Constructor.
      Acceptor (ACE::HTBP::Environment *ht_env,
                int is_inside);

      /// Destructor.
      ~Acceptor (void);

      /// @@ Helper method for the implementation repository, should go
      ///    away
      const ACE::HTBP::Addr& address (void) const;

      /// Returns the array of endpoints in this acceptor
      const ACE::HTBP::Addr *endpoints (void);

      typedef TAO_Strategy_Acceptor<Completion_Handler, ACE_SOCK_ACCEPTOR> BASE_ACCEPTOR;
      typedef Creation_Strategy<Completion_Handler> CREATION_STRATEGY;
      typedef Concurrency_Strategy<Completion_Handler> CONCURRENCY_STRATEGY;
      typedef Accept_Strategy<Completion_Handler, ACE_SOCK_ACCEPTOR> ACCEPT_STRATEGY;

      /**
       * The TAO::Acceptor methods, check the documentation in
       * Transport_Acceptor.h for details.
       */
      virtual int open (TAO_ORB_Core *orb_core,
                        ACE_Reactor *reactor,
                        int version_major,
                        int version_minor,
                        const char *address,
                        const char *options = 0);

      virtual int open_default (TAO_ORB_Core *orb_core,
                                ACE_Reactor *reactor,
                                int version_major,
                                int version_minor,
                                const char *options = 0);

      virtual int close (void);

      virtual int create_profile (const TAO::ObjectKey &object_key,
                                  TAO_MProfile &mprofile,
                                  CORBA::Short priority);

      virtual int is_collocated (const TAO_Endpoint *endpoint);

      virtual CORBA::ULong endpoint_count (void);

      virtual int object_key (IOP::TaggedProfile &profile,
                              TAO::ObjectKey &key);

      /**
       * Set the host name for the given addr.
       * A hostname may be forced by using specified_hostname.  This
       * is useful if the given address corresponds to more than one
       * hostname and the desired one cannot be determined in any
       * other way.
       */
      int hostname (TAO_ORB_Core *orb_core,
                    ACE_INET_Addr &addr,
                    char *&host,
                    const char *specified_hostname = 0);

      /**
       * Set the host name for the given address using the dotted decimal
       * format.
       */
      int dotted_decimal_address (ACE_INET_Addr &addr,
                                  char *&host);

    protected:

      /**
       * Implement the common part of the open*() methods.  This method is
       * virtual to allow a derived class implementation to be invoked
       * instead.
       */
      virtual int open_i (const ACE::HTBP::Addr &addr,
                          ACE_Reactor *reactor);

      /**
       * Probe the system for available network interfaces, and initialize
       * the <addrs_> array with an ACE::HTBP::Addr for each network
       * interface.  The port for each initialized ACE::HTBP::Addr will be
       * set in the open_i() method.  This method only gets invoked when
       * no explicit hostname is provided in the specified endpoint.
       */
      int probe_interfaces (TAO_ORB_Core *orb_core);

      /**
       * Parse protocol specific options.
       *
       * Currently supported: hostname_in_ior -- Provides the means to add
       * in an alternative hostname in the object reference, such as that
       * of the outside of a firewall.
       */
      virtual int parse_options (const char *options);

      /// Helper method to add a new profile to the mprofile for
      /// each endpoint.
      int create_new_profile (const TAO::ObjectKey &object_key,
                              TAO_MProfile &mprofile,
                              CORBA::Short priority);

      /// Helper method to create a profile that contains all of
      /// our endpoints.
      int create_shared_profile (const TAO::ObjectKey &object_key,
                                 TAO_MProfile &mprofile,
                                 CORBA::Short priority);


    protected:

      /// Array of ACE::HTBP::Addr instances, each one corresponding to a
      /// given network interface.
      ACE::HTBP::Addr *addrs_;

      /**
       * Cache the information about the endpoints serviced by this
       * acceptor.
       * There may in fact be multiple hostnames for this endpoint. For
       * example, if the IP address is INADDR_ANY (0.0.0.0) then there
       * will be possibly a different hostname for each interface.
       */
      char **hosts_;

      /// The number of host names cached in the hosts_ array (equivalent
      /// to the number of endpoints opened by this Acceptor).
      CORBA::ULong endpoint_count_;

      /**
       * Override the hostname used in the ORBEndPoint.
       */
      char *hostname_in_ior_;

      /**
       * The GIOP version for this endpoint
       * @@ Theoretically they shouldn't be here!! We need to look at a
       * way to move this out
       */
      TAO_GIOP_Message_Version version_;

      /// ORB Core.
      TAO_ORB_Core *orb_core_;

    private:

      /// the concrete acceptor, as a pointer to it's base class.
      BASE_ACCEPTOR base_acceptor_;

      /// Acceptor strategies.
      CREATION_STRATEGY *creation_strategy_;
      CONCURRENCY_STRATEGY *concurrency_strategy_;
      ACCEPT_STRATEGY *accept_strategy_;

      ACE::HTBP::Environment *ht_env_;

      /// Flag used to determine if an acceptor should use an "inside"
      /// local address, or try to compose an "outside" address.
      /// If the value is 1, then always use an HTID for local addr,
      /// otherwise use host:port if 0, or guess based on proxy config
      /// if -1.
      int inside_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/HTIOP/HTIOP_Acceptor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* HTIOP_ACCEPTOR_H */
