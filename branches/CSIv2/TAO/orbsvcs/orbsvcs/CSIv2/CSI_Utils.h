// -*- C++ -*-

//=============================================================================
/**
 * @file CSI_Util.h
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

// Forward declarations
namespace IOP
{
  class ServiceContext;
}

namespace CSI
{
  class SASContextBody;
}

namespace TAO
{
  namespace CSI_Util
  {
    /// Populate given IOP::ServiceContext with given CSI::SASContextBody.
    void create_sas_service_context (const CSI::SASContextBody & sas_context,
                                     IOP::ServiceContext & sc);

    /// Extract CSI::SASContextBody from given IOP::ServiceContext.
    /**
     * @return Success == 1, Failure == 0.
     */
    int extract_sas_service_context (const IOP::ServiceContext & sc,
                                     CSI::SASContextBody & sas_context);
  }
}

#endif  /* TAO_CSI_UTILS_H */
