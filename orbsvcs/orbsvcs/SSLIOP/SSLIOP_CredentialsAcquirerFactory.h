// -*- C++ -*-

//=============================================================================
/**
 * @file SSLIOP_CredentialsAcquirerFactory.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_CREDENTIALS_ACQUIRER_FACTORY_H
#define TAO_SSLIOP_CREDENTIALS_ACQUIRER_FACTORY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/SSLIOP/SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Security/SL3_CredentialsAcquirerFactory.h"

#include "orbsvcs/SecurityLevel3C.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {

    /**
     * @class CredentialsAcquirerFactory
     *
     * @brief SSLIOP-specific TAO::SL3::CredentialsAcquirerFactory
     *        implementation.
     *
     * This class generates SSLIOP::CredentialsAcquirer instances.
     */
    class TAO_SSLIOP_Export CredentialsAcquirerFactory
      : public virtual TAO::SL3::CredentialsAcquirerFactory
    {
    public:

      /// Create a TAO::SSLIOP::CredentialsAcquirerFactory.
      virtual SecurityLevel3::CredentialsAcquirer_ptr make (
        TAO::SL3::CredentialsCurator_ptr curator,
        const CORBA::Any & acquisition_arguments);

    };

  } // End SSLIOP namespace
}  // End TAO namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_CREDENTIALS_ACQUIRER_FACTORY_H */
