/**
 * @file Utils.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Misc utility classes for NDDS.
 */

#ifndef NDDS_UTILS_H
#define NDDS_UTILS_H

#include "ndds/ndds_cpp.h"
#include "dds4ccm_ndds_impl_export.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      DDS4CCM_NDDS_IMPL_Export const char * translate_retcode (DDS_ReturnCode_t ret);
    }
  }
}

#endif
