// -*- C++ -*-

//=============================================================================
/**
 * @file CSI_Utils.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_CSI_UTILS_H
#define TAO_CSI_UTILS_H

// No need to include "ace/pre.h" and "ace/post.h".  This header and
// the declared types/variables are not meant to be used outside TAO's
// CSIv2 implementation.

#include "tao/orbconf.h"
#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations
namespace IOP
{
  struct ServiceContext;
}

namespace CSI
{
  class SASContextBody;
}

namespace TAO
{
  namespace CSI_Utils
  {
    /// Populate given IOP::ServiceContext with given CSI::SASContextBody.
    void create_sas_service_context (const CSI::SASContextBody & sas_context,
                                     IOP::ServiceContext & sc);

    /// Extract CSI::SASContextBody from given IOP::ServiceContext.
    /**
     * @return Success == true, Failure == false.
     */
    bool extract_sas_service_context (const IOP::ServiceContext & sc,
                                      CSI::SASContextBody & sas_context);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_CSI_UTILS_H */
