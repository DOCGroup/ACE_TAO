// -*- C++ -*-

//=============================================================================
/**
 *  @file     SSLIOP_Factory.h
 *
 *  $Id$
 *
 *  SSLIOP implementation of PP Framework Protocol_Factory interface.
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_FACTORY_H
#define TAO_SSLIOP_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SecurityC.h"
#include "orbsvcs/CSIIOPC.h"

#include "tao/Protocol_Factory.h"

#include "ace/Service_Config.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Acceptor;
class TAO_Connector;

namespace TAO
{
  namespace SSLIOP
  {

    /**
     * @class Protocol_Factory
     *
     * @brief SSLIOP-specific protocol factory implementation.
     *
     * This class implements the SSLIOP-specific protocol factory
     * implementation for use in TAO's pluggable protocols framework.
     */
    class TAO_SSLIOP_Export Protocol_Factory
      : public TAO_Protocol_Factory
    {
    public:

      /// Constructor.
      Protocol_Factory (void);

      /// Destructor.
      virtual ~Protocol_Factory (void);

      // = Service Configurator hooks.
      /// Dynamic linking hook
      virtual int init (int argc, ACE_TCHAR* argv[]);

      /// Verify prefix is a match
      virtual int match_prefix (const ACE_CString & prefix);

      /// Returns the prefix used by the protocol.
      virtual const char * prefix (void) const;

      /// Return the character used to mark where an endpoint ends and
      /// where its options begin.
      virtual char options_delimiter (void) const;

      // = Check Protocol_Factory.h for a description of these methods.
      virtual TAO_Acceptor  * make_acceptor (void);
      virtual TAO_Connector * make_connector  (void);
      virtual int requires_explicit_endpoint (void) const;

    private:

      /// Parse an X509 file path, which is expected to looks like:
      ///   <X509Path> ::= <Prefix> ':' <Path>
      ///   <Prefix>   ::= 'PEM' | 'ASN1'
      ///   <Path>     ::= any-string
      /// Returns either SSL_FILETYPE_ASN1, SSL_FILETYPE_PEM or -1
      /// if the prefix can not be recognized. The *path will point
      /// to the part of the original buffer, after the initial ':',
      /// or will contain 0, if no path was specified.
      ///
      /// Beware: This function modifies the buffer pointed to by arg!
      ///
      static int parse_x509_file (char *arg, char *&path);

      /// Create and register the SSLIOP ORB initializer.
      int register_orb_initializer (
        CSIIOP::AssociationOptions csiv2_target_supports,
        CSIIOP::AssociationOptions csiv2_target_requires);

    private:

      /// Default quality-of-protection settings for the SSLIOP
      /// pluggable protocol.
      ::Security::QOP qop_;

      /// The accept() timeout.
      /**
       * This timeout includes the overall time to complete the SSL
       * handshake.  This includes both the TCP handshake and the SSL
       * handshake.
       */
      ACE_Time_Value timeout_;

      /// The SSLIOP-specific CSIv2 transport mechanism component.
      /**
       * This SSLIOP-specific structure is embedded in the CSIv2 transport
       * mechanism list of the @c CSIIOP::CompoundSecMechList IOR tagged
       * component.
       */
      // CSIIOP::TLS_SEC_TRANS * csiv2_component_;

    };
  }  // End SSLIOP namespace.
}  // End TAO namespace.

// Work around preprocessor tokenization.
typedef TAO::SSLIOP::Protocol_Factory TAO_SSLIOP_Protocol_Factory;

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_SSLIOP, TAO_SSLIOP_Protocol_Factory)
ACE_STATIC_SVC_REQUIRE (TAO_SSLIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (TAO_SSLIOP, TAO_SSLIOP_Protocol_Factory)

#include /**/ "ace/post.h"

#endif /* TAO_SSLIOP_FACTORY_H */
