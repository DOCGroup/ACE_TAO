/**
 * @file Utils.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 *
 * Misc utility classes for NDDS.
 */

#ifndef NDDS_UTILS_H
#define NDDS_UTILS_H

#include "ndds/ndds_cpp.h"


namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      const char * translate_retcode (DDS_ReturnCode_t ret);
    }
  }
}

#endif
